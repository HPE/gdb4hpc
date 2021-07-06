/*********************************************************************************\
 * Node routines
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

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "bitset.h"
#include "client.h"
#include "defs.h"
#include "df.h"
#include "useful.h"

/* static int ActiveEventCnt = 0; */

input_t *NewInputs(graph_t *, node_t *, nodedef_t *, int, token_type);
int NewInput(graph_t *, node_t *, int, token_type);
void NewOutput(node_t *, node_t *, int);
void DestroyInputs(node_t *);
void DestroyOutputs(node_t *);

extern void DFError(node_t *, char *, ...);

list_t *TreeNodesList = (list_t *)NULL; /* list of  dbgtreenodesinfo_t */

node_t *NewNode(graph_t *g, int id, nodedef_t *nd, token_t *store) {
    node_t *n;

    n = (node_t *)Alloc(sizeof(node_t));

    n->n_id = id;
    n->n_icnt = 0;

    n->n_graph = g;
    n->n_fired = 0;
    n->n_qlen = g->g_qlen;
    n->n_active = NULL;
    n->n_pactive = NULL;
    n->n_ninp = 0;
    n->n_inputs = NewInputs(g, n, nd, nd->nd_min, TK_ANY);
    n->n_nout = 0;
    n->n_outputs = NULL;
    n->n_def = nd;
    n->n_store = store;
    n->n_next = (node_t *)NULL;

    *(g->g_lastNode) = n;
    g->g_lastNode = &n->n_next;

    n->n_nevent = 0;

    return n;
}

void DestroyNodes(node_t *n) {
    node_t *np;

    while (n != (node_t *)NULL) {
        np = n->n_next;

        DestroyInputs(n);
        DestroyOutputs(n);
        Free(n);

        n = np;
    }
}

/*
** Find a node with matching id in the graph.
*/
node_t *FindNode(graph_t *g, int id) {
    node_t *n;

    for (n = g->g_nodes; n != NULL; n = n->n_next)
        if (n->n_id == id) return n;

    return (node_t *)NULL;
}

/*
** Create the minimum number of inputs for a node.
*/
input_t *NewInputs(graph_t *g, node_t *n, nodedef_t *nd, int num,
                   token_type t) {
    int c;
    input_t *i;
    input_t *ip;

    if (num <= 0 || (nd->nd_max >= 0 && num > nd->nd_max))
        return (input_t *)NULL;

    if (n->n_ninp == 0)
        ip = (input_t *)Alloc(num * sizeof(input_t));
    else
        ip = (input_t *)Resize(n->n_inputs, num * sizeof(input_t));

    for (c = n->n_ninp; c < num; c++) {
        i = &ip[c];

        i->i_tokens = NewQueue(g->g_qlen);
        i->i_from = (node_t *)NULL;
        i->i_node = n;

        if (c < nd->nd_min)
            i->i_type = nd->nd_args[c];
        else
            i->i_type = t;
    }

    n->n_ninp = num;

    return ip;
}

/*
** Verify that port is a valid input, or create new inputs if necessary.
** Will only create new inputs if the current number of inputs is greater
** than the minimum number of inputs for the node (as the minimum number
** have already been created in NewNode().
*/
int NewInput(graph_t *g, node_t *n, int port, token_type t) {
    if (!ValidTokenType(t)) {
        SetError("NewInput called with bad token, node %d", n->n_id);
        return -1;
    }

    if (port < 0) port = n->n_ninp;

    if (n->n_max >= 0 && port >= n->n_max) {
        SetError("maximum number of inputs exceeded, node %d", n->n_id);
        return -1;
    }

    if (!CheckInputType(n, port, t)) {
        SetError("invalid input type, node %d, input %d, token %s(%d)", n->n_id,
                 port, TokenTypeStr(t), t);
        return -1;
    }

    if (port >= n->n_ninp) n->n_inputs = NewInputs(g, n, n->n_def, port + 1, t);

    if (n->n_inputs[port].i_from != (node_t *)NULL) {
        SetError("port %d of node %d already connected", port, n->n_id);
        return -1;
    }

    return port;
}

/*
** Deallocate input list.
*/
void DestroyInputs(node_t *n) {
    int c;
    input_t *i;

    if (n->n_ninp == 0) return;

    i = n->n_inputs;

    for (c = 0; c < n->n_ninp; c++) {
        DestroyQueue(i->i_tokens);
        i++;
    }

    Free(n->n_inputs);
}

