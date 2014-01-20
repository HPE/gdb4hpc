/*
** Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
** See COPYRIGHT in top-level directory.
**
** Graph builder definitions.
**
 *
 * Â© 2010-2013 Cray Inc.  All Rights Reserved.
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
 *********************************************************************************
*/

#ifndef _BUILD_H
#define _BUILD_H

#include	"list.h"
#include	"bitset.h"

#define EDGE(g, n1, n2, p)		(g)->g_outEdge(g, n1, n2, p)
#define NODE(g, n)				(g)->g_outNode(g, n)
#define ICONST(g, n, v)			(g)->g_outConstInt(g, n, v)
#define FCONST(g, n, v)			(g)->g_outConstFloat(g, n, v)
#define SCONST(g, n, v)			(g)->g_outConstStr(g, n, v)
#define AIFCONST(g, n, v, t)		(g)->g_outConstAIF(g, n, v, t)
#define BITSETCONST(g, n, v)		(g)->g_outConstBitSet(g, n, v)
#define GSTART	  					g->g_startGraph
#define GEND						g->g_endGraph
#define GCHECK						g->g_checkGraph
#define STARTNODE					g->g_setStartNode

typedef struct extrainfo
{
	char hashinfo[BUFSIZ];
	char blockmapinfo[BUFSIZ];
}extrainfo;

extern void		BuildInvokeProcSet(graph_t *, graph_inv *, var_t *);
extern int 		BuildSetBPSet(graph_t *, int, int, var_t *);
extern void		BuildMPWaitProcSet(graph_t *, graph_inv *, var_t *);
extern void		BuildInvoke(graph_t *, var_t *);
extern void		BuildInvokeProc(graph_t *, graph_inv *, var_t *, int); //TODO: remove it 
extern int		BuildSetBP(graph_t *, int, int, var_t *); //TODO: remove it
extern void		BuildGetVar(graph_t *, int *, int *, var_t *, int, extrainfo *);
extern void		BuildGetVarSet(graph_t *, int *, int *, var_t *, int, int, extrainfo *);
extern int		BuildCmp(graph_t *, int, int, int, int);
extern void		BuildMPWait(graph_t *, var_t *);
extern void		BuildMPWaitProc(graph_t *, graph_inv *, var_t *, int);
extern void		BuildUnused(graph_t *, var_t *);
extern void		BuildUnusedProc(graph_t *, graph_inv *, var_t *, int);
extern void		BuildUnusedProcSet(graph_t *, graph_inv *, var_t *, bitset *);
extern int		BuildBlockMap(graph_t *, g_block_map_t *, var_t *, extrainfo *, int);
extern void		BuildMerge(graph_t *, var_t *, int, int, int);
extern int		BuildMergeProc(graph_t *, graph_inv *, var_t *, int, int, int, int);
extern void		BuildSync(graph_t *, var_t *, int, int);
extern int		BuildAssert(graph_t *, int, expr_t *, rec_set_t *, list_t *);
extern int		BuildAssign(graph_t *, char *, int, int, list_t *);
extern int		BuildExpr(graph_t *, int *, int *, list_t *, expr_t *, rec_set_t *);
extern int		BuildReduce(graph_t *, var_t *, int *, extrainfo *);
extern int		BuildDisplayP2P(graph_t *, int, int, int, int);
extern void		BuildGetVarSet_Sync(graph_t *, int *, int *, var_t *, int, int, extrainfo *, int);

#endif /* _BUILD_H */
