/*********************************************************************************\
 * Thread commands.
 *
 * Copyright 2020 Cray Inc.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
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
    } else if (argc == 3 && strcmp(argv[1],"find") == 0) {
        rep_set_t* rep_set = DbgFindThread(rec_set, argv[2]);
        ProcessErrorInRepSet(rep_set);

        // process replies
        for (auto&& reply : rep_set) {
            auto handle = reply->rep_di->d_handle;
            auto group = reply->rep_group;
            
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
        std::string arg = ArgsToString(argc-2,argv+2);
        if (RecSetHasOneProcPerReceiver(rec_set)) {
            // We try not to lose any information when you are focused on a
            // single rank.    Taking advantage of the server side not being
            // too particular about where the -v option is on the line..
            arg = "-v " + arg;
        }

        rep_set_t* rep_set = DbgThreadApply(rec_set,arg.c_str());
        ProcessErrorInRepSet(rep_set);

        // process replies
        for (auto&& reply : rep_set) {
            auto handle = reply->rep_di->d_handle;
            auto group = reply->rep_group;
            
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


static int class_info_subcmd_threads_cmd(rec_set_t *rec_set, int argc,
                                         char **argv) {

    // info threads if typically called with no arguments but it
    // does take an optional list of thread ids
    std::string arg;
    for (int i=1; i<argc; ++i) {
        if (i > 1)
            arg += " ";
        arg += argv[i];
    }

    // print out address informtation if we are focuses on a single process.  In that case,
    // the actual memory addresses are useful and combining results is no an issue.
    if (RecSetHasOneProcPerReceiver(rec_set)) {
        arg += " -v";
    }

    rep_set_t* rep_set = DbgGetThreadInfo(rec_set, arg.c_str());

    ProcessErrorInRepSet(rep_set);

    // process replies
    for (auto&& reply : rep_set) {
        auto handle = reply->rep_di->d_handle;
        auto group = reply->rep_group;

        const char* str = reply->rep_event->ev_u.ev_u_string;
        std::istringstream is(str);
        std::string line;
        while (std::getline(is,line)) {
            PrintMsgWithSetInfo(handle, group, "%s\n", line.c_str());
        }
        // add a blank line to make the output more understandable
        PrintMsgWithSetInfo(handle, group, "\n");
    }

    FreeRepSet(rep_set);
    
    return 1;
}

void InitCmd_thread(void) {
    AddCommand("thread", "t", proc_required, 0, cmd_thread_cmd,
               cmd_thread_help, "Thread control functions.");
    AddCommand("taas", "", proc_required, 0, cmd_taas_cmd,
               cmd_thread_help, nullptr);

    AddInfoCommand("threads", proc_required, 0,
                   class_info_subcmd_threads_cmd,
                   class_info_subcmd_threads_help,
                   "List currently known threads.");
}
