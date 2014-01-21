/*********************************************************************************\
 * Process set commands.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * © 2010-2013 Cray Inc.  All Rights Reserved.
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
#include 	<string.h>

#include	"client.h"
#include	"useful.h"

static int
cmd_focus_cmd(rec_set_t *rec_set, int argc, char **argv)
{
	expr_t *		ex;
	expr_elt *		ep;
	dbgprocset_t *	s;

	if ( argc < 2 )
	{
		PrintOutError("Not enough command arguments.\n");
		PrintOutError("Expected: focus $set\n");
		return 0;
	}
	
	if ( (ex = ParseExpr(argv[1])) == (expr_t *)NULL )
	{
		PrintError();
		return 0;
	}
	
	ep = POP(ex);
	
	DPRINT("cmd_focus_cmd: Popped expr_op %s\n", getExprOpStr(ep->e_op));
	
	if (ep->e_op != EXPR_DSYM)
	{
		PrintOutError("Parser error: %d\n", ep->e_op);
		PrintOutError("Expected: focus $set\n");
		FreeExpr(ex);
		return 0;
	}
	
	DPRINT("cmd_focus_cmd: Found %s\n", SymLookup(ep->e_sym));
	
	// We want to use the key for the sym since the value it holds is useless to
	// us, its null data.

	if ( (s = FindProcSet(SymLookup(ep->e_sym))) == (dbgprocset_t *)NULL )
		PrintOutError("Process set %s not found.\n", SymLookup(ep->e_sym));
	else
		SetCurrentProcSet(s);
		
	FreeExpr(ex);

	return 1;
}

static int 
cmd_viewset_cmd(rec_set_t *rec_set, int argc, char **argv)
{
	expr_t *		ex;
	expr_elt *		ep;
	dbgprocset_t *	s;

	if ( argc > 1 )
	{
		if ( (ex = ParseExpr(argv[1])) == (expr_t *)NULL )
		{
			PrintError();
			return 0;
		}
		
		ep = POP(ex);
	
		DPRINT("cmd_viewset_cmd: Popped expr_op %s\n", getExprOpStr(ep->e_op));
	
		if (ep->e_op != EXPR_DSYM)
		{
			PrintOutError("Parser error: %d\n", ep->e_op);
			PrintOutError("Expected: viewset $set\n");
			FreeExpr(ex);
			return 0;
		}
		
		DPRINT("cmd_focus_cmd: Found %s\n", SymLookup(ep->e_sym));
	
		if ( (s = FindProcSet(SymLookup(ep->e_sym))) == (dbgprocset_t *)NULL )
		{
			PrintOutError("Process set %s not found.\n", SymLookup(ep->e_sym));
			FreeExpr(ex);
			return 0;
		}

		PrintOutMsg("Name       Procs\n");
		PrintOutMsg("%-10s ", s->s_name);
		PrintProcSet(Client.cli_out_stream, s);
		PrintOutMsg("\n");

		FreeExpr(ex);

		return 1;
	}

	PrintOutMsg("Name       Procs\n");

	SetProcSet();

	while ( (s = GetProcSet()) != (dbgprocset_t *)NULL )
	{
		PrintOutMsg("%-10s ", s->s_name);
		PrintProcSet(Client.cli_out_stream, s);
		PrintOutMsg("\n");
	}

	return 1;
}

static int 
cmd_defset_cmd(rec_set_t *rec_set, int argc, char **argv)
{
	expr_t *		ex;
	expr_elt *		ep;
	char *			new_procset_name;
	char *			buf;
	val_t *			val = NULL;
	dbgprocset_t *	s;

	if ( argc < 3 )
	{
		PrintOutError("Not enough command arguments.\n");
		PrintOutError("Expected: defset $setname $proc1,$proc2\n");
		return 0;
	}
	
	if ( (ex = ParseExpr(argv[1])) == (expr_t *)NULL )
	{
		PrintError();
		return 0;
	}
	
	ep = POP(ex);
	
	DPRINT("cmd_focus_cmd: Popped expr_op %s\n", getExprOpStr(ep->e_op));
	
	if (ep->e_op != EXPR_DSYM)
	{
		PrintOutError("Parser error: %d\n", ep->e_op);
		PrintOutError("Expected: defset $set $proc1,$proc2\n");
		FreeExpr(ex);
		return 0;
	}
	
	DPRINT("cmd_focus_cmd: Found %s\n", SymLookup(ep->e_sym));
	
	new_procset_name = SymLookup(ep->e_sym);
	RESET(ex);

	if ( FindProcSet(new_procset_name) != (dbgprocset_t *)NULL )
	{
		PrintOutError("Procset \"%s\" already defined.\n", new_procset_name);
		FreeExpr(ex);
		return 0;
	}

	// After calling EvalExpr on the new_procset_name, we need to call RemoveVal
	// to ensure proper cleanup if something goes wrong and this creates a new
	// dbg variable for us. We only do this if EvalExpr returns a rval_none val
	// meaning that there was not a name collision.
	if( EvalExpr(&val, ex, rec_set) < 0 )
	{
		PrintError();
		FreeExpr(ex);
		return 0;
	}

	if(val == (val_t*)NULL)
	{
		// Note that if there is no val, we don't want to call RemoveVal
		PrintOutError("ERROR: cannot get val for procset %s!\n", argv[1]);
		return 0;
	}
	
	switch (RVAL_ACTUAL_TYPE(val))
	{
		case rval_ref:
			// This shouldn't be handled technically since we are using RVAL_ACTUAL_TYPE
		case rval_none:
			// This is what we expect the val to be
			break;
			
		case rval_aif:
			PrintOutError("A debugger variable named %s is already defined.\n", argv[1]);
			return -1;
		
		case rval_proc:
			PrintOutError("A procset named %s is already defined.\n", argv[1]);
			return -1;
			
		case rval_blockmap:
			PrintOutError("A decomposition named %s is already defined.\n", argv[1]);
			return -1;
			
		case rval_graph:
			PrintOutError("An assertion script named %s is already defined.\n", argv[1]);
			return -1;
			
		case rval_reply:
			PrintOutError("An application variable symbol named %s already exists.\n", argv[1]);
			return -1;
	}

	// Create a new procset for val
	s = NewProcSet(new_procset_name);

	buf = ArgsToBuf(&argv[2], argc-2);
	
	if ( !ParseProcSet(s, buf, rec_set) )
	{
		RemoveVal(ep->e_sym);
		FreeProcSet(s);
		Free(buf);
		return -1;
	}
	
	// done with buf
	Free(buf);
	
	// Change the rval to be a procset
	RVAL_TYPE(val) = rval_proc;
	// Assign the procset directly
	RVAL_PROCSET(val) = s;
	// Important! Set RVAL_DESTROY to false
	RVAL_DESTROY(val) = 0;

	FreeExpr(ex);

	return 1;
}

void
InitCmd_procset(void)
{
	AddCommand("defset", NULL, proc_ignore, 0, cmd_defset_cmd,
		cmd_defset_help, "Create a set of processes.");
	AddCommand("viewset", NULL, proc_ignore, 0, cmd_viewset_cmd,
		cmd_viewset_help, "Display information about process sets.");
	AddCommand("focus", NULL, proc_ignore, 0, cmd_focus_cmd,
		cmd_focus_help, "Set the current process set.");
}
