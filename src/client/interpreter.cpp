// Copyright 2023 Hewlett Packard Enterprise Development LP.
#include "interpreter.h"

// available interpreters are "console" and "mi".
#include "interpreter_console.h"
#include "interpreter_mi.h"

#include "useful.h"

#include <ranges>
#include <filesystem>

std::shared_ptr<Interpreter> Interpreter::make(std::string_view name, cli_stream_t& io, MakeOpts&& opts) {
    if (name == "console") {
        auto intr = std::make_shared<ConsoleInterpreter>(io);
        if (opts.async) intr->attach_observers();
        return intr;
    }

    if (name == "mi") {
        auto intr = std::make_shared<MiInterpreter>(io);
        if (opts.async) intr->attach_observers();
        return intr;
    }

    return nullptr;
}

std::string Interpreter::get_prompt() const {
    return "";
}

void Interpreter::Printer::print_message(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    print_message_v(fmt, args);
    va_end(args);
}

void Interpreter::Printer::print_error(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    print_error_v(fmt, args);
    va_end(args);
}

using Completions = Interpreter::Completions;

void Completions::add_filesystem_names(std::filesystem::path partial_filename) {
    // partial_filename can contain multiple directories
    // in the path leading to the filename on the end.
    // e.g. "launch $a{2} ./bin/CRAY/c/c_i<>"
    // -> partial_filename: "./bin/CRAY/c/c_i"
    try {
        const auto parent = [&](){
            auto p = std::filesystem::path{partial_filename}.remove_filename();
            if (p.empty()) p = ".";
            return p;
        }();

        auto new_possibilities = std::vector<std::string>{};

        for (const auto& dir_entry : std::filesystem::directory_iterator{parent}) {
            if (!dir_entry.path().native().starts_with(partial_filename.native())) continue;
            new_possibilities.emplace_back(dir_entry.path());
        }

        m_has_filesystem_names |= new_possibilities.size() > 0;

        // on an exact directory match, add '/'
        if (new_possibilities.size() == 1 && std::filesystem::is_directory(new_possibilities.back())) {
            new_possibilities.back() += "/";
        }

        for (auto& choice : new_possibilities) {
            this->possibilities.emplace(std::move(choice));
        }
    } catch (const std::filesystem::filesystem_error& e) {
        // silently fail
        return;
    }
}

bool Completions::has_filesystem_names() const {
    return m_has_filesystem_names;
}

void Completions::add_function_symbol_names(const DbgInvoke& di, std::string_view partial_fn_name) {
    if (auto* gdbmi = di.frontendGdb()) {
        auto fns = gdbmi->getFunctionSymbolNames(partial_fn_name);
        auto cleanup = OnScopeExit([&](){ FreeEvent(fns); });

        if (!fns || fns->ev_event != DBGEV_SYMBOLS) {
            // silently fail
            return;
        }

        for (auto& sym : fns->ev_u.ev_u_symbols->debug) {
            if (sym.name.starts_with(partial_fn_name)) {
                this->possibilities.emplace(std::move(sym.name));
                m_has_function_symbol_names = true;
            }
        }
        for (auto& sym : fns->ev_u.ev_u_symbols->nondebug) {
            if (sym.name.starts_with(partial_fn_name)) {
                this->possibilities.emplace(std::move(sym.name));
                m_has_function_symbol_names = true;
            }
        }
    }
}

bool Completions::has_function_symbol_names() const {
    return m_has_function_symbol_names;
}

// Check to see if any variable names have partial_var_name as a prefix
static bool add_possibilites_from_vars(std::set<std::string>& possibilities,
    dbgevent_t* vars, std::string_view partial_name) {

    if (!vars || vars->ev_event != DBGEV_VARS) return false;

    auto result = false;

    for (auto* p = vars->ev_u.ev_u_vars; p != nullptr; p = p->var_next) {
        if (!p->var_name) continue;

        // we only care about the first partial_var_name.size() characters of p->var_name.
        // calling string_view(p->var_name, partial_var_name.size()) could construct
        // a string_view that extends past the end of var_name, so we call ::strnlen
        // on the outside to get the minimum length we need.
        auto view = std::string_view(p->var_name, ::strnlen(p->var_name, partial_name.size()));
        if (view == partial_name) {
            possibilities.emplace(p->var_name);
            result = true;
        }
    }

    return result;
}

