/*
** Variable routines.
**
** Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
**
*/

#ifdef HAVE_CONFIG_H
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#include	<stdio.h>
#include	<string.h>

#include	"defs.h"
#include	"useful.h"
#include	"gram.h"

var_t *
NewVar(void)
{
	var_t *	v;

	v = (var_t *)Alloc(sizeof(var_t));

	v->v_line = (line_t *)NULL;
	v->v_slice = (slice_t *)NULL;
	v->v_perm = (list_t *)NULL;
	v->v_fname = NULL;
	v->v_var = NULL;
	v->v_procset = (dbgprocset_t *)NULL;
	v->v_indiv = -1;
	v->v_internal = 0;

	return v;
}

void
DestroyVar(var_t *v)
{
	if ( v == (var_t *)NULL )
		return;

	if ( v->v_line != (line_t *)NULL )
		DestroyLine(v->v_line);

	if ( v->v_slice != (slice_t *)NULL )
		DestroySlice(v->v_slice);

	if ( v->v_perm != (list_t *)NULL )
		DestroyPerm(v->v_perm);

	if ( v->v_fname != NULL )
		Free(v->v_fname);

	if ( v->v_var != NULL )
		Free(v->v_var);

	Free(v);
}
