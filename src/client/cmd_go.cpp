/*********************************************************************************\
 * Execution control commands.
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
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "client.h"
#include "useful.h"

static int cmd_continue_console(rec_set_t *rec_set, int argc, const char* const argv[]) {
    bool continueAll = false;
    if (argc == 2 && strcmp(argv[1],"-a") == 0) {
        continueAll = true;
    } else if (argc > 1) {
        PrintOutError("unrecognized command arguments.\n");
        PrintOutError("expected continue or continue -a.\n");
        return Command::Failure;
    }

    if (auto rep_set = DbgGo(rec_set, continueAll); rep_set.hasError()) {
        ProcessErrorInRepSet(rep_set);
        return Command::Failure;
    }

    return Command::Success;
}

static int cmd_finish_console(rec_set_t *rec_set) {
    if (auto rep_set = DbgFinishFunc(rec_set); rep_set.hasError()) {
        ProcessErrorInRepSet(rep_set);
        return Command::Failure;
    }

    return Command::Success;
}

void ConsoleCommandsFactory::insert_go_commands(CommandNode& root) {
    root.addSubCmd(
        "continue", "c", proc_required, cmd_continue_console,
        cmd_continue_help, "Continue execution of application.");

    root.addSubCmd(
        "finish", nullptr, proc_required, cmd_finish_console,
        cmd_finish_help,
        "Execute application until each rank's current function returns.");
}

void MiCommandsFactory::insert_go_commands(CommandNode& root) {
    root.addSubCmd({ .name = "-exec-continue", .ptype = proc_required, .fn = cmd_continue_console });
    root.addSubCmd({ .name = "-exec-return", .ptype = proc_required, .fn = cmd_finish_console });
}
