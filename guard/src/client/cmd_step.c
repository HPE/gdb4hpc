/*
** Single step commands.
**
** Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
**
*/

#ifdef HAVE_CONFIG_H
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#include	<stdio.h>
#include	<stdlib.h>
#include	<stdarg.h>
#include	<signal.h>

#include	"client.h"
#include	"useful.h"
#include	"dbg_set_async.h"

/*
** This function is able to handle both next and step by examining the first
** character of the command. If its an s, we set in to 1.
*/
static int 
StepCmd(rec_set_t *rec_set, int argc, char **argv)
{
	invoke_t *	invoke = rec_set->s_invoke_handler;
	int			in = 0;
	int			count = 1;
	rep_set_t *	rep_set;

	if ( *argv[0] == 's' )
		in++;

	if ( argc >= 2 )
		count = atoi(argv[1]);

	RemoveEventSet(invoke->i_tree, DBGEV_STEP);

	DPERFLOG(SetTimerStart("step"));
	if ((rep_set = DbgStep(rec_set, count, in)) != NULL)
	{
		ProcessErrorInRepSet(rec_set, rep_set);
		FreeRepSet(rep_set);
		return 0;
	}
	DPERFLOG(SetTimerStopAndLog("step"));

	return 1;
}

void
InitCmd_step(void)
{
	AddCommand("step", "s", proc_required, 0, StepCmd,
		cmd_step_help, "Step application until it reaches a different source line.");
	AddCommand("next", "n", proc_required, 0, StepCmd,
		cmd_next_help, "Step application, proceeding through subroutine calls.");
}
