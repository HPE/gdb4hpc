/******************************************************************************\
 * cli_funcs.c - Command line interface routines.
 *
 * © 2011-2019 Cray Inc.  All Rights Reserved.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 ******************************************************************************/

#ifdef HAVE_CONFIG_H
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#include	<fcntl.h>
#include	<stdio.h>
#include	<stdarg.h>
#include	<stdlib.h>
#include	<string.h>
#include	<unistd.h>

#include	"client.h"
#include	"useful.h"

/*
** Static internal definitions
*/
static cli_stream_t *	vNewCLIStream(cli_type_t, cli_stream_args_t *, int, va_list);
static cli_stream_t *	NewCLIStream(cli_type_t, cli_stream_args_t *, int, ...);
static void				DestroyCLIStream(cli_stream_t *);
static void				cli_handle_error(void);
static void				ProgOutLine(dbginvoke_t *, FILE *);

/*
 * Static Global Variables
 */
static cli_stream_t *	CurrentStream;	/* Current cli stream */
static list_t *			StreamQueue;	/* Queue of cli streams for nested streams */

void
InitCLI(cli_type_t default_type, cli_stream_args_t *default_args)
{
	cli_stream_t *	stream = NULL;

	// Create the cli stream
	// Note that we set the default prompt information here, that why we have
	// the need for the switch statement
	switch (default_type)
	{
		case CLI_NOTTY_STREAM:
			// There are no prompts for tty-less streams
			stream = NewCLIStream(CLI_NOTTY_STREAM, default_args, 0);
			break;
			
		case CLI_TTY_STREAM:
			stream = NewCLIStream(CLI_TTY_STREAM, default_args, 2, "dbg %s> ", "> ");
			break;
			
		case CLI_SOCKET_STREAM:
			stream = NewCLIStream(CLI_SOCKET_STREAM, default_args, 0);
			break;
		
		case CLI_FILE_STREAM:
			stream = NewCLIStream(CLI_FILE_STREAM, default_args, 0);
			break;
	}
	
	// Make sure the call didn't fail, if so its fatal. We cannot continue without
	// a default stream
	if (stream == NULL)
	{
		PrintOutError("Fatal: InitCLI() failed.\n");
		exit(1);
	}
	
	// set the current stream to the newly created cli stream.
	CurrentStream = stream;
	
	// Set the current IO FILE pointers
	Client.cli_in_stream = CurrentStream->cs_fpin;
	Client.cli_out_stream = CurrentStream->cs_fpout;
	Client.cli_err_stream = CurrentStream->cs_fperr;
	
	// Set the current IO fd's
	Client.cli_in_fd = CurrentStream->cs_fdin;
	Client.cli_out_fd = CurrentStream->cs_fdout;
	Client.cli_err_fd = CurrentStream->cs_fderr;
	
	// Create the stream queue
	StreamQueue = NewList();

	// Install the output callback handler in the dbg_client layer
	DbgHandleOutput(ProgOutLine);
	
	// Install the input callback handler in the dbg_client layer
	DbgHandleInput(Client.cli_in_fd, cli_handle_input);
}

void
DestroyCLI(void)
{
	// Destroy the stream queue
	DestroyList(StreamQueue, (void (*)(void*))DestroyCLIStream);
	
	// Destroy the CurrentStream
	DestroyCLIStream(CurrentStream);
	
	// Null out the global variables
	
	CurrentStream = NULL;
	
	Client.cli_in_stream = NULL;
	Client.cli_out_stream = NULL;
	Client.cli_err_stream = NULL;
	
	Client.cli_in_fd = -1;
	Client.cli_out_fd = -1;
	Client.cli_err_fd = -1;
}

