/*********************************************************************************\
 * Routines for building graph
 * TODO: avoid building duplicate GETVAR's.
 * andrewg: I think what the above is trying to say, is that if we have multiple
 * assertions against a single var at the same line number, we will grab the
 *value of the var for each assertion, instead of just caching it. The above
 *todo has thrown me off for quite some time.
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

// FIXME: Get rid of the set specific functions, merge into a single function.
//        This interface is f*cked up and hasn't been fixed from the Monash
//        days.

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <string.h>

#include "dense_bitset.h"
#include "defs.h"
#include "df.h"
#include "gram.h"
#include "hash.h"
#include "procset.h"
#include "useful.h"

#define PROC_PROC_NODE(p) (p)->gp_node[0]
#define PROC_SYNC_NODE(p) (p)->gp_node[1]
#define PROC_MERGE_NODE(p) (p)->gp_node[2]
#define PROC_GO_NODE(p) (p)->gp_node[3]

typedef struct extrainfo {
    char hashinfo[BUFSIZ];
    char blockmapinfo[BUFSIZ];
} extrainfo;

// static prototypes
static int BuildInvoke(graph_t *, var_t *);
static int BuildSetBPSet(graph_t *, int, int, var_t *);
static void BuildGetVar(graph_t *, int *, int *, var_t *, int, extrainfo *);
static void BuildGetVarSet(graph_t *, int *, int *, var_t *, int, int,
                           extrainfo *);
static int BuildCmp(graph_t *, int, int, int, int);
static int BuildBlockMap(graph_t *, g_block_map_t *, var_t *, extrainfo *, int);
static void BuildMerge(graph_t *, var_t *, int, int);
static int BuildMergeProc(graph_t *, graph_t::graph_inv *, var_t *, int, int, int);
static void BuildSync(graph_t *, var_t *, int, int);

static int AsyncMode(graph_t *, list_t *);
static int build_expr(graph_t *, int *, int *, list_t *, expr_t *, rec_set_t *,
                      func_t *, extrainfo *);
static int build_func(graph_t *, hentry_t *, int *, int *, list_t *, expr_t *,
                      rec_set_t *, func_t *, extrainfo *);
static int BuildOp(graph_t *, expr_op, int, int, int, int, int);
static int _validate_block_map(graph_t *, g_block_map_t *, var_t *);

// Temp location to store assertid as a means of passing it down to DbgGetVar
// without adding an extra parameter to build_expr.  Note: Can we pass assert id
// down through the vars? Look into this.
static int iAssertID = -1;

/*
** If first time, output nodes and edges to invoke program.
*/
static int BuildInvoke(graph_t *g, var_t *v) {
    dbgprocset_t *ps = v->v_procset;
    graph_t::graph_inv *gi;
    int xt1;
    int xt2;
    int xt3;
    int xt4;
    int t1;
    int t2;
    int t3;
    file_t *f = NULL;
    int built_bp = 1;

    // FIXME: This only supports DBGSET_INVOKE for now
    // Update: This will break when a process set contains multiple dbginvoke
    // objects in it. The graph layer doesn't play nicely when multiple
    // applications belonging to a process set are used in an assertion. We now
    // do nothing to check for this condition.
    if (ps == NULL) {
        SetError("Cannot use custom procset in assert script!");
        return -1;
    }

    // Try to find an existing graph_inv, otherwise create it for this di.
    if ((gi = g->FindGraphInv(ps->s_name)) == NULL) gi = g->NewGraphInv(ps);

    // Create a new bitset to figure out if we need to create a PROC_SET node
    // for this particular group
    auto token_bit_set = DenseBitset(gi->gp_di->d_nproc);

    // FIXME: The v_indiv stuff should go away.
    if (v->v_indiv < 0) {
        // Set all ranks to 1
        token_bit_set.invert();
    } else {
        // set the requested rank to one, this should be fixed to use a range
        token_bit_set.set(v->v_indiv);
    }

    // Check to see if we need to build any ranks
    token_bit_set &= *gi->gp_unbuilt_bs;

    // Only create the node if there are some ranks set
    if (!token_bit_set.isEmpty()) {
        // First build PROC_SET
        xt1 = g->outNode("PROC_SET");
        xt2 = g->outConstStr(gi->gp_di->d_handle);
        g->outEdge(g->initNode(), xt2, 0);
        xt3 = g->outConstBitSet(&token_bit_set);
        DPRINT("BuildInvokeProcSet token_bit_set[%s], length[%lu]\n",
               token_bit_set.toSetString().c_str(), token_bit_set.sizeInBits());
        g->outEdge(g->initNode(), xt3, 0);
        g->outEdge(xt2, xt1, 0);
        g->outEdge(xt3, xt1, 1);

        xt4 = g->outNode("SYNC");

        PROC_PROC_NODE(gi) = xt1;
        PROC_SYNC_NODE(gi) = xt4;

        // Now build MERGE and DBGGO
        xt1 = g->outNode("MERGE");
        xt2 = g->outNode("DBGGO_SET");

        g->outEdge(PROC_SYNC_NODE(gi), xt1, 0);
        g->outEdge(xt1, xt2, 0);

        /*
        ** Check if WaitBreak returns 0. This means the
        ** process has exited, so exit graph if it is.
        */
        t1 = g->outConstInt(0);
        t2 = g->outNode("PIEQ");
        t3 = g->outNode("EXIT");

        g->outEdge(xt2, t1, 0);
        g->outEdge(xt2, t2, 0);
        g->outEdge(t1, t2, 1);

        g->outEdge(xt1, t2, 2);
        g->outEdge(t2, t3, 0);

        PROC_MERGE_NODE(gi) = xt1;
        PROC_GO_NODE(gi) = xt2;

        // Modify the gp_unbuilt_bs to no longer include the ranks we just built
        *gi->gp_unbuilt_bs -= token_bit_set;
    }

    /*
    ** Save breakpoint info.
    */
    if (v->v_line == NULL) {
        v->v_line = (line_t *)Alloc(sizeof(line_t));
    }

    f = AddFile(&gi->gp_files, v->v_fname);
    v->v_line = AddLine(&f->f_lines, v->v_lno);

    if (v->v_bs == NULL) v->v_bs = std::make_unique<DenseBitset>(gi->gp_di->d_nproc);

    if (v->v_indiv >= 0) built_bp = v->v_bs->test(v->v_indiv);

    if (v->v_bp < 0 || !built_bp) {
        v->v_bp = BuildSetBPSet(g, PROC_PROC_NODE(gi), PROC_SYNC_NODE(gi), v);
        if (v->v_indiv < 0)
            v->v_bs->invert();
        else {
            v->v_bs->set(v->v_indiv);
            v->v_pid = -1;
        }
    }

    /*
    ** FIXME: I have no fucking clue what this does...
    */
    if (v->v_pid < 0) {
        t1 = g->outConstInt(v->v_bp);
        t2 = g->outNode("RECALL");
        t3 = g->outNode("PIEQ");
        v->v_pid = t3;

        g->outEdge(PROC_GO_NODE(gi), t1, 0);
        g->outEdge(t1, t2, 0);
        g->outEdge(t2, t3, 0);
        g->outEdge(PROC_GO_NODE(gi), t3, 1);
        g->outEdge(PROC_MERGE_NODE(gi), t3, 2);

        v->v_pid = t3;
        v->v_sync = -1;  // v->v_pid and v->v_sync should be consistent
    }

    return 0;
}

