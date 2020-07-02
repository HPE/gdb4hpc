/*********************************************************************************\
 * Breakpoint commands.
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
    rep_set_t *rep_set = nullptr;
    bp_set_t *bpset;
    dbgbp_t *bp;

    std::string threadSet;

    if (argc < 2) {
        /* get current loc */
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: break main.c:20\n");
        return 1;
    } else if (argc >= 4 &&
               strcmp(argv[2],"thread") == 0) {
        threadSet = ArgsToString(argc-3, argv+3);
    } else if (argc > 2) {
        PrintOutError("Too many command arguments.\n");
        PrintOutError("Expected: break main.c:20\n");
        PrintOutError("or: break main.c:20 thread 2\n");
        return 0;
    }

    // Grab state of brk_pending, we will try setting breakpoint again on error
    PendingBrkType pending_val = Client.brk_pending;

    arg = strdup(argv[1]);

    if ((s = strchr(arg, ':')) != nullptr) {
        file = arg;
        *s++ = '\0';
    } else {
        file = nullptr;
        s = arg;
    }

force_pending_break:

    DPERFLOG(SetTimerStart("setbreak"));

    if (IsInteger(s)) {
        rep_set = DbgSetLineBreak(rec_set, (pending_val == PendingBrkType::On), file, atoi(s), threadSet.c_str());
    } else {
        rep_set = DbgSetFuncBreak(rec_set, (pending_val == PendingBrkType::On), file, s,
                                  threadSet.c_str());
    }

    DPERFLOG(SetTimerStopAndLog("setbreak"));

    // Process replies

    for (auto&& reply : rep_set) {
        // point at the breakpoint data
        bp = EVENT_BP(reply->rep_event);

        bpset = FindBPSetForBP(reply->rep_di, bp);
        if (bpset == nullptr)  // this is not supposed to happen
        {
            PrintErrorWithSetInfo(reply->rep_di->d_handle, reply->rep_group,
                                  "ERROR: cannot find breakpoint [%d: file %s, "
                                  "line %d] in BPSetList.\n",
                                  bp->bp_id, bp->bp_loc.loc_file,
                                  bp->bp_loc.loc_line);
            continue;
        }

        std::string bpThread;
        if (bpset->b_thread_id) {
            bpThread = std::string{" thread "} + bpset->b_thread_id;
        }

        if (bpset->b_bp_pending) {
            PrintMsgWithSetInfo(reply->rep_di->d_handle, reply->rep_group,
                                "Breakpoint %d: Pending at %s%s.\n",
                                bpset->b_bp_set_id, bpset->b_loc_stmt, bpThread.c_str());
        }
        else if (bpset->b_loc_line >= 0) {
            PrintMsgWithSetInfo(reply->rep_di->d_handle, reply->rep_group,
                                "Breakpoint %d: file %s, line %d%s.\n",
                                bpset->b_bp_set_id, bpset->b_loc_file,
                                bpset->b_loc_line, bpThread.c_str());
        }
        else {
            PrintMsgWithSetInfo(reply->rep_di->d_handle, reply->rep_group,
                                "Breakpoint %d in %s%s\n", bpset->b_bp_set_id,
                                bpset->b_loc_func, bpThread.c_str());
        }
    }

    // If we had errors, query the user to set a pending breakpoint if in auto mode
    if (ProcessErrorInRepSet(rep_set) && pending_val == PendingBrkType::Auto) {
        if (cli_query("Make breakpoint pending on future code object load?", cli_query_default::NO)) {
            // Force pending_val to on and try again
            pending_val = PendingBrkType::On;
            FreeRepSet(rep_set);
            goto force_pending_break;
        }
    }

    Free(arg);
    FreeRepSet(rep_set);

    return 1;
}

