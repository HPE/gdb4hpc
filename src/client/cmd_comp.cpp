/*********************************************************************************\
 * Compare command.
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

#include "config.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "client.h"
#include "useful.h"
#include "dense_bitset.h"
#include "gram.h"
#include "eval.h"

#include "interpreter_mi.h"

namespace {
    class CachePrintElements {
    public:
        CachePrintElements(rec_set_t* rec_set) :
            m_rec_set(rec_set), m_oldVal(Client.print_elements) {
            ClientSetPrintElements(0, rec_set);
        }
        ~CachePrintElements() {
            ClientSetPrintElements(m_oldVal, m_rec_set);
        }
    private:
        rec_set_t* m_rec_set;
        int m_oldVal;
    };
}

static int cmd_compare_cmd_console(rec_set_t *rec_set, int argc, const char* const argv[]) {
    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: compare expr1 == expr2\n");
        return Command::Success;
    }

    if (argc <= 0) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: compare expr1 == expr2\n");
        return Command::Success;
    }

    auto args = ArgsToString(&argv[1], argc - 1);

    DPERFLOG(SetTimerStart("compare"));
    try {
        auto expr = ParseString(args);

        auto printElems = CachePrintElements(rec_set);

        // The EvalCompare function will read the current values of
        // Client.cmp_lower_eps and Client.cmp_upper_eps to implement epsilon
        // tolerances for floating point values.

        auto res = expr->evalCompare(rec_set);
        res->print(OutMsgStream(), rec_set, "");
    } catch (EvalError& err) {
        auto msg = std::string{err.what()} + "\n";
        PrintOutError("%s", msg.c_str());
        return Command::Failure;
    }

    DPERFLOG(SetTimerStopAndLog("compare"));

    return Command::Success;
}

static int cmd_compare_cmd_mi(MiInterpreter& mi, rec_set_t *rec_set, int argc, const char* const argv[]) {
    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: compare expr1 == expr2\n");
        return Command::Success;
    }

    if (argc <= 0) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: compare expr1 == expr2\n");
        return Command::Success;
    }

    auto args = ArgsToString(&argv[1], argc - 1);

    DPERFLOG(SetTimerStart("compare"));
    try {
        auto expr = ParseString(args);

        auto printElems = CachePrintElements(rec_set);

        // The EvalCompare function will read the current values of
        // Client.cmp_lower_eps and Client.cmp_upper_eps to implement epsilon
        // tolerances for floating point values.

        auto res = expr->evalCompare(rec_set);
        res->setLabelDiffs(false);

        auto os = std::ostringstream{};
        res->print(os, rec_set, "");
        auto val = FormatMIValue(os.str());

        // Front load the result
        auto look = std::string{"Comparison is "};
        if (auto pos = val.find(look); pos != std::string::npos) {
            val.erase(0, pos+look.size());
            val[0] = std::toupper(val[0]);
        }
        mi.set_result("compare={result=\"%s\"}", val.c_str());
    } catch (EvalError& err) {
        auto msg = std::string{err.what()} + "\n";
        PrintOutError("%s", msg.c_str());
        return Command::Failure;
    }

    DPERFLOG(SetTimerStopAndLog("compare"));

    return Command::Success;
}

void ConsoleCommandsFactory::insert_comp_commands(CommandNode& root) {
    root.addSubCmd("compare", nullptr, proc_optional, cmd_compare_cmd_console,
               cmd_compare_help, "Compare the contents of two variables.");
}

void MiCommandsFactory::insert_comp_commands(CommandNode& root) {
    root.addSubCmd({ .name = "-compare", .ptype = proc_optional,
                     .fn = cmd_compare_cmd_mi });
}