static int BuildSetBPSet(graph_t *g, int i1, int i2, var_t *v) {
    int n1;
    int n2;
    int n3;
    int n4;
    int n5;
    int n6;

    DPRINT("In BuildSetBPSet\n");

    n1 = g->outConstInt(v->v_lno);
    n2 = g->outConstStr(v->v_fname);
    n3 = g->outNode("DBGSETBRK_SET");

    g->outEdge(i1, n1, 0);
    g->outEdge(i1, n2, 0);
    g->outEdge(i1, n3, 0);
    g->outEdge(n1, n3, 1);
    g->outEdge(n2, n3, 2);

    n4 = g->outConstInt(g->lastStore());
    n5 = g->outNode("STORE");
    n6 = g->outNode("PIT");

    g->outEdge(n3, n4, 0);
    g->outEdge(n4, n5, 0);
    g->outEdge(n3, n5, 1);
    g->outEdge(i1, n6, 1);

    g->outEdge(n5, n6, 0);

    /*
    ** Link to SYNC node.
    */
    g->outEdge(n6, i2, -1);

    return g->fetchIncLastStore();
}

/*
** This code is to start any remaining parallel processes
** that haven't been referenced in an assert. These consist of
** all processes belonging to an invoke, where gp_built is
** false.
*/
void BuildUnused(graph_t *g) {
    graph_t::graph_inv *gi;
    dbginvoke_t *di;
    int xt1;
    int xt2;
    int xt3;
    int xt4;

    // hard code to reset assert id to -1
    // This is so, so, so, so, so stupidy broken
    iAssertID = -1;

    DPRINT("In BuildUnused\n");
    DASSERT_NOT_NULL(g);

    SetList(g->diList());
    while ((di = (dbginvoke_t *)GetListElement(g->diList())) != NULL) {
        gi = g->FindGraphInv(di->d_handle);
        DASSERT_NOT_NULL(gi);
        DASSERT_NOT_NULL(gi->gp_unbuilt_bs);

        // check if there are any unbuilt ranks
        if (!gi->gp_unbuilt_bs->isEmpty()) {
            xt1 = g->outNode("PROC_SET");
            xt2 = g->outConstStr(di->d_handle);
            g->outEdge(g->initNode(), xt2, 0);
            xt3 = g->outConstBitSet(gi->gp_unbuilt_bs.get());
            DPRINT("BuildUnusedSet bitset[%s], length[%lu]\n",
                   gi->gp_unbuilt_bs->toSetString().c_str(),
                   gi->gp_unbuilt_bs->sizeInBits());
            g->outEdge(g->initNode(), xt3, 0);
            g->outEdge(xt2, xt1, 0);
            g->outEdge(xt3, xt1, 1);

            xt4 = g->outNode("DBGGO_SET");
            g->outEdge(xt1, xt4, 0);

            // clear the remaining ranks
            gi->gp_unbuilt_bs->clear();
        }
    }
}

// Minh changed BuildGetVarSet() to include an extra parameters (extrainfo)
static void BuildGetVarSet(graph_t *g, int *inp, int *out, var_t *v,
                           int low_pid, int proc_count, extrainfo *extra)