int
SetNewCLIStream(cli_type_t type, cli_stream_args_t *args, int nprom, ...)
{
	va_list			pargs;
	cli_stream_t *	stream;
	
	// clear the current line this should contain a prompt
	cli_clear_line();

	// First create the new stream
	va_start(pargs, nprom);
	
	stream = vNewCLIStream(type, args, nprom, pargs);
	
	va_end(pargs);
	
	// Make sure the call didn't fail
	if (stream == NULL)
	{
		return 1;
	}
	
	// Enqueue the existing command stream
	if (CurrentStream != NULL)
	{
		AddFirst(StreamQueue, (void *)CurrentStream);
	}
	
	// Set the current stream
	CurrentStream = stream;
	
	// Set the current IO FILE pointers
	Client.cli_in_stream = CurrentStream->cs_fpin;
	Client.cli_out_stream = CurrentStream->cs_fpout;
	Client.cli_err_stream = CurrentStream->cs_fperr;
	
	// Set the current IO fd's
	Client.cli_in_fd = CurrentStream->cs_fdin;
	Client.cli_out_fd = CurrentStream->cs_fdout;
	Client.cli_err_fd = CurrentStream->cs_fderr;
	
	// call the stream restore function
	cli_reset_stream();
	
	// Reset the input handler in the dbg_client layer
	DbgHandleInput(Client.cli_in_fd, cli_handle_input);
	
	return 0;
}

void
DestroyCurrentCLIStream(void)
{
	// Destroy the current stream
	if (CurrentStream != NULL)
	{
		// clear the current line this should contain a prompt
		cli_clear_line();
		DestroyCLIStream(CurrentStream);
	}
	
	// Dequeue the top stream
	CurrentStream = (cli_stream_t *)RemoveFirst(StreamQueue);
	
	// Ensure the current stream isn't null, if it is we need to quit
	if (CurrentStream == NULL)
	{
		Client.quit = 1;
		// Remove the input handler
		DbgHandleInput(-1, NULL);
	} else
	{
		// Set the current IO FILE pointers
		Client.cli_in_stream = CurrentStream->cs_fpin;
		Client.cli_out_stream = CurrentStream->cs_fpout;
		Client.cli_err_stream = CurrentStream->cs_fperr;
	
		// Set the current IO fd's
		Client.cli_in_fd = CurrentStream->cs_fdin;
		Client.cli_out_fd = CurrentStream->cs_fdout;
		Client.cli_err_fd = CurrentStream->cs_fderr;

		// call the stream restore function
		cli_reset_stream();
		
		// Reset the input handler in the dbg_client layer
		DbgHandleInput(Client.cli_in_fd, cli_handle_input);
	}
}

void
SetDefaultCLIStream(void)
{
	cli_stream_t *	stream = NULL;

	// We only need to do stuff if the StreamQueue isn't empty
	while (SizeOfList(StreamQueue) != 0)
	{
		stream = (cli_stream_t *)RemoveFirst(StreamQueue);
		
		// Check to see if we just removed the last element
		if (SizeOfList(StreamQueue) == 0)
		{
			// Ensure that stream isn't null, if it is print error
			if (stream == NULL)
			{
				PrintOutError("Error: Could not restore default stream.\n");
				return;
			}
			
			// clear the current line this should contain a prompt
			cli_clear_line();
			
			// Destroy the saved current stream
			DestroyCLIStream(CurrentStream);
			
			// set the current stream to this stream
			CurrentStream = stream;
			
			// Set the current IO FILE pointers
			Client.cli_in_stream = CurrentStream->cs_fpin;
			Client.cli_out_stream = CurrentStream->cs_fpout;
			Client.cli_err_stream = CurrentStream->cs_fperr;
	
			// Set the current IO fd's
			Client.cli_in_fd = CurrentStream->cs_fdin;
			Client.cli_out_fd = CurrentStream->cs_fdout;
			Client.cli_err_fd = CurrentStream->cs_fderr;
		} else
		{
			// destroy it
			DestroyCLIStream(stream);
		}
	}
	
	// Reset the input handler in the dbg_client layer
	DbgHandleInput(Client.cli_in_fd, cli_handle_input);
	
	// call the stream restore function
	// Note that this should always be done. Editline gets messed up if the
	// reset isn't called after signal handling for some reason (it would be in
	// line edit mode).
	cli_reset_stream();
}

static cli_stream_t *
NewCLIStream(cli_type_t type, cli_stream_args_t *args, int nprom, ...)
{
	va_list			pargs;
	cli_stream_t *	rtn;
	
	va_start(pargs, nprom);
	
	rtn = vNewCLIStream(type, args, nprom, pargs);
	
	va_end(pargs);
	
	return rtn;
}

