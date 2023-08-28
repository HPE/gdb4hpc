/*********************************************************************************\
 * Single step commands.
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

#include "client.h"

/*
** This function is able to handle both next and step by examining the first
** character of the command. If its an s, we set in to 1.
*/
static int cmd_stepnext_console(Command::RunContext& ctx, rec_set_t* rec_set, int argc, const char* const argv[]) {
    int in = 0;
    int count = 1;

    if (*argv[0] == 's') in++;

    if (argc >= 2) count = atoi(argv[1]);

    if (auto error_rep_set = DbgStep(rec_set, count, in); error_rep_set.hasError()) {
        (void)ProcessErrorInRepSet(error_rep_set);
        return Command::Failure;
    }

    return Command::Success;
}

static int cmd_stepnext_mi(Command::RunContext& ctx, rec_set_t* rec_set, int argc, const char* const argv[]) {
    int in = 0;
    int count = 1;

    auto command = std::string_view{argv[0]};
    if (command.starts_with("-exec-s")) in++;

    if (argc >= 2) count = atoi(argv[1]);

    if (auto error_rep_set = DbgStep(rec_set, count, in); error_rep_set.hasError()) {
        (void)ProcessErrorInRepSet(error_rep_set);
        // XXX: PrintOutOk is probably wrong for the error case, but is the old behavior.
        // leaving it for now...
        PrintOutOk();
        return Command::Failure;
    }

    // XXX: PrintOutOk should probably be here instead...
    return Command::Success;
}

void ConsoleCommandsFactory::insert_step_commands(Command& root) {
    root.addSubCmd({
        .name = "step", .alias = "s", .ptype = proc_required,
        .fn = cmd_stepnext_console, .help = cmd_step_help,
        .desc = "Step application until it reaches a different source line."});
    root.addSubCmd({
        .name = "next", .alias = "n", .ptype = proc_required,
        .fn = cmd_stepnext_console, .help = cmd_next_help,
        .desc = "Step application, proceeding through subroutine calls."});
}

void MiCommandsFactory::insert_step_commands(Command& root) {
    root.addSubCmd({
        .name = "-exec-step", .ptype = proc_required, .fn = cmd_stepnext_mi });
    root.addSubCmd({
        .name = "-exec-next", .ptype = proc_required, .fn = cmd_stepnext_mi });
}