/*
** Remove tokens from all inputs. Reset input count to 0.
*/
void ResetInputs(graph_t *g, node_t *n) {
    int i;
    input_t *ip;
    token_t *t;

    if (n->n_ninp == 0) return;

    for (i = 0; i < n->n_ninp; i++) {
        ip = &n->n_inputs[i];

        if (EmptyQueue(ip->i_tokens)) continue;

        while ((t = (token_t *)RemoveFromQueue(ip->i_tokens)) !=
               (token_t *)NULL)
            DestroyToken(g, t);
    }

    n->n_icnt = 0;
}

void NewOutput(node_t *n, node_t *on, int i) {
    output_t *o;

    n->n_outputs =
        (output_t *)Resize(n->n_outputs, (n->n_nout + 1) * sizeof(output_t));

    o = &n->n_outputs[n->n_nout++];
    o->o_node = on;
    o->o_inp = i;
}

void DestroyOutputs(node_t *n) {
    if (n->n_nout > 0) Free(n->n_outputs);
}

/*
** Connect a node "from" to port "port" of node "to".
*/
int Connect(graph_t *g, int from, int to, int port) {
    node_t *fn;
    node_t *tn;

    if ((fn = FindNode(g, from)) == NULL) {
        SetError("could not find node %d", from);
        return 0;
    }

    if ((tn = FindNode(g, to)) == NULL) {
        SetError("could not find node %d", to);
        return 0;
    }

    if ((port = NewInput(g, tn, port, fn->n_otype)) < 0) return 0;

    DPRINT("connecting node %d output %d to node %d port %d\n", fn->n_id,
           fn->n_nout, tn->n_id, port);

    NewOutput(fn, tn, port);

    return 1;
}

/*
** Check that all nodes have been connected correctly.
*/
int CheckNodes(graph_t *g) {
    int c = 0;
    node_t *n;

    for (n = g->g_nodes; n != NULL; n = n->n_next) {
        if (n->n_ninp < n->n_min) {
            PrintOutError("[CheckNodes]: Not enough inputs for node %d (%s)\n",
                          n->n_id, n->n_name);
            return 0;
        }

        c++;
    }

    return c;
}

/*
** Utility routine to check a token type matches the
** type of the input to the node.
*/
int CheckInputType(node_t *n, int i, token_type t) {
    if (i >= MAXARGCHECK) return 1;

    return (int)(n->n_args[i] & t);
}

/*
** Utility routine to check tokens on all inputs to a node
** are the correct type.
*/
int CheckInputs(node_t *n) {
    int c;
    input_t *i;
    token_t *t;

    for (c = 0; c < n->n_ninp; c++) {
        i = &n->n_inputs[c];

        if ((t = (token_t *)HeadOfQueue(i->i_tokens)) == (token_t *)NULL)
            continue;

        if (!CheckInputType(n, c, t->t_type)) {
            PrintOutError(
                "Node %d has bad token type %s on input %d, expected %s\n",
                n->n_id, TokenTypeStr(t->t_type), c, TokenTypeStr(i->i_type));

            DumpNode(n);

            return 0;
        }
    }

    return 1;
}

/*
** Fire a node. Called if inputs to node have
** been satisfied.
*/
int FireNode(graph_t *g, node_t *n) {
    int i;
    input_t *ip;
    token_t *t;
    token_t *tn;

    g->g_reset = 0;

    n->n_fired++;

    /*
    ** fire node function
    */
    tn = n->n_func(g, n);

    /*
    ** Call node output routine
    */

    GenNodeOutput(g, n, tn);

    /*
    ** Input tokens have been consumed. Remove and
    ** destroy them.
    */

    for (i = 0; i < n->n_ninp; i++) {
        ip = &n->n_inputs[i];

        if (EmptyQueue(ip->i_tokens)) continue;

        if ((t = (token_t *)RemoveFromQueue(ip->i_tokens)) != (token_t *)NULL)
            DestroyToken(g, t);

        if (EmptyQueue(ip->i_tokens)) n->n_icnt--;
    }

    /*
    ** Remove node from active list if necessary.
    */
    switch (n->n_type) {
        case NODE_NORMAL:
            if (n->n_icnt < n->n_ninp) RemoveFromActive(g, n);
            break;

        case NODE_FIREONANY:
            if (n->n_icnt == 0) RemoveFromActive(g, n);
            break;
    }

    return 1;
}

/*
** Inject a token into the graph as if it was generated by a node.
*/
void InjectToken(graph_t *g, node_t *n, token_t *t) {
    /*
    ** Call node output routine
    */

    GenNodeOutput(g, n, t);
}

