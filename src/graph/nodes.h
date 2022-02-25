/*********************************************************************************\
 * Dataflow nodes
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

#ifndef _NODES_H
#define _NODES_H

// FIXME: On a scale of 1 thru 10 the brokeness of the current design of this
//         interface is a 9001.

extern token_t *NodeAdd(graph_t *, node_t *);
extern token_t *NodeAnd(graph_t *, node_t *);
extern token_t *NodeBlockMapSet(graph_t *, node_t *);
extern token_t *NodeConst(graph_t *, node_t *);
extern token_t *NodeCompare(graph_t *, node_t *);
extern token_t *NodeDbgInit(graph_t *, node_t *);
extern token_t *NodeDbgSetBreakSet(graph_t *, node_t *);
extern token_t *NodeDbgGoSet(graph_t *, node_t *);
extern token_t *NodeDbgGetVar(graph_t *, node_t *);
extern token_t *NodeDbgGetVarSet(graph_t *, node_t *);
extern token_t *NodeDivide(graph_t *, node_t *);
extern token_t *NodeDisplay(graph_t *, node_t *);
extern token_t *NodeEqual(graph_t *, node_t *);
extern token_t *NodeExit(graph_t *, node_t *);
extern token_t *NodeGetVar(graph_t *, node_t *);
extern token_t *NodeGreater(graph_t *, node_t *);
extern token_t *NodeGreaterOrEqual(graph_t *, node_t *);
extern token_t *NodeLess(graph_t *, node_t *);
extern token_t *NodeLessOrEqual(graph_t *, node_t *);
extern token_t *NodeLog(graph_t *, node_t *);
extern token_t *NodeReduce(graph_t *, node_t *);
extern token_t *NodeReduceSet(graph_t *, node_t *);
extern token_t *NodeMerge(graph_t *, node_t *);
extern token_t *NodeMultiply(graph_t *, node_t *);
extern token_t *NodeNegate(graph_t *, node_t *);
extern token_t *NodeNotEqual(graph_t *, node_t *);
extern token_t *NodeOr(graph_t *, node_t *);
extern token_t *NodePassIfEqual(graph_t *, node_t *);
extern token_t *NodePassIfNotEqual(graph_t *, node_t *);
extern token_t *NodePassIfLess(graph_t *, node_t *);
extern token_t *NodePassIfLessOrEqual(graph_t *, node_t *);
extern token_t *NodePassIfGreater(graph_t *, node_t *);
extern token_t *NodePassIfGreaterOrEqual(graph_t *, node_t *);
extern token_t *NodePassIfToken(graph_t *, node_t *);
extern token_t *NodePermute(graph_t *, node_t *);
extern token_t *NodeProcSet(graph_t *, node_t *);
extern token_t *NodeRecall(graph_t *, node_t *);
extern token_t *NodeRemainder(graph_t *, node_t *);
extern token_t *NodeSubtract(graph_t *, node_t *);
extern token_t *NodeSync(graph_t *, node_t *);
extern token_t *NodeStore(graph_t *, node_t *);
extern token_t *NodeTypeSet(graph_t *, node_t *);

#endif /* !_NODES_H */
