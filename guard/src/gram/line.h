/*
** Line number defines.
**
** Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
**
*/

#ifndef _LINE_H
#define _LINE_H

#include	"list.h"
#include	"bitset.h"

struct line
{
	int			l_lno;	/* line number */
	int			l_bp;	/* bp id for this line */
	int			l_proc;	/* proc once bp has been reached */
	int			l_sync;	/* rendezvous for assigns at this bp */
	int			l_val;	/* output of getvar graph */
	int			l_ref;
	bitset *	l_bs;
};
typedef struct line  line_t;

line_t *	AddLine(list_t **, int);
void		DestroyLine(line_t *);

#endif /* !_LINE_H */

