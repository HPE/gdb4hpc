/*********************************************************************************\
 * Function routines.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * © 2010-2019 Cray Inc.  All Rights Reserved.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 *********************************************************************************/

#ifdef HAVE_CONFIG_H
#include        <config.h>
#endif /* HAVE_CONFIG_H */

#include	<stdarg.h>
#include	<stdio.h>
#include	<signal.h>
#include	<string.h>

#include	"defs.h"
#include	"useful.h"
#include	"aif.h"
#include	"gram.h"
#include	"client_settings.h"

#define DBG_FUNC_TABLE_SIZE   7

static htable_t *	_func_table = (htable_t *)NULL;
static list_t *		_func_list = (list_t *)NULL;

void		_free_func(func_t *);

void
InitFunc(void)
{
	_func_list = NewList();
	_func_table = HashCreate(DBG_FUNC_TABLE_SIZE);
}

void
FinishFunc(void)
{
	DestroyList(_func_list, (void (*)(void*))_free_func);
	HashDestroy(_func_table, (void (*)(char *, void *))NULL);
}

/*
** Create a new function.
*/
func_t *
NewFunc(hentry_t *sym, list_t *args, expr_t *def, int (*bin)(list_t *, val_t **), const char *str, int hash)
{
	func_t *	f;
	hentry_t *	h;

	f = (func_t *)Alloc(sizeof(func_t));
	f->f_sym = sym;
	f->f_args = args;
	f->f_strdef = str != NULL ? strdup(str) : NULL;
	f->f_def = def;
	f->f_builtin = bin;

	AddToList(_func_list, (void *)f);

	if ( !hash )
		return f;

	if ( (h = HashInsert(_func_table, SymLookup(sym), (void *)f)) == (hentry_t *)NULL )
	{
		SetError("Too many functions defined");
		FreeFunc(f);
		return (func_t *)NULL;
	}

	return f;
}

void
_free_func(func_t *f)
{
	if ( f->f_args != (list_t *)NULL )
		DestroyList(f->f_args, (void (*)(void*))NULL);

	if ( f->f_def != (expr_t *)NULL )
		FreeExpr(f->f_def);

	if ( f->f_strdef != NULL )
		Free(f->f_strdef);

	Free(f);
}

/*
** Free a function.
*/
void
FreeFunc(func_t *f)
{
	hentry_t *	h;

	if ( f == (func_t *)NULL )
		return;

	RemoveFromList(_func_list, (void *)f);

	if ( (h = HashSearch(_func_table, SymLookup(f->f_sym))) != (hentry_t *)NULL )
		HashRemove(_func_table, h);

	_free_func(f);
}

/*
** Find a function.
*/
func_t *
FindFunc(hentry_t *sym)
{
	hentry_t *	h;

	if ( (h = HashSearch(_func_table, SymLookup(sym))) == (hentry_t *)NULL )
		return (func_t *)NULL;

	return (func_t *)h->h_data;
}

/*
** Check if a function has been defined.
*/
int
CheckFunc(hentry_t *sym)
{
	func_t *	f = FindFunc(sym);

	return f != (func_t *)NULL && (f->f_def != (expr_t *)NULL || f->f_builtin != (int (*)(list_t *, val_t **))NULL);
}

void
SetFunc(void)
{
	if ( _func_list != (list_t *)NULL )
		SetList(_func_list);
}

func_t *
GetFunc(void)
{
	if ( _func_list == (list_t *)NULL )
		return (func_t *)NULL;

	return (func_t *)GetListElement(_func_list);
}

arg_t *
NewArg(int t, ...)
{
	arg_t *	a;
	va_list	args;

	va_start(args, t);

	a = (arg_t *)Alloc(sizeof(arg_t));
	a->a_type = t;

	switch ( t )
	{
	case ARG_SYM:
		a->a_sym = va_arg(args, hentry_t *);
		break;

	case ARG_VAL:
		a->a_val = va_arg(args, val_t *);
		break;

	case ARG_INT:
		a->a_int = va_arg(args, int);
		break;

	case ARG_GRAPH:
		a->a_inp = va_arg(args, int);
		a->a_out = va_arg(args, int);
		a->a_vlist = va_arg(args, list_t *);
	}

	va_end(args);

	return a;
}

void
DestroyArg(arg_t *a)
{
	if ( a->a_type == ARG_VAL )

	switch ( a->a_type )
	{
	case ARG_VAL:
		DestroyVal(a->a_val);
		break;

	case ARG_GRAPH:
		DestroyList(a->a_vlist, (void (*)(void*))NULL);
		break;
	}

	Free((void *)a);
}

/*
** Find sym in formal param list fp, then return corresponding actual
** argument.
*/
int
FindArg(arg_t **v, hentry_t *sym, func_t *f)
{
	int	l1;
	int	l2;
	void *	a;
	void *	p;

	l1 = SizeOfList(f->f_args);
	l2 = SizeOfList(f->f_actual);

	if ( l1 < l2 )
	{
		SetError("Too many arguments to funcion \"%s\"", SymLookup(f->f_sym));
		return -1;
	}
	else if ( l2 < l1 )
	{
		SetError("Too few arguments to funcion \"%s\"", SymLookup(f->f_sym));
		return -1;
	}

	SetList(f->f_args);
	SetList(f->f_actual);

	while
	(
		(p = GetListElement(f->f_args)) != (void *)NULL
		&&
		(a = GetListElement(f->f_actual)) != (void *)NULL
	)
	{
		if ( sym == p )
		{
			*v = (arg_t *)a;
			return 0;
		}
	}

	*v = (arg_t *)NULL;
	return 0;
}

void
PrintFunc(FILE *fp, func_t *f)
{
	int	n = 0;
	hentry_t *	sym;

	if ( f == (func_t *)NULL )
		return;

	PrintToFd(fp, "%s(", SymLookup(f->f_sym));

	SetList(f->f_args);

	while ( (sym = (hentry_t *)GetListElement(f->f_args)) != (hentry_t *)NULL )
	{
		if ( n++ > 0 )
			PrintToFd(fp, ",");

		PrintToFd(fp, "%s", SymLookup(sym));
	}

	PrintToFd(fp, ")=%s\n", f->f_strdef);
}
