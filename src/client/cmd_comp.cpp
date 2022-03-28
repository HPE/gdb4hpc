/*********************************************************************************\
 * Compare command.
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
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "client.h"
#include "useful.h"
#include "dense_bitset.h"
#include "compare_util.h"

static int cmd_compare_cmd(rec_set_t *rec_set, int argc, char **argv) {
    char *buf;
    val_t *val_res = nullptr;
    val_t *val_diff = nullptr;
    expr_t *e;

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

    buf = ArgsToBuf(&argv[1], argc - 1);

    if ((e = ParseExpr(buf)) == (expr_t *)nullptr) {
        PrintError();
        Free(buf);
        return 0;
    }

    // done with buf
    Free(buf);

    // FIXME: this is inefficient, but since the print elements setting affects
    // what gets sent back, we need to set/reset it before evaluating a compare
    //
    // set print elements to unlimited
    int oldPrintElements = Client.print_elements;
    ClientSetPrintElements(0, rec_set);

    DPERFLOG(SetTimerStart("compare"));

    if (EvalCompare(&val_res, &val_diff, e, rec_set) < 0) {
        PrintError();
        FreeExpr(e);
        return 0;
    }

    DPERFLOG(SetTimerStopAndLog("compare"));

    // set print elements back to client setting
    ClientSetPrintElements(oldPrintElements, rec_set);

    FreeExpr(e);

    /*
    ** The EvalCompare function will read the current values of
    *Client.cmp_lower_eps
    ** and Client.cmp_upper_eps to implement epsilon tolerances for floating
    *point
    ** values.
    **
    ** TODO: We should save the difference data instead of displaying it
    **       similar to what graph does with info diff.
    */

    printCompareResult(val_res, val_diff, *rec_set);

    if (val_diff != nullptr) DestroyVal(val_diff);

    if (val_res != nullptr) DestroyVal(val_res);

    return 1;
}

void InitCmd_comp(void) {
    AddCommand("compare", NULL, proc_optional, 0, cmd_compare_cmd,
               cmd_compare_help, "Compare the contents of two variables.");
}
