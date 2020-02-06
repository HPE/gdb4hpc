/*********************************************************************************\
 * Breakpoint commands.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * © 2010-2019 Cray Inc.  All Rights Reserved.
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
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "client.h"
#include "useful.h"

static int cmd_break_cmd(rec_set_t *rec_set, int argc, char **argv) {
    char *s;
    char *file;
    char *arg;
    rep_set_t *rep_set = NULL;
    group_reply_t *reply = NULL;
    bp_set_t *bpset;
    dbgbp_t *bp;

    if (argc < 2) {
        /* get current loc */
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: break main.c:20\n");
        return 1;
    }

    if (argc > 2) {
        PrintOutError("Too many command arguments.\n");
        PrintOutError("Expected: break main.c:20\n");
        return 0;
    }

    arg = strdup(argv[1]);

    if ((s = strchr(arg, ':')) != NULL) {
        file = arg;
        *s++ = '\0';
    } else {
        file = NULL;
        s = arg;
    }

    DPERFLOG(SetTimerStart("setbreak"));

    if (IsInteger(s)) {
        rep_set = DbgSetLineBreak(rec_set, file, atoi(s));
    } else {
        rep_set = DbgSetFuncBreak(rec_set, file, s);
    }

    DPERFLOG(SetTimerStopAndLog("setbreak"));

    Free(arg);

    ProcessErrorInRepSet(rep_set);

    while ((reply = GetNextReplyInSet(rep_set)) != (group_reply_t *)NULL) {
        // point at the breakpoint data
        bp = EVENT_BP(reply->rep_event);

        bpset = FindBPSetForBP(reply->rep_di, bp);

        if (bpset == NULL)  // this is not supposed to happen
        {
            PrintErrorWithSetInfo(reply->rep_di->d_handle, reply->rep_group,
                                  "ERROR: cannot find breakpoint [%d: file %s, "
                                  "line %d] in BPSetList.\n",
                                  bp->bp_id, bp->bp_loc.loc_file,
                                  bp->bp_loc.loc_line);
            return 0;
        }

        if (bpset->b_loc_line >= 0)
            PrintMsgWithSetInfo(reply->rep_di->d_handle, reply->rep_group,
                                "Breakpoint %d: file %s, line %d.\n",
                                bpset->b_bp_id, bpset->b_loc_file,
                                bpset->b_loc_line);
        else
            PrintMsgWithSetInfo(reply->rep_di->d_handle, reply->rep_group,
                                "Breakpoint %d in %s\n", bpset->b_bp_id,
                                bpset->b_loc_func, bpset->b_loc_line);
    }

    FreeRepSet(rep_set);

    return 1;
}