static cli_stream_t *
vNewCLIStream(cli_type_t type, cli_stream_args_t *args, int nprom, va_list pargs)
{
	int				rtn;
	int				n;
	cli_stream_t *	cs;

	cs = (cli_stream_t *)Alloc(sizeof(cli_stream_t));
	// force it to be empty
	memset(cs, 0, sizeof(cli_stream_t));

	// set the type
	cs->cs_type = type;
	// Initialize the fd and fp members, note that it is the external init functions
	// responsibility to set these.
	cs->cs_fdin = -1;
	cs->cs_fdout = -1;
	cs->cs_fderr = -1;
	cs->cs_fpin = NULL;
	cs->cs_fpout = NULL;
	cs->cs_fperr = NULL;
	// prompt information
	cs->cs_nprom = 0;
	cs->cs_plevel = 0;
	cs->cs_prompts = NULL;
	cs->cs_prompt = NULL;
	cs->cs_tmp_prompt = NULL;
	// buffer information
	cs->cs_buf = NULL;
	cs->cs_buf_size = 0;
	cs->cs_buf_point = 0;
	// various state information
	cs->cs_line = 1;
	cs->cs_err = 0;
	cs->cs_saved = 0;
	cs->cs_handling = 0;
	cs->cs_seq = 0;
	cs->cs_in_cmd = 0;
	// list for deferred print
	cs->cs_deferred = NewList();
	
	// Set the prompts if the user supplied any
	if ( nprom > 0 )
	{
		cs->cs_nprom = nprom;
		cs->cs_prompts = (char **)Alloc(sizeof(char *) * nprom);

		for ( n = 0 ; n < nprom ; n++ )
		{
			cs->cs_prompts[n] = strdup(va_arg(pargs, char *));
		}
	}
	
	switch (type)
	{
		case CLI_NOTTY_STREAM:
			// TODO: Create an external definition for no tty if needed.
		case CLI_TTY_STREAM:
			cs->cs_ext_funcs = cli_get_readline_funcs();
			break;
			
		case CLI_SOCKET_STREAM:
			cs->cs_ext_funcs = cli_get_socket_funcs();
			break;
			
		case CLI_FILE_STREAM:
			cs->cs_ext_funcs = cli_get_file_funcs();
			break;
	}
	
	// set the args
	cs->cs_args = args;
	
	// An external init function is required since these set the the fp and fd
	// members of the stream
	if (cs->cs_ext_funcs->external_init == NULL)
	{
		PrintOutError("Error: No external_init() function defined for cli stream type.\n");
		// set the external destroy function to null since it wasn't able to init
		cs->cs_ext_funcs->external_destroy = NULL;
		// destroy the cs
		DestroyCLIStream(cs);
		// return null
		return (cli_stream_t *)NULL;
	}
	
	// call to external init function
	rtn = (*cs->cs_ext_funcs->external_init)(&cs);
	if (rtn)
	{
		PrintOutError("Error: cli external_init() failed.\n");
		// destroy the cs
		DestroyCLIStream(cs);
		// return null
		return (cli_stream_t *)NULL;
	}

	return cs;
}

static void
DestroyCLIStream(cli_stream_t *cs)
{
	int rtn;
	int	n;
	
	if (cs == NULL)
	{
		return;
	}

	if ( cs->cs_prompts != (char **)NULL )
	{
		for ( n = 0 ; n < cs->cs_nprom; n++ )
			Free(cs->cs_prompts[n]);

		Free(cs->cs_prompts);
	}
	
	// call to external destroy function
	if (cs->cs_ext_funcs->external_destroy != NULL)
	{
		rtn = (*cs->cs_ext_funcs->external_destroy)(&cs);
		if (rtn)
		{
			PrintOutError("Warning: external_destroy() failed.\n");
		}
	} else
	{
		// Free it
		Free(cs->cs_ext_funcs);
	}
	
	// Destroy the deferred list
	DestroyList(cs->cs_deferred, (void (*)(void*))Free);
	
	DestroyCLIArgs(cs->cs_type, cs->cs_args);

	Free(cs);
}

