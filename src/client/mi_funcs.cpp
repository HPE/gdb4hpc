// mi_funcs.cpp - Command line interface routines.
// Copyright 2022-2023 Hewlett Packard Enterprise Development LP.

#include "client.h"
#include "useful.h"
#include "interpreter_mi.h"
#include <map>
#include <regex>

const char* mi_dummyFrameAddr = "addr=\"0xf00f\",";

int launch_count=0;

static int handle_result_string(MiInterpreter& mi, std::string result) {
    // XXX: all of the mi functions in this file return strings to indicate
    // success/failure. this should be fixed to be more type safe, but leaving it be for
    // now.
    if (result == "ok") {
        return Command::Success;
    } else if (result.starts_with("ok,")) {
        mi.set_result("%s", result.substr(3).c_str());
        return Command::Success;
    } else if (result.starts_with("error,")) {
        PrintOutError("%s\n", result.substr(6).c_str());
        return Command::Failure;
    } else {
        // should never happen
        PrintOutError("%s\n", result.c_str());
        return Command::Failure;
    }
}

static int exec_args_cmd(Command::LoweredContext<MiInterpreter>& ctx, rec_set_t*, int argc, const char* const argv[]) {
    static std::regex procRE{R"((\$\w+)\{(\d+)\})"};
    ArgvStream args(argc - 1, &argv[1]);

    std::ostringstream argstr;
    ctx.caller.m_num_ranks = "1";
    ctx.caller.m_procset = "";
    std::smatch match;

    // recognize -n <num-ranks> -p <proc-set-name> or
    // $<proc-set-name>{<num-ranks>} and pass any other arguments to gdb4hpc
    // this will mean they need to use --args and/or --launcher-args
    for (size_t argc=args.argc(), i=0; i<argc; ++i) {
        auto arg = args[i];
        if (arg == "-n" && i+1 < argc) {
            ctx.caller.m_num_ranks = args[++i];
        } else if (std::regex_match(arg, match, procRE)) {
            ctx.caller.m_procset = match[1];
            ctx.caller.m_num_ranks = match[2];
        } else {
            argstr << " " << arg;
        }
    }

    ctx.caller.m_launcher_args = argstr.str() + " ";
    //this command sets launcher args and does not need to be processed. Setting to exit will skip command processing
    return handle_result_string(ctx.caller, "ok");
}

void mi_logv2(const char* tag, const char* format, va_list arg_list) {
    if (auto dir = ::getenv("CRAY_DBG_LOG_DIR")) {
        auto fname = std::string{dir} + "/mi.log";
        static int cnt = 0;
        if (FILE *f = fopen(fname.c_str(), cnt++ ? "a" : "w")) {
            if (tag) fprintf(f, tag);
            vfprintf(f, format, arg_list);
            fclose(f);
        }
    }
}

void mi_logv(const char* format, va_list arg_list) { mi_logv2(nullptr, format, arg_list); }

void mi_log(const char* format, ...) {
    va_list arg_list;
    va_start(arg_list, format);
    mi_logv(format, arg_list);
    va_end(arg_list);
}

namespace {

// a single MI variable object.  MI uses these as a persistent reference to a program
// variable
class Var {
public:
    Var(const std::string& name, const std::string& frame, const std::string& expression, const std::string& procset,
        std::string&& value);

    void update(bool allValues, bool& isFirst, std::ostream& os);
    
private:
    std::string m_name;
    std::string m_frame;
    std::string m_expression;
    std::string m_procset;
    std::string m_value;
};

// manage the full set of MI variable objects
class Vars {
public:
    std::string create(const std::string& name, const std::string& frame,
                       const std::string& expression);
    void remove(const std::string& name);
    std::string update(const std::string& name, bool allValues);

private:
    int m_nextVar = 1;
    std::map<const std::string, Var> m_vars;
};

}

// singleton for managing the MI vars
Vars miVars;    

extern std::string GetMiValue(const std::string& expression, const std::string& ps);

// The frame part of the MI variables it no implemented
Var::Var(const std::string& name, const std::string& frame, const std::string& expression, const std::string& procset,
         std::string&& value) :
    m_name(name), m_frame(frame), m_expression(expression), m_procset(procset), m_value(std::move(value))
{}

