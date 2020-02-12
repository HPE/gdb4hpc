/*********************************************************************************\
 * Graph definition
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2020 Cray Inc.
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

#include "ainfo.h"
#include "bitset.h"
#include "dbg_client.h"
#include "dbgevent.h"
#include "hash.h"
#include "list.h"
#include "node.h"
#include "token.h"

#define INVOKE_NODES 5

/*
** graph_diff is used to recall saved diff information from a graph execution.
*/
struct graph_diff {
    int gd_assert_id; /* assert id */
    char *gd_name;    /* difference name */
    AIF *gd_val;      /* AIF ptr to difference */
};
typedef struct graph_diff graph_diff;

/*
** graph_inv is used when bulding a graph to retain state information.
** TODO: Allow this to be used with DBGSET_DEFSET
*/
struct graph_inv {
    // dbginvoke_t components
    dbginvoke_t *gp_di;  // dbginvoke_t pointer

    // procset support components
    bitset *gp_unbuilt_bs; /* ranks that have yet to be built */
    list_t *gp_files;      /* per process list of files */
    int gp_node[INVOKE_NODES];
};
typedef struct graph_inv graph_inv;

/*
** graph_t holds information about the static graph, and is also
** used when executing the graph.
*/
struct graph_t {
    /*
    ** Graph information
    */
    const char *g_valname;  // String name stored in hashtable
    int g_isactive;         // is currently active bool
    int g_qlen;

    /*
    ** Graph execution results
    */
    int g_iterations;    // number of times graph nodes have fired
    int g_assertions;    // number of assertions in graph.
    int g_pass;          // number of passed assertions
    int g_warn;          // number of warned assertions
    int g_fail;          // number of failed assertions
    int *g_assert_pass;  // correlates to an assertid pass
    int *g_assert_warn;  // correlates to an assertid warn
    int *g_assert_fail;  // correlates to an assertid fail
    list_t *g_diff;      // actual difference data

    /*
    ** Graph state info
    */
    int g_reset;  // Graph has not been used.
    int g_nid;    // Last node id used.
    int g_store;  // Last store number.

    /*
    ** Per-invoke and per-process information needed when
    ** building the graph.
    */
    htable_t *g_inv;              // All the procsets used in assert or assign.
    ainfo_t **g_assert_info_tab;  // Assert info table

    /*
    ** Node number of start and init nodes.
    */
    int g_startnode;
    int g_initnode;

    /*
    ** List of procs to check when removing bp's.
    */
    list_t *g_di_list;        // All of the dbginvoke_t * used by graph
    list_t *g_bp_reply_list;  // list of group_reply for the breakpoint sets

    /*
    ** Assertion and assign information used to generate graph.
    */
    list_t *g_assigns;
    list_t *g_vars;
    list_t *g_assert_cmds;  // list of assertion command buffers

    /*
    ** Internal graph structure.
    */
    node_t *g_nodes;
    node_t **g_lastNode;

    /*
    ** Active node list for interpreter.
    */
    node_t *g_active;
    node_t *g_currActive;
    node_t *g_lastActive;
    int g_activeEvents;

    /*
    ** Tokens used by interpreter.
    */
    int g_token_id;
    token_t *g_tokens;
    token_t *g_last_token;
    token_t *g_tcurrent;

    /*
    ** Graph constructor routines.
    */
    void (*g_startGraph)(struct graph_t *, char *, int, void *);
    void (*g_endGraph)(struct graph_t *);
    int (*g_setStartNode)(struct graph_t *, int);
    int (*g_checkGraph)(struct graph_t *);
    int (*g_outNode)(struct graph_t *, const char *);
    int (*g_outConstInt)(struct graph_t *, const char *, int);
    int (*g_outConstFloat)(struct graph_t *, const char *, double);
    int (*g_outConstStr)(struct graph_t *, const char *, const char *);
    int (*g_outConstAIF)(struct graph_t *, const char *, double, int);
    int (*g_outConstBitSet)(struct graph_t *, const char *, bitset *);
    void (*g_outEdge)(struct graph_t *, int, int, int);

    /*
    ** User supplied routines.
    */
    int (*g_display)(char *, char *, char *, AIF *, char *, AIF *, char *,
                     AIF *);
    int (*g_display_p2p)(struct graph_t *, int, char *, AIF *);
    int (*g_compare)(struct graph_t *, int, char *, AIF *, char *, AIF *, int);
    void (*g_dbgout)(char *, int, int, void *);
    int (*g_dbgevent)(event_set_t *, bitset *);
};
typedef struct graph_t graph_t;

#endif /* _GRAPH_H */
