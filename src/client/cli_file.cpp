/******************************************************************************\
 * cli_file.c - External file CLI routines.
 *
 * Copyright 2013-2014,2020,2023 Hewlett Packard Enterprise Development LP.
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
    // If any of the paths are null, then we need to use the fallback stream

    // set input
    if (cs->cs_file_in_path) {
        if ((cs->cs_fpin = fopen(cs->cs_file_in_path, "r")) == NULL) {
            PrintOutError("Could not open %s for input file.\n",
                          cs->cs_file_in_path);
            return 1;
        }
        setlinebuf(cs->cs_fpin);
    } else if (cs->cs_file_fallback_stream && cs->cs_file_fallback_stream->cs_fpin) {
        cs->cs_fpin = cs->cs_file_fallback_stream->cs_fpin;
    } else {
        PrintOutError("No input available.\n");
        return 1;
    }

    // set output
    if (cs->cs_file_out_path) {
        if (auto out = fopen(cs->cs_file_out_path, "a")) {
            cs->cs_out.setOut(out);
            cs->cs_fpprog = out;
            setlinebuf(out);
        } else {
            PrintOutError("Could not open %s for output file.\n",
                          cs->cs_file_out_path);
            return 1;
        }
    } else if (cs->cs_file_fallback_stream) {
        cs->cs_out = cs->cs_file_fallback_stream->cs_out;
        cs->cs_fpprog = cs->cs_out.getOut();
    } else {
        PrintOutError("No output available.\n");
        return 1;
    }

    // set error
    if (cs->cs_file_err_path != NULL) {
        if ((cs->cs_fperr = fopen(cs->cs_file_err_path, "a")) == NULL) {
            PrintOutError("Could not open %s for error file.\n",
                          cs->cs_file_err_path);
            return 1;
        }
        setvbuf(cs->cs_fperr, NULL, _IONBF, 0);
    } else if (cs->cs_file_fallback_stream && cs->cs_file_fallback_stream->cs_fperr) {
        cs->cs_fperr = cs->cs_file_fallback_stream->cs_fperr;
    } else {
        // fall back to output
        cs->cs_fperr = cs->cs_out.getOut();
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

    // Ensure the buffer is null
    if (cs->cs_buf != NULL) {
        ::free(cs->cs_buf);
        cs->cs_buf = NULL;
    }

    if (getline(&(cs->cs_buf), &(cs->cs_buf_size), cs->cs_fpin) < 0) {
        // Reached EOF, reenable input
        return 1;
    }

    if (Client.echo == true && cs->cs_buf[0] != '\n' && cs->cs_buf[0] != '\0') {
        // cs->cs_buf has a \n on the end
        //
        // we use the output stream directly instead of PrintOutMsg so that the output
        // interpreter doesn't format the echoed line - it's required to be plain for testing.
        cs->cs_out("%d: %s", cs->cs_line, cs->cs_buf);
    }

    if (!DbgInterpret(cs->cs_buf)) {
        // cs->cs_buf has a \n on the end
        PrintOutError("Command resulted in an error: %d: %s", cs->cs_line, cs->cs_buf); 

        if (Client.keep_going == false) {
            // Reenable input
            ::free(cs->cs_buf);
            cs->cs_buf = nullptr;

            return 1;
        }
    }

    ::free(cs->cs_buf);
    cs->cs_buf = NULL;

    // increment the line count
    cs->cs_line++;

    return 0;
}

static int cli_file_handle_error(cli_stream_t **) {
    // Call the destroy current stream function and restore the previous stream
    DestroyCurrentCLIStream();

    return 0;
}
