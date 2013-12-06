/*
** Process Set Definitions.
**
** Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
**
*/

#ifndef _PROCSET_H
#define _PROCSET_H

#include "bitset.h"
#include "invoke.h"

/* Two type of Overlay Sets are supported: created by invoke group and defset cmd*/
enum dbgset_type
{
	DBGSET_INVOKE,
	DBGSET_DEFSET
};
typedef enum dbgset_type dbgset_type;

/* Each invoke (invoke_t) corresponds a set of processes, called "Original Set".
 An Overalay Set is a subset of one Original Set.
 One Overlay Set is created for each invoke group or defset.
*/
struct dbg_over_set_t
{
	char*			s_setname;		/*'all' procset may contain several trees */
	int				s_nproc;

	dbgset_type		s_type;

	invoke_t*		s_invoke;
	bitset*			s_set_bs;

	//DBGSET_INVOKE
	invoke_group*	s_ig;
	int				s_offset;
	int				s_len;

	//DBGSET_DEFSET
};
typedef struct dbg_over_set_t dbg_over_set_t;

/* multiple overlay sets may be defined over one Original Set*/
/* We maintain a reverse list mapping Original Set (created only by defset) to its Overlay Sets*/
struct dbg_ori_set_info_t
{
	invoke_t*		s_invoke;
	invoke_group*	s_ig;
	list_t*			s_over_set_list;	/*list of dbg_overlay_set_t*/
};
typedef struct dbg_ori_set_info_t dbg_ori_set_info_t;

/*
	a procset consists of one or multiple overlay sets.
*/
struct dbgprocset_t
{
	char *			s_name;
	int				s_nproc;
	list_t *		s_over_set_list;
	invoke_group *	s_ig;
	// FIXME: This is incredibly dangerous. When modifying this, it only gets
	// reset to -1 during the VAL_RETRIEVE call. But expr stuff is modifying the
	// *ACTUAL* procset...
	// Fix this by holding a ref type alongside the LVAL_GDB_PROCSET val.
	int				s_indiv_proc; //used to indicated invidual process during evaluating expression
	//TODO: keep bkpt in graph
};
typedef struct dbgprocset_t	dbgprocset_t;

#endif /* !_PROCSET_H */
