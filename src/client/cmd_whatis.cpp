/*********************************************************************************\
 * Whatis command.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2023 Hewlett Packard Enterprise Development LP.
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

#include "client.h"
#include "useful.h"
#include "interpreter_mi.h"

static int cmd_whatis_cmd(Command::RunContext&, rec_set_t *rec_set, int argc, const char* const argv[]) {
    int i;
    int argp = 1;
    int isType = 0;
    int len = 0;
    char *type_str;

    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: whatis var\n");
        return Command::Failure;
    }

    if (strcmp(argv[argp], "-type") == 0) {
        if (++argp == argc) {
            PrintOutError("Error: -type provided, but missing arguments.\n");
            return Command::Failure;
        }

        isType = 1;
    }

    // create a new string containing all of the argv
    for (i = argp; i < argc; ++i) {
        len += strlen(argv[i]);
        // add one for the ' '
        ++len;
    }
    type_str = (char *)Alloc(len + 1);
    memset(type_str, '\0', len + 1);

    for (i = argp; i < argc; ++i) {
        strncat(type_str, argv[i], len);
        strncat(type_str, " ", len);
    }

    auto rep_set = DbgGetType(rec_set, type_str, isType);
    Free(type_str);

    // process error
    ProcessErrorInRepSet(rep_set);

    // process type strings
    for (auto&& reply : rep_set) {
        auto ev = reply->rep_event->ev_event;
        if (ev == DBGEV_TYPE) {
            type_str = EVENT_TYPE(reply->rep_event);
                PrintMsgWithSetInfo(reply->rep_di.c_str(), reply->rep_group.get(), "%s\n", type_str);
        } else if (ev == DBGEV_THREADINFO) {
            PrintMultiThreadMsg(reply->rep_di.c_str(), reply->rep_group.get(),
                                reply->rep_event->ev_u.ev_u_string);
        } else {
            PrintOutError("unexpected result type.\n");
        }
    }

    return Command::Success;
}

static int cmd_ptype_cmd(Command::RunContext&, rec_set_t *rec_set, int argc, const char* const argv[]) {
    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: ptype type\n");
        return Command::Failure;
    }

    auto args = ArgsToString(&argv[1], argc - 1);

    auto rep_set = DbgGetType(rec_set, args.c_str(), true);

    // process error
    ProcessErrorInRepSet(rep_set);

    // process type strings
    for (auto&& reply : rep_set) {
        auto ev = reply->rep_event->ev_event;
        if (ev == DBGEV_TYPE) {
            auto type_str = EVENT_TYPE(reply->rep_event);

            // the result ends with a newline
            PrintMsgWithSetInfo(reply->rep_di.c_str(), reply->rep_group.get(), "%s",
                                type_str);
        } else if (ev == DBGEV_THREADINFO) {
            PrintMultiThreadMsg(reply->rep_di.c_str(), reply->rep_group.get(),
                                reply->rep_event->ev_u.ev_u_string);
        } else {
            PrintOutError("unexpected result type.\n");
        }
    }

    return Command::Success;
}

static int cmd_info_locals_cmd_console(Command::RunContext&, rec_set_t *rec_set, int, const char* const []) {
    auto rep_set = DbgGetVars(rec_set);
    ProcessErrorInRepSet(rep_set);

    // Process the replies
    for (auto&& reply : rep_set) {
        auto ev = reply->rep_event->ev_event;
        if (ev == DBGEV_VARS) {
            // point at the vars data
            auto v = EVENT_VARS(reply->rep_event);
            std::stringstream ss;
            for (; v != (dbgvars_t *)NULL; v = v->var_next) {
                PrintMsgWithSetInfo(reply->rep_di.c_str(), reply->rep_group.get(),
                                "Name:%-25s\tType:%s\n", v->var_name,
                                v->var_type);
            }
        } else if (ev == DBGEV_THREADINFO) {
            PrintMultiThreadMsg(reply->rep_di.c_str(), reply->rep_group.get(),
                                reply->rep_event->ev_u.ev_u_string);
        } else {
            PrintOutError("unexpected result type.\n");
        }
    }

    return Command::Success;
}

static int cmd_info_locals_cmd_mi(Command::LoweredContext<MiInterpreter>& ctx, rec_set_t *rec_set, int, const char* const []) {
    auto rep_set = DbgGetVars(rec_set);
    ProcessErrorInRepSet(rep_set);

    // Process the replies
    for (auto&& reply : rep_set) {
        auto ev = reply->rep_event->ev_event;
        if (ev == DBGEV_VARS) {
            // point at the vars data
            auto v = EVENT_VARS(reply->rep_event);
            std::stringstream ss;
            for (; v != (dbgvars_t *)NULL; v = v->var_next) {
                if (!ss.str().empty()){
                    ss<<",";
                }else{
                    ss<<"[";
                }
                std::string group = reply->rep_group.get()->toSetString();
                ss<<"{name=\""<<v->var_name<<"\",type=\""<<v->var_type<<"\",proc_set=\""<<reply->rep_di.c_str()
                                <<"\",group=\""<<group.substr(1,group.length()-2).c_str()<<"\"}";
            }
            ss<<"]";
            std::string group = reply->rep_group.get()->toSetString();
            ctx.caller.set_result("proc_set=\"%s\",group=\"%s\",variables=%s", reply->rep_di.c_str(),
                                group.substr(1,group.length()-2).c_str(),ss.str().c_str());
        } else if (ev == DBGEV_THREADINFO) {
            PrintMultiThreadMsg(reply->rep_di.c_str(), reply->rep_group.get(),
                                reply->rep_event->ev_u.ev_u_string);
        } else {
            PrintOutError("unexpected result type.\n");
        }
    }

    return Command::Success;
}

static int cmd_info_scope_cmd(Command::RunContext&, rec_set_t *rec_set, int argc, const char* const argv[]) {
    dbgvars_t *v;

    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: info scope arg\n");
        return Command::Failure;
    }

    auto rep_set = DbgGetScope(rec_set, argv[1]);

    ProcessErrorInRepSet(rep_set);

    for (auto&& reply : rep_set) {
        // point at the vars data
        v = EVENT_VARS(reply->rep_event);

        // This only gets printed if there is one reply
        if (rep_set.getNumReplies() == 1) {
            PrintOutMsg("Scope local variables for $%s:\n",
                        reply->rep_di.c_str());
        }

        for (; v != NULL; v = v->var_next) {
            // We don't expect there to be a different result for each rank,
            // since this command is relative to the entire application. But we
            // will allow for rank unique results anyways.
            if (rep_set.getNumReplies() == 1) {
                PrintOutMsg("Name:%-25s\tType:%s\n", v->var_name, v->var_type);
            } else {
                PrintMsgWithSetInfo(reply->rep_di.c_str(), reply->rep_group.get(),
                                    "Name:%-25s\tType:%s\n", v->var_name,
                                    v->var_type);
            }
        }
    }

    return Command::Success;
}

static int cmd_info_args_cmd(Command::RunContext&, rec_set_t *rec_set, int, const char* const []) {
    auto rep_set = DbgGetArgs(rec_set);

    ProcessErrorInRepSet(rep_set);

    // Process the replies
    for (auto&& reply : rep_set) {
        auto ev = reply->rep_event->ev_event;
        if (ev != DBGEV_THREADINFO && ev != DBGEV_STRING) {
            PrintOutError("unexpected result type.\n");
            continue;
        }

        auto is = std::istringstream{reply->rep_event->ev_u.ev_u_string};
        auto line = std::string{};
        while (std::getline(is,line)) {
            PrintMsgWithSetInfo(reply->rep_di.c_str(), reply->rep_group.get(),
                                "%s\n", line.c_str());
        }
    }

    return Command::Success;
}

void ConsoleCommandsFactory::insert_whatis_commands(Command& root) {
    root.addSubCmd("whatis", nullptr, proc_required, cmd_whatis_cmd,
               cmd_whatis_help, "Print the data type of an expression.");
    root.addSubCmd("ptype", nullptr, proc_required, cmd_ptype_cmd,
               cmd_ptype_help, "Print definition of a type.");

    root.addSubCmd("info locals", nullptr, proc_required,
                   cmd_info_locals_cmd_console, cmd_info_locals_help,
                   "List local variables of ranks in current process set.");
    root.addSubCmd("info args", nullptr, proc_required,
                   cmd_info_args_cmd,
                   cmd_info_args_help,
                   "List argument variables of the current frame.");
    root.addSubCmd("info scope", nullptr, proc_required,
                   cmd_info_scope_cmd,
                   cmd_info_scope_help,
                   "List variables local to a scope in the application.");
}

void MiCommandsFactory::insert_whatis_commands(Command& root) {
    root.addSubCmd({
        .name = "-stack-list-variables", .ptype = proc_required,
        .fn = Command::specify<MiInterpreter>(cmd_info_locals_cmd_mi) });
}