void Completions::add_variable_symbol_names(const DbgInvoke& di, const Bitset& ranks, std::string_view partial_var_name) {
    if (!di.frontendGdb()) return;
    auto *gdbmi = di.frontendGdb();

    {   // add global variables
        auto vars = gdbmi->getVariableSymbolNames(partial_var_name);
        auto cleanup = OnScopeExit([&](){ FreeEvent(vars); });

        if (vars && vars->ev_event == DBGEV_SYMBOLS && vars->ev_u.ev_u_symbols) {
            for (auto& sym : vars->ev_u.ev_u_symbols->debug) {
                if (sym.name.starts_with(partial_var_name)) {
                    this->possibilities.emplace(std::move(sym.name));
                    m_has_function_symbol_names = true;
                }
            }
            for (auto& sym : vars->ev_u.ev_u_symbols->nondebug) {
                if (sym.name.starts_with(partial_var_name)) {
                    this->possibilities.emplace(std::move(sym.name));
                    m_has_function_symbol_names = true;
                }
            }
        } else {
            // silently fail
        }
    }

    // add local variables
    if (auto* cached_frames = di.getCachedFrames()) {
        // querying mi is the bottleneck here, so we go to some length to make as few mi queries as possible.
        //
        // we limit the total amount of queries to some arbitrary amount.
        // some experiments on pea2k show a query for a function with n local variables
        // takes about (n + 10)ms to finish. lets conservatively account for 20 local variables
        // per function (probably more than the true average) to get 30 ms per query,
        // and say the total time shouldn't be more than 300ms. that gives us room for "only" 10 queries,
        // but note that these will be the 10 *most recently cached* locations, so the chance of
        // matching the variable that the user is thinking of is decent even with only 10 queries.
        //
        // we also track already seen locations and avoid making duplicate queries.

        auto seen_locs = std::set<std::string>{};
        const auto max_queries = 10;
        auto query_count = 0;

        for (const auto& loc : cached_frames->masked(ranks)) {
            const auto locstr = [&]() {
                // filename:linenum locations are the best because they can capture local variables
                // that aren't declared until later in the function or inside of brackets. if that's
                // missing, we just fall back to function names.
                if (loc.loc_file && loc.loc_line > 0) {
                    return std::string{loc.loc_file} + ":" + std::to_string(loc.loc_line);
                } else if (loc.loc_func) {
                    return std::string{loc.loc_func};
                } else {
                    return std::string{""};
                }
            }();
            if (locstr.empty() || seen_locs.contains(locstr)) continue;
            seen_locs.insert(locstr);

            if (query_count >= max_queries) break;
            query_count++;

            auto* vars = gdbmi->getScope(locstr.c_str());
            auto vars_cleanup = OnScopeExit([&](){ FreeEvent(vars); });

            if (add_possibilites_from_vars(this->possibilities, vars, partial_var_name)) {
                m_has_variable_symbol_names = true;
            }
        }
    }
}

bool Completions::has_variable_symbol_names() const {
    return m_has_variable_symbol_names;
}

void Completions::add_source_file_names(const DbgInvoke& di, std::filesystem::path partial_path) {
    // DbgInvokes do have a source path caching machanism, but that is only used in mi
    // mode and would be empty for most situations this function is called. So we look
    // them up ourselves.
    if (auto* gdbmi = di.frontendGdb()) {
        auto sources = gdbmi->getSources();
        auto sources_cleanup = OnScopeExit([&](){ FreeEvent(sources); });

        if (!sources || sources->ev_event != DBGEV_SOURCES) {
            // silently fail
            return;
        }

        for (auto* p = sources->ev_u.ev_u_sources; p != nullptr; p = p->sources_next) {
            if (!p->sources_file) continue;
            auto source_path = std::filesystem::path{p->sources_file};

            if (source_path.native().starts_with(partial_path.native())) {
                this->possibilities.emplace(source_path);
                m_has_source_file_names = true;
            }

            // also check the filename (instead of the path) to support completing things like
            // "break foo<>" when the full path given by mi is "/home/josh/code/src/foobar.c"
            const auto filename = source_path.filename();
            if (!filename.empty() && filename.native().starts_with(partial_path.native())) {
                this->possibilities.emplace(source_path.filename());
                m_has_source_file_names = true;
            }
        }
    }
}

void Completions::add_struct_member_names(const DbgInvoke& di, const Bitset& ranks, std::string_view partial_struct_name) {
    if (!di.frontendGdb()) return;
    auto *gdbmi = di.frontendGdb();

    // Get base name of struct before dot
    auto last_dot = partial_struct_name.rfind('.');
    if (last_dot == partial_struct_name.npos) {
        return;
    }
    auto struct_name = std::string{partial_struct_name.substr(0, last_dot)};

    // Try global struct members
    { auto vars = gdbmi->getStructMemberNames(struct_name, struct_name);
        auto cleanup = OnScopeExit([&](){ FreeEvent(vars); });

        if (add_possibilites_from_vars(this->possibilities, vars, partial_struct_name)) {
            m_has_struct_member_names = true;
        }
    }

    // add local variables
    if (auto* cached_frames = di.getCachedFrames()) {

        auto seen_locs = std::set<std::string>{};
        const auto max_queries = 10;
        auto query_count = 0;

        for (const auto& loc : cached_frames->masked(ranks)) {
            // ptype requires function name location, not filename:lineno
            auto locstr = (loc.loc_func) ? std::string{loc.loc_func} : std::string{};

            if (locstr.empty() || seen_locs.contains(locstr)) continue;
            seen_locs.insert(locstr);

            if (query_count >= max_queries) break;
            query_count++;

            // Get struct member variables
            auto fq_struct_name = locstr + "::" + struct_name;
            auto vars = gdbmi->getStructMemberNames(fq_struct_name, struct_name);
            auto cleanup = OnScopeExit([&](){ FreeEvent(vars); });

            if (add_possibilites_from_vars(this->possibilities, vars, partial_struct_name)) {
                m_has_struct_member_names = true;
            }
        }
    }
}

bool Completions::has_struct_member_names() const {
    return m_has_struct_member_names;
}

std::string Completions::calc_common_prefix() const {
    if (this->possibilities.size() == 0) return "";

    auto prefix = std::string_view{*this->possibilities.begin()};

    // skipping 1st since we don't have to compare the first word with itself
    for (std::string_view poss : this->possibilities | std::views::drop(1)) {
        if (prefix.empty()) break;
        auto c = size_t{0};
        while (c < prefix.size() && c < poss.size() && prefix[c] == poss[c]) c++;
        if (c != prefix.size()) prefix = prefix.substr(0, c);
    }

    return std::string{prefix};
}