/*
** Return the next node on the active list. Return NULL if
** there are no more active nodes.
*/
node_t *GetActiveNode(graph_t *g) {
    node_t *n;

    if (g->g_currActive == NULL) g->g_currActive = g->g_active;

    n = g->g_currActive;

    if (g->g_currActive != NULL) g->g_currActive = g->g_currActive->n_active;

    return n;
}

/*
** Add node to active list of graph.
*/
void AddToActive(graph_t *g, node_t *n) {
    if (g == (graph_t *)NULL || n == (node_t *)NULL) return;

    if (g->g_active == (node_t *)NULL) {
        g->g_active = n;
        g->g_lastActive = n;

        return;
    }

    g->g_lastActive->n_active = n;
    n->n_pactive = g->g_lastActive;
    g->g_lastActive = n;
}

/*
** Remove node from active list of graph.
*/
void RemoveFromActive(graph_t *g, node_t *n) {
    if (g == (graph_t *)NULL || n == (node_t *)NULL) return;

    /*
    ** Unlink node from active list.
    */
    if (n->n_pactive != (node_t *)NULL) n->n_pactive->n_active = n->n_active;

    if (n->n_active != (node_t *)NULL) n->n_active->n_pactive = n->n_pactive;

    /*
    ** If we are head of list then move head to next.
    */
    if (g->g_active == n) g->g_active = n->n_active;

    /*
    ** If we are tail of list then move tail to prev.
    */
    if (g->g_lastActive == n) g->g_lastActive = n->n_pactive;

    /*
    ** if we are current then move current to next
    */
    if (g->g_currActive == n) g->g_currActive = n->n_active;

    if (g->g_currActive == (node_t *)NULL) g->g_currActive = g->g_active;

    n->n_pactive = (node_t *)NULL;
    n->n_active = (node_t *)NULL;
}

/*
** Generic node output routine.  Sends new token to node outputs.
*/
void GenNodeOutput(graph_t *g, node_t *n, token_t *t) {
    int o;
    input_t *ip;
    output_t *op;

    /*
    ** Send new token to output(s).
    */

    if (t == (token_t *)NULL) return;

    for (o = 0; o < n->n_nout; o++) {
        op = &n->n_outputs[o];
        ip = &op->o_node->n_inputs[op->o_inp];
        GenNodeInput(g, op->o_node, ip, t);
    }
}

/*
** Generic node input routine. Puts new token on
** node input. Adds node to active list if necessary.
*/
void GenNodeInput(graph_t *g, node_t *n, input_t *i, token_t *t) {
    if (EmptyQueue(i->i_tokens)) n->n_icnt++;

    AddToQueue(i->i_tokens, (void *)DupToken(t));

    /*
    ** Add node to active list if it's not already on
    ** the list and it is ready to fire.
    */
    if (n->n_active == (node_t *)NULL &&
        (n->n_icnt == n->n_ninp || n->n_type == NODE_FIREONANY))
        AddToActive(g, n);
}

#ifdef notdef
/*
** Generic routine to check if node ready to fire.
*/
int GenReadyToFire(graph_t *g, node_t *n) {
    if ((n->n_ninp == n->n_icnt) && n->n_flow(g, n)) return 1;

    return 0;
}

/*
** Fire node if token on any input.
*/
int FireOnAny(graph_t *g, node_t *n) {
    if (n->n_icnt >= 1 && n->n_flow(g, n)) return 1;

    return 0;
}
#endif /* notdef */

/*
** Generic routine to check flow control
*/
int CheckIfQueueFull(graph_t *g, node_t *n) {
    int o;
    input_t *ip;
    output_t *op;

    for (o = 0; o < n->n_nout; o++) {
        op = &n->n_outputs[o];
        ip = &op->o_node->n_inputs[op->o_inp];

        if (FullQueue(ip->i_tokens)) {
            DFError(n, "queue full, node %d, input %d", op->o_node->n_id,
                    op->o_inp);
            return 0;
        }
    }

    return 1;
}

void DumpNodes(graph_t *g) {
    node_t *n;

    PrintOutError("==========================\n");

    for (n = g->g_nodes; n != NULL; n = n->n_next) {
        PrintOutError("  ");
        DumpNode(n);
    }

    PrintOutError("==========================\n");
}

