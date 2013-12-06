/*
** Support for graph assigns.
**
** Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
**
*/

#ifndef _ASSIGN_H
#define _ASSIGN_H

#include	"list.h"

struct assign_t
{
	char *	a_lstr;
	char *	a_rstr;
	var_t *	a_rvar;
};
typedef struct assign_t	assign_t;

extern assign_t *	AddAssign(list_t *, char *, char *, var_t *);
extern void		DestroyAssign(assign_t *);
#endif /* !_ASSIGN_H */
