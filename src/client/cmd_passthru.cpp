/******************************************************************************\
 * cmd_passthru.c - Command line interface routines.
 *
 * Copyright 2012-2023 Hewlett Packard Enterprise Development LP.
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

#include "client.h"
#include "useful.h"

#include "interpreter_console.h"

#include <memory>

namespace {

std::shared_ptr<Command> make_gdbmode_command_tree();

int cmd_maint_direct_cmd(Command::LoweredContext<ConsoleInterpreter>&, rec_set_t *rec_set, int argc, const char* const argv[]) {
    if (argc < 2) {
        PrintOutError("Usage: maint passthru <gdb command...>\n");
        return Command::Failure;
    }

    {
        auto rep_set = DbgResetBeforeDirectCmd(rec_set);
        ProcessErrorInRepSet(rep_set);
    }

    auto rep_set = DbgDirectCmd(rec_set, argc - 1, &argv[1]);

    // process error
    ProcessErrorInRepSet(rep_set);

    // process console messages
    for (auto&& reply : rep_set) {
        // Point at the cmd data
        auto* cmd = EVENT_CMD(reply->rep_event);

        // make sure the cmd is not empty
        if (strlen(cmd) == 0) continue;

        PrintMsgWithSetInfo(reply->rep_di.c_str(), reply->rep_group.get(), "%s", cmd);
    }

    DbgRestoreAfterDirectCmd(rec_set);
    ProcessErrorInRepSet(rep_set);

    return 1;
}

int cmd_directmode_cmd(Command::LoweredContext<ConsoleInterpreter>& ctx, rec_set_t *rec_set, int argc, const char* const argv[]) {
    if (argc > 1) {
        return cmd_maint_direct_cmd(ctx, rec_set, argc, argv);
    }

    ctx.caller.push_commands(make_gdbmode_command_tree());

    cli_inc_prompt();

    PrintOutMsg(
        "Entering gdb pass-thru mode. Type \"end\" to exit mode...\n\n");

    {
        auto rep_set = DbgResetBeforeDirectCmd(rec_set);
        ProcessErrorInRepSet(rep_set);
    }

    // This won't work with a "generic" debug backend, but works with gdb.
    const char *tmp_cmd[2] = {"show", "version"};
    auto rep_set = DbgDirectCmd(rec_set, 2, const_cast<char **>(tmp_cmd));

    // process error
    ProcessErrorInRepSet(rep_set);

    // process console messages
    for (auto&& reply : rep_set) {
        // Point at the cmd data
        auto cmd = EVENT_CMD(reply->rep_event);

        // make sure the cmd is not empty
        if (strlen(cmd) == 0) continue;

        if (rep_set.getNumReplies() == 1) {
            PrintOutMsg("%s\n", cmd);
        } else {
            PrintMsgWithSetInfo(reply->rep_di.c_str(), reply->rep_group.get(),
                                "%s\n", cmd);
        }
    }

    return Command::Success;
}

int cmd_directmode_end_cmd(Command::LoweredContext<ConsoleInterpreter>& ctx, rec_set_t *rec_set, int, const char* const []) {
    PrintOutMsg(
        "\nEnding gdb pass-thru mode. If program location has changed (i.e. "
        "continue) debugger is in an unknown state.\n");

    auto rep_set = DbgRestoreAfterDirectCmd(rec_set);
    ProcessErrorInRepSet(rep_set);

    cli_dec_prompt();

    ctx.caller.pop_commands();

    return 1;
}

int cmd_directmode_direct_cmd(Command::RunContext&, rec_set_t *rec_set, int argc, const char* const argv[]) {
    auto rep_set = DbgDirectCmd(rec_set, argc, argv);

    // process error
    ProcessErrorInRepSet(rep_set);

    // process console messages
    for (auto&& reply : rep_set) {
        // Point at the cmd data
        auto* cmd = EVENT_CMD(reply->rep_event);

        // make sure the cmd is not empty
        if (strlen(cmd) == 0) continue;

        if (rep_set.getNumReplies() == 1) {
            PrintOutMsg("%s\n", cmd);
        } else {
            PrintOutMsg("\n");
            PrintMsgWithSetInfo(reply->rep_di.c_str(), reply->rep_group.get(),
                                "\n%s\n", cmd);
        }
    }

    return 1;
}

int cmd_directmode_blacklist_cmd(Command::RunContext&, rec_set_t *, int argc, const char* const argv[]) {
    const auto* this_command = argc > 0 ? argv[0] : "This command";
    PrintOutError("\"%s\" is unsupported in gdbmode. Use \"end\" to exit gdbmode.\n", this_command);
    return Command::Failure;
}

int cmd_quit_cmd(Command::RunContext&, rec_set_t*, int, const char* const []) {
    ForceQuit();
    return Command::Success;
}

std::shared_ptr<Command> make_gdbmode_command_tree() {
    auto root = std::make_shared<Command>(proc_optional, cmd_directmode_direct_cmd);

    // XXX: We can't set help strings for these commands. The help command is
    // passed to the gdb backend while in gdbmode.
    root->addSubCmd({.name = "end", .ptype = proc_ignore,
                     .fn = Command::specify<ConsoleInterpreter>(cmd_directmode_end_cmd)});
    root->addSubCmd({.name = "quit", .alias = "q", .ptype = proc_ignore, .fn = cmd_quit_cmd});
    root->addSubCmd({.name = "run", .alias = "r", .ptype = proc_ignore, .fn = cmd_directmode_blacklist_cmd});
    root->addSubCmd({.name = "detach", .ptype = proc_ignore, .fn = cmd_directmode_blacklist_cmd});
    root->addSubCmd({.name = "kill", .ptype = proc_ignore, .fn = cmd_directmode_blacklist_cmd});

    return root;
}

} // anonymous namespace

void ConsoleCommandsFactory::insert_passthru_commands(Command& root) {
    root.addSubCmd({
        .name = "gdbmode", .ptype = proc_required, .fn = Command::specify<ConsoleInterpreter>(cmd_directmode_cmd),
        .help = cmd_directmode_help, .desc = "Send raw gdb commands."});
}
