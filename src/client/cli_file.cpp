/******************************************************************************\
 * cli_file.c - External file CLI routines.
 *
 * Copyright 2013-2014,2020 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 * $HeadURL$
 * $Date$
 * $Rev$
 * $Author$
 *
 ******************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "client.h"
#include "useful.h"

/*
 * Internal prototypes
 */
static int cli_file_init(cli_stream_t **);
static int cli_file_destroy(cli_stream_t **);
static int cli_file_get_line(cli_stream_t **);
static int cli_file_handle_error(cli_stream_t **);

cli_externals_t *cli_get_file_funcs(void) {
    cli_externals_t *rtn;

    rtn = (cli_externals_t *)Alloc(sizeof(cli_externals_t));

    rtn->external_init = cli_file_init;
    rtn->external_destroy = cli_file_destroy;
    rtn->external_handle_input = cli_file_get_line;
    rtn->external_handle_error = cli_file_handle_error;
    rtn->external_save_stream = NULL;
    rtn->external_restore_stream = NULL;
    rtn->external_reset_stream = NULL;
    rtn->external_clear_line = NULL;

    return rtn;
}

static int cli_file_init(cli_stream_t **stream) {
    cli_stream_t *cs = (*stream);

    // Set the stream members based on the args
    // If any of the paths are null, then we need to use the alt FILE stream

    // set input
    if (cs->cs_file_in_path == NULL) {
        // Use stdin as a last resort
            cs->cs_fpin = stdin;
    } else {
        if ((cs->cs_fpin = fopen(cs->cs_file_in_path, "r")) == NULL) {
            PrintOutError("Could not open %s for input file.\n",
                          cs->cs_file_in_path);
            return 1;
        }
        setlinebuf(cs->cs_fpin);
    }

    // set output
    if (cs->cs_file_out_path == NULL) {
        // Default to the current setting if possible
        // Note that these are not updated until after the call to this function
        // so they are still pointing at the old streams stuff
        if (Client.cli_out) cs->cs_out = *Client.cli_out;
        cs->cs_fpprog = cs->cs_out.getOut();
    } else if (auto out = fopen(cs->cs_file_out_path, "a")) {
        cs->cs_out.setOut(out);
        cs->cs_fpprog = out;
        setlinebuf(out);
                    
    } else {
        PrintOutError("Could not open %s for output file.\n",
                          cs->cs_file_out_path);
        return 1;
    }

    // set error
    if (cs->cs_file_err_path == NULL) {
        // Use stderr as a last resort
        cs->cs_fperr = stderr;
    } else {
        if ((cs->cs_fperr = fopen(cs->cs_file_err_path, "a")) == NULL) {
            PrintOutError("Could not open %s for error file.\n",
                          cs->cs_file_err_path);
            return 1;
        }
        setvbuf(cs->cs_fperr, NULL, _IONBF, 0);
    }

    // set the fd's
    cs->cs_fdin = fileno(cs->cs_fpin);

    return 0;
}

static int cli_file_destroy(cli_stream_t **stream) {
    cli_stream_t *cs = (*stream);

    Free(cs->cs_ext_funcs);

    // figure out which streams we should close based on the args
    if (cs->cs_file_in_path != NULL && cs->cs_fpin != NULL) {
        fclose(cs->cs_fpin);
    }
    cs->cs_out.flush();
    cs->cs_out.close();
    if (cs->cs_file_err_path != NULL && cs->cs_fperr != NULL) {
        fflush(cs->cs_fperr);
        fclose(cs->cs_fperr);
    }

    if (cs->cs_buf != NULL) {
        Free(cs->cs_buf);
    }

    return 0;
}

static int cli_file_get_line(cli_stream_t **stream) {
    cli_stream_t *cs = (*stream);

    // This prevents command output from saving or restoring the stream, since
    // we don't allow input to happen while processing a command.
    cs->cs_handling++;

    // Ensure the buffer is null
    if (cs->cs_buf != NULL) {
        Free(cs->cs_buf);
        cs->cs_buf = NULL;
    }

    if (getline(&(cs->cs_buf), &(cs->cs_buf_size), cs->cs_fpin) < 0) {
        // Reached EOF, reenable input
        cs->cs_handling = 0;
        return 1;
    }

    if (Client.echo == true && cs->cs_buf[0] != '\n' && cs->cs_buf[0] != '\0') {
        // cs->cs_buf has a \n on the end
        PrintOutMsg("%d: %s", cs->cs_line, cs->cs_buf); 
    }

    DbgInterpret(cs->cs_buf);

    // print an error if the err flag is set
    if (cs->cs_err) {
        // cs->cs_buf has a \n on the end
        if (!Client.MI_mode){
            PrintOutError("Command resulted in an error: %d: %s", cs->cs_line, cs->cs_buf); 
        }

        if (Client.keep_going == false) {
            // Reenable input
            cs->cs_handling = 0;

            return 1;
        } else {
            // reset error
            cs->cs_err = 0;
        }
    }

    Free(cs->cs_buf);
    cs->cs_buf = NULL;

    // Indicate command handling finished, reenable input
    cs->cs_handling = 0;

    // increment the line count
    cs->cs_line++;

    return 0;
}

static int cli_file_handle_error(cli_stream_t **stream) {
    // Call the destroy current stream function and restore the previous stream
    DestroyCurrentCLIStream();

    return 0;
}
