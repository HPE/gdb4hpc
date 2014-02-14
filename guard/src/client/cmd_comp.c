/*********************************************************************************\
 * Compare command.
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

#include	<signal.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<unistd.h>

#include	"client.h"
#include	"useful.h"

static int 
cmd_compare_cmd(rec_set_t *rec_set, int argc, char **argv)
{
	char *			buf;
	val_t *			val_res = NULL;
	val_t *			val_diff = NULL;
	int				res;
	rep_set_t *		rep_set;
	group_reply_t *	reply;
	int				print_diff = 0;
	expr_t *		e;
	char **			ap;
	AIF *			a;

	if ( argc < 2 )
	{
		PrintOutError("Not enough command arguments.\n");
		PrintOutError("Expected: compare expr1 = expr2\n");
		return 1;
	}

	ap = &argv[1];

	if ( argc <= 0 )
	{
		PrintOutError("Not enough command arguments.\n");
		PrintOutError("Expected: compare expr1 = expr2\n");
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

	if ( EvalCompare(&val_res, &val_diff, e, rec_set) < 0 )
	{
		PrintError();
		FreeExpr(e);
		return 0;
	}

	FreeExpr(e);

	/*
	** The EvalCompare function will read the current values of Client.cmp_lower_eps
	** and Client.cmp_upper_eps to implement epsilon tolerances for floating point
	** values.
	**
	** TODO: We should save the difference data instead of displaying it
	**       similar to what graph does with info diff.
	*/

	// Process the result val based on its rval type
	switch (RVAL_TYPE(val_res))
	{
		case rval_aif:
			// point at the aif result
			a = RVAL_AIF(val_res);
			// Set res to the comparison result
			AIFToInt(a, &res);
			switch (res)
			{
				case 0:
					PrintOutMsg("Comparison is true.\n");
					break;
				case -1:
				case 1:
					PrintOutMsg("Comparison is false. The difference delta is:\n");
					if ( PrintVal(val_diff, rec_set) < 0 )
						PrintError();
					break;
				case -2:
				case 2:
					PrintOutMsg("Comparison is warn. The difference delta is:\n");
					if ( PrintVal(val_diff, rec_set) < 0 )
						PrintError();
					break;
				case 3:
					PrintOutMsg("Comparison is indeterminate due to pointers.\n");
					break;
			}
			break;
			
		case rval_reply:
			// Point at the reply set
			rep_set = RVAL_REPLY(val_res);
		
			// Need to loop over the reply
			SetReplyHeadInSet(rep_set);
			while((reply = GetNextReplyInSet(rep_set)) != (group_reply_t*)NULL)
			{
				// point at the aif result
				a = EVENT_DATA(reply->rep_event);
				// Set res to the comparison result
				AIFToInt(a, &res);
				switch (res)
				{
					case 0:
						PrintMsgWithSetInfo(rec_set->s_setname, reply->rep_group, "Comparison is true.\n");
						break;
					case -1:
					case 1:
						PrintMsgWithSetInfo(rec_set->s_setname, reply->rep_group, "Comparison is false.\n");
						print_diff++;
						break;
					case -2:
					case 2:
						PrintMsgWithSetInfo(rec_set->s_setname, reply->rep_group, "Comparison is warn.\n");
						print_diff++;
						break;
					case 3:
						PrintMsgWithSetInfo(rec_set->s_setname, reply->rep_group, "Comparison is indeterminate due to pointers.\n");
						break;
				}
			}
			
			// print out the difference deltas once
			if (print_diff && val_diff != NULL)
			{
				PrintOutMsg("The difference delta is:\n");
				if ( PrintVal(val_diff, rec_set) < 0 )
					PrintError();
			}
			break;
			
		default:
			// shouldn't get here
			break;
	}

	if (val_diff != NULL)
		DestroyVal(val_diff);
		
	if (val_res != NULL)
		DestroyVal(val_res);

	return 1;
}

void
InitCmd_comp(void)
{
	AddCommand("compare", NULL, proc_optional, 0, cmd_compare_cmd,
		cmd_compare_help, "Compare the contents of two variables.");
}