// TODO: update signature.
{
    int n;
    int t1;
    int n4;
    int n6;

    int a1;

    std::string varexpr;
    std::ostringstream finalexpr;

    DPRINT("In BuildGetVarSet\n");

    if (v->v_internal) {
        n4 = g->outNode("GETVAR");
        t1 = g->outConstStr(v->v_var);
    } else {
        if (auto s = v->v_slice) {
            varexpr = s->getExpr(v->v_var);
        } else {
            varexpr = v->v_var;
        }

        finalexpr << varexpr;

        t1 = g->outConstStr(finalexpr.str().c_str());

        n4 = g->outNode("DBGGETVAR_SET");
    }

    *inp = g->outNode("PIT");

    g->outEdge(v->v_pid, *inp, 1);
    //	g->outEdge(v->v_pid(low_pid), *inp, 1);

    /* MINH TODO : NEED TO REVIEW THIS BIT OF CODE */
    /* Minh add here to implement blockmap and parallel vs parallel
     * 19/05/2009
     * Donny TODO: Probably we need to synchronize
     */
    g->outEdge(v->v_pid, *inp, 0);
    //    g->outEdge(v->v_pid(low_pid), *inp, 0);

    g->outEdge(*inp, t1, 0);
    // Const for asserid
    a1 = g->outConstInt(iAssertID);
    g->outEdge(*inp, a1, 0);

    g->outEdge(*inp, n4, 0);
    g->outEdge(t1, n4, 1);

    // Const for asserid
    g->outEdge(a1, n4, 2);

    if (v->v_perm != (list_t *)NULL) {
        n6 = g->outNode("PERMUTE");
        g->outEdge(n4, n6, 0);

        SetPerm(v->v_perm);

        n = 1;

        while (auto p = GetPerm(v->v_perm)) {
            t1 = g->outConstInt(p->p_to);

            g->outEdge(n4, t1, 0);
            g->outEdge(t1, n6, n++);
        }

        n4 = n6;
    }

    *out = n4;
}

// Minh changed BuildGetVar() to include an extra parameters (extrainfo)
static void BuildGetVar(graph_t *g, int *inp, int *out, var_t *v, int pid,
                        extrainfo *extra) {
    int n;
    int t1;
    int n4;
    int n6;

    int a1;

    int p2pmode = 0;

    DPRINT("In BuildGetVar\n");

    if (v->v_internal) {
        n4 = g->outNode("GETVAR");
        t1 = g->outConstStr(v->v_var);
    } else {
        std::ostringstream finalexpr;
        std::string varexpr;

        if (v->v_slice) {
            varexpr = v->v_slice->getExpr(v->v_var);
        } else {
            varexpr = v->v_var;
        }

        // if there is p2p mode turn on, attach blockmap info
        if (p2pmode) {
            finalexpr << "`ptop(" << v->v_pname << "-" << extra->blockmapinfo << ")`" << varexpr;
        } else {
            finalexpr << varexpr;
        }

        t1 = g->outConstStr(finalexpr.str().c_str());
        n4 = g->outNode("DBGGETVAR");
    }

    *inp = g->outNode("PIT");
    g->outEdge(v->v_pid, *inp, 1);
    //	g->outEdge(v->v_pid(pid), *inp, 1);

    /* MINH TODO : NEED TO REVIEW THIS BIT OF CODE */
    /* Minh add here to implement blockmap and parallel vs parallel
     * 19/05/2009
     * Donny TODO: Probably we need to synchronize
     */
    g->outEdge(v->v_pid, *inp, 0);
    //    g->outEdge(v->v_pid(pid), *inp, 0);

    g->outEdge(*inp, t1, 0);
    // Const for asserid
    a1 = g->outConstInt(iAssertID);
    g->outEdge(*inp, a1, 0);

    g->outEdge(*inp, n4, 0);
    g->outEdge(t1, n4, 1);

    // Const for asserid
    g->outEdge(a1, n4, 2);

    if (v->v_perm != (list_t *)NULL) {
        n6 = g->outNode("PERMUTE");
        g->outEdge(n4, n6, 0);

        SetPerm(v->v_perm);

        n = 1;

        while (auto p = GetPerm(v->v_perm)) {
            t1 = g->outConstInt(p->p_to);

            g->outEdge(n4, t1, 0);
            g->outEdge(t1, n6, n++);
        }

        n4 = n6;
    }

    *out = n4;
}

static int _validate_block_map(graph_t *g, g_block_map_t *bm, var_t *v) {
    int rank = bm->m_rank;
    int *size = (int *)Alloc(sizeof(int) * rank);
    int i;

    DPRINT("In _validate_block_map\n");

    for (i = 0; i < rank; i++) size[i] = bm->m_data[i];

    if (_get_hash_block_size(rank, bm->m_data, bm->m_distribute,
                             bm->m_blockcyclic_val, bm->m_procs, size) != 1) {
        char err_message[BUFSIZ];
        sprintf(err_message,
                "BlockMap %s decomposition is not feasible with the given "
                "array shape",
                SymLookup(bm->m_sym));
        SetError(err_message);
        Free(size);
        return 0;
    }

    Free(size);
    return 1;
}

/* Minh add this function to implement block map
 * 15/05/2009
 */
