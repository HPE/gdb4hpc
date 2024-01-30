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

        auto num_added = size_t{0};

        for (const auto& dir_entry : std::filesystem::directory_iterator{parent}) {
            if (!dir_entry.path().native().starts_with(partial_filename.native())) continue;
            this->possibilities.emplace_back(dir_entry.path());
            num_added++;
        }

        m_has_filesystem_names = num_added > 0;

        // on an exact directory match, add '/'
        if (num_added == 1 && std::filesystem::is_directory(this->possibilities.back())) {
            this->possibilities.back() += "/";
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
    auto fns = di.frontendGdb()->getFunctionSymbolNames(partial_fn_name);
    auto cleanup = OnScopeExit([&](){ FreeEvent(fns); });

    if (!fns || fns->ev_event != DBGEV_SYMBOLS) {
        // silently fail
        return;
    }

    for (auto& sym : fns->ev_u.ev_u_symbols->debug) {
        if (sym.name.starts_with(partial_fn_name)) {
            this->possibilities.emplace_back(std::move(sym.name));
            m_has_function_symbol_names = true;
        }
    }
    for (auto& sym : fns->ev_u.ev_u_symbols->nondebug) {
        if (sym.name.starts_with(partial_fn_name)) {
            this->possibilities.emplace_back(std::move(sym.name));
            m_has_function_symbol_names = true;
        }
    }
}

bool Completions::has_function_symbol_names() const {
    return m_has_function_symbol_names;
}

// TODO
//
// void Completions::add_variable_symbol_names(const DbgInvoke& /*di*/, std::string_view /*partial_word*/, std::string_view /*buffer*/) {
//     // TODO
// }

void Completions::add_source_file_names(const DbgInvoke& di, std::filesystem::path partial_path) {
    // DbgInvokes do have a source path caching machanism, but that is only used in mi
    // mode and would be empty for most situations this function is called. So we look
    // them up ourselves.
    auto sources = di.frontendGdb()->getSources();
    auto sources_cleanup = OnScopeExit([&](){ FreeEvent(sources); });

    if (!sources || sources->ev_event != DBGEV_SOURCES) {
        // silently fail
        return;
    }

    for (auto* p = sources->ev_u.ev_u_sources; p != nullptr; p = p->sources_next) {
        if (!p->sources_file) continue;
        auto source_path = std::filesystem::path{p->sources_file};

        if (source_path.native().starts_with(partial_path.native())) {
            this->possibilities.emplace_back(source_path);
            m_has_source_file_names = true;
        }

        // also check the filename (instead of the path) to support completing things like
        // "break foo<>" when the full path given by mi is "/home/josh/code/src/foobar.c"
        const auto filename = source_path.filename();
        if (!filename.empty() && filename.native().starts_with(partial_path.native())) {
            this->possibilities.emplace_back(source_path.filename());
            m_has_source_file_names = true;
        }
    }
}

std::string Completions::calc_common_prefix() const {
    if (this->possibilities.size() == 0) return "";

    auto prefix = std::string_view{this->possibilities[0]};

    // skipping 1st since we don't have to compare the first word with itself
    for (std::string_view poss : this->possibilities | std::views::drop(1)) {
        if (prefix.empty()) break;
        auto c = size_t{0};
        while (c < prefix.size() && c < poss.size() && prefix[c] == poss[c]) c++;
        if (c != prefix.size()) prefix = prefix.substr(0, c);
    }

    return std::string{prefix};
}
