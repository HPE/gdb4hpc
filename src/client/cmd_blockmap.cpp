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

#include "interpreter_console.h"
#include "interpreter_mi.h"

/*
** Static global variables
*/

namespace {

// XXX: if the user creates 2 interpreters and both modify this at once, bad things will happen.
std::shared_ptr<BlockMap> CurrentBlockMap;

std::shared_ptr<CommandNode> make_blockmap_command_tree();

/********************************
 * Info class commands
 ********************************/

int cmd_info_decomposition_cmd_console(rec_set_t *rec_set, int argc, const char* const argv[]) {
    if (argc > 1) {
        try {
            auto expr = ParseString(argv[1]);

            auto dsym = dynamic_cast<gram::DSym*>(expr.get());
            if (!dsym) {
                PrintOutError("Expected a debugger variable.\n");
                return Command::Failure;
            }

            auto val = expr->eval(rec_set);
            if (auto bmv = dynamic_cast<BlockmapValue*>(val.get())) {
                auto m = bmv->getBlockMap();
                PrintOutMsg("%s", m->msg().c_str());
                return Command::Success;
            } else {
                PrintOutError("No such decomposition \"%s\"\n", argv[1]);
                return Command::Failure;
            }

        } catch (EvalError& err) {
            PrintOutError("%s\n", err.what());
        }

        return Command::Success;
    }
    PrintOutMsg("Defined decompositions:\n");

    auto ss = std::ostringstream{};
    
    for (auto m : GetBlockMapList()) {
        std::string printed_blockmap = m->msg();
        if (printed_blockmap.empty()){
            continue;
        }
        PrintOutMsg("%s", printed_blockmap.c_str());
    }
    return Command::Success;
}

static std::string stringify_blockmap_mi(const BlockMap& bm) {
    std::stringstream sm;

    auto rank = bm.getRank();

    sm<<"{name=\""<<bm.getName();

    auto msep = [rank](int i) { return (i+1 == rank ? "" : ","); };

    sm<<"\",dimension=\"";

    for (auto i = 0; i < rank; i++) {
        const auto& d = bm.getDimension(i);
        sm << d.size << msep(i);
    }

    sm<<"\",distribute=\"";
    for (auto i = 0; i < rank; i++) {
        auto m = msep(i);
        const auto& d = bm.getDimension(i);
        switch (d.distribution) {
        case Distribution::Block:
            sm<<"block"<<m;
            break;
        case Distribution::Cyclic:
            sm<<"cyclic"<<m;
            break;
        case Distribution::Star:
            sm<<"*"<<m;
            break;
        case Distribution::BlockCyclic:
            sm<<d.blockCyclic<<m;
            break;
        default:
            PrintOutError(
                "Invalid distribute definition for decomposition.");
            return "";
        }
    }

    sm<<"\",proc_grid=\"";
    for (auto i = 0; i < rank; i++) {
        const auto& d = bm.getDimension(i);
        if (d.numProcs == 1){
            sm<<"*"<<msep(i);
        }else{
            sm<<d.numProcs<<msep(i);
        }
    }

    sm<<"\",dim_order=\"";
    for (auto i = 0; i < rank; i++) {
        const auto& d = bm.getDimension(i);
        if (d.dim_order == int(Distribution::Star)){
            sm<<"*"<<msep(i);
        }else{
            sm<<d.dim_order<<msep(i);
        }
    }

    sm<<"\"}";

    return sm.str();
}

int cmd_info_decomposition_cmd_mi(MiInterpreter& mi) {
    auto ss = std::ostringstream{};
    
    for (auto m : GetBlockMapList()) {
        std::string printed_blockmap = stringify_blockmap_mi(*m);
        if (printed_blockmap.empty()){
            continue;
        }
        if (ss.tellp()!=0){
            ss<<",";
        }else{
            ss<<"[";
        }
        ss<<printed_blockmap;
    }
    if (ss.tellp()==0)
        ss<<"[";
    ss<<"]";
    mi.set_result("decompositions=%s",ss.str().c_str());
    return Command::Success;
}

/********************************
 * Usage class commands
 ********************************/

static int cmd_usage_dimension_cmd() {
    PrintHelpEntry((char **)cmd_blockmap_dimension_help);

    return 1;
}

static int cmd_usage_distribute_cmd() {
    PrintHelpEntry((char **)cmd_blockmap_distribute_help);

    return 1;
}

static int cmd_usage_proc_grid_cmd() {
    PrintHelpEntry((char **)cmd_blockmap_proc_grid_help);

    return 1;
}

static int cmd_usage_dim_order_cmd() {
    PrintHelpEntry((char **)cmd_blockmap_dim_order_help);

    return 1;
}

/********************************
 * Regular commands
 ********************************/

int cmd_decomposition_cmd(Interpreter& intr, int argc, const char* const argv[]) {
    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: decomposition $decomposition_name\n");
        PrintOutError("  or decomposition $decomposition_name dim1 [, dim2] [, dim3] .. \n");
        return Command::Failure;
    }