static int BuildBlockMap(graph_t *g, g_block_map_t *bm, var_t *v,
                         extrainfo *extra, int blockmap_required) {
    int n1 = 0;
    int n2 = 0;
    int n3 = 0;
    int t1;
    int t2;
    int t3;
    graph_t::graph_inv *gi;

    std::ostringstream mapname;

    DPRINT("In BuildBlockMap\n");

    gi = g->FindGraphInv(v->v_pname);

    mapname << SymLookup(bm->m_sym) << "(" << v->v_var << ")";

    if (blockmap_required) {
        n1 = g->outConstStr(mapname.str().c_str());

        t3 = g->outNode("TYPE_SET");
        n3 = g->outNode("BLOCKMAP_SET");
        g->outEdge(n1, n3, 0);
        g->outEdge(t3, n3, 1);

        if (v->v_procset != (dbgprocset_t *)NULL && v->v_indiv >= 0) {
            n2 = g->outConstInt(1);
            g->outEdge(n2, n3, 2);

            BuildGetVar(g, &t1, &t2, v, v->v_indiv, extra);

            g->outEdge(t2, n3, 3);
            g->outEdge(t2, n1, 0);
            g->outEdge(t2, n2, 0);
            g->outEdge(t2, t3, 0);

            return n3;
        }

        n2 = g->outConstInt(bm->m_actual->v_procset->s_nproc);
        g->outEdge(n2, n3, 2);

        BuildGetVarSet(g, &t1, &t2, v, 0, gi->gp_di->d_nproc, extra);

        g->outEdge(t2, n3, 3);

        g->outEdge(t2, n1, 0);
        g->outEdge(t2, n2, 0);
        g->outEdge(t2, t3, 0);
    } else {
        /*
        n3 = g->outNode("PIEQ");
        n2 = g->outConstInt(0);
        n1 = g->outNode("EXIT");
        */

        if (v->v_procset != (dbgprocset_t *)NULL && v->v_indiv >= 0) {
            BuildGetVar(g, &t1, &t2, v, v->v_indiv, extra);

            g->outEdge(t2, n2, 0);
            g->outEdge(t2, n3, 1);
            g->outEdge(t2, n3, 2);
            g->outEdge(n2, n3, 3);
            g->outEdge(n3, n1, 0);

            return n3;
        }

        BuildGetVarSet(g, &t1, &t2, v, 0, gi->gp_di->d_nproc, extra);

        /*
        g->outEdge(t2, n2, 0);
        g->outEdge(t2, n3, 0);
        g->outEdge(t2, n3, 1);
        g->outEdge(n2, n3, 2);
        g->outEdge(n3, n1, 0);
        */
    }

    return n3;
}

static int BuildCmp(graph_t *g, int id, int data1, int data2, int cmpop) {
    int n1;
    int n3;
    int t1;
    int t2;

    DPRINT("In BuildCmp\n");

    n1 = g->outNode("COMPARE");
    t1 = g->outConstInt(id);
    g->outEdge(data1, t1, 0);
    g->outEdge(t1, n1, 0);
    g->outEdge(data1, n1, 1);
    g->outEdge(data2, n1, 2);
    t1 = g->outConstInt(cmpop);
    g->outEdge(data1, t1, 0);
    g->outEdge(t1, n1, 3);

    t1 = g->outConstInt(0);
    g->outEdge(n1, t1, 0);
    n3 = g->outNode("PIEQ");
    g->outEdge(n1, n3, 0);
    g->outEdge(t1, n3, 1);
    g->outEdge(n1, n3, 2);

    t2 = g->outNode("EXIT");
    g->outEdge(n3, t2, 0);

    return n1;
}

/*
** Build an expression graph.
*/
int BuildExpr(graph_t *g, int *inp, int *out, list_t *res, expr_t *e,
              rec_set_t *rec_set) {
    extrainfo extra;

    DPRINT("In BuildExpr\n");

    if (e->e_lval == 0) {
        SetError("Nothing to do!");
        return -1;
    }

    RESET(e);

    return build_expr(g, inp, out, res, e, rec_set, (func_t *)NULL, &extra);
}

