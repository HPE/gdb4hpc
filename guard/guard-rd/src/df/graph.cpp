/*********************************************************************************\
 * Graph routines
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * Copyright 2010-2020 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 **********************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bitset.h"
#include "client.h"
#include "defs.h"
#include "df.h"
#include "useful.h"

// FIXME: I don't know how they came up with this number. Whatever.
#define INVOKE_TABLE_SIZE 7 /* 2^7 = 128 */

extern int NodeEvent(event_set_t *, const Bitset*);

/*
** Create a new graph.
*/
graph_t *NewGraph(int (*compare)(graph_t *, int, char *, AIF *, char *, AIF *,
                                 int),
                  void (*out)(char *, int, int, void *),
                  int (*display_p2p)(graph_t *, int, char *, AIF *), int qlen) {
    graph_t *g;

    g = (graph_t *)Alloc(sizeof(graph_t));

    g->g_valname = (char *)NULL;
    g->g_isactive = 0;
    g->g_qlen = qlen;
    g->g_iterations = 0;
    g->g_assertions = 0;
    g->g_pass = 0;
    g->g_warn = 0;
    g->g_fail = 0;
    g->g_assert_pass = (int *)NULL;
    g->g_assert_warn = (int *)NULL;
    g->g_assert_fail = (int *)NULL;
    g->g_diff = NewList();
    g->g_reset = 1;
    g->g_nid = 0;
    g->g_store = 0;
    g->g_inv = HashCreate(INVOKE_TABLE_SIZE);
    g->g_assert_info_tab = NewAssertInfoTable();
    g->g_startnode = -1;
    g->g_initnode = -1;
    g->g_di_list = NewList();
    g->g_bp_reply_list = NewList();
    g->g_assigns = NewList();
    g->g_vars = NewList();
    g->g_assert_cmds = NewList();
    g->g_nodes = (node_t *)NULL;
    g->g_lastNode = &g->g_nodes;
    g->g_active = (node_t *)NULL;
    g->g_currActive = (node_t *)NULL;
    g->g_lastActive = (node_t *)NULL;
    g->g_activeEvents = 0;
    g->g_token_id = 0;
    g->g_tokens = (token_t *)NULL;
    g->g_last_token = (token_t *)NULL;
    g->g_tcurrent = (token_t *)NULL;

    // function pointers below
    g->g_display = NULL;
    g->g_display_p2p = display_p2p;
    g->g_compare = compare;
    g->g_dbgout = out;
    g->g_dbgevent = NodeEvent;

    return g;
}

graph_inv *NewGraphInv(graph_t *g, dbgprocset_t *ps) {
    dbg_over_set_t *over_set;
    int d;

    auto gi = new graph_inv;

    // FIXME: This assumes only a single dbginvoke - this might not be the case
    // We don't have any checks here to fail if so.
    SetOverlaySetForProcSet(ps);
    over_set = GetNextOverlaySetForProcSet(ps);

    gi->gp_di = over_set->s_di;

    gi->gp_unbuilt_bs = std::make_unique<DenseBitset>(over_set->s_di->d_nproc);
    // Set every rank to 1, since everything is currently unbuilt.
    gi->gp_unbuilt_bs->invert();
    gi->gp_files = NULL;

    for (d = 0; d < INVOKE_NODES; d++) gi->gp_node[d] = -1;

    // FIXME: This won't work well for user defined process sets...
    // Put this graph_inv in the global hash table
    if (strcmp(over_set->s_di->d_handle, ps->s_name) != 0) {
        HashInsert(g->g_inv, ps->s_name, (void *)gi);
    }

    // FIXME: We still need to insert the di handle since the graph uses it. It
    // should be modified to use the process set name instead.
    HashInsert(g->g_inv, over_set->s_di->d_handle, (void *)gi);

    // Put this dbginvoke_t in the g_di_list
    if (InList(g->g_di_list, over_set->s_di) == 0)
        AddToList(g->g_di_list, over_set->s_di);

    return gi;
}

graph_diff *NewGraphDiff(graph_t *g, int id, AIF *res, const char *stat) {
    graph_diff *gd;
    char *res_name = NULL;

    // create the graph_diff object
    gd = (graph_diff *)Alloc(sizeof(graph_diff));

    // create the name
    Asprintf(&res_name, "AssertID %d %s", id + 1, stat);

    // set the struct members
    gd->gd_assert_id = id;
    gd->gd_name = res_name;
    gd->gd_val = res;

    // add it to the g_diff list
    AddToList(g->g_diff, (void *)gd);

    return gd;
}

void AddActiveEvent(graph_t *g) { g->g_activeEvents++; }

