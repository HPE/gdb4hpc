/*
** Execution control commands.
**
** Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
**
*/

#ifdef HAVE_CONFIG_H
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#include	<stdio.h>
#include	<signal.h>
#include	<stdarg.h>

#include	"client.h"
#include	"useful.h"
#include	"dbg_set_sync.h"
#include	"dbg_set_async.h"

static int 
cmd_continue_cmd(rec_set_t *rec_set, int argc, char **argv)
{
	invoke_t *	invoke = rec_set->s_invoke_handler;
	rep_set_t *	rep_set;

	DPERFLOG(SetTimerStart("cont"));
	
	RemoveEventSet(invoke->i_tree, DBGEV_BPHIT);

	if ((rep_set = DbgGo(rec_set)) != NULL)
	{
		ProcessErrorInRepSet(rec_set, rep_set);
		FreeRepSet(rep_set);
		return 0;
	}
	
	DPERFLOG(SetTimerStopAndLog("cont"));

	return 1;
}

// Not used on Cray platform.
#if 0
static int 
cmd_run_cmd(rec_set_t *rec_set, int argc, char **argv)
{
	char *		buf;
	dbgstat		stat;

	char*	setname = rec_set->s_setname;
	bitset* receiver = rec_set->s_receiver_bs;
	invoke_t*	invoke = rec_set->s_invoke_handler;

	// FIXME: If this is ever used again, update this to handle the new status
	// format
	if ( (stat = DbgStatus(rec_set)) == DBGSTAT_INTERR )
	{
		DbgProcessError(rec_set);
		return 0;
	}

	if ( stat == DBGSTAT_STOPPED )
	{
		PrintMsgWithSetInfo(Client.cli_err_stream, setname, receiver, "The program being debugged has been started already.\n");
		if ( !cli_query("Start it from the beginning?") )
		{
			return 1;
		}

		if ( DbgKill(rec_set) < 0 )
		{
			DbgProcessError(rec_set);
		}
	}
	else if ( stat != DBGSTAT_WAITING )
	{
		PrintMsgWithSetInfo(Client.cli_err_stream, setname, receiver, "Can't run the program.\n");
		return 0;
	}

	if ( argc > 1 )
	{
		buf = ArgsToBuf(&argv[1], argc-1);

		if ( DbgSetArgs(rec_set, buf) < 0 )
		{
			DbgProcessError(rec_set);
			Free(buf);
			return 0;
		}
		
		// done with buf
		Free(buf);
	}

	PrintMsgWithSetInfo(Client.cli_out_stream, setname, receiver, "Starting program: %s\n\n", invoke->i_cmd);

	DPERFLOG(SetTimerStart("run"));
	
	RemoveAllEventSet(invoke->i_tree);

	// FIXME: This command isn't used this way anymore, fix if needed
	if ( DbgGo(rec_set) < 0 )
	{
		DbgProcessError(rec_set);
		return 0;
	}
	DPERFLOG(SetTimerStopAndLog("run"));

	return 1;
}
#endif

static int 
cmd_finish_cmd(rec_set_t *rec_set, int argc, char **argv)
{
	invoke_t *	invoke = rec_set->s_invoke_handler;
	rep_set_t *	rep_set;

	DPERFLOG(SetTimerStart("finish"));
	
	// The finish function generates a DBGEV_STEP event. If this ever changes
	// make sure to update this.
	RemoveEventSet(invoke->i_tree, DBGEV_STEP);

	if ((rep_set = DbgFinishFunc(rec_set)) != NULL)
	{
		ProcessErrorInRepSet(rec_set, rep_set);
		FreeRepSet(rep_set);
		return 0;
	}
	
	DPERFLOG(SetTimerStopAndLog("finish"));

	return 1;
}

void
InitCmd_go(void)
{
	AddCommand("continue", "c", proc_required, 0, cmd_continue_cmd,
		cmd_continue_help, "Continue execution of application.");
	AddCommand("finish", NULL, proc_required, 0, cmd_finish_cmd,
		cmd_finish_help, "Execute application until each rank's current function returns.");
#ifndef ALPS
	AddCommand("run", "r", proc_required, 0, cmd_run_cmd,
		cmd_run_help, "Start debugging program.");
#endif
}