    try {
        auto expr = ParseString(argv[1]);

        // verify argv looks like $foo
        if (!dynamic_cast<gram::DSym*>(expr.get())) {
            PrintOutError("Expected: decomposition $decomposition_name\n");
            PrintOutError("  or decomposition $decomposition_name dim1 [, dim2] [, dim3] .. \n");
            return Command::Failure;
        }

        // check if a blockmap with this name already exists
        auto val = expr->eval(nullptr);
        if (auto bm = dynamic_cast<BlockmapValue*>(val.get())) {
            if (!Client.confirm ||
                cli_query("A decomposition has already been defined using "
                          "this name, redefine?",
                          cli_query_default::NO, cli_query_default::YES)) {
                UnregisterBlockMap(bm->getBlockMap()->getName());
            } else {
                return Command::Failure;
            }
        }

        // if the value was a blockmap, it should be gone now. but we still
        // haven't checked if it collides with another symbol.
        if (!IdMap::idIsAvailable(argv[1], true /* print err msg */)) {
            // error message already printed
            return Command::Failure;
        }
    } catch (const std::exception& err) {
        PrintOutError("%s\n", err.what());
        return Command::Failure;
    }

    if (argc == 2) {
        // Old modal form for decomposition definition

        auto* console = dynamic_cast<ConsoleInterpreter*>(&intr);
        if (!console) {
            PrintOutError("Decomposition mode is only supported in the console interpreter.\n");
            return Command::Failure;
        }

        CurrentBlockMap = std::make_shared<BlockMap>(argv[1]);

        console->push_commands(make_blockmap_command_tree());

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

int cmd_blockmap_dimension_cmd(int argc, const char* const argv[]) {
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

int cmd_blockmap_distribute_cmd(int argc, const char* const argv[]) {
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

int cmd_blockmap_proc_grid_cmd(int argc, const char* const argv[]) {
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

int cmd_blockmap_dim_order_cmd(int argc, const char* const argv[]) {
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

int cmd_blockmap_end_cmd(ConsoleInterpreter& console) {
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
    console.pop_commands();

    return 1;
}

std::shared_ptr<CommandNode> make_blockmap_command_tree() {
    // blockmap alt commands
    auto root = std::make_shared<CommandNode>("", nullptr, proc_ignore, nullptr, nullptr, nullptr);

    root->addSubCmd({
        .name = "dimension", .ptype = proc_ignore,
        .fn = cmd_blockmap_dimension_cmd,
        .help = cmd_blockmap_dimension_help,
        .desc = "Provide a dimension definition."});
    root->addSubCmd({
        .name = "distribute", .ptype = proc_ignore,
        .fn = cmd_blockmap_distribute_cmd,
        .help = cmd_blockmap_distribute_help,
        .desc = "Provide a distribute definition."});
    root->addSubCmd({
        .name = "proc_grid", .ptype = proc_ignore,
        .fn = cmd_blockmap_proc_grid_cmd,
        .help = cmd_blockmap_proc_grid_help,
        .desc = "Provide a process grid definition."});
    root->addSubCmd({
        .name = "dim_order", .ptype = proc_ignore,
        .fn = cmd_blockmap_dim_order_cmd,
        .help = cmd_blockmap_dim_order_help,
        .desc = "Provide a dimension order definition."});
    root->addSubCmd({
        .name = "end", .ptype = proc_ignore,
        .fn = cmd_blockmap_end_cmd,
        .help = cmd_blockmap_end_help,
        .desc = "Finish defining the current decomposition scheme."});

    auto dummy_help = []() -> int {
        // help implementation lives in the interpreter,
        // and this function should not actually be called
        return Command::Failure;
    };
    root->addSubCmd({
        .name = "help", .alias = "h", .ptype = proc_ignore,
        .fn = dummy_help, .help = cmd_help_help,
        .desc = "Display help information about decomposition script mode commands."});

    return root;
}

} // anonymous namespace

/********************************
 * command init functions
 ********************************/

void ConsoleCommandsFactory::insert_blockmap_commands(CommandNode& root) {
    root.addSubCmd({
        .name = "decomposition", .ptype = proc_ignore,
        .fn = cmd_decomposition_cmd, .help = cmd_decomposition_help,
        .desc = "Define a decomposition scheme."});
    root.addSubCmd({
        .name = "decomposition dimension", .ptype = proc_ignore,
        .fn = cmd_usage_dimension_cmd,
        .help = cmd_blockmap_dimension_help,
        .desc = "Display help about using decomposition \"dimension\" command."});
    root.addSubCmd({
        .name = "decomposition distribute", .ptype = proc_ignore,
        .fn = cmd_usage_distribute_cmd,
        .help = cmd_blockmap_distribute_help,
        .desc = "Display help about using decomposition \"distribute\" command."});
    root.addSubCmd({
        .name = "decomposition proc_grid", .ptype = proc_ignore,
        .fn = cmd_usage_proc_grid_cmd,
        .help = cmd_blockmap_proc_grid_help,
        .desc = "Display help about using decomposition \"proc_grid\" command."});
    root.addSubCmd({
        .name = "decomposition dim_order", .ptype = proc_ignore,
        .fn = cmd_usage_dim_order_cmd,
        .help = cmd_blockmap_dim_order_help,
        .desc = "Display help about using decomposition \"dim_order\" command."});

    root.addSubCmd({
        .name = "info decomposition", .ptype = proc_ignore,
        .fn = cmd_info_decomposition_cmd_console, .help = cmd_info_decomposition_help,
        .desc = "Display currently defined decomposition schemes."});
}

void MiCommandsFactory::insert_blockmap_commands(CommandNode& root) {
    root.addSubCmd({
        .name = "-decomposition-list", .ptype = proc_ignore,
        .fn = cmd_info_decomposition_cmd_mi });
}
