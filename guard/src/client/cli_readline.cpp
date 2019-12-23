/******************************************************************************\
 * cli_readline.c - External readline CLI routines.
 *
 * © 2011-2015 Cray Inc.  All Rights Reserved.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
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
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#include	<fcntl.h>
#include	<stdio.h>
#include	<string.h>
#include	<unistd.h>
#include	<stdlib.h>
#include <sys/types.h>
#include <pwd.h>

#include	<editline/readline.h>

#include	"client.h"
#include	"useful.h"

/*
 * Internal prototypes
 */
static void	rl_handle_line(char *);
static int	cli_readline_init(cli_stream_t **);
static int	cli_readline_destroy(cli_stream_t **);
static int	cli_readline_handle_char(cli_stream_t **);
static int	cli_readline_save_stream(cli_stream_t **);
static int	cli_readline_restore_stream(cli_stream_t **);
static int	cli_readline_reset_stream(cli_stream_t **);
static int	cli_readline_clear_line(cli_stream_t **);

cli_externals_t *
cli_get_readline_funcs(void)
{
	cli_externals_t *rtn;
	
	rtn = (cli_externals_t *)Alloc(sizeof(cli_externals_t));
	
	rtn->external_init =			cli_readline_init;
	rtn->external_destroy =			cli_readline_destroy;
	rtn->external_handle_input =	cli_readline_handle_char;
	rtn->external_handle_error =	NULL;	// We don't handle debugger errors for this stream
	rtn->external_save_stream =		cli_readline_save_stream;
	rtn->external_restore_stream =	cli_readline_restore_stream;
	rtn->external_reset_stream =	cli_readline_reset_stream;
	rtn->external_clear_line =		cli_readline_clear_line;
	
	return rtn;
}

static int
cli_readline_init(cli_stream_t ** stream)
{
	cli_stream_t *	cs = (*stream);
	char *			config_loc;
	
	// Set the stream members based on the args
	
	// Note that this function handles both notty and tty right now so we need
	// to make sure we access the proper members
	switch (cs->cs_type)
	{
		case CLI_NOTTY_STREAM:
			cs->cs_fpin = cs->cs_notty_in;
			cs->cs_fpout = cs->cs_notty_out;
			cs->cs_fperr = cs->cs_notty_err;
			break;
			
		case CLI_TTY_STREAM:
			cs->cs_fpin = cs->cs_tty_in;
			cs->cs_fpout = cs->cs_tty_out;
			cs->cs_fperr = cs->cs_tty_err;
			break;
			
		case CLI_SOCKET_STREAM:
		case CLI_FILE_STREAM:
			// shouldn't happen - but catch for completeness
			return 1;
	}
	
	cs->cs_fdin = fileno(cs->cs_fpin);
	cs->cs_fdout = fileno(cs->cs_fpout);
	cs->cs_fderr = fileno(cs->cs_fperr);
	
	// Set the readline input and output streams
	rl_instream = cs->cs_fpin;
	rl_outstream = cs->cs_fpout;

	/*
	** Readline init stuff.
	*/
	rl_callback_handler_install(cli_get_prompt(), rl_handle_line);    // Setup the readline callback handler.
	
	// Setup the history
	using_history();
	
	// Get the default configuration location and read in the history file if possible.
	// We do not care if this fails. Default to /tmp
	if ((config_loc = getenv("GDB4HPC_HIST_DIR")) != NULL)
	{
		char *	histfile;
		Asprintf(&histfile, "%s/gdb4hpc_history", config_loc);
		read_history(histfile);
		Free(histfile);
	} else if ((config_loc = getenv(GUARD_CFG_DIRECTORY)) != NULL)
	{
		char *	histfile;
		Asprintf(&histfile, "%s/gdb4hpc_history", config_loc);
		read_history(histfile);
		Free(histfile);
	}

	// Set history size to 800
	stifle_history(800);
	
	// Read in the editline rc file, this will fail silently.
	struct passwd *pw = getpwuid(getuid());
	if(pw)
	{
		const char *homedir = pw->pw_dir;
		char *init_file_loc;
		Asprintf(&init_file_loc, "%s/.editrc", homedir);
		rl_read_init_file(init_file_loc);
		Free(init_file_loc);
	}
	
	return 0;
}