void DumpNode(node_t *n) {
    int o;
    input_t *ip;
    output_t *op;
    qel_t *q;

    PrintOutError("%s(%d)->[", n->n_name, n->n_id);

    for (o = 0; o < n->n_nout; o++) {
        PrintOutError("{");

        op = &n->n_outputs[o];
        ip = &op->o_node->n_inputs[op->o_inp];

        for (q = ip->i_tokens->q_head; q != NULL; q = q->q_next) {
            DumpToken((token_t *)q->q_el);

            if (q->q_next != NULL) PrintOutError(",");
        }

        PrintOutError("%d", ip->i_node->n_id);

        PrintOutError("}");

        if (o < n->n_nout - 1) PrintOutError(",");
    }

    PrintOutError("]\n");
}

void DumpActiveNodes(graph_t *g, node_t *current, FILE *log) {
    node_t *n = NULL;
    n = g->g_active;

    if (log != NULL) {
        fprintf(log, "%d: ", current->n_id);
    } else {
        DPRINT("%d: ", current->n_id);
    }

    if (n == NULL) return;

    while (n != NULL && n != g->g_lastActive) {
        if (log != NULL) {
            fprintf(log, "%d,", n->n_id);
        } else {
            DPRINT("%d,", n->n_id);
        }

        n = n->n_active;
    }

    if (log != NULL) {
        fprintf(log, "\n");
    } else {
        DPRINT("\n");
    }
}

dbgnodesetinfo_t *NewNodeSetInfo(void *node, const DenseBitset *bs) {
    auto *info = new dbgnodesetinfo_t;
    info->t_node = node;
    DASSERT_NOT_NULL(bs);
    info->t_bitset = bs->typedClone();
    DPRINT("INFO[NewNodeSetInfo]: bitset[%p]:%s\n", bs,
        ((DenseBitset*)bs)->toSet().c_str());

    return info;
}

void FreeNodeSetInfo(dbgnodesetinfo_t *info) {
    if (info != NULL) {
        info->t_node = NULL;
    }

    delete info;
}

dbgtreenodesinfo_t *NewTreeNodesInfo(void *tree) {
    dbgtreenodesinfo_t *info =
        (dbgtreenodesinfo_t *)Alloc(sizeof(dbgtreenodesinfo_t));
    info->t_tree = tree;
    info->t_node_list = NewList();
    return info;
}

void FreeTreeNodesInfo(dbgtreenodesinfo_t *info) {
    if (info != NULL) {
        info->t_tree = NULL;
        DestroyList(info->t_node_list, (void (*)(void *))FreeNodeSetInfo);
    }
}

dbgtreenodesinfo_t *GetNodesInfoByTree(void *tree) {
    dbgtreenodesinfo_t *info = NULL;

    SetList(TreeNodesList);
    while ((info = (dbgtreenodesinfo_t *)GetListElement(TreeNodesList)) !=
           (dbgtreenodesinfo_t *)NULL) {
        if (info->t_tree == tree) break;
    }

    return info;
}

void AddNodeInfoByTree(void *tree, void *node, const DenseBitset *bs) {
    dbgtreenodesinfo_t *info = NULL;
    dbgnodesetinfo_t *nodes_set = NewNodeSetInfo(node, bs);

    if (TreeNodesList == (list_t *)NULL) {
        TreeNodesList = NewList();
    }

    info = GetNodesInfoByTree(tree);
    if (info == NULL) {
        info = NewTreeNodesInfo(tree);
        AddToList(TreeNodesList, info);
    }

    AddToList(info->t_node_list, nodes_set);
    DASSERT_NOT_NULL(bs);
    DPRINT("INFO[AddNodeInfoByTree]: node[%d], bitset[%s]\n",
        ((node_t *)node)->n_id, bs->toSet().c_str());
}

void RemoveNodeForTree(void *tree, void *node) {
    dbgtreenodesinfo_t *info = NULL;
    dbgnodesetinfo_t *nodes_set = NULL;
    list_t *ns_list;

    if (TreeNodesList == (list_t *)NULL) {
        return;
    }

    info = GetNodesInfoByTree(tree);
    if (info == NULL) {
        return;
    }

    ns_list = info->t_node_list;
    while ((nodes_set = (dbgnodesetinfo_t *)GetListElement(ns_list)) !=
           (dbgnodesetinfo_t *)NULL) {
        if (nodes_set->t_node == node) break;
    }

    RemoveFromList(ns_list, nodes_set);
}

void CleanTreeNodesList() {
    if (TreeNodesList != NULL) {
        DestroyList(TreeNodesList, (void (*)(void *))FreeTreeNodesInfo);
    }
}
