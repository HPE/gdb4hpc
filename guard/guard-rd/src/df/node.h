/*********************************************************************************\
 * Node definition
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

#ifndef _NODE_H
#define _NODE_H

#include "queue.h"
#include "token.h"

struct input_t {
    struct tkqueue_t* i_tokens;
    struct node_t* i_from;
    struct node_t* i_node;
    token_type i_type;
};
typedef struct input_t input_t;

struct output_t {
    int o_inp;
    struct node_t* o_node;
};
typedef struct output_t output_t;

enum node_type { NODE_NORMAL, NODE_FIREONANY };
typedef enum node_type node_type;

struct node_t {
    int n_id;                   /* node id */
    int n_ninp;                 /* number of inputs */
    int n_nout;                 /* number of outputs */
    int n_icnt;                 /* number of tokens */
    int n_qlen;                 /* input queue length */
    int n_fired;                /* node has fired */
    struct graph_t* n_graph;    /* this nodes graph */
    struct input_t* n_inputs;   /* input array */
    struct output_t* n_outputs; /* output array */
    struct nodedef_t* n_def;    /* node definition */
    struct token_t* n_store;    /* token store */
    struct node_t* n_active;    /* active list (forward) */
    struct node_t* n_pactive;   /* active list (backward) */
    struct node_t* n_next;      /* next node */
    int n_nevent;               /* number of pending "magic" events */
};
typedef struct node_t node_t;

#define n_name n_def->nd_name
#define n_min n_def->nd_min
#define n_max n_def->nd_max
#define n_otype n_def->nd_otype
#define n_type n_def->nd_type
#define n_func n_def->nd_func
/*
#define n_fire		n_def->nd_fire
#define n_flow		n_def->nd_flow
#define n_output	n_def->nd_output
#define n_input		n_def->nd_input
*/
#define n_args n_def->nd_args

struct dbgnodesetinfo_t {
    void* t_node;
    void* t_bitset;
};
typedef struct dbgnodesetinfo_t dbgnodesetinfo_t;

/* list of nodes that are relevant to the same tree*/
struct dbgtreenodesinfo_t {
    void* t_tree;
    list_t* t_node_list; /*list of */
};
typedef struct dbgtreenodesinfo_t dbgtreenodesinfo_t;

extern void AddNodeInfoByTree(void* tree, void* node, void* bitset);
extern dbgtreenodesinfo_t* GetNodesInfoByTree(void* tree);
extern void RemoveNodeForTree(void* tree, void* node);
extern void CleanTreeNodesList();

#endif /* !_NODE_H */
