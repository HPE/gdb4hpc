/*********************************************************************************\
 * Apropos commands.
 *
 * Copyright 2025 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 **********************************************************************************/

#include <stdio.h>

#include "config.h"
#include "client.h"
#include "useful.h"


int cmd_apropos_cmd(int argc, const char* const argv[]) {
    auto search_text = ArgsToString(&argv[1], argc - 1);

    if (!search_text.c_str()) {
        PrintOutError("Usage: apropos <search text>\n");
        return Command::Failure;
    }

    DbgApropos(search_text.c_str());

    return Command::Success;
}

void ConsoleCommandsFactory::insert_apropos_commands(CommandNode& root) {
    root.addSubCmd({ .name = "apropos", .ptype = proc_ignore,
                     .fn = cmd_apropos_cmd, .help = cmd_apropos_help, 
                     .desc = "Read debugger commands from a file." });
}
