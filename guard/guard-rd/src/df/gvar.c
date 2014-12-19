/*********************************************************************************\
 * Guard Compiler
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
 **********************************************************************************/

#ifdef HAVE_CONFIG_H
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#include	<math.h>
#include	<stdio.h>
#include	<stdarg.h>
#include	<string.h>

#include	"defs.h"
#include	"useful.h"
#include	"df.h"

#define MAX_GRAPHVARS	7	/* 2^7 = 128 */

static htable_t *	GraphVarTab = (htable_t *)NULL;

void
InitGraphVars(void)
{
	double ulp;
	int zero,one,i;
	
	if ( GraphVarTab != (htable_t *)NULL )
		return;

	ulp = nextafter(0.0, 0.1);      // get the unit of least precision for this machine
	zero = 0;
	one = 1;

	GraphVarTab = HashCreate(MAX_GRAPHVARS);

	SetGraphVar(GRAPH_LOWER, VAR_DOUBLE, (void *)&ulp);
	SetGraphVar(GRAPH_UPPER, VAR_DOUBLE, (void *)&ulp);
	SetGraphVar(GRAPH_RELATIVE, VAR_INT, (void *)&zero);
	SetGraphVar(GRAPH_FORCE, VAR_INT, (void *)&zero);
	SetGraphVar(GRAPH_HALT, VAR_INT, (void *)&one);
	SetGraphVar(GRAPH_STOP, VAR_INT, (void *)&one);
	SetGraphVar(GRAPH_LOGGING, VAR_INT, (void *)&zero);
	SetGraphVar(GRAPH_LOGFILE, VAR_STRING, (void *)"");
	SetGraphVar(GRAPH_DFBLOCK, VAR_INT, (void *)&zero);
	i = DF_DEFAULT;
	SetGraphVar(GRAPH_DFMODE, VAR_INT, (void *)&i);
	SetGraphVar(GRAPH_DISPLAY, VAR_STRING, (void *)"text");
	SetGraphVar(GRAPH_OUTPUT, VAR_STRING, (void *)"guard.out");
	// Minh add here to implement reduce interface
	i = EXPR_REDUCE_NONE;
	SetGraphVar(GRAPH_REDUCE, VAR_INT, (void *)&i);
	SetGraphVar(GRAPH_HASH_LOWER, VAR_INT, (void *)&zero);
	SetGraphVar(GRAPH_HASH_UPPER, VAR_INT, (void *)&zero);
	SetGraphVar(GRAPH_LOWER_BOUND, VAR_DOUBLE, (void *)&ulp);
	SetGraphVar(GRAPH_UPPER_BOUND, VAR_DOUBLE, (void *)&ulp);
	SetGraphVar(GRAPH_HISTOGRAM_NUMBINS, VAR_INT, (void *)&zero);
	SetGraphVar(GRAPH_HISTOGRAM_START, VAR_DOUBLE, (void *)&ulp);
	SetGraphVar(GRAPH_HISTOGRAM_END, VAR_DOUBLE, (void *)&ulp);
	// Minh adds here to implement the p2p comparison function
	SetGraphVar(GRAPH_P2PMODE, VAR_INT, (void *)&zero);

}

// Removing va_args until this function is actually called with
// variable arguments
void
SetGraphVar(char *var, int type, void *val)
{
	graphvar_t *	gv;
	hentry_t *	h = (hentry_t *)NULL;

	InitGraphVars();

	if ( (h = HashSearch(GraphVarTab, var)) == (hentry_t *)NULL )
		gv = (graphvar_t *)Alloc(sizeof(graphvar_t));
	else
		gv = (graphvar_t *)h->h_data;

	switch ( type )
	{
	case VAR_DOUBLE:
		gv->gv_dval = *(double *)(val);
		break;
		
	case VAR_INT:
		gv->gv_ival = *(int *)(val);
		break;
		
	case VAR_STRING:
		if ( h != (hentry_t *)NULL && gv->gv_type == VAR_STRING )
			Free(gv->gv_sval);

		gv->gv_sval = strdup((char *)val);
		break;
	}

	gv->gv_type = type;

	if ( h == (hentry_t *)NULL )
		HashInsert(GraphVarTab, var, (void *)gv);
}

// andrewg added on 8/10/10
// If you are just going to do a memcpy on the guy you pass in,
// you shouldn't use varargs. 
void
GetGraphVar(char *var, void *val)
{
	int		len = 0;
	graphvar_t *	gv;
	hentry_t *	h;

	InitGraphVars();

	if ( (h = HashSearch(GraphVarTab, var)) == (hentry_t *)NULL )
		return;

	gv = (graphvar_t *)h->h_data;

	switch ( gv->gv_type )
	{
	case VAR_DOUBLE:
		len = sizeof(double);
		break;
		
	case VAR_INT:
		len = sizeof(int);
		break;
		
	case VAR_STRING:
		len = sizeof(char *);
		break;
	
	default:
		break;
	}

	if ( val == (void *)NULL )
		return;

	memcpy(val, (void *)&gv->gv_u, len);
}


void
_destroy_gvar(char *var, void *v)
{
	graphvar_t *	gv = (graphvar_t *)v;

	if ( gv == (graphvar_t *)NULL )
		return;

	if ( gv->gv_type == VAR_STRING )
		Free(gv->gv_sval);

	Free(v);
}

void 
FinishGraphVars(void)
{
	HashDestroy(GraphVarTab, _destroy_gvar);
}