static int
cli_readline_destroy(cli_stream_t ** stream)
{
	cli_stream_t *	cs = (*stream);
	char *			config_loc;

	Free(cs->cs_ext_funcs);
	
	// remove the prompt
	rl_set_prompt("");
	rl_replace_line("", 0);
	rl_redisplay();
	
	// Remove the readline callback handler
	rl_callback_handler_remove();
	
	// Get the default configuration location and write the history out to file
	if ((config_loc = getenv("GDB4HPC_HIST_DIR")) != NULL)
	{
		char *	histfile;
		Asprintf(&histfile, "%s/gdb4hpc_history", config_loc);
		write_history(histfile);
		Free(histfile);
	}
	else if ((config_loc = getenv(GUARD_CFG_DIRECTORY)) != NULL)
	{
		char *	histfile;
		Asprintf(&histfile, "%s/gdb4hpc_history", config_loc);
		write_history(histfile);
		Free(histfile);
	}
	
	return 0;
}

static int
cli_readline_handle_char(cli_stream_t ** stream)
{
	cli_stream_t *	cs = (*stream);

	// This prevents command output from saving or restoring the stream, since
	// we don't allow input to happen while processing a command.
	cs->cs_handling++;

	rl_callback_read_char();
	
	cs->cs_handling = 0;
	
	// Ignore stream errors for readline - we don't want to handle them in the
	// cli layer
	cs->cs_err = 0;
	
	return 0;
}

static int
cli_readline_save_stream(cli_stream_t ** stream)
{
	cli_stream_t *	cs = (*stream);

	cs->cs_tmp_prompt = cs->cs_prompt;
	cs->cs_prompt = "";
	
	cs->cs_buf_point = rl_point;
	cs->cs_buf = rl_copy_text(0, rl_end);
	rl_set_prompt("");
	rl_replace_line("", 0);
	rl_redisplay();
	
	return 0;
}

static int
cli_readline_restore_stream(cli_stream_t ** stream)
{	
	cli_stream_t *	cs = (*stream);

	cs->cs_prompt = cs->cs_tmp_prompt;

	rl_set_prompt(cs->cs_prompt);
	rl_replace_line(cs->cs_buf, 0);
	rl_point = cs->cs_buf_point;
	rl_redisplay();
	
	Free(cs->cs_buf);
	
	return 0;
}

static int
cli_readline_reset_stream(cli_stream_t ** stream)
{
	rl_prep_terminal(0);
	rl_set_prompt(cli_get_prompt());
	rl_replace_line("", 0);
	rl_redisplay();
	
	return 0;
}

static int
cli_readline_clear_line(cli_stream_t ** stream)
{
	rl_set_prompt("");
	rl_replace_line("", 0);
	rl_redisplay();
	
	return 0;
}

static void
rl_handle_line(char *line)
{
	// If line is NULL, something went very very wrong as readline triggered
	// that there was a complete line of text, but it returned nothing to us.
	if (!line)
	{
		ForceQuit();
		return;
	}
	
	// If there is something other than a null terminator, maybe add it to the
	// history. Note that we do not want to allow repeat history entries.
	if (*line)
	{
		HIST_ENTRY *entry;
		// Get the current history and check if it matches current line
		if ((entry = current_history()) != NULL)
		{
			// If line is not the same length and entry->line, they are different
			// strings
			if (strlen(line) != strlen(entry->line))
			{
				// Add the line to the history
				add_history(line);
			} else
			{
				// compare line to the history line, if true the two differ
				// If they match, do nothing. We don't want repeat history
				// entries since that is annoying to key through.
				if (strncmp(line, entry->line, strlen(entry->line)))
				{
					// Add the line to the history
					add_history(line);
				}
			}
		} else
		{
			// Add the line to the history
			add_history(line);
		}
	}
	
	// replace the current line entry with a blank line. This is so that output
	// will look correct.
	rl_replace_line("", 0);
	
	// execute the command
	ExecuteCommand(line);
	
	// done with line
	Free(line);
	
	// Check for quit condition, on quit we reset prompt to empty.
	if (!Client.quit)
	{
		rl_set_prompt(cli_get_prompt());
		rl_redisplay();
	} else
	{
		rl_set_prompt("");
		rl_redisplay();
	}
}

