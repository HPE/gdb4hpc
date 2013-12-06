/*
** Variable defines.
**
** Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
**
*/

#ifndef _VAR_H
#define _VAR_H

#include	"list.h"
#include	"slice.h"
#include	"line.h"

struct var_t
{
	struct dbgprocset_t *	v_procset;	/* process number */
	int						v_indiv;	/*extention for individual process */
	char *					v_var;		/* name of var */
	char *					v_fname;	/* file name */
	int						v_lno;		/* line number */
	slice_t *				v_slice;	/* array slice (optional) */
	list_t *				v_perm;		/* array permute (optional) */
	int						v_internal;	/* var is an internal variable */
	/*
	** v_line[0] is used to store breakpoint information for the var
	** when building graph. If v_proc == NULL,
	** then this var represents a collection of processes and v_line 
	** holds breakpoint information for each process. 
	*/
	line_t *				v_line;		/* break point line number */
	
#define v_isym		v_procset->s_ig->ig_sym
#define v_bp		v_line->l_bp		/* break point id */
#define v_pid		v_line->l_proc		/* proc once bp reached */
#define v_sync		v_line->l_sync		/* rendezvous for procs */
#define v_value	v_line->l_val		/* rendezvous for procs */
#define v_bs		v_line->l_bs		/* rendezvous for procs */
};
typedef struct var_t	var_t;

var_t *	NewVar(void);
void	DestroyVar(var_t *);

#endif /* !_VAR_H */

