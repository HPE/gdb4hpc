/******************************************************************************\
 * cli_readline.c - External readline CLI routines.
 *
 * © 2011-2013 Cray Inc.  All Rights Reserved.
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

#ifdef HAVE_LIBEDITLINE
// These are required for editline support
#ifdef CUSTOM_EDITLINE
#include	"editline/readline.h"
/* CUSTOM_EDITLINE */
#else
#include	<editline/readline.h>
#endif /* !defined(CUSTOM_EDITLINE) */
#endif /* HAVE_LIBEDITLINE */

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
	
	return 0;
}

static int
cli_readline_destroy(cli_stream_t ** stream)
{
	cli_stream_t *	cs = (*stream);

	Free(cs->cs_ext_funcs);
	
	// remove the prompt
	rl_set_prompt("");
	rl_replace_line("", 0);
	rl_redisplay();
	
	// Remove the readline callback handler
	rl_callback_handler_remove();
	
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
	if (!line)
	{
		ForceQuit();
		return;
	}
	
	if (*line)
		add_history(line);
	
	rl_replace_line("", 0);
	
	ExecuteCommand(line);
	Free(line);
	
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

