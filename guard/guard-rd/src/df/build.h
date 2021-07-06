/*********************************************************************************\
 * Graph builder definitions.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2013,2020 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 * $HeadURL$
 * $Date$
 * $Rev$
 * $Author$
 *
 **********************************************************************************/

#ifndef _BUILD_H
#define _BUILD_H

#include "expr.h"
#include "graph.h"
#include "list.h"

#define EDGE(g, n1, n2, p) (g)->g_outEdge(g, n1, n2, p)
#define NODE(g, n) (g)->g_outNode(g, n)
#define ICONST(g, n, v) (g)->g_outConstInt(g, n, v)
#define FCONST(g, n, v) (g)->g_outConstFloat(g, n, v)
#define SCONST(g, n, v) (g)->g_outConstStr(g, n, v)
#define AIFCONST(g, n, v, t) (g)->g_outConstAIF(g, n, v, t)
#define BITSETCONST(g, n, v) (g)->g_outConstBitSet(g, n, v)
#define GSTART g->g_startGraph
#define GEND g->g_endGraph
#define GCHECK g->g_checkGraph
#define STARTNODE g->g_setStartNode

extern void BuildUnused(graph_t *);
extern int BuildAssert(graph_t *, int, expr_t *, rec_set_t *, list_t *);
extern int BuildAssign(graph_t *, char *, int, int, list_t *);
extern int BuildExpr(graph_t *, int *, int *, list_t *, expr_t *, rec_set_t *);

#endif /* _BUILD_H */
