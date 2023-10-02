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
#include "interpreter_mi.h"

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

static dbgprocset_t* cmd_focus(int argc, const char* const argv[]) {
    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: focus $set\n");
        return nullptr;
    }

    auto [procset, isTmp] = IdMap::getProcset(argv[1]);

    if (!procset) {
        PrintOutError("Invalid process set argument %s.\n", argv[1]);
        return nullptr;
    }
    SetCurrentProcSet(procset);

    return procset;
}

static int cmd_focus_console(int argc, const char* const argv[]) {
    return cmd_focus(argc, argv) != nullptr ? Command::Success : Command::Failure;
}

static int cmd_focus_mi(MiInterpreter& mi, int argc, const char* const argv[]) {
    auto* pset = cmd_focus(argc, argv);
    if (pset) {
        std::string proc_str = ProcSetToString(pset);
        std::stringstream ss;
        ss<<"{name=\""<<pset->s_name<<"\",proc_set=\""<<proc_str<<"\"}";

        mi.set_result("focus=%s",ss.str().c_str());
        return Command::Success;
    } else {
        // error message already printed
        return Command::Failure;
    }
}

static std::vector<dbgprocset_t*>
get_procset_ptrs(int argc, const char* const argv[]) {
    if (argc > 1) {
        auto [procset, _] = IdMap::getProcset(argv[1]);

        if (!procset) {
            PrintOutError("Invalid process set argument %s.\n", argv[1]);
            return {};
        }

        return { procset };
    }

    // IdMap::getProcset above returns a raw dbgprocset*, so we need to make the return
    // type of this function the lowest common denominator.
    auto raws = std::vector<dbgprocset_t*>{};
    std::ranges::transform(GetProcSets(), std::back_inserter(raws), [](auto& sh_ptr){ return sh_ptr.get(); });
    return raws;
}


static int cmd_viewset_console(int argc, const char* const argv[]) {
    auto procsets = get_procset_ptrs(argc, argv);

    if (procsets.empty()) {
        // error message already printed
        return Command::Failure;
    }

    PrintOutMsg("Name       Procs\n");
    for (auto* procset : procsets) {
        PrintOutMsg("%-10s ", procset->s_name.c_str());
        PrintProcSet(procset);
        PrintOutMsg("\n");
    }

    return Command::Success;
}

static int cmd_viewset_mi(MiInterpreter& mi, int argc, const char* const argv[]) {
    auto procsets = get_procset_ptrs(argc, argv);

    if (procsets.empty()) {
        // error message already printed
        return Command::Failure;
    }

    std::stringstream ss;
    for (auto* procset : procsets) {
        if (!ss.str().empty()){
            ss<<",";
        }else{
            ss<<"[";
        }
        std::string proc_str = ProcSetToString(procset);
        ss<<"{name=\""<<procset->s_name<<"\",proc_set=\""<<proc_str.c_str()<<"\"}";
    }
    ss<<"]";
    mi.set_result("pe_sets=%s",ss.str().c_str());

    return Command::Success;
}

static int cmd_defset_cmd(int argc, const char* const argv[]) {
    if (argc < 3) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: defset $setname $proc1,$proc2\n");
        return Command::Failure;
    }

    static const std::regex psRE{R"(\$\w+)"};

    if (!std::regex_match(argv[1], psRE)) {
        PrintOutError("%s is a not a valid procset name.\n", argv[1]);
        return Command::Failure;
    }

    auto procDef = ArgsToString(&argv[2], argc-2);

    try {
        auto defExpr = ParseString(procDef);
        if (!CreateProcSet(argv[1], *defExpr))
            return Command::Failure;
    } catch (EvalError& err) {
        PrintOutError("%s\n", err.what());
        return Command::Failure;
    }

    return Command::Success;
}

void ConsoleCommandsFactory::insert_procset_commands(CommandNode& root) {
    root.addSubCmd("defset", nullptr, proc_ignore, cmd_defset_cmd,
                   cmd_defset_help, "Create a set of processes.");
    root.addSubCmd("viewset", nullptr, proc_ignore, cmd_viewset_console,
                   cmd_viewset_help, "Display information about process sets.");
    root.addSubCmd("focus", nullptr, proc_ignore, cmd_focus_console,
                   cmd_focus_help, "Set the current process set.");
}

void MiCommandsFactory::insert_procset_commands(CommandNode& root) {
    root.addSubCmd({ .name = "-procset-define", .ptype = proc_ignore, .fn = cmd_defset_cmd });
    root.addSubCmd({ .name = "-procset-list", .ptype = proc_ignore, .fn = cmd_viewset_mi });
    root.addSubCmd({ .name = "-procset-focus", .ptype = proc_ignore, .fn = cmd_focus_mi });
}
