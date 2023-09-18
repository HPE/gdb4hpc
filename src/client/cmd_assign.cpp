/*********************************************************************************\
 * Assign command.
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

#include <signal.h>
#include <stdio.h>
#include <string.h>

#include "client.h"
#include "useful.h"
#include "gram.h"

/*
** TODO: array assignment.
*/
static int cmd_assign_cmd(Command::RunContext&, rec_set_t *rec_set, int argc, const char* const argv[]) {
    if (argc < 3) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: assign $target 20\n");
        PrintOutError(
            "Where $target is the name of the target variable and the "
            "argument\n");
        PrintOutError("is the value to assign to the target.\n");
        return Command::Failure;
    }

    try {

        auto varExpr = ParseString(argv[1]);
        auto val = varExpr->eval(rec_set);

        auto rv = dynamic_cast<ReplyValue*>(val.get());
        if (!rv) {
            PrintOutError("Only assignment to program variables is supported.\n");
            return Command::Failure;
        }

        auto value = ArgsToString(&argv[2], argc - 2);

        // run through the parser just to check for validity
        auto valExpr = ParseString(value);

        auto rep_set = DbgSetVar(rv->getRecSet(), rv->getExpr().c_str(), value.c_str());

        ProcessErrorInRepSet(rep_set);
        
    } catch (EvalError& err) {
        PrintOutError("Could not parse expression: %s\n", err.what());
        return Command::Failure;
    }

    return Command::Success;
}

void ConsoleCommandsFactory::insert_assign_commands(Command& root) {
    root.addSubCmd("assign", nullptr, proc_optional,
                   cmd_assign_cmd, cmd_assign_help,
                   "Change the value of an application or debugger variable.");
}