// Determine if the value of a program variable has changed
// If the value has changed, send the MI formatted result to "os"
void Var::update(bool allValues, bool& isFirst, std::ostream& os)
{
    auto newValue = GetMiValue(m_expression, m_procset);

    //remove procset info from value string
    std::string val="";
    static std::regex value_regex(R"(\w+\{?[\s\S]*\}?\:([\s\S]*))"); 
    std::smatch match;
    if (std::regex_match(newValue, match, value_regex)) {
        val = match[1].str();
    }

    if (val == m_value) return;

    m_value = std::move(val);
    auto inScope = !m_value.empty();

    if (isFirst) isFirst = false;
    else os << ",";

    os << "{name=\"" << m_name << "\", proc_set=\"" << m_procset <<"\",";
    if (allValues && inScope) {
        os << "value=\"" << m_value << "\",";
    }
    os << "in_scope=\"" << (inScope ? "true" : "false")
       << R"(",type_changed="false"})";
}

// Create a new MI variable with name "inname" for a given expression.
// The frame element is unimplemented so it will act like "-"
std::string Vars::create(const std::string& inname, const std::string& frame,
                         const std::string& expression) {
    std::ostringstream os;

    // Follow the MI specification to generate a name if one isn't
    // given
    auto name = inname;
    if (name == "-") {
        std::ostringstream nm;
        nm << "var" << std::setfill('0') << std::setw(6) << m_nextVar++;
        name = nm.str();
    }
    //create variable for current procset
    auto ps = CurrentProcSet();

    // get the new current value
    auto value = GetMiValue(expression, ps->s_name);

    if (value.empty()) return "error,unable to evaluate";

    //get procset and remove it from value string
    std::string procset="";
    std::string val="";
    static std::regex value_regex(R"((\w+)\{?[\s\S]*\}?\:([\s\S]*))"); 
    std::smatch match;
    if (std::regex_match(value, match, value_regex)) {
        procset = match[1].str();
        val = match[2].str();
    }
    
    os << "ok,name=\"" << name << "\", proc_set=\"" << procset << R"(",numchild="0",in_scope="true", has_more="0", value=")"<< val << "\"";
    // and store it
    m_vars.insert( std::make_pair(name, Var{name, frame, expression, procset, std::move(val)}));

    return os.str();
}

void Vars::remove(const std::string& name) {
    m_vars.erase(name);
}

// Update one or all MI variables, and send the MI output to os
std::string Vars::update(const std::string& name, bool allValues) {
    auto os = std::ostringstream{};

    os << "ok,changelist=[";
    auto isFirst = true;
    if (name == "*") {
        for (auto& [name,var] : m_vars) {
            var.update(allValues, isFirst, os);
        }
    } else if (auto loc = m_vars.find(name); loc != m_vars.end()) {
        loc->second.update(allValues, isFirst, os);
    }
    os << "]";
    return os.str();
}

static int var_create_cmd(Command::LoweredContext<MiInterpreter>& ctx, rec_set_t*, int argc, const char* const argv[]) {
    // XXX: preserving old behavior
    auto args = ArgsToString(&argv[1], argc - 1);
    auto is = std::istringstream{args};
    std::string name, frame, expression;
    is >> name  >> frame >> std::quoted(expression);
    return handle_result_string(ctx.caller, miVars.create(name, frame, expression));
}

static int var_delete_cmd(Command::LoweredContext<MiInterpreter>& ctx, rec_set_t*, int, const char* const argv[]) {
    miVars.remove(argv[1]);
    return handle_result_string(ctx.caller, "ok");
}

static int var_update_cmd(Command::LoweredContext<MiInterpreter>& ctx, rec_set_t*, int argc, const char* const argv[]) {
    auto allValues = false;
    for (auto i = 1; i < argc; i++) {
        auto arg = std::string_view{argv[i]};
        if (arg == "--all-values") { allValues = true; }
        else if ( arg[0] == '-') { /* unhandled */ }
        else return handle_result_string(ctx.caller, miVars.update(std::string{arg}, allValues));
    }

    return handle_result_string(ctx.caller, "ok");
}

void MiCommandsFactory::insert_mi_commands(Command& root) {
    const auto lower = Command::specify<MiInterpreter>;

    root.addSubCmd({
        .name = "-exec-arguments", .ptype = proc_ignore, .fn = lower(exec_args_cmd) });
    root.addSubCmd({
        .name = "-var-create", .ptype = proc_ignore, .fn = lower(var_create_cmd)});
    root.addSubCmd({
        .name = "-var-delete", .ptype = proc_ignore, .fn = lower(var_delete_cmd)});
    root.addSubCmd({
        .name = "-var-update", .ptype = proc_ignore, .fn = lower(var_update_cmd)});
}
