/*********************************************************************************\
 * Source list command.
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

#include "client.h"
#include "useful.h"

static int cmd_list_cmd(rec_set_t *rec_set, int argc, char **argv) {
    char *s = NULL;
    dbglist_t *l;
    dbglist_t *lp;
    rep_set_t *rep_set = NULL;
    group_reply_t *reply;
    char *tmp_str;

    if (argc > 1) s = argv[1];

    rep_set = DbgList(rec_set, s);

    ProcessErrorInRepSet(rep_set);

    while ((reply = GetNextReplyInSet(rep_set)) != (group_reply_t *)NULL) {
        // point at the list data
        lp = EVENT_LIST(reply->rep_event);

        for (l = lp; l != (dbglist_t *)NULL; l = l->list_next) {
            // Replace any \\t or \\n with \t and "" respectively
            tmp_str = ReplaceStr(l->list_line, "\\t", "\t");
            // Free the old list_line
            Free(l->list_line);
            l->list_line = ReplaceStr(tmp_str, "\\n", "");
            // Free the tmp_str
            Free(tmp_str);
            PrintMsgWithSetInfo(reply->rep_di->d_handle, reply->rep_group,
                                "%d\t%s\n", l->list_lno, l->list_line);
        }
    }

    FreeRepSet(rep_set);

    return 1;
}

static int class_info_subcmd_sources_cmd(rec_set_t *rec_set, int argc,
                                         char **argv) {
    rep_set_t *rep_set;
    group_recv_t *recv_ptr;
    group_reply_t *reply;
    dbgsources_t *s;
    dbgsources_t *sp;
    int print_header;

    rep_set = DbgGetSources(rec_set);

    ProcessErrorInRepSet(rep_set);

    // Need to loop this over each di in the rec_set
    SetReceiverHeadInSet(rec_set);
    while ((recv_ptr = GetNextReceiverInSet(rec_set)) != NULL) {
        // reset print_header
        print_header = 1;

        // process replies
        SetReplyHeadInSet(rep_set);
        while ((reply = GetNextReplyInSet(rep_set)) != NULL) {
            if (recv_ptr->rec_di != reply->rep_di) {
                continue;
            }

            // Only want to print the header message once
            if (print_header) {
                PrintOutMsg("\nSource files for $%s:\n",
                            recv_ptr->rec_di->d_handle);
                print_header = 0;
            }

            // point at the sources data
            sp = EVENT_SOURCES(reply->rep_event);

            for (s = sp; s != NULL; s = s->sources_next) {
                PrintOutMsg("%s\n", s->sources_file);
            }
        }
    }

    FreeRepSet(rep_set);

    return 1;
}

void InitCmd_list(void) {
    // Regular commands
    AddCommand("list", "l", proc_required, 0, (int (*)())cmd_list_cmd,
               cmd_list_help, "List source for specified function or line.");

    // Info commands
    AddInfoCommand("sources", proc_required, 0,
                   (int (*)())class_info_subcmd_sources_cmd,
                   class_info_subcmd_sources_help,
                   "Display the source files in the application.");
}