/*
** Evaluate a declarative expression.
**
** XXX Should only allow one debugger variable per exprsession.
*/
static int build_expr(graph_t *g, int *inp, int *out, list_t *vlist, expr_t *e,
                      rec_set_t *rec_set, func_t *f, extrainfo *extra) {
    int out1;
    int out2;
    int out3;
    int inp1;
    int inp2;
    int inp3;
    list_t *vl;
    var_t *v;
    arg_t *a;
    expr_elt *ep;
    graph_t::graph_inv *gi;

    DPRINT("In build_expr\n");

    ep = POP(e);

    DPRINT("build_expr: Popped expr_op %s\n", getExprOpStr(ep->e_op));

    switch (ep->e_op) {
        case EXPR_OPCOND:
            vl = NewList();

            if (build_expr(g, &inp1, &out1, vl, e, rec_set, f, extra) < 0)
                return -1;

            AppendList(vlist, vl);
            DestroyList(vl, (void (*)(void *))NULL);

            /* drop through */

        case EXPR_OPADD:
        case EXPR_OPSUB:
        case EXPR_OPMUL:
        case EXPR_OPDIV:
        case EXPR_OPREM:
        case EXPR_OPEQUAL:
        case EXPR_OPNOTEQUAL:
        case EXPR_OPLEQ:
        case EXPR_OPGEQ:
        case EXPR_OPLT:
        case EXPR_OPGT:
        case EXPR_OPAND:
        case EXPR_OPOR:
            vl = NewList();

            if (build_expr(g, &inp2, &out2, vl, e, rec_set, f, extra) < 0)
                return -1;

            AppendList(vlist, vl);
            DestroyList(vl, (void (*)(void *))NULL);

            /* drop through */

        case EXPR_OPPOS:
        case EXPR_OPNEG:
            vl = NewList();

            if (build_expr(g, &inp3, &out3, vl, e, rec_set, f, extra) < 0)
                return -1;

            if ((*out = BuildOp(g, ep->e_op, out3, inp2, out2, inp1, out1)) < 0)
                return -1;

            *inp = inp3;

            AppendList(vlist, vl);
            DestroyList(vl, (void (*)(void *))NULL);

            return 0;

        case EXPR_OPSUBSCR:
            PUSH(e);

            /* drop through */

        case EXPR_VAR:
            if ((v = ReadVarDecl(e, rec_set)) == NULL) return -1;

            AddToList(vlist, (void *)v);

            BuildInvoke(g, v);
            if (v->v_procset != (dbgprocset_t *)NULL && v->v_indiv >= 0) {
                BuildGetVar(g, inp, out, v, v->v_indiv, extra);
            } else {
                gi = g->FindGraphInv(v->v_pname);
                BuildGetVarSet(g, inp, out, v, 0, gi->gp_di->d_nproc,
                                extra);
            }
            return 0;

        case EXPR_INT:
            *out = g->outConstInt(ep->e_int);
            *inp = *out;
            return 0;

        case EXPR_FLOAT:
            *out =g->outConstFloat(ep->e_float);
            *inp = *out;
            return 0;

        case EXPR_DSYM:
            SetError("Must specify location information in declarative mode");
            return -1;

        case EXPR_FUNC:
            vl = NewList();

            if (build_func(g, ep->e_sym, inp, out, vl, e, rec_set, f, extra) <
                0)
                return -1;

            AppendList(vlist, vl);
            DestroyList(vl, (void (*)(void *))NULL);
            return 0;

        case EXPR_SYM:
            if (f != (func_t *)NULL) {
                if (FindArg(&a, ep->e_sym, f) < 0) return -1;

                if (a != (arg_t *)NULL) {
                    *inp = a->a_inp;
                    *out = a->a_out;
                    AppendList(vlist, a->a_vlist);

                    return 0;
                }

                /*
                ** Must be a program var. Drop through...
                */
            }

        case EXPR_STRING:
            SetError("Must specify process in declarative mode");
            return -1;

        default:
            SetError("Tried to evaluate an unknown expression: %d", ep->e_op);
            return -1;
    }
}

/*
** Declarative function evaluation.
*/
static int build_func(graph_t *g, hentry_t *sym, int *res_inp, int *res_out,
                      list_t *res_vlist, expr_t *e, rec_set_t *rec_set,
                      func_t *f, extrainfo *extra) {
    int n;
    int inp;
    int out;
    arg_t *a;
    func_t *fp;
    list_t *aarg;
    list_t *sarg;
    list_t *vlist;
    expr_elt *ep;

    DPRINT("In build_func\n");

    if ((fp = FindFunc(sym)) == (func_t *)NULL) {
        SetError("Function \"%s\" not defined", SymLookup(sym));
        return -1;
    }

    ep = POP(e);

    /*
    ** We first expect to find a list start token.
    */
    if (ep->e_op != EXPR_LIST_START) {
        SetError("Internal error: expected EXPR_LIST_START");
        return -1;
    }

    ep = POP(e);

    /*
    ** Evaluate the arguments for the function. The number of
    ** arguments follows...
    */

    if (ep->e_op != EXPR_INT) {
        SetError("Internal error: expected EXPR_INT");
        return -1;
    }

    n = ep->e_int;

    aarg = NewList();

    while (n-- > 0) {
        ep = POP(e);

        if (ep->e_op != EXPR_LIST) {
            SetError("Internal error: expected EXPR_LIST");
            DestroyList(aarg, (void (*)(void *))DestroyArg);
            return -1;
        }

        vlist = NewList();

        if (build_expr(g, &inp, &out, vlist, e, rec_set, f, extra) < 0) {
            DestroyList(aarg, (void (*)(void *))DestroyArg);
            return -1;
        }

        /*
        ** Save the argument for when the function is
        ** evaluated.
        */

        a = NewArg(ARG_GRAPH, inp, out, vlist);
        AddFirst(aarg, (void *)a);
    }

    if (fp->f_builtin != (int (*)(list_t *, val_t **))NULL) {
        SetError("Can't use builtin function in delarative mode");
        DestroyList(aarg, (void (*)(void *))DestroyArg);
        return -1;
    }

    sarg = fp->f_actual;
    fp->f_actual = aarg;

    /*
    ** Apply function to its arguments.
    */
    RESET(fp->f_def);

    vlist = NewList();

    if (build_expr(g, &inp, &out, vlist, fp->f_def, rec_set, fp, extra) < 0) {
        DestroyList(aarg, (void (*)(void *))DestroyArg);
        return -1;
    }

    /*
    ** Restore args.
    */
    fp->f_actual = sarg;
    DestroyList(aarg, (void (*)(void *))DestroyArg);

    *res_inp = inp;
    *res_out = out;
    AppendList(res_vlist, vlist);
    DestroyList(vlist, (void (*)(void *))NULL);

    return 0;
}

