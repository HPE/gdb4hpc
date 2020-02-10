/*********************************************************************************\
 * Source command.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * © 2010-2020 Cray Inc.
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

#include <stdio.h>

#include "client.h"
#include "useful.h"

static int cmd_source_cmd(rec_set_t *rec_set, int argc, char **argv) {
    cli_stream_args_t *stream_args;

    if (argc < 2) {
        PrintOutError("source command requires pathname of file to source.\n");
        return 0;
    }

    stream_args = NewCLIArgs(CLI_FILE_STREAM, argv[1], NULL, NULL);

    // Create and set the new stream
    if (SetNewCLIStream(CLI_FILE_STREAM, stream_args, 0)) {
        PrintOutError("Could not create a cli stream from the given file.\n");
        return 0;
    }

    return 1;
}

void InitCmd_source(void) {
    AddCommand("source", NULL, proc_ignore, 0, (int (*)())cmd_source_cmd,
               cmd_source_help, "Read debugger commands from a file.");
}