void RemoveActiveEvent(graph_t *g) { g->g_activeEvents--; }

int ActiveEvents(graph_t *g) { return g->g_activeEvents > 0; }

graph_inv *FindGraphInv(graph_t *g, char *inv) {
    hentry_t *h;

    if (g->g_inv == (htable_t *)NULL) return (graph_inv *)NULL;

    if ((h = HashSearch(g->g_inv, inv)) == (hentry_t *)NULL)
        return (graph_inv *)NULL;

    return (graph_inv *)h->h_data;
}

void destroyInv(char *key, void *arg) {
    graph_inv *gi = (graph_inv *)arg;

    DestroyList(gi->gp_files, (void (*)(void *))DestroyFile);

    delete gi;
}

void destroyGDiff(graph_diff *gd) {
    if (gd->gd_name != (char *)NULL) Free(gd->gd_name);
    if (gd->gd_val != (AIF *)NULL) AIFFree(gd->gd_val);

    Free(gd);
}

/*
** Deallocate a graph.
*/
void DestroyGraph(graph_t *g) {
    if (g == (graph_t *)NULL) return;

    DestroyNodes(g->g_nodes);
    HashDestroy(g->g_inv, destroyInv);
    DestroyList(g->g_diff, (void (*)(void *))destroyGDiff);
    DestroyList(g->g_di_list, (void (*)(void *))NULL);
    DestroyList(g->g_bp_reply_list, (void (*)(void *))FreeGroupReply);
    DestroyList(g->g_assigns, (void (*)(void *))DestroyAssign);
    DestroyList(g->g_vars, (void (*)(void *))DestroyVar);
    DestroyList(g->g_assert_cmds, (void (*)(void *))Free);
    DestroyAssertInfo(g->g_assert_info_tab, g->g_assertions);

    if (g->g_assert_pass != NULL) Free(g->g_assert_pass);
    if (g->g_assert_warn != NULL) Free(g->g_assert_warn);
    if (g->g_assert_fail != NULL) Free(g->g_assert_fail);

    while (g->g_tokens != (token_t *)NULL) DestroyToken(g, g->g_tokens);

    Free(g);
}

/*
** Reset a graph. This involves:
** - removing tokens from the inputs of all nodes
** - removing all nodes from the active list
** - recreating start token
*/
void ResetGraph(graph_t *g) {
    node_t *n;

    if (g->g_reset) return;

    for (n = g->g_nodes; n != NULL; n = n->n_next) ResetInputs(g, n);

    g->g_active = (node_t *)NULL;
    g->g_currActive = (node_t *)NULL;
    g->g_lastActive = (node_t *)NULL;
    g->g_activeEvents = 0;

    g->g_setStartNode(g, g->g_startnode);

    g->g_reset = 1;
}

void DebugPrintGraph(graph_t *g) {
    static int print_index = 0;
    char print_name[128];
    FILE *file = Client.cli_out_stream;

    sprintf(print_name, "graph-%d.dot", print_index++);
    file = fopen(print_name, "w");
    PrintGraph(file, g);
}
/*
** Print a graph.
*/
void PrintGraph(FILE *fp, graph_t *g) {
    int i;
    node_t *n;
    output_t *o;
    token_t *t;

    PrintToFd(fp, "digraph a {\n");

    for (n = g->g_nodes; n != (node_t *)NULL; n = n->n_next) {
        PrintToFd(fp, "%d [", n->n_id);

        if ((t = n->n_store) != (token_t *)NULL) {
            switch (t->t_type) {
                case TK_STR:
                    PrintToFd(fp, " label=\"%s\"", t->t_str);
                    break;

                case TK_INT:
                    PrintToFd(fp, " label=%d", t->t_int);
                    break;

                case TK_BIT_SET:
                    PrintToFd(fp, " label=\"%s\"", t->t_bit_set->toSet().c_str());
                    break;

                case TK_FLOAT:
                    PrintToFd(fp, " label=%#g", t->t_float);
                    break;
                case TK_DATA:
                    PrintToFd(fp, " label=");
                    AIFPrint(fp, 0, t->t_data);
                    break;
            }

            PrintToFd(fp, " shape=ellipse ]\n");
        } else {
            PrintToFd(fp, " label=\"%s(%d)\"", n->n_def->nd_name, n->n_id);
            PrintToFd(fp, " shape=box ]\n");
        }

        for (i = 0; i < n->n_nout; i++) {
            o = &n->n_outputs[i];
            PrintToFd(fp, "%d -> %d\n", n->n_id, o->o_node->n_id);
        }
    }

    PrintToFd(fp, "}\n");
}
