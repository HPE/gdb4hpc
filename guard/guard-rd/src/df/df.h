/*********************************************************************************\
 * Main library include.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2013,2020 Cray Inc.
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

#ifndef _DF_H
#define _DF_H

#include "build.h"
#include "gram.h"
#include "graph.h"
#include "gvar.h"
#include "nodedef.h"
#include "nodes.h"

/*
** graph.h
*/
extern graph_t *NewGraph(int (*)(graph_t *, int, char *, AIF *, char *, AIF *,
                                 int),
                         void (*)(char *, int, int, void *),
                         int (*)(graph_t *, int, char *, AIF *), int);
extern graph_inv *NewGraphInv(graph_t *, dbgprocset_t *);
extern graph_diff *NewGraphDiff(graph_t *, int, AIF *, const char *);
extern void AddActiveEvent(graph_t *);
extern void RemoveActiveEvent(graph_t *);
extern int ActiveEvents(graph_t *);
extern graph_inv *FindGraphInv(graph_t *, char *);
extern void DestroyGraph(graph_t *);
extern void ResetGraph(graph_t *);
extern void PrintGraph(FILE *, graph_t *);
extern void DebugPrintGraph(graph_t *);

/*
** node.h
*/
extern node_t *NewNode(graph_t *, int, nodedef_t *, token_t *);
extern void DestroyNodes(node_t *);
extern node_t *FindNode(graph_t *, int);
extern input_t *NewInputs(graph_t *, node_t *, nodedef_t *, int, token_type);
extern int NewInput(graph_t *, node_t *, int, token_type);
extern void DestroyInputs(node_t *);
extern void ResetInputs(graph_t *, node_t *);
extern void NewOutput(node_t *, node_t *, int);
extern void DestroyOutputs(node_t *);
extern int Connect(graph_t *, int, int, int);
extern int CheckNodes(graph_t *);
extern int CheckInputType(node_t *, int, token_type);
extern int CheckInputs(node_t *);
extern int FireNode(graph_t *, node_t *);
extern void InjectToken(graph_t *, node_t *, token_t *);
extern node_t *GetActiveNode(graph_t *);
extern void AddToActive(graph_t *, node_t *);
extern void RemoveFromActive(graph_t *, node_t *);
extern void GenNodeOutput(graph_t *, node_t *, token_t *);
extern void GenNodeInput(graph_t *, node_t *, input_t *, token_t *);
extern int CheckIfQueueFull(graph_t *, node_t *);
extern void DumpNodes(graph_t *);
extern void DumpNode(node_t *);

/*
** token.h
*/
extern const char *TokenTypeStr(token_type);
extern int SpaceForTokens(void);
extern token_t *NewToken(graph_t *, token_type);
extern token_t *DupToken(token_t *);
extern void DestroyToken(graph_t *, token_t *);
extern token_t *GetTokenFromInput(node_t *, int);
extern int ValidTokenType(token_type);
extern void SetToken(graph_t *);
extern token_t *NextToken(graph_t *);
extern void DumpToken(token_t *);

/*
** df.h
*/
#define DF_EXIT 0
#define DF_NOACTIVE 1
#define DF_ACTIVE 2

extern void InitDF(void);
extern void FinishDF(void);
extern void StartDF(graph_t *);
extern int DispatchDF(void);
extern void EndActiveDF(void);
extern void EndDF(graph_t *);
extern void ExitDF(void);
extern int ActiveDF(void);
extern graph_t *StatusDF(void);
extern void DFError(node_t *, const char *, ...);
extern void DFMsg(const char *, ...);
extern void DFErrMsg(const char *, ...);

#endif /* !_DF_H */
