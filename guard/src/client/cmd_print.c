/*
** Print command.
**
** Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
**
*/

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

static void
RealPrintVal(FILE *fp, AIF* val, rec_set_t *rec_set, bitset* src)
{
	dbg_over_set_t *over_set = NULL;

	if(rec_set != NULL && src != NULL)
	{
		over_set = rec_set->s_over_set;
		PrintMsgWithSetInfoByOffset(fp, rec_set->s_setname, src, over_set->s_offset, over_set->s_len, NULL);
	} else
		PrintMsgWithSetInfo(fp, NULL, NULL, NULL);

	// AIFPrint doesn't play nice with editline...
	AIFPrint(fp, 0, val);
	PrintToFd(fp, "\n");
}

// external exposure
int
PrintVal(FILE *fp, val_t *val, rec_set_t *rec_set)
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
		
		// Print the result
		RealPrintVal(fp, RVAL_AIF(val), NULL, NULL);
		
		break;
	
	case rval_reply:
		// Retrieve the contents of the val
		if (VAL_RETRIEVE(val, rec_set))
		{
			return -1;
		}
		
		// If the gdb procset is set, replace the rec_set with the one created
		// by calling VAL_RETRIEVE
		if (LVAL_GDB_PROCSET(val) != NULL)
		{
			rec_set = LVAL_GDB_RECSET(val);
		}
		
		// Print each reply
		rep_set = RVAL_REPLY(val);
		SetReplyHeadInSet(rep_set);
		while((reply = GetNextReplyInSet(rep_set)) != (group_reply_t*)NULL)
		{
			RealPrintVal(fp, EVENT_DATA(reply->rep_event), rec_set, reply->rep_group);
		}
		
		break;

	case rval_proc:
		// assign the procset directly
		if ((pr = RVAL_PROCSET(val)) == NULL)
		{
			PrintToFd(fp, "<proc NULL>");
			break;
		}

		PrintToFd(fp, "<proc %s[%d]>\n", pr->s_name, pr->s_nproc);

		break;

	case rval_graph:
		// assign the graph directly
		if ((g = RVAL_GRAPH(val)) == NULL)
		{
			PrintToFd(fp, "NULL graph\n");
			break;
		}

		PrintGraph(fp, g);
		
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
	FILE *		fp = Client.cli_out_stream;
	char *		buf;

	if ( argc < 2 )
		return 1;

	ap = &argv[1];

	while ( argc > 1 )
	{
		if ( strcmp(ap[0], "-output") == 0 )
		{
			if ( argc < 4 )
			{
				PrintOutError("Not enough command arguments.\n");
				return 1;
			}

			if
			(
				access(ap[1], W_OK) == 0
				&&
				!cli_query("File exists, overwrite?")
			)
				return 1;

			fp = (FILE *)NULL;
			ap += 2;
			argc -= 2;

			continue;
		}

		if ( strcmp(ap[0], "-nopr") == 0 )
		{
			rec_set = NULL;
			ap++;
			argc--;

			continue;
		}

		break;
	}

	if ( argc <= 0 )
	{
		PrintOutError("Not enough command arguments.\n");
		PrintOutError("Expected: print expr\n");
		return 1;
	}

	buf = ArgsToBuf(ap, argc-1);

	if ( (e = ParseExpr(buf)) == (expr_t *)NULL )
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

	if (fp == (FILE *)NULL)
	{
		if ((fp = fopen(argv[2], "w")) == (FILE *)NULL)
		{
			perror(argv[2]);
			return 0;
		}
	}
	
	// Ensure we have a recset provided to us if the lval is a gdb/blockmap variable
	switch (LVAL_TYPE(val))
	{
		case lval_gdb:
		case lval_blockmap:
			if (rec_set == NULL)
			{
				PrintOutError("No process to print symbol. Use launch/attach first.\n");
				DestroyVal(val);
				if ( fp != Client.cli_out_stream )
					fclose(fp);
				return 0;
			}
			break;
		default:
			// We should be able to print this, do nothing.
			break;
	}

	DPERFLOG(SetTimerStart("print"));

	PrintVal(fp, val, rec_set);

	DPERFLOG(SetTimerStopAndLog("print"));

	DestroyVal(val);

	if ( fp != Client.cli_out_stream )
		fclose(fp);

	return 1;
}

void
InitCmd_print(void)
{
	AddCommand("print", "p", proc_optional, 0, cmd_print_cmd,
		cmd_print_help, "Print the value of an expression.");
}
