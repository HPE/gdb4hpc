// mi_funcs.cpp - Command line interface routines.
// Copyright 2022-2023 Hewlett Packard Enterprise Development LP.

#include "client.h"
#include "useful.h"
#include <map>
#include <regex>

const char* mi_dummyFrameAddr = "addr=\"0xf00f\",";

static std::string launcher_args;
static std::string num_ranks;
static std::string procset;

static std::string exec_args_cmd(const char *);
static std::string file_exec_and_sym_cmd(const char *);
static std::string var_create_cmd(const char *);
static std::string var_delete_cmd(const char *);
static std::string var_update_cmd(const char *);

typedef std::string (*fp)(const char *);

//map for commands that have to be parsed for cli command
static std::map<std::string, fp> func_cmd_map =
{
 {"-exec-arguments", &exec_args_cmd },
 {"-file-exec-and-symbols", &file_exec_and_sym_cmd},
 {"-var-create", &var_create_cmd},
 {"-var-delete", &var_delete_cmd},
 {"-var-update", &var_update_cmd}
};

//map for commands that have a default cli command
static std::map<std::string, std::string> return_cmd_map=
{
 {"-break-after", "ignore"},
 {"-break-condition", "condition"},
 {"-break-delete", "delete"},
 {"-break-disable", "disable" },
 {"-break-enable", "enable" },
 {"-break-insert", "break" },
 {"-compare", "compare"},
 {"-data-list-register-names", "ok,register-names=[]"},
 {"-enable-frame-filters", "ok"},
 {"-exec-continue", "continue"},
 {"-exec-return", "finish"},
 {"-exec-run", "run"},
 {"-gdb-exit", "quit"},
 {"-list-features", "ok,features=[]"},
 {"-list-target-features", "ok,features=[async]"}, 
};

int launch_count=0;

std::string convertMiToCli(const std::string& cmdLine) {
    mi_log("input mi: \"%s\"\n", cmdLine.c_str());

    static std::regex cmdRE(R"((\d*)(-[^ "\n]+) ?(.*)\n?)");
    std::smatch match;
    if (!std::regex_match(cmdLine, match, cmdRE)) {
        return cmdLine;
    }

    auto tag = match[1].str(), cmd = match[2].str(), args = match[3].str();

    cli_set_prefix(tag);

    if (auto loc = return_cmd_map.find(cmd); loc != return_cmd_map.end()) {
        if (args.empty() || loc->second.starts_with("ok")) return loc->second;
        else return loc->second + " " + args;
    } else if (auto loc = func_cmd_map.find(cmd); loc != func_cmd_map.end()) {
        return (*loc->second)(args.c_str());
    } else {
        mi_log("not a recognized MI command \"%s\"\n", cmd.c_str());
        throw cmd + ": not a recognized MI command";
    }
}

static std::string exec_args_cmd(const char * in_args){
    static std::regex procRE{R"((\$\w+)\{(\d+)\})"};

    ArgvStream args;
    args.parseAndAppendArgs(in_args);
    
    std::ostringstream argstr;
    num_ranks = "1";
    procset = "";
    std::smatch match;

    // recognize -n <num-ranks> -p <proc-set-name> or
    // $<proc-set-name>{<num-ranks>} and pass any other arguments to gdb4hpc
    // this will mean they need to use --args and/or --launcher-args
    for (size_t argc=args.argc(), i=0; i<argc; ++i) {
        auto arg = args[i];
        if (arg == "-n" && i+1 < argc) {
            num_ranks = args[++i];
        } else if (std::regex_match(arg, match, procRE)) {
            procset = match[1];
            num_ranks = match[2];
        } else {
            argstr << " " << arg;
        }
    }
                
    launcher_args = argstr.str() + " ";
    //this command sets launcher args and does not need to be processed. Setting to exit will skip command processing
    return "ok";
}

static std::string file_exec_and_sym_cmd(const char * args){
    //check a procset was received
    if (procset.empty()){
        // create a default if one wasn't specified
        procset = "$App";
    }
    
    auto app=procset;
    //add rank information if available
    if (!num_ranks.empty()){
        app.append("{");
        app.append(num_ranks);
        app.append("}");
    }

    auto func = std::string{};

    return "launch " + app + launcher_args + args;
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

    if (value.empty()) return "error,unable to evaluate\n";

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

static std::string var_create_cmd(const char *args){
    auto is = std::istringstream{args};
    std::string name, frame, expression;
    is >> name  >> frame >> std::quoted(expression);
    return miVars.create(name, frame, expression);
}

static std::string var_delete_cmd(const char *args) {
    auto is = std::istringstream{args};
    std::string name;
    is >> name;
    miVars.remove(name);
    return "ok";
}

static std::string var_update_cmd(const char *args) {
    auto is = std::istringstream{args};
    auto arg = std::string{};

    auto allValues = false;
    while (is >> arg) {
        if (arg == "--all-values") { allValues = true; }
        else if ( arg[0] == '-') { /* unhandled */ }
        else return miVars.update(arg, allValues);
    }
    return "ok";
}
        
