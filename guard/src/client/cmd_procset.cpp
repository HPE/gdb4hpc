/*********************************************************************************\
 * Process set commands.
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
#include <stdio.h>
#include <string.h>

#include "client.h"
#include "useful.h"

static int cmd_focus_cmd(rec_set_t *rec_set, int argc, char **argv) {
    expr_t *ex;
    val_t *val;
    dbgprocset_t *procset;

    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: focus $set\n");
        return 0;
    }

    if ((ex = ParseExpr(argv[1])) == NULL) {
        PrintError();
        return 0;
    }

    if (EvalExpr(&val, ex, rec_set) < 0) {
        PrintError();
        return 0;
    }

    FreeExpr(ex);

    if (RVAL_ACTUAL_TYPE(val) != rval_proc) {
        PrintOutError("Invalid process set argument %s.\n", argv[1]);
        return 0;
    }

    // Check to see if we need to create a new temporary procset
    if (RVAL_TYPE(val) == rval_ref) {
        // Create a temporary proc_set
        procset = CreateTempProcSetSlice(RVAL_ACTUAL_PROCSET(val),
                                         RVAL_REF_SLICE(val));

        if (procset == NULL) {
            PrintOutError("Failed to create temporary procset for focus!\n");
            return 0;
        }
    } else {
        // assign procset directly
        procset = RVAL_ACTUAL_PROCSET(val);
    }

    SetCurrentProcSet(procset);

    return 1;
}

static int cmd_viewset_cmd(rec_set_t *rec_set, int argc, char **argv) {
    expr_t *ex;
    val_t *val;
    dbgprocset_t *procset;

    if (argc > 1) {
        if ((ex = ParseExpr(argv[1])) == NULL) {
            PrintError();
            return 0;
        }

        if (EvalExpr(&val, ex, rec_set) < 0) {
            PrintError();
            return 0;
        }

        FreeExpr(ex);

        if (RVAL_ACTUAL_TYPE(val) != rval_proc) {
            PrintOutError("Invalid process set argument %s.\n", argv[1]);
            return 0;
        }

        // assign procset directly
        procset = RVAL_ACTUAL_PROCSET(val);

        PrintOutMsg("Name       Procs\n");
        PrintOutMsg("%-10s ", procset->s_name);
        PrintProcSet(procset);
        PrintOutMsg("\n");

        return 1;
    }

    PrintOutMsg("Name       Procs\n");

    SetProcSet();
    while ((procset = GetProcSet()) != NULL) {
        PrintOutMsg("%-10s ", procset->s_name);
        PrintProcSet(procset);
        PrintOutMsg("\n");
    }

    return 1;
}

static int cmd_defset_cmd(rec_set_t *rec_set, int argc, char **argv) {
    expr_t *e_name;
    expr_t *e_set;

    if (argc < 3) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: defset $setname $proc1,$proc2\n");
        return 0;
    }

    if ((e_name = ParseExpr(argv[1])) == NULL) {
        PrintError();
        return 0;
    }

    if ((e_set = ParseExpr(argv[2])) == NULL) {
        PrintError();
        return 0;
    }

    // Create a new procset for val
    if (!CreateProcSet(e_name, e_set)) {
        FreeExpr(e_name);
        FreeExpr(e_set);
        return 0;
    }

    FreeExpr(e_name);
    FreeExpr(e_set);

    return 1;
}

void InitCmd_procset(void) {
    AddCommand("defset", NULL, proc_ignore, 0, (int (*)())cmd_defset_cmd,
               cmd_defset_help, "Create a set of processes.");
    AddCommand("viewset", NULL, proc_ignore, 0, (int (*)())cmd_viewset_cmd,
               cmd_viewset_help, "Display information about process sets.");
    AddCommand("focus", NULL, proc_ignore, 0, (int (*)())cmd_focus_cmd,
               cmd_focus_help, "Set the current process set.");
}
