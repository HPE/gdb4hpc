/*********************************************************************************\
 * Process set commands.
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

#include <regex>
#include "client.h"
#include "useful.h"
#include "eval.h"
#include "gram.h"

extern dbgprocset_t* CreateProcSet(const std::string& name, const gram::Expression& def);

std::string ProcSetToString(dbgprocset_t *s) {

    if (s->s_over_sets.empty()) return "none";

    // Here we want to print the "actual" ranks in the procset, not the "offset"
    // ranks.
    int n = 0;
    std::stringstream ss;
    for (auto over_set : s->s_over_sets) {
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

static int cmd_focus_cmd(Command::RunContext& ctx, rec_set_t *rec_set, int argc, const char* const argv[]) {

    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: focus $set\n");
        return 0;
    }

    auto [procset, isTmp] = IdMap::getProcset(argv[1]);

    if (!procset) {
        PrintOutError("Invalid process set argument %s.\n", argv[1]);
        return 0;
    }
    SetCurrentProcSet(procset);

    if (Client.MI_mode) {
        std::string proc_str = ProcSetToString(procset);
        std::stringstream ss;
        ss<<"{name=\""<<procset->s_name<<"\",proc_set=\""<<proc_str<<"\"}";

        PrintOutMIResult(MIResult::DONE, "focus=%s\n%s",ss.str().c_str(),
                         GetMIEpilogue());
    }

    return 1;
}

static int cmd_viewset_cmd(Command::RunContext& ctx, rec_set_t *rec_set, int argc, const char* const argv[]) {

    if (argc > 1) {
        auto [procset, isTmp] = IdMap::getProcset(argv[1]);

        if (!procset) {
            PrintOutError("Invalid process set argument %s.\n", argv[1]);
            return 0;
        }

        PrintOutMsg("Name       Procs\n");
        PrintOutMsg("%-10s ", procset->s_name.c_str());
        PrintProcSet(procset);
        PrintOutMsg("\n");

        return 1;
    }

    if (Client.MI_mode){
        std::stringstream ss;
        for (auto procset : GetProcSets()) {
            if (!ss.str().empty()){
                ss<<",";
            }else{
                ss<<"[";
            }
            std::string proc_str = ProcSetToString(procset.get());
            ss<<"{name=\""<<procset->s_name<<"\",proc_set=\""<<proc_str.c_str()<<"\"}";
        }
        ss<<"]";
        PrintOutMIResult(MIResult::DONE, "pe_sets=%s\n%s",ss.str().c_str(),GetMIEpilogue());
    }else{
        PrintOutMsg("Name       Procs\n");
        for (auto procset : GetProcSets()) {
            PrintOutMsg("%-10s ", procset->s_name.c_str());
            PrintProcSet(procset.get());
            PrintOutMsg("\n");
        }
    }
    return 1;
}

static int cmd_defset_cmd(Command::RunContext& ctx, rec_set_t *rec_set, int argc, const char* const argv[]) {

    if (argc < 3) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: defset $setname $proc1,$proc2\n");
        return 0;
    }

    static const std::regex psRE{R"(\$\w+)"};

    if (!std::regex_match(argv[1], psRE)) {
        PrintOutError("%s is a not a valid procset name.\n", argv[1]);
        return 0;
    }

    auto procDef = ArgsToString(&argv[2], argc-2);

    try {
        auto defExpr = ParseString(procDef);
        if (!CreateProcSet(argv[1], *defExpr))
            return 0;
    } catch (EvalError& err) {
        PrintOutError("%s\n", err.what());
        return 0;
    }

    return 1;
}

void InitCmd_procset(void) {
    AddCommand("defset", proc_ignore, cmd_defset_cmd,
               cmd_defset_help, "Create a set of processes.");
    AddCommand("viewset", proc_ignore, cmd_viewset_cmd,
               cmd_viewset_help, "Display information about process sets.");
    AddCommand("focus", proc_ignore, cmd_focus_cmd,
               cmd_focus_help, "Set the current process set.");
}