static int cmd_tbreak_cmd(rec_set_t *rec_set, int argc, char **argv) {
    char *s;
    char *file;
    char *arg;
    rep_set_t *rep_set;
    rep_set_t *enb_rep;
    group_reply_t *reply = NULL;
    bp_set_t *bpset;
    dbgbp_t *bp;
    rec_set_t *new_rec;

    if (argc < 2) {
        /* get current loc */
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: tbreak main.c:20\n");
        return 1;
    }

    if (argc > 2) {
        PrintOutError("Too many command arguments.\n");
        PrintOutError("Expected: tbreak main.c:20\n");
        return 0;
    }

    arg = strdup(argv[1]);

    if ((s = strchr(arg, ':')) != NULL) {
        file = arg;
        *s++ = '\0';
    } else {
        file = NULL;
        s = arg;
    }

    DPERFLOG(SetTimerStart("settbreak"));

    // Set the breakpoint
    if (IsInteger(s)) {
        rep_set = DbgSetLineBreak(rec_set, file, atoi(s));
    } else {
        rep_set = DbgSetFuncBreak(rec_set, file, s);
    }

    Free(arg);

    // process the replies
    ProcessErrorInRepSet(rep_set);

    while ((reply = GetNextReplyInSet(rep_set)) != (group_reply_t *)NULL) {
        // point at the breakpoint data
        bp = EVENT_BP(reply->rep_event);

        // Now set this to a temporary breakpoint using the enable command for
        // this reply group
        new_rec = NewRecSet();
        AddReceiverToSet(new_rec,
                         NewGroupReceiver(reply->rep_di, reply->rep_group));

        bpset = FindBPSetForBP(reply->rep_di, bp);

        if (bpset == NULL)  // this is not supposed to happen
        {
            PrintErrorWithSetInfo(reply->rep_di->d_handle, reply->rep_group,
                                  "ERROR: cannot find breakpoint [%d: file %s, "
                                  "line %d] in BPSetList.\n",
                                  bp->bp_id, bp->bp_loc.loc_file,
                                  bp->bp_loc.loc_line);
            FreeRecSet(new_rec);
            continue;
        }

        if ((enb_rep = DbgEnableBreak(new_rec, bpset->b_bp_id, 0, 1)) != NULL) {
            ProcessErrorInRepSet(enb_rep);
            PrintOutError("ERROR: Could not set temporary breakpoint.\n");
            FreeRepSet(enb_rep);
            FreeRecSet(new_rec);
            continue;
        }

        FreeRecSet(new_rec);

        // Print out status
        if (bpset->b_loc_line >= 0) {
            PrintMsgWithSetInfo(reply->rep_di->d_handle, reply->rep_group,
                                "Temporary breakpoint %d: file %s, line %d.\n",
                                bpset->b_bp_id, bpset->b_loc_file,
                                bpset->b_loc_line);
        } else {
            PrintMsgWithSetInfo(reply->rep_di->d_handle, reply->rep_group,
                                "Temporary breakpoint %d in %s\n",
                                bpset->b_bp_id, bpset->b_loc_func,
                                bpset->b_loc_line);
        }
    }

    DPERFLOG(SetTimerStopAndLog("settbreak"));

    FreeRepSet(rep_set);

    return 1;
}

static int cmd_watch_cmd(rec_set_t *rec_set, int argc, char **argv) {
    char *arg;
    rep_set_t *rep_set = NULL;
    group_reply_t *reply = NULL;
    bp_set_t *bpset;
    dbgbp_t *bp;
    dbgbp_type bp_type;

    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: watch -a foo\n");
        return 1;
    }

    if (argc > 3) {
        PrintOutError("Too many command arguments.\n");
        PrintOutError("Expected: watch -a foo\n");
        return 0;
    }

    // check if we have an "-a", "-r", or "-w" argument.
    if (argc == 3) {
        if (strcmp(argv[1], "-a") == 0) {
            bp_type = DBGBP_AWATCH_TYPE;
            // FIXME: There is a gdb bug that breaks read watchpoints. It will
            // cause
            //         a segfault, so they are disabled for now.
            //} else if ( strcmp(argv[1], "-r") == 0 )
            //{
            //	bp_type = DBGBP_RWATCH_TYPE;
        } else if (strcmp(argv[1], "-w") == 0) {
            bp_type = DBGBP_WATCH_TYPE;
        } else {
            PrintOutError("Unknown command argument %s.\n", argv[1]);
            return 0;
        }

        // FIXME: This should go through the parser...
        arg = strdup(argv[2]);
    } else {
        // default to write watchpoint
        bp_type = DBGBP_WATCH_TYPE;

        // FIXME: This should go through the parser...
        arg = strdup(argv[1]);
    }

    rep_set = DbgSetWatch(rec_set, bp_type, arg);

    Free(arg);

    ProcessErrorInRepSet(rep_set);

    while ((reply = GetNextReplyInSet(rep_set)) != (group_reply_t *)NULL) {
        // Point at the breakpoint data
        bp = EVENT_BP(reply->rep_event);

        bpset = FindBPSetForBP(reply->rep_di, bp);

        if (bpset == NULL)  // this is not supposed to happen
        {
            PrintErrorWithSetInfo(reply->rep_di->d_handle, reply->rep_group,
                                  "ERROR: cannot find watchpoint [%d: variable "
                                  "%s] in BPSetList.\n",
                                  bp->bp_id, bp->bp_exp);
        }

        switch (bpset->b_bp_type) {
            case DBGBP_WATCH_TYPE:
                PrintMsgWithSetInfo(reply->rep_di->d_handle, reply->rep_group,
                                    "Watchpoint %d: Set on variable %s\n",
                                    bpset->b_bp_id, bpset->b_bp_exp);
                break;
            case DBGBP_AWATCH_TYPE:
                PrintMsgWithSetInfo(
                    reply->rep_di->d_handle, reply->rep_group,
                    "Access Watchpoint %d: Set on variable %s\n",
                    bpset->b_bp_id, bpset->b_bp_exp);
                break;
            case DBGBP_RWATCH_TYPE:
                PrintMsgWithSetInfo(reply->rep_di->d_handle, reply->rep_group,
                                    "Read Watchpoint %d: Set on variable %s\n",
                                    bpset->b_bp_id, bpset->b_bp_exp);
                break;
            default:
                PrintMsgWithSetInfo(
                    reply->rep_di->d_handle, reply->rep_group,
                    "Unknown Watchpoint %d: Set on variable %s\n",
                    bpset->b_bp_id, bpset->b_bp_exp);
        }
    }

    FreeRepSet(rep_set);

    return 1;
}

