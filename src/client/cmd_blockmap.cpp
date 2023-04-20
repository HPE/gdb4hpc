/*********************************************************************************\
 * Block Map command: using the HPF data distribution idea
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
static std::shared_ptr<Command> BlockMapCmds;
static Command* OldBlockCmds = nullptr;
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
        PrintOutError("  or decomposition $decomposition_name dim1 [, dim2] [, dim3] .. \n");
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

    if (argc == 2) {
        // Old modal form for decomposition definition
        CurrentBlockMap = std::make_shared<BlockMap>(argv[1]);

        OldBlockCmds = SetCommands(BlockMapCmds.get());

        cli_inc_prompt();
        return 1;
    } else {
        // Create the decomposition from a single line definition
        try {
            auto bm = std::make_shared<BlockMap>(argv[1], ArgsToString(argv+2, argc-2));
            RegisterBlockMap(bm);
            return 1;
        } catch ( std::exception& err) {
            PrintOutError("Bad decomposition definition: %s\n", err.what());
            return 0;
        }
    }
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
 * command init functions
 ********************************/

void InitCmd_blockmap(void) {
    // info commands
    AddCommand("info decomposition", proc_ignore,
                   cmd_info_decomposition_cmd,
                   cmd_info_decomposition_help,
                   "Display currently defined decomposition schemes.");

    // usage commands
    AddCommand(
        "usage dimension", proc_ignore,
        cmd_usage_dimension_cmd,
        cmd_usage_decomposition_help,
        "Display help about using decomposition \"dimension\" command.");
    AddCommand(
        "usage distribute", proc_ignore,
        cmd_usage_distribute_cmd,
        cmd_usage_decomposition_help,
        "Display help about using decomposition \"distribute\" command.");
    AddCommand(
        "usage proc_grid", proc_ignore,
        cmd_usage_proc_grid_cmd,
        cmd_usage_decomposition_help,
        "Display help about using decomposition \"proc_grid\" command.");
    AddCommand(
        "usage dim_order", proc_ignore,
        cmd_usage_dim_order_cmd,
        cmd_usage_decomposition_help,
        "Display help about using decomposition \"dim_order\" command.");

    // regular commands
    AddCommand("decomposition", NULL, proc_ignore,
               cmd_decomposition_cmd, cmd_blockmap_help,
               "Define a decomposition scheme.");

    // blockmap alt commands
    BlockMapCmds = std::make_shared<Command>("", nullptr, proc_ignore, nullptr,
                                             nullptr, nullptr);
        
    BlockMapCmds->addSubCmd("dimension", NULL, proc_ignore,
                  cmd_blockmap_dimension_cmd,
                  cmd_blockmap_dimension_help,
                  "Provide a dimension definition.");
    BlockMapCmds->addSubCmd("distribute", NULL, proc_ignore,
                  cmd_blockmap_distribute_cmd,
                  cmd_blockmap_distribute_help,
                  "Provide a distribute definition.");
    BlockMapCmds->addSubCmd("proc_grid", NULL, proc_ignore,
                  cmd_blockmap_proc_grid_cmd,
                  cmd_blockmap_proc_grid_help,
                  "Provide a process grid definition.");
    BlockMapCmds->addSubCmd("dim_order", NULL, proc_ignore,
                  cmd_blockmap_dim_order_cmd,
                  cmd_blockmap_dim_order_help,
                  "Provide a dimension order definition.");
    BlockMapCmds->addSubCmd("end", NULL, proc_ignore,
                  cmd_blockmap_end_cmd,
                  cmd_blockmap_end_help,
                  "Finish defining the current decomposition scheme.");
    BlockMapCmds->addSubCmd(
        "help", "h", proc_ignore, cmd_help_cmd,
        cmd_help_help,
        "Display help information about decomposition script mode commands.");
}

void FinishCmd_blockmap(void) {
    FinishBlockMap();
}
