/*********************************************************************************\
 * Assign command.
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
#include	<signal.h>
#include	<string.h>

#include	"client.h"
#include	"useful.h"

/*
** TODO: array assignment.
*/
static int
cmd_assign_cmd(rec_set_t *rec_set, int argc, char **argv)
{
	expr_t *	e1;
	expr_t *	e2;
	char *		buf;
	int		force = 0;

	if ( argc < 3 )
	{
		PrintOutError("Not enough command arguments.\n");
		PrintOutError("Expected: assign $target 20\n");
		PrintOutError("Where $target is the name of the target variable and the argument\n");
		PrintOutError("is the value to assign to the target.\n");
		return 0;
	}

	if ( strcmp(argv[argc-1], "-force") == 0 )
	{
		force = 1;
		argc--;
	}

	if ( (e1 = ParseExpr(argv[1])) == (expr_t *)NULL )
	{
		PrintError();
		return 0;
	}

	buf = ArgsToBuf(&argv[2], argc-2);

	if ( (e2 = ParseExpr(buf)) == (expr_t *)NULL )
	{
		PrintError();
		Free(buf);
		return 0;
	}
	
	//done with buf
	Free(buf);

	if ( AssignExpr(e1, e2, rec_set) < 0 )
	{
		PrintError();
		FreeExpr(e1);
		FreeExpr(e2);
		return 0;
	}

	FreeExpr(e1);
	FreeExpr(e2);
	return 1;
}

void
InitCmd_assign(void)
{
	AddCommand("assign", NULL, proc_optional, 0, cmd_assign_cmd,
		cmd_assign_help, "Change the value of an application or debugger variable.");
}