/*
** Build unary, binary or tertiary operator.
*/
static int BuildOp(graph_t *g, expr_op op, int o1, int i2, int o2, int i3,
                   int o3) {
    int n;
    int n1;
    int n2;
    int n3;

    DPRINT("In BuildOp\n");

    switch (op) {
        case EXPR_OPCOND:
            n1 = g->outConstInt(0);
            g->outEdge(o1, n1, 0);
            g->outEdge(o1, i2, 0);
            g->outEdge(o1, i3, 0);

            n2 = g->outNode("PIEQ");
            g->outEdge(n1, n2, 0);
            g->outEdge(o1, n2, 1);
            g->outEdge(o2, n2, 2);

            n3 = g->outNode("PINE");
            g->outEdge(n1, n3, 0);
            g->outEdge(o1, n3, 1);
            g->outEdge(o3, n3, 2);

            n = g->outNode("MERGE");
            g->outEdge(n2, n, 0);
            g->outEdge(n3, n, 1);

            break;

        case EXPR_OPADD:
            n = g->outNode("ADD");
            break;

        case EXPR_OPSUB:
            n = g->outNode("SUB");
            break;

        case EXPR_OPMUL:
            n = g->outNode("MUL");
            break;

        case EXPR_OPDIV:
            n = g->outNode("DIV");
            break;

        case EXPR_OPREM:
            n = g->outNode("REM");
            break;

        case EXPR_OPEQUAL:
            n = g->outNode("EQUAL");
            break;

        case EXPR_OPNOTEQUAL:
            n = g->outNode("NEQ");
            break;

        case EXPR_OPLEQ:
            n = g->outNode("LEQ");
            break;

        case EXPR_OPGEQ:
            n = g->outNode("GEQ");
            break;

        case EXPR_OPLT:
            n = g->outNode("LT");
            break;

        case EXPR_OPGT:
            n = g->outNode("GT");
            break;

        case EXPR_OPAND:
            n = g->outNode("AND");
            break;

        case EXPR_OPOR:
            n = g->outNode("OR");
            break;

        case EXPR_OPPOS:
            break;

        case EXPR_OPNEG:
            n = g->outNode("NEG");
            break;

        default:
            SetError("Invalid operation");
            return -1;
    }

    switch (op) {
        case EXPR_OPADD:
        case EXPR_OPSUB:
        case EXPR_OPMUL:
        case EXPR_OPDIV:
        case EXPR_OPREM:
        case EXPR_OPEQUAL:
        case EXPR_OPNOTEQUAL:
        case EXPR_OPLEQ:
        case EXPR_OPGEQ:
        case EXPR_OPLT:
        case EXPR_OPGT:
        case EXPR_OPAND:
        case EXPR_OPOR:
            g->outEdge(o1, n, 0);
            g->outEdge(o1, i2, 0);
            g->outEdge(o2, n, 1);
            break;

        case EXPR_OPNEG:
            g->outEdge(o1, n, 0);
            break;

        case EXPR_OPPOS:
            n = o1;
            break;

        default:
            break;
    }

    return n;
}

/*
** FIXME: I have no clue how to fix this.............................
*/
static void BuildMerge(graph_t *g, var_t *v, int from, int async) {
    int r;
    graph_t::graph_inv *gi;

    DPRINT("In BuildMerge\n");

    gi = g->FindGraphInv(v->v_pname);

    if (v->v_procset != NULL && v->v_indiv >= 0) {
        r = BuildMergeProc(g, gi, v, v->v_indiv, from, async);
#ifdef DEBUG
        g->debugPrint();
#endif
        BuildSync(g, v, v->v_indiv, r);
#ifdef DEBUG
        g->debugPrint();
#endif
    } else {
        // We use proc low_pid (0) as the "base".
        r = BuildMergeProc(g, gi, v, 0, from, async);
        BuildSync(g, v, 0, r);
    }
}

/*
** FIXME: I have no clue how to fix this.............................
*/
static int BuildMergeProc(graph_t *g, graph_t::graph_inv *gi, var_t *v, int pid,
                          int from, int async) {
    int t1;
    int t2;

    DPRINT("In BuildMergeProc\n");

    /*
    ** Skip unused procs.
    */
    // if ( !gp->gp_builtinv )
    //	return -1;

    if (!async) {
        t1 = g->outConstInt(0);
        g->outEdge(from, t1, 0);

        t2 = g->outNode("PINE");
        g->outEdge(from, t2, 0);
        g->outEdge(t1, t2, 1);
        g->outEdge(v->v_pid, t2, 2);
    } else {
        t2 = g->outNode("PIT");
        g->outEdge(from, t2, 0);
        g->outEdge(v->v_pid, t2, 1);
    }

    return t2;
}

/*
** Build sync graph. This feeds a proc token back to the main "MERGE" node
** for the process. Since multiple asserts/assigns can share a single
** break point address, we must also prevent multiple procs reaching
** the "MERGE". We do this by routing the procs to a per breakpoint "SYNC"
** node first.
**
** NOTE: The proc resulting from an assert is conditional on the result
** of the "EPS", since it is sent via a "PILE". If the result is greater
** than 0, no token will reach the "SYNC" and the graph will block. This
** not currently a problem as a token is also routed to an "EXIT" node
** in this situation. This supports the semantics that an out-of-bounds
** error on any assertion will stop the graph. If we change this semantics
** then the sync graph will need to change.
*/
static void BuildSync(graph_t *g, var_t *v, int pid, int from) {
    graph_t::graph_inv *gi;

    DPRINT("In BuildSync\n");

    gi = g->FindGraphInv(v->v_pname);

    /*
    ** Skip unused procs.
    ** FIXME: I have no clue how to fix this....
    */
    // if ( !gp->gp_builtinv )
    //	return;

    if (v->v_sync < 0) {
        v->v_sync = g->outNode("SYNC");
        g->outEdge(v->v_sync, PROC_MERGE_NODE(gi), -1);
    }

    g->outEdge(from, v->v_sync, -1);
}