cli_stream_args_t *
NewCLIArgs(cli_type_t type, ...)
{
	cli_stream_args_t *	rtn;
	va_list				args;
	char *				tmpstr;
	
	// Create and initialize the return type
	rtn = (cli_stream_args_t *)Alloc(sizeof(cli_stream_args_t));
	memset(rtn, 0, sizeof(cli_stream_args_t));
	
	va_start(args, type);
	
	switch (type)
	{
		case CLI_NOTTY_STREAM:
			rtn->notty_in = va_arg(args, FILE *);
			rtn->notty_out = va_arg(args, FILE *);
			rtn->notty_err = va_arg(args, FILE *);
			break;
			
		case CLI_TTY_STREAM:
			rtn->tty_in = va_arg(args, FILE *);
			rtn->tty_out = va_arg(args, FILE *);
			rtn->tty_err = va_arg(args, FILE *);
			break;
			
		case CLI_SOCKET_STREAM:
			if ((tmpstr = va_arg(args, char *)) != NULL)
			{
				rtn->socket_host = strdup(tmpstr);
			}
			if ((tmpstr = va_arg(args, char *)) != NULL)
			{
				rtn->socket_port = strdup(tmpstr);
			}
			if ((tmpstr = va_arg(args, char *)) != NULL)
			{
				rtn->socket_error_port = strdup(tmpstr);
			}
			break;
		
		case CLI_FILE_STREAM:
			if ((tmpstr = va_arg(args, char *)) != NULL)
			{
				rtn->file_in_path = strdup(tmpstr);
			}
			if ((tmpstr = va_arg(args, char *)) != NULL)
			{
				rtn->file_out_path = strdup(tmpstr);
			}
			if ((tmpstr = va_arg(args, char *)) != NULL)
			{
				rtn->file_err_path = strdup(tmpstr);
			}
			break;
	}
	
	va_end(args);
	
	return rtn;
}

void
DestroyCLIArgs(cli_type_t type, cli_stream_args_t *args)
{
	switch (type)
	{
		case CLI_NOTTY_STREAM:
		case CLI_TTY_STREAM:
			Free(args);
			break;
			
		case CLI_SOCKET_STREAM:
			Free(args->socket_host);
			Free(args->socket_port);
			Free(args->socket_error_port);
			Free(args);
			break;
			
		case CLI_FILE_STREAM:
			if (args->file_in_path != NULL)
				Free(args->file_in_path);
			if (args->file_out_path != NULL)
				Free(args->file_out_path);
			if (args->file_err_path != NULL)
				Free(args->file_err_path);
			Free(args);
			break;
	}
}

void
cli_handle_input(void)
{
	int rtn;
	
	if (CurrentStream == NULL)
		return;
	
	// ensure that there is a handle input function, otherwise we have no way
	// of reading input and should quit
	if (CurrentStream->cs_ext_funcs->external_handle_input == NULL)
	{
		PrintOutError("Fatal: external_handle_input() is null. Avoiding deadlock.\n");
		Client.quit = 1;
		return;
	}

	// call to external handle input function
	rtn = (*CurrentStream->cs_ext_funcs->external_handle_input)(&CurrentStream);
	if (rtn)
	{
		// Call the error handling function to figure out what to do
		cli_handle_error();
	}
}

void
cli_set_error(void)
{
	if (CurrentStream == NULL)
		return;
		
	// set the error condition flag on the CurrentStream
	CurrentStream->cs_err++;
}

static void
cli_handle_error(void)
{
	int rtn;
	
	if (CurrentStream == NULL)
		return;

	// call to external handle error function
	// Note that this may close the CurrentStream and reset it to the
	// DefaultStream.
	if (CurrentStream->cs_ext_funcs->external_handle_error != NULL)
	{
		rtn = (*CurrentStream->cs_ext_funcs->external_handle_error)(&CurrentStream);
		if (rtn)
		{
			PrintOutError("Warning: external_handle_error() failed.\n");
		}
	}
}

void
cli_save_stream(void)
{
	int rtn;
	
	if (CurrentStream == NULL)
		return;

	// not thread safe
	// If we currently have a stream saved or if we are handling a line, return.
	// We don't want to save a stream without first restoring.
	// We don't want to save a stream if we are handling a completed line.
	if (CurrentStream->cs_saved || CurrentStream->cs_handling)
		return;
		
	++CurrentStream->cs_saved;
		
	// we need to disable input processing while we are printing
	DbgMaskInput();
	
	// call to external save stream function
	if (CurrentStream->cs_ext_funcs->external_save_stream != NULL)
	{
		rtn = (*CurrentStream->cs_ext_funcs->external_save_stream)(&CurrentStream);
		if (rtn)
		{
			PrintOutError("Warning: external_save_stream() failed.\n");
		}
	}
}

