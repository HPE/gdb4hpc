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

static std::shared_ptr<Command> DirectModeCmds;
static Command* OldCmds;

static int cmd_maint_direct_cmd(rec_set_t *rec_set, int argc,
                                         char **argv) {
    char *cmd;

    if (argc < 2) {
        PrintOutError("Usage: maint passthru <gdb command...>\n");
        return 0;
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
        cmd = EVENT_CMD(reply->rep_event);

        // make sure the cmd is not empty
        if (strlen(cmd) == 0) continue;

        PrintMsgWithSetInfo(reply->rep_di.c_str(), reply->rep_group.get(), "%s", cmd);
    }

    DbgRestoreAfterDirectCmd(rec_set);
    ProcessErrorInRepSet(rep_set);

    return 1;
}

static int cmd_directmode_cmd(rec_set_t *rec_set, int argc, char **argv) {

    if (rec_set == NULL) {
        PrintOutError("No processes in focus.\n");
        return 0;
    }

    if (argc > 1) {
        return cmd_maint_direct_cmd(rec_set, argc, argv);
    }

    OldCmds = SetCommands(DirectModeCmds.get());

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

    return 1;
}

static int cmd_directmode_end_cmd(rec_set_t *rec_set, int argc,
                                      char **argv) {
    PrintOutMsg(
        "\nEnding gdb pass-thru mode. If program location has changed (i.e. "
        "continue) debugger is in an unknown state.\n");

    auto rep_set = DbgRestoreAfterDirectCmd(rec_set);
    ProcessErrorInRepSet(rep_set);
    
    cli_dec_prompt();
    SetCommands(OldCmds);

    return 1;
}

static int cmd_directmode_direct_cmd(rec_set_t *rec_set, int argc,
                                         char **argv) {

    if (Client.MI_mode) {
        auto directCmds = SetCommands(OldCmds);
        auto command = ArgsToString(argv, argc);
        mi_log("forwarding %s to old interpreter\n", command.c_str());
        ExecuteCommand(command.c_str());
        OldCmds = SetCommands(directCmds);
        return 1;
    }

    char *cmd;

    if (rec_set == NULL) {
        PrintOutError("No processes in focus.\n");
        return 0;
    }

    auto rep_set = DbgDirectCmd(rec_set, argc, argv);

    // process error
    ProcessErrorInRepSet(rep_set);

    // process console messages
    for (auto&& reply : rep_set) {
        // Point at the cmd data
        cmd = EVENT_CMD(reply->rep_event);

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

static int cmd_directmode_interpreter_exec_cmd(rec_set_t *rec_set, int argc,
                                               char **argv) {

    if (argc > 2 || strstr(argv[1], "mi")) {
        return cmd_directmode_direct_cmd(rec_set, argc, argv);
    }
        
    auto command = std::string{};
    if (argc > 3 || argv[2][0] != '"') {
        command = ArgsToString(argv+2, argc-2);
    } else {
        auto len = strlen(argv[2]);
        command = std::string{argv[2]+1, std::max(size_t(2),len)-2};
    }

    mi_log("forwarding directmode %s\n", command.c_str());
    auto mi = false;
    std::swap(Client.MI_mode, mi);
    ExecuteCommand(command.c_str());
    std::swap(Client.MI_mode, mi);
    PrintOutOk();
    return 1;
}

static int cmd_directmode_blacklist_cmd(rec_set_t *rec_set, int argc,
                                            char **argv) {
    if (argc > 0) {
        PrintOutError("%s is unsupported.\n", argv[0]);
    } else {
        PrintOutError("This command is unsupported.\n");
    }

    return 1;
}

class DirectMode : public Command {
public:
    DirectMode();
    void failed() override;
};

DirectMode::DirectMode() : Command(proc_optional, cmd_directmode_direct_cmd) {}

void DirectMode::failed()
{
    cli_dec_prompt();
    SetCommands(OldCmds);
}

void InitCmd_passthru(void) {
    // regular commands
    AddCommand("gdbmode", proc_required, cmd_directmode_cmd,
               cmd_directmode_help, "Send raw gdb commands.");

    // maint commands
    AddCommand(
        "maint passthru", proc_required, cmd_maint_direct_cmd,
        cmd_maint_direct_help, "Pass command to gdb backend.");

    // gdbmode alt commands
    DirectModeCmds = std::make_shared<DirectMode>();

    // XXX: The help strings below are not viewable. The help command is passed
    // to the gdb backend while in gdbmode.
    DirectModeCmds->addSubCmd("end", nullptr, proc_ignore, 
                              cmd_directmode_end_cmd, nullptr, nullptr);
    DirectModeCmds->addSubCmd("quit", nullptr, proc_ignore,
                              cmd_quit_cmd, nullptr, nullptr );
    DirectModeCmds->addSubCmd("run", "r", proc_ignore,
                              cmd_directmode_blacklist_cmd, nullptr, nullptr);
    DirectModeCmds->addSubCmd("detach", nullptr, proc_ignore,
                              cmd_directmode_blacklist_cmd, nullptr, nullptr);
    DirectModeCmds->addSubCmd("kill", nullptr, proc_ignore,
                              cmd_directmode_blacklist_cmd, nullptr, nullptr);
    DirectModeCmds->addSubCmd("interpreter-exec", nullptr, proc_required,
                              cmd_directmode_interpreter_exec_cmd, nullptr, nullptr);
}
