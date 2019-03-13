/*********************************************************************************\
 * Print command.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * © 2010-2014 Cray Inc.  All Rights Reserved.
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
 *********************************************************************************/

#ifdef HAVE_CONFIG_H
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<signal.h>
#include	<unistd.h>

#include	"client.h"
#include	"df.h"
#include	"useful.h"

// external exposure
int
PrintVal(val_t *val, rec_set_t *rec_set)
{
	graph_t *			g;
	dbgprocset_t *		pr = NULL;
	rep_set_t *			rep_set;
	group_reply_t *		reply;
	
	if (val == NULL)
		return -1;

	switch ( RVAL_TYPE(val) )
	{
	case rval_aif:
		// Retrieve the contents of the val
		if (VAL_RETRIEVE(val, rec_set))
		{
			return -1;
		}
		
		//
		// Print the result
		//
		// AIFPrint doesn't play nice with editline...
		AIFPrint(Client.cli_out_stream, 0, RVAL_AIF(val));
		PrintOutMsg("\n");
		
		break;
	
	case rval_reply:
		// Retrieve the contents of the val
		if (VAL_RETRIEVE(val, rec_set))
		{
			return -1;
		}
		
		// Print each reply
		rep_set = RVAL_REPLY(val);
		SetReplyHeadInSet(rep_set);
		while((reply = GetNextReplyInSet(rep_set)) != NULL)
		{
			// Print the set info - Note that the rep_di can be null here if the reply set was the result of an operation on
			// two different sets.
			PrintMsgWithSetInfo(((reply->rep_di == NULL) ? (char*)"" : reply->rep_di->d_handle), reply->rep_group, NULL);
			
			// Print the AIF value
			// AIFPrint doesn't play nice with editline...
			AIFPrint(Client.cli_out_stream, 0, EVENT_DATA(reply->rep_event));
			PrintOutMsg("\n");
		}
		
		break;

	case rval_proc:
		// assign the procset directly
		if ((pr = RVAL_PROCSET(val)) == NULL)
		{
			PrintOutMsg("<proc NULL>");
			break;
		}

		PrintOutMsg("<proc %s[%d]>\n", pr->s_name, pr->s_nproc);

		break;

	case rval_graph:
		// assign the graph directly
		if ((g = RVAL_GRAPH(val)) == NULL)
		{
			PrintOutMsg("NULL graph\n");
			break;
		}

		PrintGraph(Client.cli_out_stream, g);
		
		break;
		
	default:
		// do nothing
		return 0;
	}

	return 0;
}

static int 
cmd_print_cmd(rec_set_t *rec_set, int argc, char **argv)
{
	val_t *		val;
	char **		ap;
	expr_t *	e;
	char *		buf;

	if ( argc < 2 )
		return 1;

	ap = &argv[1];

	if ( argc <= 0 )
	{
		PrintOutError("Not enough command arguments.\n");
		PrintOutError("Expected: print expr\n");
		return 1;
	}

	buf = ArgsToBuf(ap, argc-1);

	if ( (e = ParseExpr(buf)) == NULL )
	{
		PrintError();
		Free(buf);
		return 0;
	}
	
	// done with buf
	Free(buf);

	if ( EvalExpr(&val, e, rec_set) < 0 )
	{
		PrintError();
		FreeExpr(e);
		return 0;
	}

	FreeExpr(e);
	
	// Ensure we have a recset provided to us if the lval is a gdb/blockmap variable
	switch (LVAL_TYPE(val))
	{
		case lval_gdb:
		case lval_blockmap:
			if (rec_set == NULL)
			{
				PrintOutError("No process to print symbol. Use launch/attach first.\n");
				DestroyVal(val);
				return 0;
			}
			break;
		default:
			// We should be able to print this, do nothing.
			break;
	}

	DPERFLOG(SetTimerStart("print"));

	PrintVal(val, rec_set);

	DPERFLOG(SetTimerStopAndLog("print"));

	DestroyVal(val);

	return 1;
}

void
InitCmd_print(void)
{
	AddCommand("print", "p", proc_optional, 0, (int (*)())cmd_print_cmd,
		cmd_print_help, "Print the value of an expression.");
}
