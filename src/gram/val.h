/*********************************************************************************\
 * Definitions used when evaluating expressions.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2014,2020 Hewlett Packard Enterprise Development LP.
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
 *********************************************************************************/

#ifndef _VAL_H
#define _VAL_H

#include <stdarg.h>

#include "AIFStream.h"
#include "blockmap.h"
#include "procset.h"
#include "graph.h"

#define LVAL_SYM_VAL(sym) (val_t *)MakeTransitoryLval((sym))

enum lval_type {
    lval_none,     /* no lvalue */
    lval_blockmap, /* blockmap value */
    lval_gdb,      /* gdb value */
    lval_internal  /* internal debugger value */
};
typedef enum lval_type lval_type;

enum rval_type {
    rval_none,     /* no rvalue */
    rval_aif,      /* default rval */
    rval_blockmap, /* blockmap rval pointer */
    rval_graph,    /* graph rval pointer */
    rval_reply,    /* group of rep_set_t replies */
    rval_proc,     /* procset rval pointer */
    rval_ref       /* sliced rval type*/
};
typedef enum rval_type rval_type;

// LVAL defines
#define LVAL_TYPE(v) (v)->val_lval_type
#define LVAL_VAL(v) (v)->lval_u

// LVAL val union accessors
#define LVAL_SYM(v) (v)->lval_u.lval_sym
#define LVAL_GDB_PROCSET(v) (v)->lval_u.lval_gdb.gdb_procset
#define LVAL_GDB_EXPR(v) (v)->lval_u.lval_gdb.gdb_expr
#define LVAL_BM_BLOCKMAP(v) (v)->lval_u.lval_blockmap.bm_blockmap
#define LVAL_BM_ARRAY_SIZE(v) (v)->lval_u.lval_blockmap.bm_array_size
#define LVAL_BM_GDB_PROCSET(v) (v)->lval_u.lval_blockmap.bm_gdb.gdb_procset
#define LVAL_BM_GDB_EXPR(v) (v)->lval_u.lval_blockmap.bm_gdb.gdb_expr

// RVAL defines
#define RVAL_TYPE(v) (v)->val_rval_type
#define RVAL_VAL(v) (v)->rval_u

// RVAL val union accessors
#define RVAL_AIF(v) (v)->rval_u.rval_aif
#define RVAL_BLOCKMAP(v) (v)->rval_u.rval_blockmap
#define RVAL_GRAPH(v) (v)->rval_u.rval_graph
#define RVAL_REPLY(v) (v)->rval_u.rval_reply
#define RVAL_PROCSET(v) (v)->rval_u.rval_procset
// The following are used to access the different parts of the rval ref val
#define RVAL_REF(v) (v)->rval_u.rval_ref
#define RVAL_REF_SLICE(v) (v)->rval_u.rval_ref->ref_slice
#define RVAL_REF_TYPE(v) (v)->rval_u.rval_ref->ref_type
#define RVAL_REF_VAL(v) *((v)->rval_u.rval_ref->ref_val)

// These are used to ignore ref types and just get at the actual base
// Note that graph and blockmap types are not implemented. This is because
// slicing a blockmap or graph lval isn't allowed.
#define RVAL_ACTUAL_TYPE(v) (rval_type) GetActualRvalType((v))
#define RVAL_ACTUAL_AIF(v) (AIF *)GetActualAIF((v))
#define RVAL_ACTUAL_REPLY(v) (rep_set_t *)GetActualReply((v))
#define RVAL_ACTUAL_PROCSET(v) (dbgprocset_t *)GetActualProcset((v))

// Used to determine if the RVAL should be destroyed or not. A value of 0 allows
// the RVAL to persist through the destruction of the val.
#define RVAL_DESTROY(v) (v)->rval_destroy

// Used to refresh vals that depend on backend data
#define VAL_RETRIEVE(v, p) RetrieveContents(v, p)

// Returns 1 if the val refers to something that is local to the client and is a
// constant
#define VAL_IS_LOCAL_CONST(v)                                            \
    ((LVAL_TYPE((v)) == lval_none || LVAL_TYPE((v)) == lval_internal) && \
     RVAL_TYPE((v)) == rval_aif)

// Returns 1 if the val refers to something that is local to the client and is a
// rep_set, we should already be holding onto the result
#define VAL_IS_LOCAL_REPLY(v)                                            \
    ((LVAL_TYPE((v)) == lval_none || LVAL_TYPE((v)) == lval_internal) && \
     RVAL_TYPE((v)) == rval_reply)

struct val_gdb {
    dbgprocset_t *gdb_procset;
    char *gdb_expr;
};

struct val_blockmap {
    g_block_map_t *bm_blockmap;
    int bm_array_size;
    struct val_gdb bm_gdb;
};

union lval_val {
    hentry_t *lval_sym;
    struct val_gdb lval_gdb;
    struct val_blockmap lval_blockmap;
};
typedef union lval_val lval_val;

struct rval_ref_val {
    // slice info
    slice_t *ref_slice;
    // rval type
    rval_type ref_type;
    // rval val
    union rval_val *ref_val;
};
typedef struct rval_ref_val rval_ref_val;

union rval_val {
    AIFStream *rval_aif;
    g_block_map_t *rval_blockmap;
    graph_t *rval_graph;
    rep_set_t *rval_reply;
    dbgprocset_t *rval_procset;
    struct rval_ref_val *rval_ref;
#define AIF_VAL(rval) (rval).rval_aif
#define BLOCKMAP_VAL(rval) (rval).rval_blockmap
#define GRAPH_VAL(rval) (rval).rval_graph
#define REPLY_VAL(rval) (rval).rval_reply
#define PROCSET_VAL(rval) (rval).rval_procset
#define REF_VAL(rval) (rval).rval_ref
};
typedef union rval_val rval_val;

struct val_t {
    // this is the lval type
    lval_type val_lval_type;
    // this holds the lval value
    lval_val lval_u;

    // this is the rval type
    rval_type val_rval_type;
    // This holds the rval value
    rval_val rval_u;
    // this dictates if we should destroy the rval
    int rval_destroy;
};
typedef struct val_t val_t;

#endif /* !_VAL_H */
