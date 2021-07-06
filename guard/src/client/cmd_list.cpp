/*********************************************************************************\
 * Source list command.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2020 Hewlett Packard Enterprise Development LP.
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

#include "client.h"
#include "useful.h"

static int cmd_list_cmd(rec_set_t *rec_set, int argc, char **argv) {
    char *s = NULL;
    dbglist_t *l;
    dbglist_t *lp;
    rep_set_t *rep_set = NULL;
    char *tmp_str;

    if (argc > 1) s = argv[1];

    rep_set = DbgList(rec_set, s);

    ProcessErrorInRepSet(rep_set);

    for (auto&& reply : rep_set) {
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
            PrintMsgWithSetAndThreadInfo(reply->rep_di->d_handle, reply->rep_group.get(),
                                         l->list_threads,
                                         "%d\t%s\n", l->list_lno, l->list_line);
        }
    }

    FreeRepSet(rep_set);

    return 1;
}

static int class_info_subcmd_sources_cmd(rec_set_t *rec_set, int argc,
                                         char **argv) {
    if (!rec_set) return 1;

    auto rep_set = DbgGetSources(rec_set);

    ProcessErrorInRepSet(rep_set);

    // Need to loop this over each di in the rec_set
    for (auto&& recv_ptr : rec_set) {
        auto print_header = true;

        // process replies
        for (auto&& reply : rep_set) {
            if (recv_ptr->rec_di != reply->rep_di) {
                continue;
            }

            // Only want to print the header message once
            if (print_header) {
                PrintOutMsg("\nSource files for $%s:\n",
                            recv_ptr->rec_di->d_handle);
                print_header = false;
            }

            // point at the sources data
            auto sp = EVENT_SOURCES(reply->rep_event);

            for (auto s = sp; s != nullptr; s = s->sources_next) {
                PrintOutMsg("%s\n", s->sources_file);
            }
        }
    }

    FreeRepSet(rep_set);

    return 1;
}

void InitCmd_list(void) {
    // Regular commands
    AddCommand("list", "l", proc_required, 0, cmd_list_cmd,
               cmd_list_help, "List source for specified function or line.");

    // Info commands
    AddInfoCommand("sources", proc_required, 0,
                   class_info_subcmd_sources_cmd,
                   class_info_subcmd_sources_help,
                   "Display the source files in the application.");
}
