// mi_funcs.cpp - Command line interface routines.
// Copyright 2022 Hewlett Packard Enterprise Development LP.

#include "client.h"
#include "useful.h"
#include <map>
#include <regex>

std::string launcher_args;
std::string num_ranks;

static std::string exec_args_cmd(const char *);
static std::string file_exec_and_sym_cmd(const char *);
static std::string var_create_cmd(const char *);

typedef std::string (*fp)(const char *);

//map for commands that have to be parsed for cli command
static std::map<std::string, fp> func_cmd_map =
{
 {"-exec-arguments", &exec_args_cmd },
 {"-file-exec-and-symbols", &file_exec_and_sym_cmd},
 {"-var-create", &var_create_cmd}
};

//map for commands that have a default cli command
static std::map<std::string, std::string> return_cmd_map=
{
 {"-break-delete", "delete"},
 {"-break-insert", "break" },
 {"-break-list", "info breakpoints"},
 {"-enable-frame-filters", "ok"},
 {"-enable-pretty-printing", "ok"},
 {"-exec-continue", "continue"},
 {"-exec-next", "next"},
 {"-exec-run", "-exec-run"},
 {"-exec-step", "step"},
 {"-file-list-exec-source-file", "info source"},
 {"-file-list-exec-source-files", "info sources"},
 {"-gdb-exit", "quit"},
 {"-gdb-set", "set"},
 {"-gdb-show", "show"},
 {"-inferior-tty-set", "set inferior-tty"},
 {"-interpreter-exec", "interpreter-exec" },
 {"-list-features", "ok,features=[]"},
 {"-list-target-features", "ok,features=[]"}, 
 {"-special-launch", "launch $a{4} ./bd_dt/C++/cpp_bt.x" }, // temporary
 {"-stack-info-frame", "frame"},
 {"-stack-list-frames","backtrace"},
 {"-stack-list-variables", "info locals"},
 {"-thread-info", "info threads -v"},
 {"-thread-select", "thread" },
 {"-var-update", "ok"}
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
        if (args.empty()) return loc->second;
        else return loc->second + " " + args;
    } else if (auto loc = func_cmd_map.find(cmd); loc != func_cmd_map.end()) {
        return (*loc->second)(args.c_str());
    } else {
        mi_log("not a recognized MI command \"%s\"\n", cmd.c_str());
        throw cmd + ": not a recognized MI command";
    }
}

static std::string exec_args_cmd(const char * args){
    auto is = std::istringstream{args};
    auto token = std::string{};

    while (is >> token) {
        if (token == "-r") {
            is >> num_ranks;
        } else {
            if (!launcher_args.empty()) {
                launcher_args.append(" ");
            }
            launcher_args.append(token);
        }
    }
    launcher_args.erase(std::remove(launcher_args.begin(), launcher_args.end(), '\n'),
        launcher_args.end());
    //this command sets launcher args and does not need to be processed. Setting to exit will skip command processing
    return "ok";
}

static std::string file_exec_and_sym_cmd(const char * args){
    auto app = std::to_string(launch_count);
    if (!num_ranks.empty()){
        app.append("{");
        app.append(num_ranks);
        app.append("}");
    }
    launch_count++;

    auto func = std::string{};
    if (!launcher_args.empty()){
        return "launch $App"+app+" "+ args+ " -a \""+launcher_args+"\"";
    } else {
        return "launch $App"+app+" "+ args;
    }
}

static std::string var_create_cmd(const char *args){
    auto is = std::istringstream{args};
    auto str2 = std::string{};
    // ignoring the first two arguments
    is >> str2 >> str2 >> str2;
    return "print "+ str2.substr(1,str2.length()-3);
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
