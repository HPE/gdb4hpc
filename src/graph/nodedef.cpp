/*********************************************************************************\
 * Node definition routines.
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdarg.h>
#include <stdio.h>

#include "defs.h"
#include "df.h"
#include "useful.h"

#define MAXNODETYPES 7 /* 2^7 = 128 */

static htable_t *NodeTypes = (htable_t *)NULL;

nodedef_t *AddNodeDef(const char *name, token_t *(*func)(graph_t *, node_t *),
                      token_type res, int min, int max, ...) {
    int a;
    int last;
    va_list args;
    nodedef_t *nd;

    if (NodeTypes == (htable_t *)NULL) NodeTypes = HashCreate(MAXNODETYPES);

    nd = (nodedef_t *)Alloc(sizeof(nodedef_t));
    nd->nd_name = name;
    nd->nd_func = func;
    nd->nd_otype = res;
    nd->nd_min = min;
    nd->nd_max = max;
    nd->nd_type = NODE_NORMAL;
    /*
    nd->nd_flow = GenNodeFlow;
    nd->nd_fire = GenReadyToFire;
    nd->nd_input = GenNodeInput;
    nd->nd_output = GenNodeOutput;
    */

    va_start(args, max);

    /*
    ** Set types of non-optional args
    */
    for (a = 0; a < min; a++) nd->nd_args[a] = va_arg(args, token_type);

    if (max < 0) {
        /*
        ** Set remaining arg types
        */
        last = va_arg(args, token_type);

        for (; a < MAXARGCHECK; a++) nd->nd_args[a] = last;
    } else {
        /*
        ** Set types of optional args
        */
        for (; a < max; a++) nd->nd_args[a] = va_arg(args, token_type);

        /*
        ** Set types of remaining args
        */
        for (; a < MAXARGCHECK; a++) nd->nd_args[a] = TK_BAD;
    }

    va_end(args);

    HashInsert(NodeTypes, name, (void *)nd);

    return nd;
}

void _free_nodedef(char *n, void *d) { Free(d); }

void FinishNodeDef(void) {
    HashDestroy(NodeTypes, _free_nodedef);
    NodeTypes = NULL;
}

/*
** Utility routine to convert a string to a nodedef entry.
*/
nodedef_t *FindNodeDef(const char *s) {
    hentry_t *h;

    if ((h = HashSearch(NodeTypes, s)) == (hentry_t *)NULL)
        return (nodedef_t *)NULL;

    return (nodedef_t *)h->h_data;
}

