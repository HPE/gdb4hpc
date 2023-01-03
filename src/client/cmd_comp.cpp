/*********************************************************************************\
 * Compare command.
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
 *********************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

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
        
static int cmd_compare_cmd(rec_set_t *rec_set, int argc, char **argv) {

    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: compare expr1 == expr2\n");
        return 1;
    }

    if (argc <= 0) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: compare expr1 == expr2\n");
        return 1;
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

        if (!Client.MI_mode) {
            res->print(Client.cli_out->ostream(), rec_set, "");
        } else {
            auto os = std::ostringstream{};
            res->print(os, rec_set, "");
            auto val = FormatMIValue(os.str());

            // Front load the result
            auto look = std::string{"Comparison is "};
            if (auto pos = val.find(look); pos != std::string::npos) {
                val.erase(0, pos+look.size());
                val[0] = std::toupper(val[0]);
            }
            PrintOutMIResult(MIResult::DONE,"compare={result=\"%s\"}\n", val.c_str());
        } 
    } catch (EvalError& err) {
        auto msg = std::string{err.what()} + "\n";
        PrintOutError(msg.c_str());
        return 0;
    }

    DPERFLOG(SetTimerStopAndLog("compare"));

    return 1;
}

void InitCmd_comp(void) {
    AddCommand("compare", NULL, proc_optional, 0, cmd_compare_cmd,
               cmd_compare_help, "Compare the contents of two variables.");
}