static int cmd_tbreak_cmd(rec_set_t *rec_set, int argc, char **argv) {
    char *s;
    char *file;
    char *arg;
    rep_set_t *rep_set;
    rep_set_t *enb_rep;
    bp_set_t *bpset;
    dbgbp_t *bp;
    rec_set_t *new_rec;
    std::string threadSet;

    if (argc < 2) {
        /* get current loc */
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: tbreak main.c:20\n");
        return 1;
    } else if (argc >= 4 &&
               strcmp(argv[2],"thread") == 0) {
        threadSet = ArgsToString(argc-3, argv+3);
    } else if (argc > 2) {
        PrintOutError("Too many command arguments.\n");
        PrintOutError("Expected: tbreak main.c:20\n");
        PrintOutError("or: tbreak main.c:20 thread 2\n");
        return 0;
    }

    // Grab state of brk_pending, we will try setting breakpoint again on error
    PendingBrkType pending_val = Client.brk_pending;

    arg = strdup(argv[1]);

    if ((s = strchr(arg, ':')) != nullptr) {
        file = arg;
        *s++ = '\0';
    } else {
        file = nullptr;
        s = arg;
    }

force_pending_tbreak:

    DPERFLOG(SetTimerStart("settbreak"));

    // Set the breakpoint
    if (IsInteger(s)) {
        rep_set = DbgSetLineBreak(rec_set, (pending_val == PendingBrkType::On),
                                  file, atoi(s), threadSet.c_str());
    } else {
        rep_set = DbgSetFuncBreak(rec_set, (pending_val == PendingBrkType::On),
                                  file, s, threadSet.c_str());
    }

    DPERFLOG(SetTimerStopAndLog("settbreak"));

    // Process the replies

    for (auto&& reply : rep_set) {
        // point at the breakpoint data
        bp = EVENT_BP(reply->rep_event);

        // Now set this to a temporary breakpoint using the enable command for
        // this reply group
        new_rec = NewRecSet();
        AddReceiverToSet(new_rec,
                         NewGroupReceiver(reply->rep_di, reply->rep_group));

        bpset = FindBPSetForBP(reply->rep_di, bp);
        if (bpset == nullptr)  // this is not supposed to happen
        {
            PrintErrorWithSetInfo(reply->rep_di->d_handle, reply->rep_group,
                                  "ERROR: cannot find breakpoint [%d: file %s, "
                                  "line %d] in BPSetList.\n",
                                  bp->bp_id, bp->bp_loc.loc_file,
                                  bp->bp_loc.loc_line);
            FreeRecSet(new_rec);
            continue;
        }

        if ((enb_rep = DbgEnableBreak(new_rec, bpset->b_bp_set_id, false, true)) != nullptr) {
            ProcessErrorInRepSet(enb_rep);
            PrintOutError("ERROR: Could not set temporary breakpoint.\n");
            FreeRepSet(enb_rep);
            FreeRecSet(new_rec);
            continue;
        }

        FreeRecSet(new_rec);

        std::string bpThread;
        if (bpset->b_thread_id) {
            bpThread = std::string{" thread "} + bpset->b_thread_id;
        }

        // Print out status
        if (bpset->b_bp_pending) {
            PrintMsgWithSetInfo(reply->rep_di->d_handle, reply->rep_group,
                                "Temporary breakpoint %d: Pending at %s%s.\n",
                                bpset->b_bp_set_id, bpset->b_loc_stmt, bpThread.c_str());
        }
        else if (bpset->b_loc_line >= 0) {
            PrintMsgWithSetInfo(reply->rep_di->d_handle, reply->rep_group,
                                "Temporary breakpoint %d: file %s, line %d%s.\n",
                                bpset->b_bp_set_id, bpset->b_loc_file,
                                bpset->b_loc_line, bpThread.c_str());
        } else {
            PrintMsgWithSetInfo(reply->rep_di->d_handle, reply->rep_group,
                                "Temporary breakpoint %d in %s%s\n",
                                bpset->b_bp_set_id, bpset->b_loc_func,
                                bpThread.c_str());
        }
    }

    // If we had errors, query the user to set a pending breakpoint if in auto mode
    if (ProcessErrorInRepSet(rep_set) && pending_val == PendingBrkType::Auto) {
        if (cli_query("Make breakpoint pending on future code object load?", cli_query_default::NO)) {
            // Force pending_val to on and try again
            pending_val = PendingBrkType::On;
            FreeRepSet(rep_set);
            goto force_pending_tbreak;
        }
    }

    Free(arg);
    FreeRepSet(rep_set);

    return 1;
}

