/*********************************************************************************\
 * Block Map command: using the HPF data distribution idea
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2022 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 **********************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include "client.h"
#include "useful.h"
#include "blockmap_client.h"
#include "gram.h"

/*
** Static global variables
*/
static cmdlist_t *BlockMapCmds;
static cmdlist_t *OldBlockCmds;
static std::shared_ptr<BlockMap> CurrentBlockMap;

/********************************
 * Info class commands
 ********************************/

static int cmd_info_decomposition_cmd(rec_set_t *rec_set, int argc,
                                          char **argv) {

    if (argc > 1) {
        //currently a single decomposition does not need to be printed out in mi
        if (Client.MI_mode) return 1;

        try {
            auto expr = ParseString(argv[1]);

            auto dsym = dynamic_cast<gram::DSym*>(expr.get());
            if (!dsym) {
                PrintOutError("Expected a debugger variable.\n");
                return 0;
            }

            auto val = expr->eval(rec_set);
            if (auto bmv = dynamic_cast<BlockmapValue*>(val.get())) {
                auto m = bmv->getBlockMap();
                PrintOutMsg("%s", m->msg().c_str());
                return 1;
            } else {
                PrintOutError("No such decomposition \"%s\"\n", argv[1]);
                return 0;
            }

        } catch (EvalError& err) {
            PrintOutError("%s\n", err.what());
        }

        return 1;
    }
    if (!Client.MI_mode){
        PrintOutMsg("Defined decompositions:\n");
    }

    auto ss = std::ostringstream{};
    
    for (auto m : GetBlockMapList()) {
        std::string printed_blockmap = m->msg();
        if (printed_blockmap.empty()){
            continue;
        }
        if (Client.MI_mode){
            if (ss.tellp()!=0){
                ss<<",";
            }else{
                ss<<"[";
            }
            ss<<printed_blockmap;
        }else{
            PrintOutMsg(printed_blockmap.c_str());
        }
    }
    if (Client.MI_mode){
        if (ss.tellp()==0)
            ss<<"[";
        ss<<"]";
        PrintOutMIResult(MIResult::DONE,
                         "decompositions=%s\n%s",ss.str().c_str(),GetMIEpilogue());
    }
    return 1;
}

/********************************
 * Usage class commands
 ********************************/

static int cmd_usage_dimension_cmd(rec_set_t *rec_set, int argc,
                                            char **argv) {
    PrintHelpEntry((char **)cmd_blockmap_dimension_help);

    return 1;
}

static int cmd_usage_distribute_cmd(rec_set_t *rec_set, int argc,
                                             char **argv) {
    PrintHelpEntry((char **)cmd_blockmap_distribute_help);

    return 1;
}

static int cmd_usage_proc_grid_cmd(rec_set_t *rec_set, int argc,
                                            char **argv) {
    PrintHelpEntry((char **)cmd_blockmap_proc_grid_help);

    return 1;
}

static int cmd_usage_dim_order_cmd(rec_set_t *rec_set, int argc,
                                            char **argv) {
    PrintHelpEntry((char **)cmd_blockmap_dim_order_help);

    return 1;
}

/********************************
 * Regular commands
 ********************************/

static int cmd_decomposition_cmd(rec_set_t *rec_set, int argc, char **argv) {
    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: decomposition $decomposition_name\n");
        return 0;
    }

    try {
        auto expr = ParseString(argv[1]);

        try {
            auto val = expr->eval(rec_set);
            if (dynamic_cast<ProcsetValue*>(val.get())) {
                PrintOutError("A process set has already been defined using this name.\n");
                return 0;
            } else if (auto bm = dynamic_cast<BlockmapValue*>(val.get())) {
                if (!Client.confirm ||
                    cli_query("A decomposition has already been defined using "
                              "this name, redefine?",
                              cli_query_default::NO, cli_query_default::YES)) {
                    UnregisterBlockMap(bm->getBlockMap()->getName());
                } else {
                    return 0;
                }
            } else {
                PrintOutError("A debugger variable has already been defined using this "
                "name.\n");
                return 0;
            } 
        } catch (EvalError& err) {
            // I wanted this evaluation to fail
        }

    } catch (EvalError& err) {
        PrintOutError("%s\n", err.what());
    }

    bool debug = false;
    if (argc > 2) {
        if (strcmp(argv[2], "-debug") == 0) {
            debug = true;
        } else {
            PrintOutError("Invalid decomposition switch \"%s\".\n", argv[2]);
            return 0;
        }
    }

    CurrentBlockMap = std::make_shared<BlockMap>(argv[1], debug);

    OldBlockCmds = SetCommands(BlockMapCmds);

    cli_inc_prompt();

    return 1;
}

/********************************
 * Alt commands
 ********************************/