void
cli_restore_stream(void)
{
	int rtn;
	
	if (CurrentStream == NULL)
		return;
	
	// ensure there is a saved stream and we are not handling a line.
	// If we are handling a line, we need to wait until the line has been
	// parsed and handled before restoring.
	if (!CurrentStream->cs_saved || CurrentStream->cs_handling)
		return;
	
	// call to external restore stream function
	if (CurrentStream->cs_ext_funcs->external_restore_stream != NULL)
	{
		rtn = (*CurrentStream->cs_ext_funcs->external_restore_stream)(&CurrentStream);
		if (rtn)
		{
			PrintOutError("Warning: external_restore_stream() failed.\n");
		}
	}
	
	CurrentStream->cs_saved = 0;
	
	// restore input processing
	DbgUnmaskInput();
}

void
cli_reset_stream(void)
{
	int rtn;
	
	if (CurrentStream == NULL)
		return;
	
	// call to external reset stream function
	if (CurrentStream->cs_ext_funcs->external_reset_stream != NULL)
	{
		rtn = (*CurrentStream->cs_ext_funcs->external_reset_stream)(&CurrentStream);
		if (rtn)
		{
			PrintOutError("Warning: external_reset_stream() failed.\n");
		}
	}
}

void
cli_clear_line(void)
{
	int rtn;
	
	if (CurrentStream == NULL)
		return;
	
	// call to external clear line function
	if (CurrentStream->cs_ext_funcs->external_clear_line != NULL)
	{
		rtn = (*CurrentStream->cs_ext_funcs->external_clear_line)(&CurrentStream);
		if (rtn)
		{
			PrintOutError("Warning: external_clear_line() failed.\n");
		}
	}
}

void
cli_inc_prompt(void)
{
	if (CurrentStream == NULL)
		return;

	if ( CurrentStream->cs_plevel >= CurrentStream->cs_nprom - 1 )
		return;

	CurrentStream->cs_plevel++;
}

void
cli_dec_prompt(void)
{
	if (CurrentStream == NULL)
		return;

	if (CurrentStream->cs_plevel == 0)
		return;

	CurrentStream->cs_plevel--;
}

char *
cli_get_prompt(void)
{
	cli_stream_t *	cs = CurrentStream;
	dbgprocset_t *	s = CurrentProcSet();
	const char *	set_name;
	int				plen = 0;
	
	if (cs == NULL)
		return NULL;
	
	if (cs->cs_nprom <= 0)
		return NULL;

	// free the old prompt
	if (cs->cs_prompt != NULL)
		Free(cs->cs_prompt);
		
	if (s != (dbgprocset_t *)NULL)
		set_name = s->s_name;
	else
		set_name = "none";
		
	plen = strlen(cs->cs_prompts[cs->cs_plevel]) + strlen(set_name) + 1;
	
	// Alloc the new prompt. This will be slightly longer than we actually need if there are
	// format characters in the CommandStream prompt string.
	cs->cs_prompt = (char *)Alloc(sizeof(char)*plen);
	
	// Create the new prompt and return it
	snprintf(cs->cs_prompt, plen, cs->cs_prompts[cs->cs_plevel], set_name);
	
	return cs->cs_prompt;
}

int
cli_query(const char *s)
{
	int	resp;
	
	// Make sure we are currently in confirm mode, if not return true.
	// Also ensure that there is an input stream to read from in the current stream
	if (!Client.confirm || CurrentStream == NULL || CurrentStream->cs_fpin == NULL)
		return 1;
	
	PrintOutMsg("%s (y or n) ", s);
	
	for ( ;; )
	{
		resp = fgetc(CurrentStream->cs_fpin);
		
		if (resp == 'n' || resp == 'N')
		{
			PrintOutMsg("\n");
			return 0;
		}

		if ( resp == 'y' || resp == 'Y' )
			break;

		PrintOutMsg("\nPlease answer y or n:\n");
	}
	
	PrintOutMsg("\n");

	return 1;
}