/*
** Evaluate and build an assert.
*/
int BuildAssert(graph_t *g, int id, expr_t *e, rec_set_t *rec_set,
                list_t *vlist) {
    int i1;
    int i2;
    int o1;
    int o2;
    int cmp1 = 0;
    int cmp2 = 0;
    int cmp;
    int cmpop;  // Minh add here for different type of assertions
    var_t *v;
    var_t *tmp_v = NULL;
    int async;
    extrainfo extra;

    // initialise the values in extra
    strcpy(extra.hashinfo, "");
    strcpy(extra.blockmapinfo, "");

    list_t *v1 = (list_t *)NULL;
    list_t *v2 = (list_t *)NULL;

    g_block_map_t *bm = (g_block_map_t *)NULL;   // Minh
    g_block_map_t *bm2 = (g_block_map_t *)NULL;  // Minh

    expr_elt *ep;

    iAssertID = id;

    DPRINT("In BuildAssert for id %d\n", iAssertID);

    if (e->e_lval == 0) return -1;

    RESET(e);

    ep = POP(e);

    cmpop = ep->e_op;

    DPRINT("BuildAssert: Comparison operator is %s\n", getExprOpStr(ep->e_op));

    ep = TOP(e);

    DPRINT("BuildAssert: Building lval.\n");
    DPRINT("BuildAssert: The starting op for lval is %s\n",
           getExprOpStr(ep->e_op));

    switch (ep->e_op) {
        case EXPR_BLOCKMAP:
            DPRINT("BuildAssert: Evaluating blockmap for lval\n");
            if ((bm = EvaluateBlockMap(e, rec_set, CMD_MODE_DEFERRED)) ==
                (g_block_map_t *)NULL)
                return -1;
            break;

        // Minh adds these to allow assert against constants
        // Just int and float for now
        case EXPR_INT:
            DPRINT("BuildAssert: Found const INT for lval\n");
            o1 = g->outConstAIF(ep->e_int, AIF_INTEGER);
            i1 = o1;
            ep = POP(e);
            break;

        case EXPR_FLOAT:
            DPRINT("BuildAssert: Found const FLOAT for lval\n");
            o1 = g->outConstAIF(ep->e_float, AIF_FLOATING);
            i1 = o1;
            ep = POP(e);
            break;

        default:
            DPRINT("BuildAssert: Building expression for lval\n");
            v1 = NewList();

            if (build_expr(g, &i1, &o1, v1, e, rec_set, (func_t *)NULL,
                           &extra) < 0)
                return -1;

            if (EmptyList(v1)) {
                SetError(
                    "Must specify breakpoint in the lval of the assertion!");
                return -1;
            }

            break;
    }

    ep = TOP(e);

    DPRINT("BuildAssert: Building rval.\n");
    DPRINT("BuildAssert: The starting op for rval is %s\n",
           getExprOpStr(ep->e_op));

    switch (ep->e_op) {
        case EXPR_BLOCKMAP:
            DPRINT("BuildAssert: Evaluating blockmap for rval\n");
            if (bm != (g_block_map_t *)NULL) {
                tmp_v = bm->m_actual;
                if ((bm2 = EvaluateBlockMap(e, rec_set, CMD_MODE_DEFERRED)) ==
                    (g_block_map_t *)NULL) {
                    DestroyList(v1, (void (*)(void *))NULL);
                    return -1;
                }
                break;
            }

            if ((bm = EvaluateBlockMap(e, rec_set, CMD_MODE_DEFERRED)) ==
                (g_block_map_t *)NULL) {
                DestroyList(v1, (void (*)(void *))NULL);
                return -1;
            }

            break;

        case EXPR_INT:
            DPRINT("BuildAssert: Found const INT for rval\n");
            o2 = g->outConstAIF(ep->e_int, AIF_INTEGER);
            i2 = o2;
            ep = POP(e);
            break;

        case EXPR_FLOAT:
            DPRINT("BuildAssert: Found const FLOAT for rval\n");
            o2 = g->outConstAIF(ep->e_float, AIF_FLOATING);
            i2 = o2;
            ep = POP(e);
            break;

        default:
            DPRINT("BuildAssert: Building expression for rval\n");
            v2 = NewList();

            if (build_expr(g, &i2, &o2, v2, e, rec_set, (func_t *)NULL,
                           &extra) < 0) {
                if (bm != (g_block_map_t *)NULL)  // Minh
                    DestroyBlockMap(bm);
                else
                    DestroyList(v1, (void (*)(void *))NULL);

                return -1;
            }
            if (EmptyList(v2)) {
                SetError(
                    "Must specify breakpoint in the rval of the assertion!");
                return -1;
            }
            break;
    }

    // Original: if ( v1 == (list_t *)NULL )
    /* Minh modify here to implement blockmap
     * 19/05/2009
     */
    if (v1 == (list_t *)NULL && v2 != (list_t *)NULL) {
        cmp1 = o2;

        /*
        ** Connect first (any) proc to input of expression graph.
        */
        SetList(v2);
        v = (var_t *)GetListElement(v2);
        if (v->v_procset != (dbgprocset_t *)NULL && v->v_indiv >= 0) {
            g->outEdge(v->v_pid, i2, 0);

            /*
            ** set v_value to output of expression
            */
            v->v_value = o2;
        }

        AppendList(vlist, v2);
        DestroyList(v2, (void (*)(void *))NULL);

        // check if there is constant
        if (bm == (g_block_map_t *)NULL) {
            g->outEdge(cmp1, i1, 0);
            cmp2 = o1;
        }
    }
    // Original: else if ( v2 == (list_t *)NULL )
    // Minh
    else if (v2 == (list_t *)NULL && v1 != (list_t *)NULL) {
        cmp1 = o1;

        /*
        ** Connect first (any) proc to input of expression graph.
        */
        SetList(v1);
        v = (var_t *)GetListElement(v1);
        if (v->v_procset != (dbgprocset_t *)NULL && v->v_indiv >= 0) {
            g->outEdge(v->v_pid, i1, 0);
            /*
            ** set v_value to output of expression
            */
            v->v_value = o1;
        }

        AppendList(vlist, v1);
        DestroyList(v1, (void (*)(void *))NULL);

        // check if there is constant
        if (bm == (g_block_map_t *)NULL) {
            g->outEdge(cmp1, i2, 0);
            cmp2 = o2;
        }
    }
    // Original: else
    // Minh
    else if (v2 != (list_t *)NULL && v2 != (list_t *)NULL) {
        cmp1 = o1;
        cmp2 = o2;

        /*
        ** Connect first (any) proc to input of expression graphs.
        */
        // TODO: How to fix this???
#if 0
		SetList(v1);
		v = (var_t *)GetListElement(v1);
		if (v->v_procset != (dbgprocset_t *)NULL && v->v_procset->s_indiv_proc >= 0) {
			g->outEdge(v->v_pid, i1, 0);

			/*
			** set v_value to output of expression
			*/
			v->v_value = o1;
		}

		SetList(v2);
		v = (var_t *)GetListElement(v2);
		if (v->v_procset != (dbgprocset_t *)NULL && v->v_procset->s_indiv_proc >= 0) {
			g->outEdge(v->v_pid, i2, 0);

			/*
			** set v_value to output of expression
			*/
			v->v_value = o2;
		}
#endif

        AppendList(vlist, v1);
        AppendList(vlist, v2);
        DestroyList(v1, (void (*)(void *))NULL);
        DestroyList(v2, (void (*)(void *))NULL);
    }

    // Minh
    if ((bm != NULL) && (bm2 != NULL)) {
        AddToList(vlist, (void *)tmp_v);

        BuildInvoke(g, tmp_v);

        AddToList(vlist, (void *)bm2->m_actual);

        BuildInvoke(g, bm2->m_actual);

        // check if the blockmap function is pplication for this program
        if (bm != (g_block_map_t *)NULL) {
            if (_validate_block_map(g, bm, tmp_v) == 0) return -1;
        }
        if (bm2 != (g_block_map_t *)NULL) {
            if (_validate_block_map(g, bm2, bm2->m_actual) == 0) return -1;
        }

        cmp1 = BuildBlockMap(g, bm, tmp_v, &extra, 1);
        if (cmp1 == -1) {
            // DestroyBlockMap(bm); //Why destroy the blockmap definition??
            return -1;
        }
        cmp2 = BuildBlockMap(g, bm2, bm2->m_actual, &extra, 1);
        if (cmp2 == -1) {
            // DestroyBlockMap(bm2);
            return -1;
        }
    } else if (bm != (g_block_map_t *)NULL) {
        int blockmap_node;

        AddToList(vlist, (void *)bm->m_actual);

        BuildInvoke(g, bm->m_actual);

        blockmap_node = BuildBlockMap(g, bm, bm->m_actual, &extra, 1);
        if (blockmap_node == -1) {
            // DestroyBlockMap(bm);
            return -1;
        }
        if (v2 == (list_t *)NULL && v1 == (list_t *)NULL) {
            cmp1 = blockmap_node;
            cmp2 = o1;
            g->outEdge(cmp1, i1, 0);
        } else
            cmp2 = blockmap_node;
    }

    cmp = BuildCmp(g, id, cmp1, cmp2, cmpop);

#ifdef DEBUG
    g->debugPrint();
#endif

    async = AsyncMode(g, vlist);

    SetList(vlist);
    while ((v = (var_t *)GetListElement(vlist)) != (var_t *)NULL) {
        BuildMerge(g, v, cmp, async);
    }

    DPRINT("BuildAssert: Finished building assertion for assert id %d\n",
           iAssertID);

    return 0;
}

/*
** Take a graph and a list of var_t and return 1 if all of the variables in the
** list are from the same graph_inv. Returns 0 otherwise.
*/
static int AsyncMode(graph_t *g, list_t *vlist) {
    var_t *v;
    graph_t::graph_inv *gi = NULL;

    DPRINT("In AsyncMode\n");

    SetList(vlist);

    while ((v = (var_t *)GetListElement(vlist)) != (var_t *)NULL) {
        if (gi == (graph_t::graph_inv *)NULL) {
            gi = g->FindGraphInv(v->v_pname);
            continue;
        }

        if (gi != g->FindGraphInv(v->v_pname)) return 0;
    }

    return 1;
}