static int cmd_blockmap_dimension_cmd(rec_set_t *rec_set, int argc,
                                          char **argv) {
    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: dimension 64,64,64\n");
        return 0;
    }

    if (CurrentBlockMap->getDims()) {
        PrintOutError("Dimension already defined.\n");
        if (!cli_query("Re-define dimension?")) return 1;
    }

    auto args = ArgsToString(argv+1,argc-1);
    args.erase(std::remove_if(args.begin(),args.end(),isspace), args.end());

    auto dims = std::vector<int>{};

    char* e;
    for (const char* s = args.c_str(); *s; s = e) {
        auto dim = strtol(s,&e,10);
        if (e == s) {
            PrintOutError("Expected integer values 64,64,64.\n");
            return 0;
        } else if (dim <= 0) {
            PrintOutError("Dimensions must be positive.\n");
            return 0;
        }
        dims.push_back(dim);
        if (*e == ',') ++e;
    }

    if (CurrentBlockMap->isRerank(dims.size())) {
        PrintOutError("Rank mismatch between previous "
                      "dimension/distribute/proc_grid/dim_order definition!\n");
        if (!cli_query("Override warning?")) {
            return 0;
        }
    }

    // Allocate the data array inside the blockmap definition
    CurrentBlockMap->setDims(dims);
    return 1;
}

static int cmd_blockmap_distribute_cmd(rec_set_t *rec_set, int argc,
                                           char **argv) {
    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: distribute block,block,*\n");
        return 0;
    }

    if (CurrentBlockMap->getDistribution()) {
        PrintOutError("Distribute already defined.\n");
        if (!cli_query("Re-define distribute?")) return 1;
    }

    auto args = ArgsToString(argv+1,argc-1);
    args.erase(std::remove_if(args.begin(),args.end(),isspace), args.end());

    auto distribute = std::vector<Distribution>{};
    auto blockCyclic = std::vector<int>{};

    auto is = std::istringstream{args};
    auto arg = std::string{};
    while (std::getline(is, arg, ',')) {
        Distribution d = Distribution::Star; int bc = 0;
        if (arg == "block") {
            d = Distribution::Block;
        } else if (arg == "cyclic") {
            d = Distribution::Cyclic;
        } else if (arg == "*") {
            d = Distribution::Star;
        } else {
            try {
                d = Distribution::BlockCyclic;
                bc = std::stoi(arg);
            } catch (...) {
                PrintOutError("Unrecognized distribution value %s.\n",arg.c_str());
                return 0;
            }
        }

        distribute.push_back(d);
        blockCyclic.push_back(bc);
    }

    auto rank = int(distribute.size());
        

    // Set the rank value inside the blockmap definition if is has not been set
    // yet
    if (CurrentBlockMap->isRerank(rank)) {
        // rank has already been set, ensure that what the user provided us
        // matches
        PrintOutError(
            "Rank mismatch between previous "
            "dimension/distribute/proc_grid/dim_order definition!\n");
        if (!cli_query("Override warning?")) {
            return 0;
        }
    }

    CurrentBlockMap->setDistribution(distribute, blockCyclic);
    return 1;
}

static int cmd_blockmap_proc_grid_cmd(rec_set_t *rec_set, int argc,
                                          char **argv) {
    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: proc_grid 32,64,*\n");
        return 0;
    }

    if (CurrentBlockMap->getProcGrid()) {
        PrintOutError("proc_grid already defined.\n");
        if (!cli_query("Re-define proc_grid?")) return 1;
    }

    auto args = ArgsToString(argv+1,argc-1);
    args.erase(std::remove_if(args.begin(),args.end(),isspace), args.end());

    auto procs = std::vector<int>{};
    
    auto is = std::istringstream{args};
    auto arg = std::string{};
    while (std::getline(is, arg, ',')) {
        int sz = 1;
        if (arg == "*") {
            // 1 is the same as being non-distributed
            sz = 1;
        } else {
            try {
                sz = std::stoi(arg);
            } catch (...) {
                PrintOutError("Grid size must be an integer or *, %s.\n",arg.c_str());
                return 0;
            }
        }
        procs.push_back(sz);
    }

    auto rank = int(procs.size());

    // Set the rank value inside the blockmap definition if is has not been set yet
    if (CurrentBlockMap->isRerank(rank)) {
        // rank has already been set, ensure that what the user provided us
        // matches
        PrintOutError(
            "Rank mismatch between previous "
            "dimension/distribute/proc_grid/dim_order definition!\n");
        if (!cli_query("Override warning?")) return 0;
    }

    CurrentBlockMap->setProcGrid(procs);
    return 1;
}

