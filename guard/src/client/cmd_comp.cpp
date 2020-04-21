/*********************************************************************************\
 * Compare command.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2020 Cray Inc.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
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

static int cmd_compare_cmd(rec_set_t *rec_set, int argc, char **argv) {
    char *buf;
    val_t *val_res = NULL;
    val_t *val_diff = NULL;
    int res;
    rep_set_t *rep_set;
    group_reply_t *reply;
    expr_t *e;
    char **ap;
    AIFStream *a;
    int print_diff = 0;
    bitset *true_bitset = NULL;
    bitset *false_bitset = NULL;
    bitset *warn_bitset = NULL;
    bitset *indet_bitset = NULL;

    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: compare expr1 == expr2\n");
        return 1;
    }

    ap = &argv[1];

    if (argc <= 0) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: compare expr1 == expr2\n");
        return 1;
    }

    buf = ArgsToBuf(ap, argc - 1);

    if ((e = ParseExpr(buf)) == (expr_t *)NULL) {
        PrintError();
        Free(buf);
        return 0;
    }

    // done with buf
    Free(buf);

    DPERFLOG(SetTimerStart("compare"));

    if (EvalCompare(&val_res, &val_diff, e, rec_set) < 0) {
        PrintError();
        FreeExpr(e);
        return 0;
    }

    DPERFLOG(SetTimerStopAndLog("compare"));

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

    // Process the result val based on its rval type
    switch (RVAL_TYPE(val_res)) {
        case rval_aif:
            // point at the aif result
            a = RVAL_AIF(val_res);
            // Set res to the comparison result
            a->toInt(res);
            switch (res) {
                case 0:
                    PrintOutMsg("Comparison is true.\n");
                    break;
                case -1:
                case 1:
                    PrintOutMsg(
                        "Comparison is false. The difference delta is:\n");
                    if (PrintVal(val_diff, rec_set) < 0) PrintError();
                    break;
                case -2:
                case 2:
                    PrintOutMsg(
                        "Comparison is warn. The difference delta is:\n");
                    if (PrintVal(val_diff, rec_set) < 0) PrintError();
                    break;
                case 3:
                    PrintOutMsg(
                        "Comparison is indeterminate due to pointers.\n");
                    break;
            }
            break;

        case rval_reply:
            // Point at the reply set
            rep_set = RVAL_REPLY(val_res);

            // Fix for BUG 817334 - This used to print the results of the comp
            // operation directly, but now it will try to group like messages
            // together and only print a summary over all the ranks.

            // Need to loop over the reply
            SetReplyHeadInSet(rep_set);
            while ((reply = GetNextReplyInSet(rep_set)) !=
                   (group_reply_t *)NULL) {
                // point at the aif result
                a = EVENT_DATA(reply->rep_event);
                // Set res to the comparison result
                a->toInt(res);
                switch (res) {
                    case 0:
                        if (true_bitset == NULL) {
                            // create a new true bitset based off the reply
                            // group. Note that every reply will have the same
                            // set size.
                            true_bitset =
                                bitset_new(bitset_size(reply->rep_group));
                        }
                        // Add this group to the true_bitset
                        bitset_oreq(true_bitset, reply->rep_group);
                        break;
                    case -1:
                    case 1:
                        if (false_bitset == NULL) {
                            // create a new false bitset based off the reply
                            // group. Note that every reply will have the same
                            // set size.
                            false_bitset =
                                bitset_new(bitset_size(reply->rep_group));
                        }
                        // Add this group to the false_bitset
                        bitset_oreq(false_bitset, reply->rep_group);
                        print_diff++;
                        break;
                    case -2:
                    case 2:
                        if (warn_bitset == NULL) {
                            // create a new warn bitset based off the reply
                            // group. Note that every reply will have the same
                            // set size.
                            warn_bitset =
                                bitset_new(bitset_size(reply->rep_group));
                        }
                        // Add this group to the warn_bitset
                        bitset_oreq(warn_bitset, reply->rep_group);
                        print_diff++;
                        break;
                    case 3:
                        if (indet_bitset == NULL) {
                            // create a new indeterminate bitset based off the
                            // reply group. Note that every reply will have the
                            // same set size.
                            indet_bitset =
                                bitset_new(bitset_size(reply->rep_group));
                        }
                        // Add this group to the warn_bitset
                        bitset_oreq(indet_bitset, reply->rep_group);
                        break;
                }
            }

            // XXX: Note that for the PrintMsgWithSetInfo command we don't want
            // to use the handle in the set,
            //      since that is not really a valid representation of what we
            //      are looking at. What we have is a delta between the two
            //      sets, and since a di was needed to be set in the reply, it
            //      just arbitrarily chose the first one. This is a flaw and
            //      someday should be fixed. For now just use the string ""

            // print the result summary
            if (true_bitset != NULL) {
                PrintMsgWithSetInfo("", true_bitset, "Comparison is true.\n");
                bitset_free(true_bitset);
            }
            if (false_bitset != NULL) {
                PrintMsgWithSetInfo("", false_bitset, "Comparison is false.\n");
                bitset_free(false_bitset);
            }
            if (warn_bitset != NULL) {
                PrintMsgWithSetInfo("", warn_bitset, "Comparison is warn.\n");
                bitset_free(warn_bitset);
            }
            if (indet_bitset != NULL) {
                PrintMsgWithSetInfo(
                    "", indet_bitset,
                    "Comparison is indeterminate due to pointers.\n");
                bitset_free(indet_bitset);
            }

            // print out the difference deltas once
            if (print_diff && val_diff != NULL) {
                PrintOutMsg("The difference delta is:\n");
                if (PrintVal(val_diff, rec_set) < 0) PrintError();
            }
            break;

        default:
            // shouldn't get here
            break;
    }

    if (val_diff != NULL) DestroyVal(val_diff);

    if (val_res != NULL) DestroyVal(val_res);

    return 1;
}

void InitCmd_comp(void) {
    AddCommand("compare", NULL, proc_optional, 0, (int (*)())cmd_compare_cmd,
               cmd_compare_help, "Compare the contents of two variables.");
}
