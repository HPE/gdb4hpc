/*
** Block Map routines.
**
** Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
**
*/

#ifndef _GBLOCKMAP_H
#define _GBLOCKMAP_H

#include	"aif.h"
#include	"list.h"
#include	"hash.h"
#include	"var.h"
#include	"procset.h"
#include	"dbg_set_sync.h"

struct g_block_map_t
{
	hentry_t *	m_sym;
	var_t *		m_actual;
	int			m_debug;
	int *		m_distribute;			// store the mode of the block, cyclic
	int *		m_data;					// store the upperbound of the dimensions
	int *		m_procs;				// store the process grid
	int *		m_blockcyclic_val;		// store clock-cyclic value
	int *		m_dim_order;			// store dimension order of mapping
	int			m_rank;		        	// store the rank of the blockmap function
	int			(*m_builtin)(list_t *);

	/*
	** Fields filled in when map is being evaluated.
	*/
	int			m_current;
	AIFIndex *	m_ix;
};
typedef struct g_block_map_t	g_block_map_t;

extern void				SetBlockMap(void);
extern g_block_map_t *	GetBlockMap(void);
extern g_block_map_t *	FindBlockMap(char *);
extern int				CheckBlockMap(hentry_t *);
extern int				VerifyBlockMap(hentry_t *);
extern g_block_map_t *	NewBlockMap(hentry_t *);
extern void				DestroyBlockMap(g_block_map_t *);
extern AIF *			ApplyBlockMap(g_block_map_t *, char *, int, AIF **, char *);
extern void				PrintBlockMap(FILE *, g_block_map_t *);
extern void             FinishBlockMap(void);
extern void				BlockMapToString(g_block_map_t *, var_t *, int, char *);
extern AIF * 			construct_blockmap_data(g_block_map_t *bm, dbgprocset_t*, int array_size, rep_set_t *replies);

#endif /* !_GBLOCKMAP_H */
