/******************************************************************************\
 * cmd_passthru.c - Command line interface routines.
 *
 * Copyright 2012-2020 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 ******************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <algorithm>

#include "client.h"
#include "useful.h"

static cmdlist_t *DirectModeCmds;
static cmdlist_t *OldCmds;

static int class_maint_subcmd_direct_cmd(rec_set_t *rec_set, int argc,
                                         char **argv) {
    rep_set_t *rep_set;
    char *cmd;

    if (argc < 2) {
        PrintOutError("Usage: maint passthru <gdb command...>\n");
        return 0;
    }

    rep_set = DbgResetBeforeDirectCmd(rec_set);
    ProcessErrorInRepSet(rep_set);
    FreeRepSet(rep_set);

    rep_set = DbgDirectCmd(rec_set, argc - 1, &argv[1]);

    // process error
    ProcessErrorInRepSet(rep_set);

    // process console messages
    for (auto&& reply : rep_set) {
        // Point at the cmd data
        cmd = EVENT_CMD(reply->rep_event);

        // make sure the cmd is not empty
        if (strlen(cmd) == 0) continue;

        PrintMsgWithSetInfo(reply->rep_di->d_handle, reply->rep_group,
                            "GDB CONSOLE OUTPUT:\n\n%s\n\n", cmd);
    }

    FreeRepSet(rep_set);

    DbgRestoreAfterDirectCmd(rec_set);
    ProcessErrorInRepSet(rep_set);
    FreeRepSet(rep_set);

    return 1;
}

static int cmd_directmode_cmd(rec_set_t *rec_set, int argc, char **argv) {
    rep_set_t *rep_set;
    char *cmd;
    const char *tmp_cmd[2] = {"show", "version"};

    if (rec_set == NULL) {
        PrintOutError("No processes in focus.\n");
        return 0;
    }

    OldCmds = SetCommands(DirectModeCmds);

    cli_inc_prompt();

    PrintOutMsg(
        "Entering gdb pass-thru mode. Type \"end\" to exit mode...\n\n");

    rep_set = DbgResetBeforeDirectCmd(rec_set);
    ProcessErrorInRepSet(rep_set);
    FreeRepSet(rep_set);

    // This won't work with a "generic" debug backend, but works with gdb.
    rep_set = DbgDirectCmd(rec_set, 2, const_cast<char **>(tmp_cmd));

    // process error
    ProcessErrorInRepSet(rep_set);

    // process console messages
    for (auto&& reply : rep_set) {
        // Point at the cmd data
        cmd = EVENT_CMD(reply->rep_event);

        // make sure the cmd is not empty
        if (strlen(cmd) == 0) continue;

        if (GetNumberOfReply(rep_set) == 1) {
            PrintOutMsg("%s\n", cmd);
        } else {
            PrintMsgWithSetInfo(reply->rep_di->d_handle, reply->rep_group,
                                "%s\n", cmd);
        }
    }

    FreeRepSet(rep_set);

    return 1;
}

static int cmd_directmode_alt_end_cmd(rec_set_t *rec_set, int argc,
                                      char **argv) {
    PrintOutMsg(
        "\nEnding gdb pass-thru mode. If program location has changed (i.e. "
        "continue) debugger is in an unknown state.\n");

    rep_set_t *rep_set = DbgRestoreAfterDirectCmd(rec_set);
    ProcessErrorInRepSet(rep_set);
    FreeRepSet(rep_set);
    
    cli_dec_prompt();
    SetCommands(OldCmds);

    return 1;
}

static int cmd_directmode_alt_direct_cmd(rec_set_t *rec_set, int argc,
                                         char **argv) {
    char *cmd;
    rep_set_t *rep_set;

    if (rec_set == NULL) {
        PrintOutError("No processes in focus.\n");
        return 0;
    }

    rep_set = DbgDirectCmd(rec_set, argc, argv);

    // process error
    ProcessErrorInRepSet(rep_set);

    // process console messages
    for (auto&& reply : rep_set) {
        // Point at the cmd data
        cmd = EVENT_CMD(reply->rep_event);

        // make sure the cmd is not empty
        if (strlen(cmd) == 0) continue;

        if (GetNumberOfReply(rep_set) == 1) {
            PrintOutMsg("%s\n", cmd);
        } else {
            PrintOutMsg("\n");
            PrintMsgWithSetInfo(reply->rep_di->d_handle, reply->rep_group,
                                "\n%s\n", cmd);
        }
    }

    FreeRepSet(rep_set);

    return 1;
}

static int cmd_directmode_alt_blacklist_cmd(rec_set_t *rec_set, int argc,
                                            char **argv) {
    if (argc > 0) {
        PrintOutError("%s is unsupported.\n", argv[0]);
    } else {
        PrintOutError("This command is unsupported.\n");
    }

    return 1;
}

/********************************
 * Helper functions
 ********************************/

static void directmodenotfound(const char *cmd) { CmdNotFound(cmd); }

static void directmodeambig(const char *cmd) {
    AmbiguousAltCmd(DirectModeCmds, cmd);
}

static void directmodefailed(void) {
    cli_dec_prompt();
    SetCommands(OldCmds);
}

void InitCmd_passthru(void) {
    // regular commands
    AddCommand("gdbmode", NULL, proc_required, 0, cmd_directmode_cmd,
               cmd_directmode_help, "Enter direct gdb mode (experimental).");

    // maint commands
    AddMaintCommand(
        "passthru", proc_required, 0, class_maint_subcmd_direct_cmd,
        class_maint_subcmd_direct_help, "Pass command to gdb backend.");

    // gdbmode alt commands
    DirectModeCmds =
        NewCommandList(directmodenotfound, directmodeambig, directmodefailed);

    // XXX: The help strings below are not viewable. The help command is passed
    // to the gdb backend while in gdbmode.
    AddAltCommand(DirectModeCmds, "_default_cmd", NULL, proc_required, 0,
                  cmd_directmode_alt_direct_cmd, dummy_help, "");
    AddAltCommand(DirectModeCmds, "end", NULL, proc_ignore, 0,
                  cmd_directmode_alt_end_cmd, dummy_help, "");
    AddAltCommand(DirectModeCmds, "quit", NULL, proc_ignore, 0,
                  cmd_quit_cmd, dummy_help, "");

    AddAltCommand(DirectModeCmds, "run", "r", proc_ignore, 0,
                  cmd_directmode_alt_blacklist_cmd, dummy_help, "");
    AddAltCommand(DirectModeCmds, "detach", NULL, proc_ignore, 0,
                  cmd_directmode_alt_blacklist_cmd, dummy_help, "");
    AddAltCommand(DirectModeCmds, "kill", NULL, proc_ignore, 0,
                  cmd_directmode_alt_blacklist_cmd, dummy_help, "");

    SetDefaultCommand(DirectModeCmds, "_default_cmd");
}
