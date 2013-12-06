/*
** Support for graph assigns.
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

list_t *	_assign_list = (list_t *)NULL;

assign_t *
AddAssign(list_t *l, char *lval, char *rval, var_t *v)
{
	assign_t *	a;

	a = (assign_t *)Alloc(sizeof(assign_t));

	a->a_lstr = strdup(lval);
	a->a_rstr = strdup(rval);
	a->a_rvar = v;

	AddToList(l, (void *)a);

	return a;
}

void
DestroyAssign(assign_t *a)
{
	Free(a->a_lstr);
	Free(a->a_rstr);
	Free(a);
}
