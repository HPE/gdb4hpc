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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdio.h>

#include "client.h"
#include "useful.h"

static int cmd_source_cmd(int argc, const char* const argv[]) {
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
    if (SetNewCLIStream(CLI_FILE_STREAM, interpreter_type::CLI, stream_args, 0)) {
        PrintOutError("Could not create a cli stream from the given file.\n");
        return Command::Failure;
    }

    return Command::Success;
}

void ConsoleCommandsFactory::insert_source_commands(CommandNode& root) {
    root.addSubCmd("source", nullptr, proc_ignore, cmd_source_cmd,
               cmd_source_help, "Read debugger commands from a file.");
}
