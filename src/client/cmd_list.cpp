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
#include <string.h>

#include "client.h"
#include "useful.h"

static int cmd_list_cmd(rec_set_t *rec_set, int argc, char **argv) {
    char *s = nullptr;
    dbglist_t *l;
    dbglist_t *lp;
    rep_set_t *rep_set = nullptr;

    if (argc > 1) s = argv[1];

    rep_set = DbgList(rec_set, s);

    ProcessErrorInRepSet(rep_set);

    for (auto&& reply : rep_set) {
        // point at the list data
        lp = EVENT_LIST(reply->rep_event);

        for (l = lp; l != (dbglist_t *)nullptr; l = l->list_next) {
            auto line = std::string{l->list_line};
            formatSourceLine(line);
            // We use 5 characters of space for the line number, plus a tab.
            // We add a tab on the end to ensure that source code lines start
            // printing on a tab boundary, in case the source code contains tabs itself.
            PrintMsgWithSetAndThreadInfo(reply->rep_di->d_handle, reply->rep_group.get(),
                                         l->list_threads,
                                         "%-5d\t%s\n", l->list_lno, line.c_str());
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

    std::stringstream ss;
    // Need to loop this over each di in the rec_set
    for (auto&& recv_ptr : rec_set) {
        auto print_header = true;

        // process replies
        for (auto&& reply : rep_set) {
            if (recv_ptr->rec_di != reply->rep_di) {
                continue;
            }
            // Only want to print the header message once
            if (print_header && !Client.MI_mode) {
                PrintOutMsg("\nSource files for $%s:\n",
                            recv_ptr->rec_di->d_handle);
                print_header = false;
            }

            PrintOutMsg("\nSource files for $%s:\n",
                            recv_ptr->rec_di->d_handle);

            // point at the sources data
            auto sp = EVENT_SOURCES(reply->rep_event);

            for (auto s = sp; s != nullptr; s = s->sources_next) {
                if (!Client.MI_mode){
                    PrintOutMsg("%s\n", s->sources_file);
                }else{
                    if (!ss.str().empty()){
                        ss<<",";
                    }else{
                        ss<<"[";
                    }
                    
                    //create message to send back to gui
                    ss<<"{proc_set=\""<<recv_ptr->rec_di->d_handle<<"\",file=\""<<s->sources_file<<"\",fullname=\""<<s->sources_file<<"\"}";
                    
                    //save fullpath to source list
                    recv_ptr->rec_di->source_paths.storeSourcePath(s->sources_file);
                }
            }
        }
    }
    if (Client.MI_mode && !ss.str().empty()){
        ss<<"]";
        PrintOutMsg("^done,files=%s",ss.str().c_str());
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