char *
cli_get_line(void)
{
	char *res = NULL;
	size_t n;
	
	// Make sure we currently have a stream, otherwise reading is impossible
	if (CurrentStream == NULL || CurrentStream->cs_fpin == NULL)
	{
		return NULL;
	}
	
	// read a line of input
	if (getline(&res, &n, CurrentStream->cs_fpin) < 0)
	{
		PrintOutError("getline failed.\n");
		perror("getline");
		return NULL;
	}
	
	return res;
}

int
cli_start_response(int seq)
{
	if (CurrentStream == NULL)
		return 1;
		
	// Set the in_cmd
	CurrentStream->cs_in_cmd = 1;
	
	// Only continue if we are in XML mode.
	if (!Client.XML_mode)
		return 1;
	
	if ((CurrentStream->cs_seq) != seq)
	{
		// The sequence numbers don't match!
		PrintOutError(XML_SYNC, CurrentStream->cs_seq);
		// Respond to the invalid seq num with an empty response
		PrintOutMsg(XML_RESPONSE_START, seq);
		PrintOutMsg(XML_RESPONSE_END);
		if (CurrentStream->cs_fpout != NULL)
			fflush(CurrentStream->cs_fpout);
		return 0;
	}
	
	PrintOutMsg(XML_RESPONSE_START, CurrentStream->cs_seq);
	if (CurrentStream->cs_fpout != NULL)
		fflush(CurrentStream->cs_fpout);
	
	// Increment the seq num
	CurrentStream->cs_seq = ++(CurrentStream->cs_seq) % XML_MAX_SEQ;
	
	return 1;
}

void
cli_end_response(void)
{
	char *	xml_msg;
	
	if (CurrentStream == NULL)
		return;

	// Reset the in_cmd
	CurrentStream->cs_in_cmd = 0;

	// Only continue if we are in XML mode.
	if (!Client.XML_mode)
		return;
		
	PrintOutMsg(XML_RESPONSE_END);
	if (CurrentStream->cs_fpout != NULL)
		fflush(CurrentStream->cs_fpout);
	
	// Print any deferred messages
	while ((xml_msg = (char *)RemoveFirst(CurrentStream->cs_deferred)) != NULL)
	{
		PrintOutMsg("%s", xml_msg);
		Free(xml_msg);
	}
}

char *
XMLStartStatusBlock(char *set_name)
{
	char *	status_block = NULL;
	
	if (!Client.XML_mode || set_name == NULL || CurrentStream == NULL)
		return NULL;
	
	Asprintf(&status_block, XML_STATUS_START, set_name);
	
	return status_block;
}

void
XMLPrintStatusBlock(char *status_block)
{
	char *	full_status_block = NULL;
	
	if (!Client.XML_mode || status_block == NULL || CurrentStream == NULL)
		return;
	
	// Add the closing tag to the block
	Asprintf(&full_status_block, "%s%s", status_block, XML_STATUS_END);
	
	// Cleanup
	Free(status_block);
	
	// Make sure we are not inside a command, otherwise register this as a
	// deferred line to print.
	if (CurrentStream->cs_in_cmd)
	{
		AddToList(CurrentStream->cs_deferred, (void *)full_status_block);
	} else
	{
		PrintOutMsg("%s", full_status_block);
		Free(full_status_block);
	}
}

