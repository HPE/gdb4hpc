/*
** Permute defines.
**
** Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
**
*/

#ifndef _PERM_H
#define _PERM_H


#include	"list.h"

struct perm_t
{
	int		p_from;
	int		p_to;
};
typedef struct perm_t  perm_t;

void		AddPerm(list_t **, int, int);
void		DestroyPerm(list_t *);
void		SetPerm(list_t *);
perm_t *	GetPerm(list_t *);

#endif /* !_PERM_H */