static int cmd_delete_cmd(rec_set_t *rec_set, int argc, char **argv) {
    int bp_id = 0;
    rep_set_t *rep_set;

    if (argc < 2) {
        /*
        ** delete all
        */
        if (!cli_query("Delete all breakpoints in focus?")) return 1;
    } else {
        if (!IsInteger(argv[1])) {
            PrintOutError("Numeric argument required (breakpoint number).\n");
            return 0;
        }

        bp_id = atoi(argv[1]);

        if (bp_id <= 0) {
            PrintOutError("Invalid breakpoint %d.\n", bp_id);
            return 0;
        }
    }

    // The dbg_client layer handles all the of the details here
    if ((rep_set = DbgDeleteBreak(rec_set, bp_id)) != NULL) {
        ProcessErrorInRepSet(rep_set);
        FreeRepSet(rep_set);

        return 0;
    }

    return 1;
}

static int cmd_disable_cmd(rec_set_t *rec_set, int argc, char **argv) {
    int bp_id = 0;
    rep_set_t *rep_set;

    if (argc > 1) {
        if (!IsInteger(argv[1])) {
            PrintOutError("Numeric argument required (breakpoint number).\n");
            return 0;
        }

        bp_id = atoi(argv[1]);
    }

    // The dbg_client layer handles all the of the details here
    if ((rep_set = DbgDisableBreak(rec_set, bp_id)) != NULL) {
        ProcessErrorInRepSet(rep_set);
        FreeRepSet(rep_set);

        return 0;
    }

    return 1;
}

static int cmd_enable_cmd(rec_set_t *rec_set, int argc, char **argv) {
    int argp = 1;
    int once = 0;
    int delete_cmd = 0;
    int bp_id = 0;
    rep_set_t *rep_set;

    // FIXME: Currently GDB has no way to turn a breakpoint that was marked as
    //         delete or disable back to keep. We could solve this by internaly
    //         deleting the original breakpoint and creating a new breakpoint
    //         but this would involve modifying the bp_id_set_t objects in the
    //         bpset. I don't want to do that right now, and besides doing so
    //         would make us "better" than gdb.

    while (argp < argc) {
        if (strcmp(argv[argp], "-once") == 0) {
            ++argp;
            ++once;
        } else if (strcmp(argv[argp], "-delete") == 0) {
            ++argp;
            ++delete_cmd;
        } else if (IsInteger(argv[argp])) {
            bp_id = atoi(argv[argp]);
            ++argp;
        } else {
            PrintOutError("Unknown command argument %s.\n", argv[argp]);
            return 0;
        }
    }

    // you cannot set once and delete at the same time
    if (once && delete_cmd) {
        PrintOutError(
            "The -once and -delete arguments cannot be used at the same "
            "time.\n");
        return 0;
    }

    // The dbg_client layer handles all the of the details here
    if ((rep_set = DbgEnableBreak(rec_set, bp_id, once, delete_cmd)) != NULL) {
        ProcessErrorInRepSet(rep_set);
        FreeRepSet(rep_set);

        return 0;
    }

    return 1;
}