static int cmd_blockmap_dim_order_cmd(rec_set_t *rec_set, int argc,
                                          char **argv) {
    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: dim_order 1,2,*\n");
        return 0;
    }

    if (CurrentBlockMap->getDimOrder()) {
        PrintOutError("dim_order already defined.\n");
        if (!cli_query("Re-define dim_order?")) return 1;
    }

    auto args = ArgsToString(argv+1,argc-1);
    args.erase(std::remove_if(args.begin(),args.end(),isspace), args.end());

    auto order = std::vector<int>{};

    auto is = std::istringstream{args};
    auto arg = std::string{};
    while (std::getline(is, arg, ',')) {
        int n = 0;
        if (arg == "*") {
            // 1 is the same as being non-distributed
            n = int(Distribution::Star);
        } else {
            try {
                n = std::stoi(arg);
            } catch (...) {
                PrintOutError("Order must be an integer or *, %s.\n",arg.c_str());
                return 0;
            }
        }
        order.push_back(n);
    }

    auto rank = int(order.size());


    // Set the rank value inside the blockmap definition if is has not been set
    // yet
    if (CurrentBlockMap->isRerank(rank)) {
        // rank has already been set, ensure that what the user provided us
        // matches
        PrintOutError(
            "Rank mismatch between previous "
            "dimension/distribute/proc_grid/dim_order definition!\n");
        if (!cli_query("Override warning?")) return 0;
    }

    // Allocate the m_dim_order array inside the blockmap definition
    CurrentBlockMap->setDimOrder(order);
    return 1;
}

static int cmd_blockmap_end_cmd(rec_set_t *rec_set, int argc, char **argv) {
    if (CurrentBlockMap->verify()) {
        // add to the block map table
        RegisterBlockMap(CurrentBlockMap);
    } else {
        PrintError();
        if (!cli_query("Exit script mode and destroy current decomposition?")) {
            // keep editting
            return 0;
        }
    }

    // reset to normal command processing
    CurrentBlockMap.reset();
    cli_dec_prompt();
    SetCommands(OldBlockCmds);

    return 1;
}

/********************************
 * Helper functions
 ********************************/

static void blockmapnotfound(const char *cmd) { CmdNotFound(cmd); }

static void blockmapambig(const char *cmd) {
    AmbiguousAltCmd(BlockMapCmds, cmd);
}

static void blockmapfailed(void) {
    // Do nothing
    return;
}

/********************************
 * command init functions
 ********************************/

void InitCmd_blockmap(void) {
    // info commands
    AddInfoCommand("decomposition", proc_ignore, 0,
                   cmd_info_decomposition_cmd,
                   cmd_info_decomposition_help,
                   "Display currently defined decomposition schemes.");

    // usage commands
    AddUsageCommand(
        "dimension", proc_ignore, 0,
        cmd_usage_dimension_cmd,
        cmd_usage_decomposition_help,
        "Display help about using decomposition \"dimension\" command.");
    AddUsageCommand(
        "distribute", proc_ignore, 0,
        cmd_usage_distribute_cmd,
        cmd_usage_decomposition_help,
        "Display help about using decomposition \"distribute\" command.");
    AddUsageCommand(
        "proc_grid", proc_ignore, 0,
        cmd_usage_proc_grid_cmd,
        cmd_usage_decomposition_help,
        "Display help about using decomposition \"proc_grid\" command.");
    AddUsageCommand(
        "dim_order", proc_ignore, 0,
        cmd_usage_dim_order_cmd,
        cmd_usage_decomposition_help,
        "Display help about using decomposition \"dim_order\" command.");

    // regular commands
    AddCommand("decomposition", NULL, proc_ignore, 0,
               cmd_decomposition_cmd, cmd_blockmap_help,
               "Define a decomposition scheme.");

    // blockmap alt commands
    BlockMapCmds =
        NewCommandList(blockmapnotfound, blockmapambig, blockmapfailed);

    AddAltCommand(BlockMapCmds, "dimension", NULL, proc_ignore, 0,
                  cmd_blockmap_dimension_cmd,
                  cmd_blockmap_dimension_help,
                  "Provide a dimension definition.");
    AddAltCommand(BlockMapCmds, "distribute", NULL, proc_ignore, 0,
                  cmd_blockmap_distribute_cmd,
                  cmd_blockmap_distribute_help,
                  "Provide a distribute definition.");
    AddAltCommand(BlockMapCmds, "proc_grid", NULL, proc_ignore, 0,
                  cmd_blockmap_proc_grid_cmd,
                  cmd_blockmap_proc_grid_help,
                  "Provide a process grid definition.");
    AddAltCommand(BlockMapCmds, "dim_order", NULL, proc_ignore, 0,
                  cmd_blockmap_dim_order_cmd,
                  cmd_blockmap_dim_order_help,
                  "Provide a dimension order definition.");
    AddAltCommand(BlockMapCmds, "end", NULL, proc_ignore, 0,
                  cmd_blockmap_end_cmd,
                  cmd_blockmap_end_help,
                  "Finish defining the current decomposition scheme.");
    AddAltCommand(
        BlockMapCmds, "help", "h", proc_ignore, 0, cmd_help_cmd,
        cmd_help_help,
        "Display help information about decomposition script mode commands.");
}

void FinishCmd_blockmap(void) {
    FinishBlockMap();
    DestroyCommandList(BlockMapCmds);
}
