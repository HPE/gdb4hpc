/*********************************************************************************\
 * Process invocation defines.
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



#ifndef _INVOKE_H
#define _INVOKE_H

#include	"list.h"
#include	"hash.h"

#define INVOKE_TABLE_SIZE       7	/* 2^7 = 128 */

// Used to determine if the invoke object cooresponds with a created app or
// an attached app
#define PROC_CREATED				0
#define PROC_ATTACHED				1

struct invoke_group
{
	hentry_t *			ig_sym;		/* name of this process group */
	int					ig_nproc;	/* number of procs in group */
	int					ig_offset;
	int					ig_rank;	/* rank of value */
	struct invoke_t *	ig_invoke;	/* ptr to invoke */
};
typedef struct invoke_group	invoke_group;

struct invoke_t
{
	list_t *			i_groups;	/* list of process groups */
	int					i_nproc;	/* total number of processes */
	int					i_ref;		/* reference count */
	char *				i_cmd;		/* program to run */
	char *				i_arch;		/* MP arch or NULL */
	void *				i_iexec;    /* dbginvoke_t pointer */
	void *				i_tree;
	void *				i_set_bs;  	/* a bitset representing dbgsrvs in the invoke*/
	int					i_attached;	/* was attached to, not created */
};
typedef struct invoke_t invoke_t;
#endif /* !_INVOKE_H */
