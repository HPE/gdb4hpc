/*********************************************************************************\
 * Whatis command.
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

static int cmd_whatis_cmd(rec_set_t *rec_set, int argc, char **argv) {
    rep_set_t *rep_set;
    int i;
    int argp = 1;
    int isType = 0;
    int len = 0;
    char *type_str;

    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: whatis var\n");
        return 0;
    }

    if (strcmp(argv[argp], "-type") == 0) {
        if (++argp == argc) {
            PrintOutError("Error: -type provided, but missing arguments.\n");
            return 0;
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

    rep_set = DbgGetType(rec_set, type_str, isType);
    Free(type_str);

    // process error
    ProcessErrorInRepSet(rep_set);

    // process type strings
    for (auto&& reply : rep_set) {
        auto ev = reply->rep_event->ev_event;
        if (ev == DBGEV_TYPE) {
            type_str = EVENT_TYPE(reply->rep_event);

            PrintMsgWithSetInfo(reply->rep_di->d_handle, reply->rep_group, "%s\n",
                                type_str);
        } else if (ev == DBGEV_THREADINFO) {
            PrintMultiThreadMsg(reply->rep_di->d_handle, reply->rep_group,
                                reply->rep_event->ev_u.ev_u_string);
        } else {
            PrintOutError("unexpected result type.\n");
        }
    }

    FreeRepSet(rep_set);

    return 1;
}

static int cmd_ptype_cmd(rec_set_t *rec_set, int argc, char **argv) {
    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: ptype type\n");
        return 0;
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

            PrintMsgWithSetInfo(reply->rep_di->d_handle, reply->rep_group, "%s\n",
                                type_str);
        } else if (ev == DBGEV_THREADINFO) {
            PrintMultiThreadMsg(reply->rep_di->d_handle, reply->rep_group,
                                reply->rep_event->ev_u.ev_u_string);
        } else {
            PrintOutError("unexpected result type.\n");
        }
    }

    FreeRepSet(rep_set);

    return 1;
}


/*
** NOTE: This used to be info globals, but with the upgrade to libmi, I changed
** this to info locals instead. Now it displays the local variables of the
** current frame.
*/
static int class_info_subcmd_locals_cmd(rec_set_t *rec_set, int argc,
                                        char **argv) {
    auto rep_set = DbgGetVars(rec_set);

    ProcessErrorInRepSet(rep_set);

    // Process the replies
    for (auto&& reply : rep_set) {
        auto ev = reply->rep_event->ev_event;
        if (ev == DBGEV_VARS) {
            // point at the vars data
            auto v = EVENT_VARS(reply->rep_event);

            for (; v != (dbgvars_t *)NULL; v = v->var_next) {
                PrintMsgWithSetInfo(reply->rep_di->d_handle, reply->rep_group,
                                    "Name:%-25s\tType:%s\n", v->var_name,
                                    v->var_type);
            }
        } else if (ev == DBGEV_THREADINFO) {
            PrintMultiThreadMsg(reply->rep_di->d_handle, reply->rep_group,
                                reply->rep_event->ev_u.ev_u_string);
        } else {
            PrintOutError("unexpected result type.\n");
        }
    }

    FreeRepSet(rep_set);

    return 1;
}

static int class_info_subcmd_scope_cmd(rec_set_t *rec_set, int argc,
                                       char **argv) {
    rep_set_t *rep_set;
    dbgvars_t *v;

    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: info scope arg\n");
        return 0;
    }

    rep_set = DbgGetScope(rec_set, argv[1]);

    ProcessErrorInRepSet(rep_set);

    for (auto&& reply : rep_set) {
        // point at the vars data
        v = EVENT_VARS(reply->rep_event);

        // This only gets printed if there is one reply
        if (GetNumberOfReply(rep_set) == 1) {
            PrintOutMsg("Scope local variables for $%s:\n",
                        reply->rep_di->d_handle);
        }

        for (; v != NULL; v = v->var_next) {
            // We don't expect there to be a different result for each rank,
            // since this command is relative to the entire application. But we
            // will allow for rank unique results anyways.
            if (GetNumberOfReply(rep_set) == 1) {
                PrintOutMsg("Name:%-25s\tType:%s\n", v->var_name, v->var_type);
            } else {
                PrintMsgWithSetInfo(reply->rep_di->d_handle, reply->rep_group,
                                    "Name:%-25s\tType:%s\n", v->var_name,
                                    v->var_type);
            }
        }
    }

    FreeRepSet(rep_set);

    return 1;
}

void InitCmd_whatis(void) {
    AddCommand("whatis", NULL, proc_required, 0, cmd_whatis_cmd,
               cmd_whatis_help, "Print the data type of an expression.");
    AddCommand("ptype", NULL, proc_required, 0, cmd_ptype_cmd,
               cmd_ptype_help, "Print definition of a type.");


    AddInfoCommand("locals", proc_required, 0,
                   class_info_subcmd_locals_cmd,
                   class_info_subcmd_locals_help,
                   "List local variables of ranks in current process set.");
    AddInfoCommand("scope", proc_required, 0,
                   class_info_subcmd_scope_cmd,
                   class_info_subcmd_scope_help,
                   "List variables local to a scope in the application.");
}