/*
** Initialise nodes
*/
void InitNodeDef(void) {
    nodedef_t *nd;

    /* token NodeConst(token) */
    (void)AddNodeDef("CONST", NodeConst, TK_ANY, 1, 1, TK_ANY);

    /* int NodeCompare(int, data, data) */
    (void)AddNodeDef("COMPARE", NodeCompare, TK_INT, 4, 4, TK_INT, TK_DATA,
                     TK_DATA, TK_INT);

    /* int NodeDisplayP2P(int, data) */
    (void)AddNodeDef("DISPLAY_P2P", NodeDisplayP2P, TK_INT, 2, 2, TK_INT,
                     TK_DATA);
    /* int NodeDbgOpenSocketSet(proc_set) */
    (void)AddNodeDef("DBGOPENSOCKET_SET", NodeDbgOpenSocketSet, TK_INT_SET, 3,
                     3, TK_PROC_SET, TK_STR, TK_INT);
    /* int NodeClosrSocket(int, data) */
    (void)AddNodeDef("DBGCLOSESOCKET_SET", NodeDbgCloseSocketSet, TK_INT_SET, 3,
                     3, TK_PROC_SET, TK_PROC_SET, TK_ANY);

    /* token NodeDbgInit(string) */
    (void)AddNodeDef("DBGINIT", NodeDbgInit, TK_ANY, 1, 1, TK_STR);

    /* proc NodeDbgSetBreak(proc,token[,string]) */
    //	(void)AddNodeDef("DBGSETBRK", NodeDbgSetBreak, TK_PROC_SET, 2, 3,
    //		TK_PROC_SET, TK_ANY, TK_STR);
    /*old code
            (void)AddNodeDef("DBGSETBRK", NodeDbgSetBreak, TK_PROC, 2, 3,
                    TK_PROC, TK_ANY, TK_STR);
    */

    (void)AddNodeDef("DBGSETBRK_SET", NodeDbgSetBreakSet, TK_PROC_SET, 2, 3,
                     TK_PROC_SET, TK_ANY, TK_STR);

    /* proc NodeDbgGo(proc) */
    //	(void)AddNodeDef("DBGGO", NodeDbgGo, TK_PROC_SET, 1, 1, TK_PROC_SET);
    //	(void)AddNodeDef("DBGGO", NodeDbgGo, TK_PROC, 1, 1, TK_PROC);

    (void)AddNodeDef("DBGGO_SET", NodeDbgGoSet, TK_PROC_SET, 1, 1, TK_PROC_SET);

    /* data NodeDbgGetVar(proc,string) */
    //(void)AddNodeDef("DBGGETVAR", NodeDbgGetVar, TK_DATA, 2, 2,
    //	TK_PROC, TK_STR);
    // Extra input for assert identifier to support partial assertions.

    // TODO: remove it
    (void)AddNodeDef("DBGGETVAR", NodeDbgGetVar, TK_DATA, 2, 3, TK_PROC_SET,
                     TK_STR, TK_INT);
    //	(void)AddNodeDef("DBGGETVAR", NodeDbgGetVar, TK_DATA, 2, 3,
    //		TK_PROC, TK_STR, TK_INT);

    (void)AddNodeDef("DBGREQUESTVAR_SET", NodeDbgRequestVarSet, TK_BIT_SET, 2,
                     3, TK_PROC_SET, TK_STR, TK_INT);
    (void)AddNodeDef("DBGRECVVAR_SET", NodeDbgRecvVarSet, TK_DATA_SET, 2, 5,
                     TK_PROC_SET, TK_INT, TK_STR, TK_BIT_SET, TK_BIT_SET);
    (void)AddNodeDef("DBGGETVAR_SET", NodeDbgGetVarSet, TK_DATA_SET, 2, 3,
                     TK_PROC_SET, TK_STR, TK_INT);

    /* void NodeDbgSetVar(proc,string,data) */
    (void)AddNodeDef("DBGSETVAR", NodeDbgSetVar, TK_ANY, 3, 3, TK_PROC_SET,
                     TK_STR, TK_DATA);
    //	(void)AddNodeDef("DBGSETVAR", NodeDbgSetVar, TK_ANY, 3, 3,
    //		TK_PROC, TK_STR, TK_DATA);

    /* void NodeDbgKill(proc) */
    (void)AddNodeDef("DBGKILL", NodeDbgKill, TK_ANY, 1, 1, TK_PROC_SET);
    //	(void)AddNodeDef("DBGKILL", NodeDbgKill, TK_ANY, 1, 1, TK_PROC);

    /* token NodePassIfEqual(int,int,token) */
    (void)AddNodeDef("PIEQ", NodePassIfEqual, TK_ANY, 3, 3, TK_ANY, TK_ANY,
                     TK_ANY);

    /* token NodePassIfNotEqual(int,int,token) */
    (void)AddNodeDef("PINE", NodePassIfNotEqual, TK_ANY, 3, 3, TK_ANY, TK_ANY,
                     TK_ANY);

    /* token NodePassIfLess(int,int,token) */
    (void)AddNodeDef("PILT", NodePassIfLess, TK_ANY, 3, 3, TK_ANY, TK_ANY,
                     TK_ANY);

    /* token NodePassIfLessOrEqual(int,int,token) */
    (void)AddNodeDef("PILE", NodePassIfLessOrEqual, TK_ANY, 3, 3, TK_ANY,
                     TK_ANY, TK_ANY);

    /* token NodePassIfGreater(int,int,token) */
    (void)AddNodeDef("PIGT", NodePassIfGreater, TK_ANY, 3, 3, TK_ANY, TK_ANY,
                     TK_ANY);

    /* token NodePassIfGreaterOrEqual(int,int,token) */
    (void)AddNodeDef("PIGE", NodePassIfGreaterOrEqual, TK_ANY, 3, 3, TK_ANY,
                     TK_ANY, TK_ANY);

    /* token NodePassIfToken(token,token) */
    (void)AddNodeDef("PIT", NodePassIfToken, TK_ANY, 2, 2, TK_ANY, TK_ANY);

    /* data NodeSlice(data,int,int,...) */
    (void)AddNodeDef("SLICE", NodeSlice, TK_DATA, 3, -1, TK_DATA, TK_INT,
                     TK_INT, TK_INT);

    /* data NodePermute(data,int,...) */
    (void)AddNodeDef("PERMUTE", NodePermute, TK_DATA, 2, -1, TK_DATA, TK_INT,
                     TK_INT);

    /* token NodeSync(...) */
    (void)AddNodeDef("SYNC", NodeSync, TK_ANY, 0, -1, TK_ANY);

    /* token NodeMerge(token,token,...) */
    nd = AddNodeDef("MERGE", NodeMerge, TK_ANY, 1, -1, TK_ANY, TK_ANY);
    nd->nd_type = NODE_FIREONANY;

    /* void NodeStore(int,token) */
    (void)AddNodeDef("STORE", NodeStore, TK_ANY, 2, 2, TK_INT, TK_ANY);

    /* token NodeRecall(int) */
    (void)AddNodeDef("RECALL", NodeRecall, TK_ANY, 1, 1, TK_INT);

    /* proc NodeProc(inv,str,int) */
    //	(void)AddNodeDef("PROC", NodeProc, TK_PROC_SET, 2, 2, TK_STR, TK_INT);
    //	(void)AddNodeDef("PROC", NodeProc, TK_PROC, 2, 2, TK_STR, TK_INT);

    (void)AddNodeDef("PROC_SET", NodeProcSet, TK_PROC_SET, 2, 2, TK_STR,
                     TK_BIT_SET);

    /* token NodeAdd(token,token) */
    (void)AddNodeDef("ADD", NodeAdd, TK_ANY, 2, 2, TK_ANY, TK_ANY);

    /* token NodeSubtract(token,token) */
    (void)AddNodeDef("SUB", NodeSubtract, TK_ANY, 2, 2, TK_ANY, TK_ANY);

    /* token NodeMultiply(token,token) */
    (void)AddNodeDef("MUL", NodeMultiply, TK_ANY, 2, 2, TK_ANY, TK_ANY);

    /* token NodeDivide(token,token) */
    (void)AddNodeDef("DIV", NodeDivide, TK_ANY, 2, 2, TK_ANY, TK_ANY);

    /* token NodeRemainder(token,token) */
    (void)AddNodeDef("REM", NodeRemainder, TK_ANY, 2, 2, TK_ANY, TK_ANY);

    /* token NodeEqual(token,token) */
    (void)AddNodeDef("EQU", NodeEqual, TK_ANY, 2, 2, TK_ANY, TK_ANY);

    /* token NodeNotEqual(token,token) */
    (void)AddNodeDef("NEQ", NodeNotEqual, TK_ANY, 2, 2, TK_ANY, TK_ANY);

    /* token NodeLessOrEqual(token,token) */
    (void)AddNodeDef("LEQ", NodeLessOrEqual, TK_ANY, 2, 2, TK_ANY, TK_ANY);

    /* token NodeGreaterOrEqual(token,token) */
    (void)AddNodeDef("GEQ", NodeGreaterOrEqual, TK_ANY, 2, 2, TK_ANY, TK_ANY);

    /* token NodeLess(token,token) */
    (void)AddNodeDef("LT", NodeLess, TK_ANY, 2, 2, TK_ANY, TK_ANY);

    /* token NodeGreater(token,token) */
    (void)AddNodeDef("GT", NodeGreater, TK_ANY, 2, 2, TK_ANY, TK_ANY);

    /* token NodeAnd(token,token) */
    (void)AddNodeDef("AND", NodeAnd, TK_ANY, 2, 2, TK_ANY, TK_ANY);

    /* token NodeOr(token,token) */
    (void)AddNodeDef("OR", NodeOr, TK_ANY, 2, 2, TK_ANY, TK_ANY);

    /* token NodeNegate(token,token) */
    (void)AddNodeDef("NEG", NodeNegate, TK_ANY, 2, 2, TK_ANY, TK_ANY);

#if 0
	/* int NodeEPS(float,float,int,data) */
	(void)AddNodeDef("EPS", NodeEPS, TK_INT, 4, 4,
		TK_FLOAT, TK_FLOAT, TK_INT, TK_DATA);
#endif

    /* string NodeType(data) */
    (void)AddNodeDef("TYPE", NodeType, TK_STR, 1, 1, TK_DATA);
    (void)AddNodeDef("TYPE_SET", NodeTypeSet, TK_STR, 1, 1, TK_DATA_SET);

    /* token NodeInsert(str,token) */
    (void)AddNodeDef("HSTORE", NodeInsert, TK_ANY, 2, 2, TK_STR, TK_ANY);

    /* token NodeSearch(str) */
    (void)AddNodeDef("SEARCH", NodeSearch, TK_ANY, 1, 1, TK_STR);

    /* int NodeHashEntry(str) */
    (void)AddNodeDef("HENTRY", NodeHashEntry, TK_INT, 1, 1, TK_STR);

    /* token NodeExit(token) */
    (void)AddNodeDef("EXIT", NodeExit, TK_ANY, 1, 1, TK_ANY);

    /* void NodeLog(data, string, string) */
    (void)AddNodeDef("LOG", NodeLog, TK_ANY, 3, 3, TK_DATA, TK_STR, TK_STR);

    /* Minh add here to implement blockmap 15/05/2009 */
    /* data NodeBlockMap(string, string, int, data, ...) */
    (void)AddNodeDef("BLOCKMAP", NodeBlockMap, TK_DATA, 4, -1, TK_STR, TK_STR,
                     TK_INT, TK_DATA, TK_DATA);

    /* Minh add here to implement reduce functions 22/02/2010 */
    /* data NodeReduce(int, data, ...) */
    (void)AddNodeDef("REDUCE", NodeReduce, TK_DATA, 3, -1, TK_STR, TK_INT,
                     TK_DATA);

    (void)AddNodeDef("BLOCKMAP_SET", NodeBlockMapSet, TK_DATA, 4, -1, TK_STR,
                     TK_STR, TK_INT, TK_DATA_SET, TK_DATA_SET);

    /* Minh add here to implement reduce functions 22/02/2010 */
    /* data NodeReduceSet(int, data, ...) */
    (void)AddNodeDef("REDUCE_SET", NodeReduceSet, TK_DATA, 3, -1, TK_STR,
                     TK_INT, TK_DATA_SET);

    /* data NodeAssign(string, data) */
    (void)AddNodeDef("ASSIGN", NodeAssign, TK_DATA, 2, 2, TK_STR, TK_DATA);

    /* data NodeGetVar(proc,string) */
    //(void)AddNodeDef("GETVAR", NodeGetVar, TK_DATA, 2, 2, TK_PROC, TK_STR);
    // Extra input for assert identifier to support partial assertions.

    (void)AddNodeDef("GETVAR", NodeGetVar, TK_DATA, 2, 3, TK_PROC_SET, TK_STR);
    //	(void)AddNodeDef("GETVAR", NodeGetVar, TK_DATA, 2, 3, TK_PROC, TK_STR);
}
