/*********************************************************************************\
 * Process Set Definitions.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * © 2010-2014 Cray Inc.  All Rights Reserved.
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

#ifndef _PROCSET_H
#define _PROCSET_H

#include "bitset.h"
#include "dbg_client.h"

/* Two type of Overlay Sets are supported: created by invoke group and defset cmd*/
enum dbgset_type
{
	DBGSET_INVOKE,
	DBGSET_DEFSET,
	DBGSET_INTERNAL
};
typedef enum dbgset_type dbgset_type;

/* 
** A procset can point at multiple dbginvoke_t (di) objects. For each di, the
** procset can point at only a subset of all ranks. An overlay set (over_set)
** is used to keep track of which ranks the procset is pointing at for the di.
**
** The offset only comes into play with DBGSET_DEFSET to keep track of which 
** index in the procset maps onto which dbginvoke_t for targeted commands.
*/
struct dbg_over_set_t
{
	dbginvoke_t *	s_di;			// pointer to dbginvoke
	bitset *		s_di_bs;		// mask for bitset in di, this is the "full set" for this overlay set
	int				s_ps_offset;	// offset for this overset, used when multiple oversets come into play with defset
									// calculate which index corresponds to which over_set
};
typedef struct dbg_over_set_t dbg_over_set_t;

/*
	a procset consists of one or multiple overlay sets.
*/
struct dbgprocset_t
{
	char *			s_name;				// name of procset	
	dbgset_type		s_type;				// type of procset
	int				s_nproc;			// number of ranks in procset
	int				s_init;				// Used for DBGSET_INVOKE, if the set was finalized
	list_t *		s_over_set_list;	// list of dbg_over_set_t
	// FIXME: This absolutely must be removed
	int				s_indiv_proc;
};
typedef struct dbgprocset_t	dbgprocset_t;

#endif /* !_PROCSET_H */