static int class_info_subcmd_breakpoints_cmd(rec_set_t *rec_set, int argc,
                                             char **argv) {
    int bid = 0;
    group_recv_t *recv_ptr;
    bitset *group, *disp_group, *enabled_group, *disabled_group;
    bp_set_t *bpset;
    bp_disp_set_t *disp_set;

    if (argc > 1) {
        if (!IsInteger(argv[1])) {
            PrintOutError("Numeric argument required (breakpoint number).\n");
            return 0;
        }

        bid = atoi(argv[1]);
    }

    PrintOutMsg("Procset\t\t%-3s %-14s %-4s %-3s %s\n", "Num", "Type", "Disp",
                "Enb", "What");

    // Need to loop over each di in the rec_set
    SetReceiverHeadInSet(rec_set);
    while ((recv_ptr = GetNextReceiverInSet(rec_set)) != NULL) {
        if (EmptyBPSetList(recv_ptr->rec_di)) {
            PrintMsgWithSetInfo(recv_ptr->rec_di->d_handle, recv_ptr->rec_group,
                                "No breakpoints or watchpoints.\n");
            continue;
        }

        if (bid > 0 && (bpset = FindBPSetByID(recv_ptr->rec_di, bid)) == NULL) {
            PrintErrorWithSetInfo(
                recv_ptr->rec_di->d_handle, recv_ptr->rec_group,
                "No breakpoint or watchpoint number %d.\n", bid);
            continue;
        }

        ResetBPSetList(recv_ptr->rec_di);
        while ((bpset = GetNextBPSet(recv_ptr->rec_di)) != NULL) {
            if (bid > 0 && bpset->b_bp_id != bid) continue;

            group = bitset_and(bpset->b_group, recv_ptr->rec_group);

            if (bitset_isempty(group)) {
                bitset_free(group);
                continue;
            }

            // The info we print is dependent on the type of breakpoint and the
            // enabled/disabled setting and disposition
            switch (bpset->b_bp_type) {
                case DBGBP_WATCH_TYPE:
                case DBGBP_AWATCH_TYPE:
                case DBGBP_RWATCH_TYPE:
                    // loop over the disposition list
                    SetList(bpset->b_bp_disp);
                    while ((disp_set = (bp_disp_set_t *)GetListElement(
                                bpset->b_bp_disp)) != NULL) {
                        disp_group = bitset_and(disp_set->b_group, group);
                        enabled_group =
                            bitset_and(bpset->b_bp_enabled, disp_group);

                        if (!bitset_isempty(enabled_group)) {
                            // First print out watchpoints that are enabled
                            PrintMsgWithSetInfo(
                                recv_ptr->rec_di->d_handle, enabled_group,
                                "\t%-3d %-14s %-4s %-3s %s\n", bpset->b_bp_id,
                                BPTypeToStr(bpset->b_bp_type),
                                BPDispToStr(disp_set->b_bp_disp), "y",
                                bpset->b_bp_exp);
                        }

                        // Compute the disabled set
                        bitset_invert(enabled_group);
                        disabled_group = bitset_and(enabled_group, disp_group);

                        if (!bitset_isempty(disabled_group)) {
                            // Next print out watchpoints that are disabled
                            PrintMsgWithSetInfo(
                                recv_ptr->rec_di->d_handle, disabled_group,
                                "\t%-3d %-14s %-4s %-3s %s\n", bpset->b_bp_id,
                                BPTypeToStr(bpset->b_bp_type),
                                BPDispToStr(disp_set->b_bp_disp), "n",
                                bpset->b_bp_exp);
                        }

                        // Free the bitsets
                        bitset_free(disp_group);
                        bitset_free(enabled_group);
                        bitset_free(disabled_group);
                    }
                    break;

                case DBGBP_BREAK_TYPE:
                default:
                    // loop over the disposition list
                    SetList(bpset->b_bp_disp);
                    while ((disp_set = (bp_disp_set_t *)GetListElement(
                                bpset->b_bp_disp)) != NULL) {
                        disp_group = bitset_and(disp_set->b_group, group);
                        enabled_group =
                            bitset_and(bpset->b_bp_enabled, disp_group);

                        if (bpset->b_loc_line > 0) {
                            if (!bitset_isempty(enabled_group)) {
                                // First print out breakpoints that are enabled
                                PrintMsgWithSetInfo(
                                    recv_ptr->rec_di->d_handle, enabled_group,
                                    "\t%-3d %-14s %-4s %-3s in %s at %s:%d\n",
                                    bpset->b_bp_id,
                                    BPTypeToStr(bpset->b_bp_type),
                                    BPDispToStr(disp_set->b_bp_disp), "y",
                                    bpset->b_loc_func, bpset->b_loc_file,
                                    bpset->b_loc_line);
                            }

                            // Compute the disabled set
                            bitset_invert(enabled_group);
                            disabled_group =
                                bitset_and(enabled_group, disp_group);

                            if (!bitset_isempty(disabled_group)) {
                                // Next print out breakpoints that are disabled
                                PrintMsgWithSetInfo(
                                    recv_ptr->rec_di->d_handle, disabled_group,
                                    "\t%-3d %-14s %-4s %-3s in %s at %s:%d\n",
                                    bpset->b_bp_id,
                                    BPTypeToStr(bpset->b_bp_type),
                                    BPDispToStr(disp_set->b_bp_disp), "n",
                                    bpset->b_loc_func, bpset->b_loc_file,
                                    bpset->b_loc_line);
                            }
                        } else {
                            if (!bitset_isempty(enabled_group)) {
                                // First print out breakpoints that are enabled
                                PrintMsgWithSetInfo(
                                    recv_ptr->rec_di->d_handle, enabled_group,
                                    "\t%-3d %-14s %-4s %-3s in %s\n",
                                    bpset->b_bp_id,
                                    BPTypeToStr(bpset->b_bp_type),
                                    BPDispToStr(disp_set->b_bp_disp), "y",
                                    bpset->b_loc_func);
                            }

                            // Compute the disabled set
                            bitset_invert(enabled_group);
                            disabled_group =
                                bitset_and(enabled_group, disp_group);

                            if (!bitset_isempty(disabled_group)) {
                                // Next print out breakpoints that are disabled
                                PrintMsgWithSetInfo(
                                    recv_ptr->rec_di->d_handle, disabled_group,
                                    "\t%-3d %-14s %-4s %-3s in %s\n",
                                    bpset->b_bp_id,
                                    BPTypeToStr(bpset->b_bp_type),
                                    BPDispToStr(disp_set->b_bp_disp), "n",
                                    bpset->b_loc_func);
                            }
                        }

                        // Free the bitsets
                        bitset_free(disp_group);
                        bitset_free(enabled_group);
                        bitset_free(disabled_group);
                    }
                    break;
            }

            bitset_free(group);

            if (bid >= 0 && bpset->b_bp_id == bid) break;
        }
    }

    return 1;
}

void InitCmd_break(void) {
    AddCommand("break", "b", proc_required, 0, (int (*)())cmd_break_cmd,
               cmd_break_help, "Set breakpoint at specified line or function.");
    AddCommand("delete", "d", proc_required, 0, (int (*)())cmd_delete_cmd,
               cmd_delete_help, "Delete a breakpoint.");
    AddCommand("disable", NULL, proc_required, 0, (int (*)())cmd_disable_cmd,
               cmd_disable_help, "Disable a breakpoint.");
    AddCommand("enable", NULL, proc_required, 0, (int (*)())cmd_enable_cmd,
               cmd_enable_help, "Enable a breakpoint.");
    AddCommand("tbreak", NULL, proc_required, 0, (int (*)())cmd_tbreak_cmd,
               cmd_tbreak_help,
               "Set a temporary breakpoint at specified line or function.");
    AddCommand("watch", NULL, proc_required, 0, (int (*)())cmd_watch_cmd,
               cmd_watch_help, "Set watchpoint on specified expression.");

    AddInfoCommand("breakpoints", proc_required, 0,
                   (int (*)())class_info_subcmd_breakpoints_cmd,
                   class_info_subcmd_breakpoints_help,
                   "Show status of breakpoints.");
}
