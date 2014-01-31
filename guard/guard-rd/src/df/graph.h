/*********************************************************************************\
 * Graph definition
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
 **********************************************************************************/

#ifndef _GRAPH_H
#define _GRAPH_H

#include	"list.h"
#include	"hash.h"

#include	"dbgevent.h"
#include	"dbg_client.h"
#include	"bitset.h"
#include	"token.h"
#include	"node.h"
#include	"ainfo.h"

#define INVOKE_NODES	5

/*
** graph_diff is used to recall saved diff information from a graph execution.
*/
struct graph_diff
{
	int		gd_assert_id;	/* assert id */	
	char *	gd_name;		/* difference name */
	AIF  *	gd_val;			/* AIF ptr to difference */
};
typedef struct graph_diff graph_diff;

/*
** graph_procset and graph_inv are used when bulding a graph to 
** retain state information.
*/
struct graph_procset
{
	list_t *	gp_files;		/* per process list of files */
	int			gp_builtinv;	/* built invoke? */
	int			gp_built;		/* build finished? */

	bitset* 	gp_builtinv_bs;
	bitset* 	gp_built_bs;

	invoke_t *	gp_iproc;
	int			gp_node[INVOKE_NODES];
};
typedef struct graph_procset	graph_procset;

struct graph_inv
{
	int				gi_nproc;
	invoke_group *	gi_inv;
	int				gi_builtinv;		/* built invoke? */
	int				gi_builtunused;		/* build unused? */
	int				gi_node[INVOKE_NODES];
	graph_procset *	gi_procset;
#define gi_sym		gi_inv->ig_sym
#define gi_rank	gi_inv->ig_rank
#define gi_dims	gi_inv->ig_dims
};
typedef struct graph_inv	graph_inv;

/*
** graph_t holds information about the static graph, and is also
** used when executing the graph.
*/
struct graph_t
{
	/*
	** Graph information
	*/
	char *		g_valname;      // String name stored in hashtable
	int			g_isactive;     // is currently active bool
	int			g_qlen;
	
	/*
	** Graph execution results
	*/
	int			g_iterations;   // number of times graph nodes have fired
	int			g_assertions;   // number of assertions in graph.
	int			g_pass;			// number of passed assertions
	int			g_warn;			// number of warned assertions
	int			g_fail;			// number of failed assertions
	int *		g_assert_pass;  // correlates to an assertid pass
	int *		g_assert_warn;  // correlates to an assertid warn
	int *		g_assert_fail;  // correlates to an assertid fail
	list_t *	g_diff;			// actual difference data

	/*
	** Graph state info
	*/
	int			g_reset;		// Graph has not been used.
	int			g_nid;			// Last node id used.
	int			g_store;		// Last store number.

	/*
	** Per-invoke and per-process information needed when
	** building the graph.
	*/
	htable_t *	g_inv;			//a list of procsets used in assert or assign. These procsets may be created by defset
	ainfo_t **	g_assert_info_tab; // Assert info table

	/*
	** Node number of start and init nodes.
	*/
	int			g_startnode;
	int			g_initnode;

	/*
	** List of procs to check when removing bp's.
	*/
	list_t *	g_invoke_list;	// FIXME: This should hold dbginvoke_t, not invoke_t
	list_t *	g_bp_list;	// list of bp_t * set by graph

	/*
	** Assertion and assign information used to generate graph.
	*/
	list_t *	g_assigns;
	//list_t *	g_maps;	 // not currently used
	list_t *	g_vars;
	list_t *	g_assert_cmds;      // list of assertion command buffers

	/*
	** Internal graph structure.
	*/
	node_t *	g_nodes;
	node_t **	g_lastNode;

	/*
	** Active node list for interpreter.
	*/
	node_t *	g_active;
	node_t *	g_currActive;
	node_t *	g_lastActive;
	int			g_activeEvents;
	
	/*
	** Tokens used by interpreter.
	*/
	int			g_token_id;
	token_t *	g_tokens;
	token_t *	g_last_token;
	token_t *	g_tcurrent;

	/*
	** Graph constructor routines.
	*/
	void	(*g_startGraph)(struct graph_t *, char *, int, void *);
	void	(*g_endGraph)(struct graph_t *);
	int		(*g_setStartNode)(struct graph_t *, int);
	int		(*g_checkGraph)(struct graph_t *);
	int		(*g_outNode)(struct graph_t *, char *);
	int		(*g_outConstInt)(struct graph_t *, char *, int);
	int		(*g_outConstFloat)(struct graph_t *, char *, double);
	int		(*g_outConstStr)(struct graph_t *, char *, char *);
	int		(*g_outConstAIF)(struct graph_t *, char *, double, int);
	int		(*g_outConstBitSet)(struct graph_t *, char *, bitset *);
	void	(*g_outEdge)(struct graph_t *, int, int, int);

	/*
	** User supplied routines.
	*/
	int		(*g_display)(char *, char *, char *, AIF *, char *, AIF *, char *, AIF *);
	int		(*g_display_p2p)(struct graph_t *, int, char *, AIF *);
	int		(*g_compare)(struct graph_t *, int, char *, AIF *, char *, AIF *, int);
	void	(*g_dbgout)(char *, int, int, void *);
	int		(*g_dbgevent)(event_set_t *, bitset *);
};
typedef struct graph_t	graph_t;

#endif /* _GRAPH_H */

