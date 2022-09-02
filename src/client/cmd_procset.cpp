/*********************************************************************************\
 * Process set commands.
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

    if ((ex = ParseExpr(argv[1])) == nullptr) {
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

        if (procset == nullptr) {
            PrintOutError("Failed to create temporary procset for focus!\n");
            return 0;
        }

        if (procset->s_nproc == 0) {
            PrintOutError("Rank selection \"%s\" is invalid.\n", 
                RVAL_REF_SLICE(val)->getArgs());
            FreeProcSet(procset);
            return 0;
        }
    } else {
        // assign procset directly
        procset = RVAL_ACTUAL_PROCSET(val);
    }

    SetCurrentProcSet(procset);

    return 1;
}

std::string ProcSetToString(dbgprocset_t *s) {
    int n = 0;
    std::stringstream ss;
    dbg_over_set_t *over_set;

    if (EmptyList(s->s_over_set_list)) {
        ss<<"none";
        return ss.str();
    }

    SetList(s->s_over_set_list);

    // Here we want to print the "actual" ranks in the procset, not the "offset"
    // ranks.
    while ((over_set = (dbg_over_set_t *)GetListElement(s->s_over_set_list)) !=
           NULL) {
        ss<<over_set->s_di.c_str();
        std::string si = over_set->s_di_bs->toSetString();
        if (s->s_threads) {
            // insert the thread info
            std::string setInfo{si};
            auto loc = setInfo.rfind('}');
            if (loc != std::string::npos) {
                setInfo = setInfo.substr(0,loc) + ":" + s->s_threads + "}";
            }
            ss<<setInfo.c_str();
        } else {
            ss<<si.c_str();
        }

        n += over_set->s_di_bs->numSet();
        if (n < s->s_nproc) {
            ss<<",";
        }
    }
    return ss.str();
}

static int cmd_viewset_cmd(rec_set_t *rec_set, int argc, char **argv) {
    expr_t *ex;
    val_t *val;
    dbgprocset_t *procset;

    if (argc > 1) {
        if ((ex = ParseExpr(argv[1])) == nullptr) {
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

    SetProcSet();
    if (Client.MI_mode){
        std::stringstream ss;
        while ((procset = GetProcSet()) != nullptr) {
            if (!ss.str().empty()){
                ss<<",";
            }else{
                ss<<"[";
            }
            std::string proc_str = ProcSetToString(procset);
            ss<<"{name=\""<<procset->s_name<<"\",proc_set=\""<<proc_str.c_str()<<"\"}";
        }
        ss<<"]";
        PrintOutMIResult(MIResult::DONE, "pe_sets=%s\n(gdb)\n",ss.str().c_str());
    }else{
        PrintOutMsg("Name       Procs\n");
        while ((procset = GetProcSet()) != nullptr) {
            PrintOutMsg("%-10s ", procset->s_name);
            PrintProcSet(procset);
            PrintOutMsg("\n");
        }
    }
    return 1;
}

static int cmd_defset_cmd(rec_set_t *rec_set, int argc, char **argv) {

    if (argc < 3) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: defset $setname $proc1,$proc2\n");
        return 0;
    }

    expr_t *e_name = ParseExpr(argv[1]);
    if (!e_name) {
        PrintError();
        return 0;
    }

    std::string procDef = ArgsToString(&argv[2], argc - 2);
    expr_t* e_set = ParseExpr(procDef.c_str());
    if (!e_set) {
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
    AddCommand("defset", nullptr, proc_ignore, 0, cmd_defset_cmd,
               cmd_defset_help, "Create a set of processes.");
    AddCommand("viewset", nullptr, proc_ignore, 0, cmd_viewset_cmd,
               cmd_viewset_help, "Display information about process sets.");
    AddCommand("focus", nullptr, proc_ignore, 0, cmd_focus_cmd,
               cmd_focus_help, "Set the current process set.");
}
