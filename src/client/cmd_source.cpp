/*********************************************************************************\
 * Source command.
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

#include "config.h"

#include <stdio.h>

#include "client.h"
#include "useful.h"

namespace {

int cmd_source_cmd(int argc, const char* const argv[]) {
    const auto [filename, echo, keep_going] = [&]() {
        struct Args {
            const char* filename = nullptr;
            cli_option echo = CLI_INHERIT;
            cli_option keep_going = CLI_INHERIT;
        };

        auto result = Args{};

        // starting at 1 to skip "source"
        for (auto i = 1; i < argc; i++) {
            auto arg = std::string_view{argv[i]};
            if (arg == "--echo") {
                result.echo = CLI_ENABLE;
            } else if (arg == "--no-echo") {
                result.echo = CLI_DISABLE;
            } else if (arg == "--keep-going") {
                result.keep_going = CLI_ENABLE;
            } else if (arg == "--no-keep-going") {
                result.keep_going = CLI_DISABLE;
            } else {
                // interpret anything else as the filename and stop parsing
                result.filename = arg.data();
                break;
            }
        }

        return result;
    }();

    if (!filename) {
        PrintOutError("Usage: source [--echo|--no-echo|--keep-going|--no-keep-going]* <filename>\n");
        return Command::Failure;
    }

    auto* stream_args = NewCLIArgs(CLI_FILE_STREAM, filename, nullptr, nullptr, echo, keep_going);

    // Create and set the new stream
    if (SetNewCLIStream(CLI_FILE_STREAM, interpreter_type::CLI, stream_args)) {
        PrintOutError("Could not create a cli stream from the given file.\n");
        return Command::Failure;
    }

    return Command::Success;
}

Interpreter::Completions cmd_source_completion(
    std::string_view partial_word, std::string_view, const Interpreter&, const dbgprocset_t*, const ArgvStream&)
{
    auto comps = Interpreter::Completions{};
    comps.add_filesystem_names(partial_word);
    return comps;
}

int cmd_set_gdbinit_cmd(int argc, const char* const argv[]) {
    if (argc < 2) {
        PrintOutError("Usage: set gdbinit <true/false/on/off/yes/no>\n");
        return Command::Failure;
    }

    auto s = std::string{argv[1]};
    if (s == "true" || s == "on" || s == "yes") {
        Client.gdbinit = true;
    } else if (s == "false" || s == "off" || s == "no") {
        Client.gdbinit = false;
    } else {
        PrintOutError("Expected: set gdbinit <true/false/on/off/yes/no>\n");
        return Command::Failure;
    }

    return Command::Success;
}

int cmd_show_gdbinit_cmd() {
    if (Client.gdbinit)
        PrintOutMsg("GDB init file enabled.\n");
    else
        PrintOutMsg("GDB init file disabled.\n");

    return Command::Success;
}

} // anonymous namespace

void ConsoleCommandsFactory::insert_source_commands(CommandNode& root) {
    root.addSubCmd({ .name = "source", .ptype = proc_ignore,
                     .fn = cmd_source_cmd, .completion = cmd_source_completion,
                     .help = cmd_source_help, .desc = "Read debugger commands from a file." });

    root.addSubCmd({ .name = "set gdbinit", .ptype = proc_ignore,
                     .fn = cmd_set_gdbinit_cmd, .help = set_gdbinit_help,
                     .desc = "Change if the backend gdbs read the .gdbinit/.gdbearlyinit files." });

    root.addSubCmd({ .name = "show gdbinit", .ptype = proc_ignore,
                     .fn = cmd_show_gdbinit_cmd, .help = set_gdbinit_help /* same as set */,
                     .desc = "Show if the backend gdbs read the .gdbinit/.gdbearlyinit files." });
}