static int cmd_watch_cmd(rec_set_t *rec_set, int argc, char **argv) {

    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: watch foo\n");
        return 1;
    }

    int expIdx = 1;

    dbgbp_type bp_type = DBGBP_WATCH_TYPE;
    if (strcmp(argv[1], "-a") == 0) {
        bp_type = DBGBP_AWATCH_TYPE;
        expIdx = 2;
    } else if (strcmp(argv[1], "-r") == 0) {
        bp_type = DBGBP_RWATCH_TYPE;
        expIdx = 2;
    } else if (strcmp(argv[1], "-w") == 0) {
        bp_type = DBGBP_WATCH_TYPE;
        expIdx = 2;
    }

    int n = argc - expIdx;
    if (n < 1) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: watch %s foo\n", argv[1]);
        return 1;
    } else if (n == 2 || (n == 3 && strcmp(argv[expIdx+1], "thread") != 0) || n > 3) {
        PrintOutError("Too many command arguments.\n");
        PrintOutError("Expected: watch foo\n");
        PrintOutError("      or: watch -a foo\n");
        PrintOutError("      or: watch foo thread 2\n");
        return 1;
    }

    std::string exprStr{argv[expIdx]};
    if (n == 3) {
        exprStr = "\"" + exprStr + " thread " + argv[expIdx+2] + "\"";
    }

    char* expression = strdup(exprStr.c_str());

    rep_set_t *rep_set = DbgSetWatch(rec_set, bp_type, expression);

    Free(expression);

    ProcessErrorInRepSet(rep_set);

    for (auto&& reply : rep_set) {
        // Point at the breakpoint data
        dbgbp_t *bp = EVENT_BP(reply->rep_event);

        if (bp_set_t *bpset = FindBPSetForBP(reply->rep_di, bp)) {
            switch (bpset->b_bp_type) {
            case DBGBP_WATCH_TYPE:
                PrintMsgWithSetInfo(reply->rep_di->d_handle, reply->rep_group,
                                    "Watchpoint %d: Set on variable %s\n",
                                    bpset->b_bp_set_id, bpset->b_bp_exp);
                break;
            case DBGBP_AWATCH_TYPE:
                PrintMsgWithSetInfo(
                    reply->rep_di->d_handle, reply->rep_group,
                    "Access Watchpoint %d: Set on variable %s\n",
                    bpset->b_bp_set_id, bpset->b_bp_exp);
                break;
            case DBGBP_RWATCH_TYPE:
                PrintMsgWithSetInfo(reply->rep_di->d_handle, reply->rep_group,
                                    "Read Watchpoint %d: Set on variable %s\n",
                                    bpset->b_bp_set_id, bpset->b_bp_exp);
                break;
            default:
                PrintMsgWithSetInfo(
                    reply->rep_di->d_handle, reply->rep_group,
                    "Unknown Watchpoint %d: Set on variable %s\n",
                    bpset->b_bp_set_id, bpset->b_bp_exp);
            }
        } else {
            PrintErrorWithSetInfo(reply->rep_di->d_handle, reply->rep_group,
                                  "ERROR: cannot find watchpoint [%d: variable "
                                  "%s] in BPSetList.\n",
                                  bp->bp_id, bp->bp_exp);
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
    if ((rep_set = DbgDeleteBreak(rec_set, bp_id)) != nullptr) {
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
    if ((rep_set = DbgDisableBreak(rec_set, bp_id)) != nullptr) {
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
    if ((rep_set = DbgEnableBreak(rec_set, bp_id, once, delete_cmd)) != nullptr) {
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
    while ((recv_ptr = GetNextReceiverInSet(rec_set)) != nullptr) {
        if (EmptyBPSetList(recv_ptr->rec_di)) {
            PrintMsgWithSetInfo(recv_ptr->rec_di->d_handle, recv_ptr->rec_group,
                                "No breakpoints or watchpoints.\n");
            continue;
        }

        if (bid > 0 && (bpset = FindBPSetByID(recv_ptr->rec_di, bid)) == nullptr) {
            PrintErrorWithSetInfo(
                recv_ptr->rec_di->d_handle, recv_ptr->rec_group,
                "No breakpoint or watchpoint number %d.\n", bid);
            continue;
        }

        ResetBPSetList(recv_ptr->rec_di);
        while ((bpset = GetNextBPSet(recv_ptr->rec_di)) != nullptr) {
            if (bid > 0 && bpset->b_bp_set_id != bid) continue;

            group = bitset_and(bpset->b_group, recv_ptr->rec_group);

            if (bitset_isempty(group)) {
                bitset_free(group);
                continue;
            }

            std::string bpThread;
            if (bpset->b_thread_id)
                bpThread = std::string{" thread "} + bpset->b_thread_id;

            // The info we print is dependent on the type of breakpoint and the
            // enabled/disabled setting and disposition
            switch (bpset->b_bp_type) {
                case DBGBP_WATCH_TYPE:
                case DBGBP_AWATCH_TYPE:
                case DBGBP_RWATCH_TYPE:
                    // loop over the disposition list
                    SetList(bpset->b_bp_disp);
                    while ((disp_set = (bp_disp_set_t *)GetListElement(
                                bpset->b_bp_disp)) != nullptr) {
                        disp_group = bitset_and(disp_set->b_group, group);
                        enabled_group =
                            bitset_and(bpset->b_bp_enabled, disp_group);

                        if (!bitset_isempty(enabled_group)) {
                            // First print out watchpoints that are enabled
                            PrintMsgWithSetInfo(
                                recv_ptr->rec_di->d_handle, enabled_group,
                                "\t%-3d %-14s %-4s %-3s %s%s\n", bpset->b_bp_set_id,
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
                                "\t%-3d %-14s %-4s %-3s %s\n", bpset->b_bp_set_id,
                                BPTypeToStr(bpset->b_bp_type),
                                BPDispToStr(disp_set->b_bp_disp), "n",
                                bpset->b_bp_exp);;
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
                                bpset->b_bp_disp)) != nullptr) {
                        disp_group = bitset_and(disp_set->b_group, group);
                        enabled_group =
                            bitset_and(bpset->b_bp_enabled, disp_group);

                        if (bpset->b_bp_pending) {
                            if (!bitset_isempty(enabled_group)) {
                                // First print out breakpoints that are enabled
                                PrintMsgWithSetInfo(
                                    recv_ptr->rec_di->d_handle, enabled_group,
                                    "\t%-3d %-14s %-4s %-3s at %s%s <PENDING>\n",
                                    bpset->b_bp_set_id,
                                    BPTypeToStr(bpset->b_bp_type),
                                    BPDispToStr(disp_set->b_bp_disp), "y",
                                    bpset->b_loc_stmt, bpThread.c_str());
                            }

                            // Compute the disabled set
                            bitset_invert(enabled_group);
                            disabled_group =
                                bitset_and(enabled_group, disp_group);

                            if (!bitset_isempty(disabled_group)) {
                                // Next print out breakpoints that are disabled
                                PrintMsgWithSetInfo(
                                    recv_ptr->rec_di->d_handle, disabled_group,
                                    "\t%-3d %-14s %-4s %-3s at %s%s <PENDING>\n",
                                    bpset->b_bp_set_id,
                                    BPTypeToStr(bpset->b_bp_type),
                                    BPDispToStr(disp_set->b_bp_disp), "n",
                                    bpset->b_loc_stmt, bpThread.c_str());
                            }
                        }
                        else if (bpset->b_loc_line > 0) {
                            if (!bitset_isempty(enabled_group)) {
                                // First print out breakpoints that are enabled
                                PrintMsgWithSetInfo(
                                    recv_ptr->rec_di->d_handle, enabled_group,
                                    "\t%-3d %-14s %-4s %-3s in %s at %s:%d%s\n",
                                    bpset->b_bp_set_id,
                                    BPTypeToStr(bpset->b_bp_type),
                                    BPDispToStr(disp_set->b_bp_disp), "y",
                                    bpset->b_loc_func, bpset->b_loc_file,
                                    bpset->b_loc_line, bpThread.c_str());
                            }

                            // Compute the disabled set
                            bitset_invert(enabled_group);
                            disabled_group =
                                bitset_and(enabled_group, disp_group);

                            if (!bitset_isempty(disabled_group)) {
                                // Next print out breakpoints that are disabled
                                PrintMsgWithSetInfo(
                                    recv_ptr->rec_di->d_handle, disabled_group,
                                    "\t%-3d %-14s %-4s %-3s in %s at %s:%d%s\n",
                                    bpset->b_bp_set_id,
                                    BPTypeToStr(bpset->b_bp_type),
                                    BPDispToStr(disp_set->b_bp_disp), "n",
                                    bpset->b_loc_func, bpset->b_loc_file,
                                    bpset->b_loc_line, bpThread.c_str());
                            }
                        } else {
                            if (!bitset_isempty(enabled_group)) {
                                // First print out breakpoints that are enabled
                                PrintMsgWithSetInfo(
                                    recv_ptr->rec_di->d_handle, enabled_group,
                                    "\t%-3d %-14s %-4s %-3s in %s%s\n",
                                    bpset->b_bp_set_id,
                                    BPTypeToStr(bpset->b_bp_type),
                                    BPDispToStr(disp_set->b_bp_disp), "y",
                                    bpset->b_loc_func, bpThread.c_str());
                            }

                            // Compute the disabled set
                            bitset_invert(enabled_group);
                            disabled_group =
                                bitset_and(enabled_group, disp_group);

                            if (!bitset_isempty(disabled_group)) {
                                // Next print out breakpoints that are disabled
                                PrintMsgWithSetInfo(
                                    recv_ptr->rec_di->d_handle, disabled_group,
                                    "\t%-3d %-14s %-4s %-3s in %s%s\n",
                                    bpset->b_bp_set_id,
                                    BPTypeToStr(bpset->b_bp_type),
                                    BPDispToStr(disp_set->b_bp_disp), "n",
                                    bpset->b_loc_func, bpThread.c_str());
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

            if (bid >= 0 && bpset->b_bp_set_id == bid) break;
        }
    }

    return 1;
}

static int class_set_break_subcmd_pending_cmd(rec_set_t *rec_set, int argc,
                                                 char **argv) {
    const char *s;
    PendingBrkType pending_val;
    ct_val cv;

    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: set breakpoint pending [auto|on|off]\n");
        return 0;
    }

    // only look at the first arg
    s = argv[1];

    switch (ScanString(&cv, &s)) {
        case CT_AUTO:
            pending_val = PendingBrkType::Auto;
            break;

        case CT_TRUE:
            pending_val = PendingBrkType::On;
            break;

        case CT_FALSE:
            pending_val = PendingBrkType::Off;
            break;

        default:
            PrintOutError("Encountered something other than auto or on or off.\n");
            PrintOutError(
                "See \"help set breakpoint pending\" for more information.\n");
            return 0;
    }

    Client.brk_pending = pending_val;

    return 1;
}

static int class_show_break_subcmd_pending_cmd(rec_set_t *rec_set, int argc,
                                                  char **argv) {
    if (Client.brk_pending == PendingBrkType::Auto)
        PrintOutMsg("Allow setting of pending breakpoints will ask user first. (auto)\n");
    else if (Client.brk_pending == PendingBrkType::On)
        PrintOutMsg("Allow setting of pending breakpoints enabled.\n");
    else
        PrintOutMsg("Allow setting of pending breakpoints disabled.\n");

    return 1;
}

void InitCmd_break(void) {
    AddCommand("break", "b", proc_required, 0, cmd_break_cmd,
               cmd_break_help, "Set breakpoint at specified line or function.");
    AddCommand("delete", "d", proc_required, 0, cmd_delete_cmd,
               cmd_delete_help, "Delete a breakpoint.");
    AddCommand("disable", nullptr, proc_required, 0, cmd_disable_cmd,
               cmd_disable_help, "Disable a breakpoint.");
    AddCommand("enable", nullptr, proc_required, 0, cmd_enable_cmd,
               cmd_enable_help, "Enable a breakpoint.");
    AddCommand("tbreak", nullptr, proc_required, 0, cmd_tbreak_cmd,
               cmd_tbreak_help,
               "Set a temporary breakpoint at specified line or function.");
    AddCommand("watch", nullptr, proc_required, 0, cmd_watch_cmd,
               cmd_watch_help, "Set watchpoint on specified expression.");

    // info commands
    AddInfoCommand("breakpoints", proc_required, 0,
        class_info_subcmd_breakpoints_cmd,
        class_info_subcmd_breakpoints_help,
        "Show status of breakpoints.");

    // set breakpoint commands
    AddSetBreakCommand("pending", proc_ignore, 0,
        class_set_break_subcmd_pending_cmd,
        class_set_break_subcmd_pending_help,
        "Set behavior for pending breakpoints.");

    // show breakpoint commands
    AddShowBreakCommand("pending", proc_ignore, 0,
        class_show_break_subcmd_pending_cmd,
        class_show_break_subcmd_pending_help,
        "Show behavior for pending breakpoints.");
}