void
XMLAddStatus(char **status_block, char *set, dbgstat stat, dbgloc_t *location)
{
	const char *func = NULL;
	const char *file = NULL;
	int		line = 0;
	char *	status_str = NULL;
	char *	new_status_block = NULL;

	if (!Client.XML_mode || status_block == NULL || *status_block == NULL || CurrentStream == NULL)
		return;
		
	// Get the location information for status types that contain it
	switch (stat)
	{
		// The following should all have location information
		case DBGSTAT_WAITING:
		case DBGSTAT_STOPPED:
		case DBGSTAT_EXIT:
			if (location != NULL)
			{
				// point at the dbgloc_t members
				func = location->loc_func;
				file = location->loc_file;
				line = location->loc_line;
				
				// Make sure none of these are null pointers
				if (func == NULL)
					func = "null";
				if (file == NULL)
					file = "null";
			} else
			{
				// location is null, something went wrong. Print the status, but
				// omit the location information
				func = "null";
				file = "null";
				line = 0;
			}
			break;
				
		// These do not have location information
		case DBGSTAT_INITIALISING:
		case DBGSTAT_RUNNING:
		case DBGSTAT_DBGERR:
		case DBGSTAT_INTERR:
			break;
	}
	
	// Create the status string based on the stat
	switch (stat)
	{
		case DBGSTAT_INITIALISING:
			Asprintf(&status_str, XML_STATUS_INIT, set);
			break;
		
		case DBGSTAT_WAITING:
			Asprintf(&status_str, XML_STATUS_WAITING, set, func, file, line);
			break;

		case DBGSTAT_STOPPED:
			Asprintf(&status_str, XML_STATUS_STOPPED, set, func, file, line);
			break;

		case DBGSTAT_EXIT:
			Asprintf(&status_str, XML_STATUS_EXIT, set, func, file, line);
			break;

		case DBGSTAT_RUNNING:
			Asprintf(&status_str, XML_STATUS_RUNNING, set);
			break;

		case DBGSTAT_DBGERR:
		case DBGSTAT_INTERR:
			Asprintf(&status_str, XML_STATUS_ERROR, set);
			break;
	}
	
	// Add this status to the status block
	Asprintf(&new_status_block, "%s%s", *status_block, status_str);
	
	// Cleanup
	Free(status_str);
	Free(*status_block);
	
	// Set new status block
	*status_block = new_status_block;
}

void
XMLPrintRemovedMsg(char *set_name)
{
	char *	removed_str = NULL;

	if (!Client.XML_mode || set_name == NULL || CurrentStream == NULL)
		return;
	
	// Create the removed string
	Asprintf(&removed_str, XML_REMOVED, set_name);
	
	// Make sure we are not inside a command, otherwise register this as a
	// deferred line to print.
	if (CurrentStream->cs_in_cmd)
	{
		AddToList(CurrentStream->cs_deferred, (void *)removed_str);
	} else
	{
		PrintOutMsg("%s", removed_str);
		Free(removed_str);
	}
}

void
XMLPrintStartGraphMsg(char *set_name, char *set_str)
{
	char 	*s1, *s2, *s3;

	if (!Client.XML_mode || set_name == NULL || CurrentStream == NULL)
		return;
	
	// Create a new status block
	s1 = XMLStartStatusBlock(set_name);
	
	// Create the graph string
	Asprintf(&s2, XML_STATUS_GRAPH, set_str);
	
	// Add this status to the status block
	Asprintf(&s3, "%s%s", s1, s2);
	
	// Cleanup
	Free(s1);
	Free(s2);
	
	// Now finish the status block
	Asprintf(&s1, "%s%s", s3, XML_STATUS_END);
	
	// Cleanup
	Free(s3);
	
	// Make sure we are not inside a command, otherwise register this as a
	// deferred line to print.
	if (CurrentStream->cs_in_cmd)
	{
		AddToList(CurrentStream->cs_deferred, (void *)s1);
	} else
	{
		PrintOutMsg("%s", s1);
		Free(s1);
	}
}

/*
** Output a program line recieved on a program pipe.
*/
static void
ProgOutLine(dbginvoke_t *di, FILE *o_stream)
{
	char *	line;
	
	// ensure that there is an output stream to print to in the current stream
	if (CurrentStream == NULL || CurrentStream->cs_fpout == NULL)
		return;
	
	line = (char *)Alloc(BUFSIZ);
	
	cli_save_stream();
	
	while (fgets(line, BUFSIZ, o_stream) > 0)
	{
		if (Client.XML_mode)
		{
			fprintf(CurrentStream->cs_fpout, XML_PROG_OUTPUT_START, di->d_handle, (unsigned int)strlen(line));
			fprintf(CurrentStream->cs_fpout, "%s", line);
			fprintf(CurrentStream->cs_fpout, XML_PROG_OUTPUT_END);
		} else
		{
			fprintf(CurrentStream->cs_fpout, "<$%s>: %s", di->d_handle, line);
		}
	}
	
	cli_restore_stream();
	
	Free(line);
}

