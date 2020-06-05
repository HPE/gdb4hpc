/*********************************************************************************\
 * Stack frame commands.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2020 Cray Inc.
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
#include <vector>

#include "client.h"
#include "useful.h"
#include "bttree.h"

void PrintFrameSet(char *setname, bitset *group, dbgframe_t *f) {
    PrintMsgWithSetInfo(setname, group, "#%-2d ", f->frame_level);

    if (*f->frame_loc.loc_addr != '\0')
        PrintOutMsg("%s in ", f->frame_loc.loc_addr);

    PrintOutMsg("%s", f->frame_loc.loc_func);

    if (*f->frame_loc.loc_file != '\0')
        PrintOutMsg(" at %s:%d", f->frame_loc.loc_file, f->frame_loc.loc_line);

    PrintOutMsg("\n");
}

static int movecmd(rec_set_t *rec_set, int argc, char **argv) {
    int up = 0;
    int count = 1;
    dbgframe_t *f;

    if (*argv[0] == 'u') up++;

    if (argc >= 2) count = atoi(argv[1]);

    rep_set_t *rep_set;

    rep_set = DbgMoveFrame(rec_set, count, up);

    // process error
    ProcessErrorInRepSet(rep_set);

    // process frames
    for (auto&& reply : rep_set) {
        // Point at the frame data
        f = EVENT_FRAME(reply->rep_event);

        PrintFrameSet(reply->rep_di->d_handle, reply->rep_group, f);
    }

    FreeRepSet(rep_set);

    return 1;
}

static int cmd_backtrace_cmd(rec_set_t *rec_set, int argc, char **argv) {
    int count = 0;
    dbgframe_t *f;

    if (argc >= 2) count = atoi(argv[1]);

    rep_set_t *rep_set = DbgShowFrame(rec_set, count);

    // process error
    ProcessErrorInRepSet(rep_set);

    // turn backtraces into tree format ////////////////////////////////////////

    BTTree root;
    // root is a blank root node which serves to hold multiple trees.
    // it needs a dummy bitset for the recursive group bitset merging to work.
    root.allocateGroup(1);

    std::vector<BTTree> tracePool; // to hold information for cleaning up memory later

    for (auto&& reply : rep_set) {
        // reply sets hold stack frames in "reverse" (deepest first) order, while
        // we want to print them in most shallow (i.e. main) first order.
        BTTree path;
        BTTree child;
        bool available_child = false;
        
        for (f = EVENT_FRAME(reply->rep_event); f != (dbgframe_t *)NULL; f = f->frame_next) {
            path = BTTree(reply->rep_di->d_handle, reply->rep_group, f);

            if (available_child == true) {
                path.insertChild(child);
            }

            child = path;
            available_child = true;
        }

        tracePool.push_back(path); // save for freeing group bitsets later
        root.insertBacktrace(path); // insert into root tree and merge common paths
    }

    root.condenseTree();
    root.print();

    // clean up - each tree node had its own group bitset allocated.
    // the original pointers are still stored in tracePool.
    for (auto &path : tracePool) {
        path.freeTreeGroups();
    }
    root.freeGroup();
    FreeRepSet(rep_set);

    return 1;
}

static int cmd_frame_cmd(rec_set_t *rec_set, int argc, char **argv) {
    rep_set_t *rep_set;
    dbgframe_t *fp;

    rep_set = DbgGetFrame(rec_set);

    // process error
    ProcessErrorInRepSet(rep_set);

    // process frame - there should only be one
    for (auto&& reply : rep_set) {
        // Point at the frame data
        fp = EVENT_FRAME(reply->rep_event);

        PrintFrameSet(reply->rep_di->d_handle, reply->rep_group, fp);
    }

    FreeRepSet(rep_set);

    return 1;
}

void InitCmd_frame(void) {
    AddCommand("up", NULL, proc_required, 0, movecmd, cmd_up_help,
               "Move up one or more stack frames.");
    AddCommand("down", NULL, proc_required, 0, movecmd,
               cmd_down_help, "Move down one or more stack frames.");
    AddCommand("backtrace", "bt", proc_required, 0,
               cmd_backtrace_cmd, cmd_backtrace_help,
               "Print backtrace of all stack frames.");
    AddCommand("frame", NULL, proc_required, 0, cmd_frame_cmd,
               cmd_frame_help, "Print the currently selected stack frame.");
}
