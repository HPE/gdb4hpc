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

#include "client.h"
#include "useful.h"

static int cmd_thread_cmd(rec_set_t *rec_set, int argc, char **argv) {

    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: thread 5\n");
        return 1;
    }

    if (argc > 2) {
        PrintOutError("Too many command arguments.\n");
        PrintOutError("Expected: thread 5\n");
    }

    const char* arg = argv[1];
    rep_set_t *rep_set = DbgSetThread(rec_set, arg);

    ProcessErrorInRepSet(rep_set);

    FreeRepSet(rep_set);
    
    return 1;
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

    rep_set_t* rep_set = DbgGetThreadInfo(rec_set, arg.c_str());

    ProcessErrorInRepSet(rep_set);

    // Need to loop this over each di in the rec_set
    SetReceiverHeadInSet(rec_set);
    while (auto recv_ptr = GetNextReceiverInSet(rec_set)) {
        bool printHeader = true;

        // process replies
        for (auto&& reply : rep_set) {
            if (recv_ptr->rec_di != reply->rep_di) {
                continue;
            }

            // Only want to print the header message once
            if (printHeader) {
                PrintOutMsg("\nThreads for $%s:\n",
                            recv_ptr->rec_di->d_handle);
                printHeader = false;
            }

            // point at the sources data
            const char* str = reply->rep_event->ev_u.ev_u_string;
            PrintOutMsg(str);
        }
    }

    FreeRepSet(rep_set);
    
    return 1;
}

void InitCmd_thread(void) {
    AddCommand("thread", "t", proc_required, 0, (int (*)())cmd_thread_cmd,
               cmd_thread_help, "switch to specified thread.");

    AddInfoCommand("threads", proc_required, 0,
                   (int (*)())class_info_subcmd_threads_cmd,
                   class_info_subcmd_threads_help,
                   "List currently known threads.");
}
