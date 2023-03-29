/*********************************************************************************\
 * Thread commands.
 *
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <algorithm>
#include <regex>

#include "client.h"
#include "useful.h"

static int cmd_thread_cmd(rec_set_t *rec_set, int argc, char **argv) {

    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: thread <thread-number> or thread name <thread-name>,\n");
        PrintOutError("  or thread find <pattern> or thread apply <operation>.\n");
        return 1;
    }

    if ((argc == 3 || argc == 2) && strcmp(argv[1],"name") == 0) {
        rep_set_t* rep_set = DbgSetThreadName(rec_set, argc == 3 ? argv[2] : "");
        ProcessErrorInRepSet(rep_set);
        FreeRepSet(rep_set);
        return 1;

    } else if (argc >= 3 && strcmp(argv[1],"find") == 0) {
        auto arg = ArgsToString(&argv[2], argc - 2);
        rep_set_t* rep_set = DbgFindThread(rec_set, arg.c_str());
        ProcessErrorInRepSet(rep_set);

        // process replies
        for (auto&& reply : rep_set) {
            auto handle = reply->rep_di.c_str();
            const auto group = reply->rep_group.get();

            const char* str = reply->rep_event->ev_u.ev_u_string;
            std::istringstream is(str);
            std::string line;
            while (std::getline(is,line)) {
                PrintMsgWithSetInfo(handle, group, "%s\n", line.c_str());
            }
        }

        FreeRepSet(rep_set);
        return 1;
    } else if (argc >= 3 && strcmp(argv[1],"apply") == 0) {
        std::string arg = ArgsToString(&argv[2], argc - 2);
        rep_set_t* rep_set = DbgThreadApply(rec_set,arg.c_str());
        ProcessErrorInRepSet(rep_set);

        // process replies
        for (auto&& reply : rep_set) {
            auto handle = reply->rep_di.c_str();
            const auto group = reply->rep_group.get();

            const char* str = reply->rep_event->ev_u.ev_u_string;
            std::istringstream is(str);
            std::string line;
            while (std::getline(is,line)) {
                PrintMsgWithSetInfo(handle, group, "%s\n", line.c_str());
            }
        }

        FreeRepSet(rep_set);
        return 1;
    } else if (argc > 2) {
        PrintOutError("Too many command arguments.\n");
        PrintOutError("Expected: thread 5\n");
        return 0;
    } else {
        const char* arg = argv[1];
        rep_set_t *rep_set = DbgSetThread(rec_set, arg);

        ProcessErrorInRepSet(rep_set);

        FreeRepSet(rep_set);

        return 1;
    }
}

// shortcut for thread apply all -s
static int cmd_taas_cmd(rec_set_t *rec_set, int argc, char **argv) {
    if (argc < 1) return 1;
    std::vector<const char*> newArgs{"thread", "apply", "all", "-s"};
    std::copy(argv+1, argv+argc, std::back_inserter(newArgs));
    return cmd_thread_cmd(rec_set, newArgs.size(), const_cast<char**>(&newArgs[0]));
}

static void PrintMIThread(const std::string& line,
                          std::ostringstream& ss, std::string& current_thread_id) {

    auto stoppedRE = std::regex{R"((\*)?\s+([- 0-9]*\d)\s[^"]*["]([^"]*)["]\s*(\w+).*\sat\s([^:]*):(\d+))"};
    auto runningRE = std::regex{R"((\*)?\s+([- 0-9]*\d)\s[^"]*["]([^"]*)["]\s*\(running\))"};
    // ccdb uses the frame address to match the backtrace to the current thread,
    // create a dummy frame address tag is this the current thread and set
    // current_thread_id
    auto frameAddr = [&current_thread_id] (const std::smatch& match) {
        if (!match[1].length()) return "";
        current_thread_id = match[2].str();
        return mi_dummyFrameAddr;
    };

    std::string word;
    std::string id="id=\"";
    std::string target_id="target-id=\"";
    std::string addr="";
    std::string func="func=\"";
    std::string file="file=\"";
    std::string atLine="line=\"";
    std::string thread_state="state=\"";

    auto match = std::smatch{};
    if (std::regex_match(line, match, stoppedRE)) {
        id += match[2];
        target_id += match[3];
        func += match[4];
        file += match[5];
        atLine += match[6];
        addr = frameAddr(match);
        thread_state += "break";
    } else if (std::regex_match(line, match, runningRE)) {
        id += match[2];
        target_id += match[3];
        thread_state += "running";
        frameAddr(match); // just setting current thread
    } else {
        // I hope this was the header
        return;
    }

    if (ss.tellp() == std::streampos()) ss << "[";
    else ss << ",";

    ss << "{"<<id << "\","
       << target_id
       << "\",frame={"
       << addr
       << func << "\","
       << file << "\","
       << atLine << "\"},"
       << thread_state<<"\"}";
}


static int cmd_info_threads_cmd(rec_set_t *rec_set, int argc,
                                         char **argv) {

    if (Client.MI_mode && !rec_set) {
        PrintOutOk();
        return 1;
    }

    // info threads if typically called with no arguments but it
    // does take an optional list of thread ids
    std::string arg = ArgsToString(&argv[1], argc - 1);
    rep_set_t* rep_set = DbgGetThreadInfo(rec_set, arg.c_str());

    ProcessErrorInRepSet(rep_set);

    std::ostringstream ss;
    std::string current_thread_id="current-thread-id=\"";

    // process replies
    for (auto&& reply : rep_set) {
        auto handle = reply->rep_di.c_str();
        const auto group = reply->rep_group.get();

        const char* str = reply->rep_event->ev_u.ev_u_string;

        std::istringstream is(str);
        std::string line;

        while (std::getline(is,line)) {
            if (Client.MI_mode){
                PrintMIThread(line, ss, current_thread_id);
            } else{
                PrintMsgWithSetInfo(handle, group, "%s\n", line.c_str());
            }
        }
        // add a blank line to make the output more understandable
        if (!Client.MI_mode) {
            PrintMsgWithSetInfo(handle, group, "\n");
        }

        if (Client.MI_mode && !ss.str().empty()){
            ss<<"],"<<current_thread_id<<"\"";
            PrintOutMIResult(MIResult::DONE, "proc_set=\"%s\",threads=%s\n%s",handle,ss.str().c_str(),
                             GetMIEpilogue());
        }
    }

    FreeRepSet(rep_set);

    return 1;
}

static int cmd_set_scheduler_locking_cmd(rec_set_t *rec_set, int argc, char **argv)
{
    if (argc != 2) {
        PrintOutError("Wrong number of command arguments, expected: off|on|step|replay.\n");
        return 1;
    }
    static const std::array<std::string,4> vals = { "off", "on", "step", "replay" };
    if (std::find(vals.begin(), vals.end(), argv[1]) == vals.end()) {
        PrintOutError("Unknown mode %s\n", argv[1]);
        return 1;
    }

    rep_set_t* rep_set = DbgSetSchedulerLocking(rec_set, argv[1]);
    ProcessErrorInRepSet(rep_set);
    FreeRepSet(rep_set);
    return 1;
}

static int cmd_show_scheduler_locking_cmd(rec_set_t *rec_set, int argc, char **argv)
{
    if (argc != 1) {
        PrintOutError("Unexpected command arguments.\n");
        return 1;
    }

    rep_set_t* rep_set = DbgGetSchedulerLocking(rec_set);
    ProcessErrorInRepSet(rep_set);

    for (auto&& reply : rep_set) {
        char* mode = reply->rep_event->ev_u.ev_u_string;
        PrintMsgWithSetInfo(reply->rep_di.c_str(), reply->rep_group.get(),
                            "%s\n", mode);
    }


    FreeRepSet(rep_set);
    return 1;
}

// enable/disable non-stop mode which lets other threads in a process continue
// running instead of stopping every thread
static int cmd_set_non_stop_cmd(rec_set_t *rec_set, int argc, char **argv)
{
    if (argc != 2) {
        PrintOutError("Wrong number of command arguments, expected: off|on.\n");
        return 1;
    }

    bool isNonStop = false;
    if (strcmp(argv[1],"off") == 0 || strcmp(argv[1], "0") == 0) {
        isNonStop = false;
    } else if (strcmp(argv[1],"on") == 0 ||  strcmp(argv[1], "1") == 0) {
        isNonStop = true;
    } else {
        PrintOutError("Unknown mode %s\n", argv[1]);
        return 1;
    }

    // note that the usual gdb back-ends do not support changing the non-stop
    // mode once the process is running, so we set a flag to set the
    // mode during the next launch.
    Client.non_stop = isNonStop;

    // however, some back end *might* support changing the mode either now
    // or in the future - so we'll just give that try.   If it's not supported
    // the user will get an error message pointing him toward calling it before
    // running.
    if (rec_set) {
        rep_set_t* rep_set = DbgSetNonStop(rec_set, argv[1]);
        ProcessErrorInRepSet(rep_set);
        if (SetHasError(rep_set)) {
            // The gdb command is in terms of an inferior or process, add a line
            // to clarify it for the gdb4hpc context
            PrintOutMsg("non-stop mode will be applied on future \"launch\" commands\n");
        }
        FreeRepSet(rep_set);
    }
    PrintOutOk();
    return 1;
}

// on means other threads can continue to run when one is stopped
static int cmd_show_non_stop_cmd(rec_set_t *rec_set, int argc, char **argv)
{
    if (argc != 1) {
        PrintOutError("Unexpected command arguments.\n");
        return 1;
    }

    if (rec_set) {
        rep_set_t* rep_set = DbgGetNonStop(rec_set);
        ProcessErrorInRepSet(rep_set);

        for (auto&& reply : rep_set) {
            char* mode = reply->rep_event->ev_u.ev_u_string;
            PrintMsgWithSetInfo(reply->rep_di.c_str(), reply->rep_group.get(),
                                "non-stop mode is %s\n", mode);
        }

        FreeRepSet(rep_set);
    } else {
        PrintOutMsg("non-stop mode is %s\n", Client.non_stop ? "on" : "off");
    }

    return 1;
}

// command to interface with the ompd plugin
static int cmd_ompd_cmd(rec_set_t *rec_set, int argc, char **argv) {

    auto arg = ArgsToString(&argv[1], argc - 1);
    auto rep_set = DbgOmpd(rec_set, arg.c_str());
    ProcessErrorInRepSet(rep_set);

    // process replies
    for (auto&& reply : rep_set) {
        auto handle = reply->rep_di.c_str();
        const auto group = reply->rep_group.get();

        if (reply->rep_event->ev_event == DBGEV_THREADINFO) {
            const char* str = reply->rep_event->ev_u.ev_u_string;
            std::istringstream is(str);
            std::string line;
            while (std::getline(is,line)) {
                PrintMsgWithSetInfo(handle, group, "%s\n", line.c_str());
            }
        }
    }

    FreeRepSet(rep_set);
    return 1;
}

// command to interface with the cuda-gdb extensions
static int cmd_cuda_cmd(rec_set_t *rec_set, int argc, char **argv) {

    auto arg = ArgsToString(&argv[1], argc - 1);
    auto rep_set = DbgCuda(rec_set, arg.c_str());
    ProcessErrorInRepSet(rep_set);

    // process replies
    for (auto&& reply : rep_set) {
        auto handle = reply->rep_di.c_str();
        const auto group = reply->rep_group.get();

        if (reply->rep_event->ev_event == DBGEV_THREADINFO) {
            const char* str = reply->rep_event->ev_u.ev_u_string;
            std::istringstream is(str);
            std::string line;
            while (std::getline(is,line)) {
                PrintMsgWithSetInfo(handle, group, "%s\n", line.c_str());
            }
        }
    }

    FreeRepSet(rep_set);
    return 1;
}

static int cmd_info_cuda_cmd(rec_set_t *rec_set, int argc, char **argv) {
    auto args = ArgvStream{"cuda", "info"};
    args << ArgvStream{argc-1, argv+1};
    return cmd_cuda_cmd(rec_set, args.argc(), const_cast<char**>(args.c_strs().data()));
    
}

static int cmd_set_cuda_cmd(rec_set_t *rec_set, int argc, char **argv) {
    auto args = ArgvStream{"cuda", "set"};
    args << ArgvStream{argc-1, argv+1};
    return cmd_cuda_cmd(rec_set, args.argc(), const_cast<char**>(args.c_strs().data()));
    
}

static int cmd_show_cuda_cmd(rec_set_t *rec_set, int argc, char **argv) {
    auto args = ArgvStream{"cuda", "show"};
    args << ArgvStream{argc-1, argv+1};
    return cmd_cuda_cmd(rec_set, args.argc(), const_cast<char**>(args.c_strs().data()));
    
}


void InitCmd_thread(void) {
    AddCommand("thread", "t", proc_required, 0, cmd_thread_cmd,
               cmd_thread_help, "Thread control functions.");
    AddCommand("taas", "", proc_required, 0, cmd_taas_cmd,
               cmd_thread_help, nullptr);
    AddCommand("ompd", "", proc_required, 0, cmd_ompd_cmd,
               cmd_ompd_help, "OpenMP debugging functions.");
    AddCommand("cuda", "", proc_required, 0, cmd_cuda_cmd,
               cmd_cuda_help, "Cuda debugging functions.");
    AddInfoCommand("cuda", proc_required, 0, cmd_info_cuda_cmd,
                   cmd_info_cuda_help,
                   "Display cuda specific information.");
    AddSetCommand("cuda", proc_required, 0, cmd_set_cuda_cmd,
                  cmd_set_cuda_help,
                  "Set cuda specific information.");
    AddShowCommand("cuda", proc_required, 0, cmd_show_cuda_cmd,
                   cmd_show_cuda_help,
                   "Show cuda specific settings.");


    AddInfoCommand("threads", proc_optional_mi, 0,
                   cmd_info_threads_cmd,
                   cmd_info_threads_help,
                   "List currently known threads.");

    AddSetCommand("scheduler-locking", proc_required, 0,
                  cmd_set_scheduler_locking_cmd,
                  cmd_set_scheduler_locking_help,
                  "Set mode for locking scheduler during execution.");
    AddShowCommand("scheduler-locking", proc_required, 0,
                   cmd_show_scheduler_locking_cmd,
                   cmd_show_scheduler_locking_help,
                   "Show mode for locking scheduler during execution.");

    AddSetCommand("non-stop", proc_optional, 0,
                  cmd_set_non_stop_cmd,
                  cmd_set_non_stop_help,
                  "Set mode for thread stopping during execution.");
    AddShowCommand("non-stop", proc_optional, 0,
                   cmd_show_non_stop_cmd,
                   cmd_show_non_stop_help,
                   "Show mode for thread stopping during execution.");
}