/*
** fprintf a line to stdout.
*/
void
PrintOutLine(const char *format, ...)
{
	va_list	 arg_list;
	
	// ensure that there is an output stream to print to in the current stream
	if (CurrentStream == NULL || CurrentStream->cs_fpout == NULL)
		return;
	
	cli_save_stream();
	
	// setup the va_list
	va_start(arg_list, format);
	
	// vfprintf the line we recieved.
	vfprintf(CurrentStream->cs_fpout, format, arg_list);
	
	// end the va_list
	va_end(arg_list);
	
	cli_restore_stream();
}

/*
** fprintf a line to stderr.
*/
void
PrintOutError(const char *format, ...)
{
	va_list arg_list;
	
	// setup the va_list
	va_start(arg_list, format);
	
	vPrintOutError(format, arg_list);
	
	// end the va_list
	va_end(arg_list);
}

void
vPrintOutError(const char *format, va_list arg_list)
{
	// we need to disable input processing while we are printing
	DbgMaskInput();
	
	// ensure that there is an error stream to print to in the current stream
	if (CurrentStream == NULL || CurrentStream->cs_fperr == NULL)
	{
		// otherwise default to stderr
		vfprintf(stderr, format, arg_list);
		
		// restore input processing
		DbgUnmaskInput();
		
		return;
	}
	
	// clear the current line this should contain a prompt
	cli_clear_line();
	
	// XXX: If we are currently in a command and running with XML_mode, then 
	//      we want to output the error in an XML format ON STANDARD OUT!!!
	// FIXME: This is a current CCDB GUI requirement. I don't like this, but
	//         its what we have to deal with.
	if (CurrentStream->cs_in_cmd && CurrentStream->cs_fpout && Client.XML_mode)
	{
		// Print out XML error start
		fprintf(CurrentStream->cs_fpout, XML_RESPONSE_ERROR);
		
		// Print out the message
		vfprintf(CurrentStream->cs_fpout, format, arg_list);
		
		// Print out XML error end
		fprintf(CurrentStream->cs_fpout, XML_RESPONSE_ERROR_END);
	} else
	{
		// vfprintf the line we recieved
		vfprintf(CurrentStream->cs_fperr, format, arg_list);
	}
	
	// restore input processing
	DbgUnmaskInput();
}

/*
** fprintf a line to stdout without saving input
*/
void
PrintOutMsg(const char *format, ...)
{
	va_list arg_list;
	
	// setup the va_list
	va_start(arg_list, format);
	
	vPrintOutMsg(format, arg_list);
	
	// end the va_list
	va_end(arg_list);
}

void
vPrintOutMsg(const char *format, va_list arg_list)
{
	// we need to disable input processing while we are printing
	DbgMaskInput();
	
	// ensure that there is an output stream to print to in the current stream
	if (CurrentStream == NULL || CurrentStream->cs_fpout == NULL)
	{
		// otherwise default to stdout
		vfprintf(stdout, format, arg_list);
		
		// restore input processing
		DbgUnmaskInput();
		
		return;
	}
	
	// clear the current line this should contain a prompt
	cli_clear_line();
	
	// vfprintf the line we recieved
	vfprintf(CurrentStream->cs_fpout, format, arg_list);
	
	// restore input processing
	DbgUnmaskInput();
}

// TODO: These are very kludge... Try to rewrite the functions that use this
// to instead call the appropriate stream print functions.

void
PrintToFd(FILE *fd, const char *format, ...)
{
	va_list arg_list;
	
	// setup the va_list
	va_start(arg_list, format);
	
	vPrintToFd(fd, format, arg_list);
	
	// end the va_list
	va_end(arg_list);
}

void
vPrintToFd(FILE *fd, const char *format, va_list arg_list)
{
	// we need to disable input processing while we are printing
	DbgMaskInput();
	
	// clear the current line this should contain a prompt
	cli_clear_line();
	
	// vfprintf the line we recieved
	vfprintf(fd, format, arg_list);
	
	// restore input processing
	DbgUnmaskInput();
}

/*
** fprintf the current saved error string
*/
void
PrintError()
{
	PrintOutError("%s\n", GetError());
}

