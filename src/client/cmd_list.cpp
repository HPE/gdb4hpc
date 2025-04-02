/*********************************************************************************\
 * Source list command.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2024 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/

#include "config.h"

#include "client.h"
#include "useful.h"
#include "interpreter_mi.h"

static int cmd_list_cmd(rec_set_t *rec_set, int argc, const char* const argv[]) {
    const char *s = nullptr;
    dbglist_t *l;
    dbglist_t *lp;

    if (argc > 1) s = argv[1];

    auto rep_set = DbgList(rec_set, s);

    ProcessErrorInRepSet(rep_set);

    for (auto&& reply : rep_set) {
        // XXX: PE-52386: DbgList can sometimes return non-error, non-list events
        if (reply->rep_event->ev_event == DBGEV_LIST) {
            // point at the list data
            lp = EVENT_LIST(reply->rep_event);

            for (l = lp; l != (dbglist_t *)nullptr; l = l->list_next) {
                auto line = std::string{l->list_line};
                formatSourceLine(line);
                // We use 5 characters of space for the line number, plus a tab.
                // We add a tab on the end to ensure that source code lines start
                // printing on a tab boundary, in case the source code contains tabs itself.
                PrintMsgWithSetAndThreadInfo(reply->rep_di.c_str(), reply->rep_group.get(),
                                             l->list_threads,
                                             "%-5d\t%s\n", l->list_lno, line.c_str());
            }
        }
    }

    return Command::Success;
}

static int cmd_info_sources_cmd_console(rec_set_t *rec_set, int,
                                         const char * const []) {
    if (!rec_set) return Command::Success;

    auto rep_set = DbgGetSources(rec_set);

    ProcessErrorInRepSet(rep_set);

    // Need to loop this over each di in the rec_set
    for (auto&& recv_ptr : rec_set) {
        auto print_header = true;

        // process replies
        for (auto&& reply : rep_set) {
            std::stringstream ss;
            if (recv_ptr->rec_di.handle() != reply->rep_di) {
                continue;
            }
            // Only want to print the header message once
            if (print_header) {
                PrintOutMsg("\nSource files for $%s:\n",
                            recv_ptr->rec_di.handle().c_str());
                print_header = false;
            }

            // point at the sources data
            auto sp = EVENT_SOURCES(reply->rep_event);

            for (auto s = sp; s != nullptr; s = s->sources_next) {
                PrintOutMsg("%s\n", s->sources_file);
            }
        }
    }

    return Command::Success;
}

static int cmd_info_sources_cmd_mi(MiInterpreter& mi, rec_set_t *rec_set, int, const char * const []) {
    if (!rec_set) return Command::Success;

    auto rep_set = DbgGetSources(rec_set);

    ProcessErrorInRepSet(rep_set);

    // Need to loop this over each di in the rec_set
    for (auto&& recv_ptr : rec_set) {
        auto print_header = true;

        // process replies
        for (auto&& reply : rep_set) {
            std::stringstream ss;
            if (recv_ptr->rec_di.handle() != reply->rep_di) {
                continue;
            }
            // Only want to print the header message once
            if (print_header) {
                PrintOutMsg("\nSource files for $%s:\n",
                            recv_ptr->rec_di.handle().c_str());
                print_header = false;
            }

            // point at the sources data
            auto sp = EVENT_SOURCES(reply->rep_event);

            for (auto s = sp; s != nullptr; s = s->sources_next) {
                if (!ss.str().empty()){
                    ss<<",";
                }else{
                    ss<<"[";
                }

                ss<<"{file=\""<<s->sources_file<<"\",fullname=\""<<s->sources_file<<"\"}";

                //save fullpath to source list
                recv_ptr->rec_di.sourcePaths().storeSourcePath(s->sources_file);
            }
            if (ss.str().empty()) ss << "[";
            ss<<"]";
            mi.set_result("proc_set=\"%s\",files=%s",
                             recv_ptr->rec_di.handle().c_str(),ss.str().c_str());
        }
    }

    return Command::Success;
}

static int cmd_info_source_cmd_console(rec_set_t *rec_set) {
    if (!rec_set) return Command::Success;

    auto rep_set = DbgGetFrame(rec_set, false);
    ProcessErrorInRepSet(rep_set);

    for (auto&& reply : rep_set) {
        // Point at the frame data
        auto fp = EVENT_FRAME(reply->rep_event);
        auto loc = fp->getLoc();
        PrintMsgWithSetInfo(reply->rep_di.c_str(), reply->rep_group.get(),"In file %s.\n",
                            loc.loc_file);
    }

    return Command::Success;
}

static int cmd_info_source_cmd_mi(MiInterpreter& mi, rec_set_t *rec_set) {
    if (!rec_set) return Command::Success;

    auto rep_set = DbgGetFrame(rec_set, false);
    ProcessErrorInRepSet(rep_set);

    auto reply = rep_set.getFirstReply();
    auto f = EVENT_FRAME(reply->rep_event);
    auto loc = f->getLoc();
    std::string group = reply->rep_group.get()->toSetString();
    mi.set_result(
        "proc_set=\"%s\",group=\"%s\",line=\"%d\",file=\"%s\",fullname=\"%s\"",
        reply->rep_di.name().c_str(),
        group.substr(1, group.length() - 2).c_str(), loc.loc_line, loc.loc_file,
        loc.loc_file);

    return Command::Success;
}

void ConsoleCommandsFactory::insert_list_commands(CommandNode& root) {
    root.addSubCmd("list", "l", proc_required, cmd_list_cmd,
               cmd_list_help, "List source for specified function or line.");

    root.addSubCmd("info source", nullptr, proc_required,
                   cmd_info_source_cmd_console,
                   cmd_info_source_help,
                   "Display information about the current source file.");

    root.addSubCmd("info sources", nullptr, proc_required,
                   cmd_info_sources_cmd_console,
                   cmd_info_sources_help,
                   "Display the source files in the application.");
}

void MiCommandsFactory::insert_list_commands(CommandNode& root) {
    root.addSubCmd({
        .name = "-file-list-exec-source-file", .ptype = proc_required,
        .fn = cmd_info_source_cmd_mi });
    root.addSubCmd({
        .name = "-file-list-exec-source-files", .ptype = proc_required,
        .fn = cmd_info_sources_cmd_mi });
}
