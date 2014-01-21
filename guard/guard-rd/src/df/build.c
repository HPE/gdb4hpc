/*********************************************************************************\
 * Routines for building graph
 * TODO: avoid building duplicate GETVAR's.
 * andrewg: I think what the above is trying to say, is that if we have multiple
 * assertions against a single var at the same line number, we will grab the value
 * of the var for each assertion, instead of just caching it. The above todo has
 * thrown me off for quite some time.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * © 2010-2013 Cray Inc.  All Rights Reserved.
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

#ifdef HAVE_CONFIG_H
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#include	<stdio.h>
#include	<string.h>

#include	"defs.h"
#include	"useful.h"
#include	"df.h"
#include	"bitset.h"
#include	"procset.h"
#include	"hash.h"

#define PROC_PROC_NODE(p)	(p)->gp_node[0]
#define PROC_SYNC_NODE(p)	(p)->gp_node[1]
#define PROC_MERGE_NODE(p)	(p)->gp_node[2]
#define PROC_GO_NODE(p)		(p)->gp_node[3]

int	AsyncMode(graph_t *, list_t *, dfmode_t);
int	build_expr(graph_t *, int *, int *, list_t *, expr_t *, rec_set_t *, func_t *, extrainfo *);
int	build_func(graph_t *, hentry_t *, int *, int *, list_t *, expr_t *, rec_set_t *, func_t *, extrainfo *);
int	BuildOp(graph_t *, expr_op , int , int , int , int , int );


// Temp location to store assertid as a means of passing it down to DbgGetVar without adding
// an extra parameter to build_expr.  Note: Can we pass assert id down through the vars?  
// Look into this.
int iAssertID = -1;
int GetVarSyncNode = -1;
dbgprocset_t *other_procset;

#ifdef notdef
/*
** Set breakpoint at main, and wait for breakpoint to
** to be hit. Passes input token when done.
**
** Not currently used. Debugger must handle stop at main().
*/
int
buildbrkmain(graph_t *g, int n)
{
	int	t1;
	int	t2;
	int	t3;
	int	t4;
	int	t5;
	
	DPRINT("In buildbrkmain\n");

	t1 = SCONST(g, "CONST", "main");
	t2 = NODE(g, "DBGSETBRK");
	t3 = NODE(g, "DBGGO");
	t4 = NODE(g, "DBGWAITBRK");
	t5 = NODE(g, "PIT");
	EDGE(g, n, t1, 0);
	EDGE(g, n, t2, 0);
	EDGE(g, t1, t2, 1);
	EDGE(g, t2, t3, 0);
	EDGE(g, t3, t4, 0);
	EDGE(g, t4, t5, 0);
	EDGE(g, n, t5, 1);

	return t5;
}
#endif /* notdef */

/*
** If first time, output nodes and edges to invoke program.
*/
void
BuildInvoke(graph_t *g, var_t *v)
{
	graph_inv *	gi;

	if ( (gi = FindGraphInv(g, SymLookup(v->v_isym))) == (graph_inv *)NULL )
		gi = NewGraphInv(g, v->v_procset->s_ig);

	BuildInvokeProcSet(g, gi, v); 
}

void
BuildInvokeProcSet(graph_t *g, graph_inv *gi, var_t *v)
//TODO: BuildInvokeProcSet -> BuildProcSet()
//Building ProcSet called in var
//old interface: BuildInvokeProcSet(graph_t *g, graph_inv *gi, var_t *v, int low_pid, int proc_count)
{

	int		xt1;
	int		xt2;
	int		xt3;
	int		xt4;

	bitset *	token_bit_set;
	dbg_over_set_t* over_set = NULL;
	dbgprocset_t* procset = NULL;
	int	proc_count = 0;
	int built_indiv = 1;
	int	built_bp = 1;
	file_t *	f = NULL;
	graph_procset *	gp = NULL;

	gp = gi->gi_procset;
	procset = v->v_procset;
	
	DPRINT("In BuildInvokeProcSet\n");

	SetList(procset->s_over_set_list);
	over_set = GetListElement(procset->s_over_set_list);

	proc_count = procset->s_nproc;

	token_bit_set = bitset_new(proc_count);
	if(v->v_indiv < 0)
		bitset_invert(token_bit_set);
	else
	{
		bitset_set(token_bit_set, v->v_indiv);
		built_indiv = bitset_test(gp->gp_builtinv_bs, v->v_indiv);
	}

//	if ( !gp->gp_builtinv )
	if ( !gp->gp_builtinv || !built_indiv)
	{
		xt1 = NODE(g, "PROC_SET");
		xt2 = SCONST(g, "CONST", SymLookup(gi->gi_sym));
		EDGE(g, g->g_initnode, xt2, 0);
		xt3 = BITSETCONST(g, "CONST", token_bit_set);
		DPRINT("BuildInvokeProcSet token_bit_set[%s], length[%d]\n", bitset_to_set(token_bit_set), bitset_size(token_bit_set));
		EDGE(g, g->g_initnode, xt3, 0);
		EDGE(g, xt2, xt1, 0);
		EDGE(g, xt3, xt1, 1);

		xt4 = NODE(g, "SYNC");

		PROC_PROC_NODE(gp) = xt1;
		PROC_SYNC_NODE(gp) = xt4;

		gp->gp_builtinv = 1;
		if(v->v_indiv < 0)
			bitset_invert(gp->gp_builtinv_bs);
		else
			bitset_set(gp->gp_builtinv_bs, v->v_indiv);

	}
	bitset_free(token_bit_set);

	/*
	** Save breakpoint info.
	*/
	if ( v->v_line == (line_t *)NULL )
	{
		v->v_line = (line_t *)Alloc(sizeof(line_t));
	}

	/*
	** If this is the first time we have
	** seen this process, add index.
	*/

	xt1 = PROC_PROC_NODE(gp);
	xt4 = PROC_SYNC_NODE(gp);

	f = AddFile(&gp->gp_files, v->v_fname);
	v->v_line = AddLine(&f->f_lines, v->v_lno);
	if(v->v_bs == NULL)
		v->v_bs = bitset_new(proc_count);	
	
	if(v->v_indiv >= 0)
		built_bp = bitset_test(v->v_bs, v->v_indiv);

	if(v->v_bp < 0 || !built_bp)
	{
		v->v_bp = BuildSetBPSet(g, xt1, xt4, v);
		if(v->v_indiv < 0)
			bitset_invert(v->v_bs);
		else
		{
			bitset_set(v->v_bs, v->v_indiv);
			v->v_pid = -1;
		}
	}
}

int
BuildSetBPSet(graph_t *g, int i1, int i2, var_t *v)
{
	int		n1;
	int		n2;
	int		n3;
	int		n4;
	int		n5;
	int		n6;
	int		p2pmode;

	DPRINT("In BuildSetBPSet\n");

	n1 = ICONST(g, "CONST", v->v_lno);
	n2 = SCONST(g, "CONST", v->v_fname);
	n3 = NODE(g, "DBGSETBRK_SET");

	EDGE(g, i1, n1, 0);
	EDGE(g, i1, n2, 0);
	EDGE(g, i1, n3, 0);
	EDGE(g, n1, n3, 1);
	EDGE(g, n2, n3, 2);

	n4 = ICONST(g, "CONST", g->g_store);
	n5 = NODE(g, "STORE");
	n6 = NODE(g, "PIT");

	EDGE(g, n3, n4, 0);
	EDGE(g, n4, n5, 0);
	EDGE(g, n3, n5, 1);
	EDGE(g, i1, n6, 1);

	GetGraphVar(GRAPH_P2PMODE, (int *)&p2pmode);
	if (p2pmode)
	{
		int		n7;
		int		n8;
		int		n9;
		n7 = NODE(g, "DBGOPENSOCKET_SET");	// Minh adds here to do p2p
		// use global var other_procset to send the name of the other procset
		n8 = SCONST(g, "CONST", other_procset->s_name);
		n9 = ICONST(g, "CONST", other_procset->s_nproc);
		EDGE(g, i1, n7, 0);
		EDGE(g, n5, n8, 0);
		EDGE(g, n5, n9, 0);
		EDGE(g, n8, n7, 1);
		EDGE(g, n9, n7, 2);
		EDGE(g, n7, n6, 0);
	}
	else
		EDGE(g, n5, n6, 0);

	/*
	** Link to SYNC node.
	*/
	EDGE(g, n6, i2, -1);

	return g->g_store++;
}

void
BuildMPWait(graph_t *g, var_t *v)
{
	graph_inv *	gi;

	gi = FindGraphInv(g, SymLookup(v->v_isym));

	BuildMPWaitProcSet(g, gi, v);
}

void
//BuildMPWaitProcSet(graph_t *g, graph_inv *gi, var_t *v, int low_pid, int proc_count)
BuildMPWaitProcSet(graph_t *g, graph_inv *gi, var_t *v)
{
	int		t1;
	int		t2;
	int		t3;
	graph_procset *	gp;

	int		xt1=0;
	int		xt2=0;
	int		built_indiv = 1;
	
	DPRINT("In BuildMPWaitProcSet\n");
	if(v->v_indiv >= 0)
	{
		built_indiv = bitset_test(gi->gi_procset->gp_built_bs, v->v_indiv);
	}

//	if ( !gi->gi_procset->gp_built )
	if ( !gi->gi_procset->gp_built || !built_indiv)
	{
		xt1 = NODE(g, "MERGE");
		xt2 = NODE(g, "DBGGO_SET");
		
		EDGE(g, PROC_SYNC_NODE(gi->gi_procset), xt1, 0);
		EDGE(g, xt1, xt2, 0);

		/*
		** Check if WaitBreak returns 0. This means the
		** process has exited, so exit graph if it is.
		*/
		t1 = ICONST(g, "CONST", 0);
		t2 = NODE(g, "PIEQ");
		t3 = NODE(g, "EXIT");

		EDGE(g, xt2, t1, 0);
		EDGE(g, xt2, t2, 0);
		EDGE(g, t1, t2, 1);

		EDGE(g, xt1, t2, 2);
		EDGE(g, t2, t3, 0);

		PROC_MERGE_NODE(gi->gi_procset) = xt1;
		PROC_GO_NODE(gi->gi_procset) = xt2;

		gi->gi_procset->gp_built = 1;
		if(v->v_indiv < 0)
			bitset_invert(gi->gi_procset->gp_built_bs);
		else
	    	bitset_set(gi->gi_procset->gp_built_bs, v->v_indiv);
	}

	gp = gi->gi_procset;

	xt1 = PROC_MERGE_NODE(gi->gi_procset);
	xt2 = PROC_GO_NODE(gi->gi_procset);

	if ( !gp->gp_built )
	{
		PROC_MERGE_NODE(gp) = xt1;
		PROC_GO_NODE(gp) = xt2;
		gp->gp_built = 1;
	}

	if ( v->v_pid < 0 || !built_indiv)
	{
		t1 = ICONST(g, "CONST", v->v_bp);
		t2 = NODE(g, "RECALL");
		t3 = NODE(g, "PIEQ");
		v->v_pid = t3;

		EDGE(g, xt2, t1, 0);
		EDGE(g, t1, t2, 0);
		EDGE(g, t2, t3, 0);
		EDGE(g, xt2, t3, 1);
		EDGE(g, xt1, t3, 2);

		v->v_pid = t3;
		v->v_sync = -1; //v->v_pid and v->v_sync should be consistent
	}
}

/*
** This code is to start any remaining parallel processes
** that haven't been referenced in an assert. These consist of
** all processes belonging to an invoke, where gp_built is
** false.
*/
void
BuildUnused(graph_t *g, var_t *v)
{
	graph_inv *	gi;
	invoke_t *	i;
	invoke_group *	ig;
	
	//hard code to reset assert id to -1
	iAssertID = -1;
	
	DPRINT("In BuildUnused\n");
	
	gi = FindGraphInv(g, SymLookup(v->v_isym));

	/*
	** Check if we've been here before...
	*/
	if ( gi->gi_builtunused )
		return;

	i = gi->gi_inv->ig_invoke;

	/*
	** Check all process belonging to the invoke.
	*/

	SetInvokeGroup(i);

	while ( (ig = GetInvokeGroup(i)) != (invoke_group *)NULL )
	{
		if ( (gi = FindGraphInv(g, SymLookup(ig->ig_sym))) == (graph_inv *)NULL )
			gi = NewGraphInv(g, ig);

		if ( v->v_procset != (dbgprocset_t *)NULL && v->v_procset->s_indiv_proc >= 0)
		{
			BuildUnusedProcSet(g, gi, v, NULL);
		}
		else
		{
			bitset *	token_bit_set;
			token_bit_set = bitset_new(gi->gi_nproc);
			bitset_invert(token_bit_set);

			BuildUnusedProcSet(g, gi, v, token_bit_set);

			bitset_free(token_bit_set);
		}
	}

	gi->gi_builtunused = 1;
}

void
//BuildUnusedProcSet(graph_t *g, graph_inv *gi, var_t *v, int low_pid, int proc_count)
BuildUnusedProcSet(graph_t *g, graph_inv *gi, var_t *v, bitset *bit_set)
{
	int		xt1;
	int		xt2;
	int		xt3;
	int		xt4;
	
	DPRINT("In BuildUnusedProcSet\n");

	if(gi->gi_procset->gp_built && bitset_isfull(gi->gi_procset->gp_built_bs))
		return;

	dbg_over_set_t* over_set = NULL;
	SetList(v->v_procset->s_over_set_list);
	over_set = GetListElement(v->v_procset->s_over_set_list);

	bitset* internal_bs = bitset_dup(gi->gi_procset->gp_built_bs);
	bitset_invert(internal_bs);
	xt1 = NODE(g, "PROC_SET");
	xt2 = SCONST(g, "CONST", SymLookup(gi->gi_sym));
	EDGE(g, g->g_initnode, xt2, 0);
	xt3 = BITSETCONST(g, "CONST", internal_bs);
	DPRINT("BuildUnusedSet internal_bs[%s], length[%d]\n", bitset_to_set(internal_bs), bitset_size(internal_bs));
	EDGE(g, g->g_initnode, xt3, 0);
	EDGE(g, xt2, xt1, 0);
	EDGE(g, xt3, xt1, 1);

	xt4 = NODE(g, "DBGGO_SET");
	EDGE(g, xt1, xt4, 0);

	gi->gi_procset->gp_built = 1;
	bitset_clear(gi->gi_procset->gp_built_bs);
	bitset_invert(gi->gi_procset->gp_built_bs);
	bitset_free(internal_bs);
}

void
BuildGetVarSet_Sync(graph_t *g, int *inp, int *out, var_t *v, int low_pid, int proc_count, extrainfo *extra, int blockmap_required)
{
	int		n;
	int		t1;
	int		n4;
	int		n5=0;
	
	int		a1;
	
	slice_t *	s;

	char	finalexpr[BUFSIZ]="";
	char	varexpr[BUFSIZ]="";
	int		reducefunc, p2pmode;
	
	DPRINT("In BuildGetVarSet_Sync\n");

	GetGraphVar(GRAPH_P2PMODE, (int *)&p2pmode);
	GetGraphVar(GRAPH_REDUCE, (int *)&reducefunc);

	if ( (s = v->v_slice) != (slice_t *)NULL )
	{
		char sliceexpr[BUFSIZ]="|slice(";
		for ( n = 0 ; n < s->s_rank ; n++ )
		{
			sprintf(sliceexpr, "%s%d,%d", sliceexpr, s->s_low[n], s->s_high[n]);
			if (n<s->s_rank-1)
				strcat(sliceexpr, ",");
		}
		strcat(sliceexpr, ")");
		sprintf(varexpr, "%s%s|", sliceexpr, v->v_var);
	}
	else
		strcpy(varexpr, v->v_var);

	/* check if reduce function is on
	** Minh 09/06/2009
	*/
	if (reducefunc != EXPR_REDUCE_NONE)
	{
		char reduceexpr[BUFSIZ]="";
		double lower_bound;
		double upper_bound;
		switch (reducefunc) {
		case EXPR_SUM:
			strcpy(reduceexpr, "`sum`");
			break;
		case EXPR_PRODUCT:
			strcpy(reduceexpr, "`product`");
			break;
		case EXPR_AVERAGE:
			strcpy(reduceexpr, "`average`");
			break;
		case EXPR_MIN:
			strcpy(reduceexpr, "`min`");
			break;
		case EXPR_MAX:
			strcpy(reduceexpr, "`max`");
			break;
		case EXPR_STDEV:
			strcpy(reduceexpr, "`stdev`");
			break;
		case EXPR_COUNTELEM:
			GetGraphVar(GRAPH_LOWER_BOUND, (double *)&lower_bound);
			GetGraphVar(GRAPH_UPPER_BOUND, (double *)&upper_bound);
			sprintf(reduceexpr, "`countelem(%lf,%lf)`", lower_bound, upper_bound);
			break;
		case EXPR_HASH_BOB:
			sprintf(reduceexpr, "`hash(%s)`", extra->hashinfo);
			break;
		default:
			SetError("Unknown reduction function %d", reducefunc);
			return;
		}
		sprintf(finalexpr, "%s%s", reduceexpr, varexpr);
	}

	// if there is p2p mode turn on, attach blockmap info
	else if (p2pmode)
	{
		char p2pexpr[BUFSIZ]="";
		sprintf(p2pexpr, "`ptop(%s-%s)`", SymLookup(v->v_isym), extra->blockmapinfo);
		sprintf(finalexpr, "%s%s", p2pexpr, varexpr);
	}
	else
		strcpy(finalexpr, varexpr);

	t1 = SCONST(g, "CONST", finalexpr);

	n4 = NODE(g, "DBGREQUESTVAR_SET");

    *inp = NODE(g, "PIT");

	// We use proc low_pid (0) as the "base".
	EDGE(g, v->v_pid, *inp, 1);
/*#else
	EDGE(g, v->v_pid(low_pid), *inp, 1);
#endif*/
	/* MINH TODO : NEED TO REVIEW THIS BIT OF CODE */
	/* Minh add here to implement blockmap and parallel vs parallel
	** 19/05/2009
	** Donny TODO: Probably we need to synchronize
	*/
	// We use proc low_pid (0) as the "base".
   EDGE(g, v->v_pid, *inp, 0);
/*#else
   EDGE(g, v->v_pid(low_pid), *inp, 0);
#endif*/

	EDGE(g, *inp, t1, 0);
    // Const for asserid
    a1 = ICONST(g, "CONST", iAssertID);
    EDGE(g, *inp, a1, 0);

    EDGE(g, *inp, n4, 0);
    EDGE(g, t1, n4, 1);

	// Const for asserid
	EDGE(g, a1, n4 , 2);

	if (blockmap_required)
	{
		// now connect recvvar node
		n5 = NODE(g, "DBGRECVVAR_SET");
		EDGE(g, *inp, n5, 0);	// procset
		EDGE(g, a1, n5, 1);		// asserid
		EDGE(g, t1, n5, 2);		// expr string
		EDGE(g, n4, n5, 3);		// bitset
	}

	// now connect to sync node
	if (GetVarSyncNode < 0)
	{
		GetVarSyncNode = NODE(g, "SYNC");
		EDGE(g, n4, GetVarSyncNode, 0);
		if (blockmap_required)
			EDGE(g, GetVarSyncNode, n5, 4);
	}
	else
	{
		EDGE(g, n4, GetVarSyncNode, 1);
		if (blockmap_required)
			EDGE(g, GetVarSyncNode, n5, 4);
		GetVarSyncNode = -1;	// reset GetVarSyncNode
	}
	

	/*
    if ( v->v_perm != (list_t *)NULL )
    {
	n6 = NODE(g, "PERMUTE");
	EDGE(g, n4, n6, 0);

	SetPerm(v->v_perm);

	n = 1;

	while ( (p = GetPerm(v->v_perm)) != (perm_t *)NULL )
	{
	    t1 = ICONST(g, "CONST", p->p_to);

	    EDGE(g, n4, t1, 0);
	    EDGE(g, t1, n6, n++);
	}

	n4 = n6;
    }
	*/

	*out = n5;
}

// Minh changed BuildGetVarSet() to include an extra parameters (extrainfo)
void
BuildGetVarSet(graph_t *g, int *inp, int *out, var_t *v, int low_pid, int proc_count, extrainfo *extra)
//TODO: update signature. 
{
	int		n;
	int		t1;
	int		n4;
	int		n6;
	
	int		a1;
	
	slice_t *	s;
	perm_t *	p;

	char	varexpr[BUFSIZ]="";
	char	finalexpr[BUFSIZ]="";
	int		reducefunc, p2pmode;
	
	DPRINT("In BuildGetVarSet\n");

	GetGraphVar(GRAPH_P2PMODE, (int *)&p2pmode);
	GetGraphVar(GRAPH_REDUCE, (int *)&reducefunc);

	if ( v->v_internal )
	{
		n4 = NODE(g, "GETVAR");
		t1 = SCONST(g, "CONST", v->v_var);
	}
	else
	{
		if ( (s = v->v_slice) != (slice_t *)NULL )
		{
			char sliceexpr[BUFSIZ]="|slice(";
			for ( n = 0 ; n < s->s_rank ; n++ )
			{
				sprintf(sliceexpr, "%s%d,%d", sliceexpr, s->s_low[n], s->s_high[n]);
				if (n<s->s_rank-1)
					strcat(sliceexpr, ",");

			}
			strcat(sliceexpr, ")");
			sprintf(varexpr, "%s%s|", sliceexpr, v->v_var);
		}
		else
			strcpy(varexpr, v->v_var);

		/* check if reduce function is on
		** Minh 09/06/2009
		*/
		if (reducefunc != EXPR_REDUCE_NONE)
		{
			char reduceexpr[BUFSIZ]="";
			double lower_bound;
			double upper_bound;

			switch (reducefunc) {
			case EXPR_SUM:
				strcpy(reduceexpr, "`sum`");
				break;
			case EXPR_PRODUCT:
				strcpy(reduceexpr, "`product`");
				break;
			case EXPR_AVERAGE:
				strcpy(reduceexpr, "`average`");
				break;
			case EXPR_MIN:
				strcpy(reduceexpr, "`min`");
				break;
			case EXPR_MAX:
				strcpy(reduceexpr, "`max`");
				break;
			case EXPR_STDEV:
				strcpy(reduceexpr, "`stdev`");
				break;
			case EXPR_COUNTELEM:
				GetGraphVar(GRAPH_LOWER_BOUND, (double *)&lower_bound);
				GetGraphVar(GRAPH_UPPER_BOUND, (double *)&upper_bound);
				sprintf(reduceexpr, "`countelem(%lf,%lf)`", lower_bound, upper_bound);
				break;
			case EXPR_HASH_BOB:
				sprintf(reduceexpr, "`hash(%s)`", extra->hashinfo);
				break;
			default:
				SetError("Unknown reduction function %d", reducefunc);
				return;
			}
			sprintf(finalexpr, "%s%s", reduceexpr, varexpr);
		}

		// if there is p2p mode turn on, attach blockmap info
		else if (p2pmode)
		{
			char p2pexpr[BUFSIZ]="";
			sprintf(p2pexpr, "`ptop(%s-%s)`", SymLookup(v->v_isym), extra->blockmapinfo);
			sprintf(finalexpr, "%s%s", p2pexpr, varexpr);
		}
		else
			strcpy(finalexpr, varexpr);

		t1 = SCONST(g, "CONST", finalexpr);

		n4 = NODE(g, "DBGGETVAR_SET");
	}

    *inp = NODE(g, "PIT");

	EDGE(g, v->v_pid, *inp, 1);
//	EDGE(g, v->v_pid(low_pid), *inp, 1);

    /* MINH TODO : NEED TO REVIEW THIS BIT OF CODE */
    /* Minh add here to implement blockmap and parallel vs parallel
     * 19/05/2009
     * Donny TODO: Probably we need to synchronize
     */
    EDGE(g, v->v_pid, *inp, 0);
//    EDGE(g, v->v_pid(low_pid), *inp, 0);

    EDGE(g, *inp, t1, 0);
    // Const for asserid
    a1 = ICONST(g, "CONST", iAssertID);
    EDGE(g, *inp, a1, 0);

    EDGE(g, *inp, n4, 0);
    EDGE(g, t1, n4, 1);

	// Const for asserid
	EDGE(g, a1, n4 , 2);

    if ( v->v_perm != (list_t *)NULL )
    {
	n6 = NODE(g, "PERMUTE");
	EDGE(g, n4, n6, 0);

	SetPerm(v->v_perm);

	n = 1;

	while ( (p = GetPerm(v->v_perm)) != (perm_t *)NULL )
	{
	    t1 = ICONST(g, "CONST", p->p_to);

	    EDGE(g, n4, t1, 0);
	    EDGE(g, t1, n6, n++);
	}

	n4 = n6;
    }

	*out = n4;
}

// Minh changed BuildGetVar() to include an extra parameters (extrainfo)
void
BuildGetVar(graph_t *g, int *inp, int *out, var_t *v, int pid, extrainfo *extra)
{
	int		n;
	int		t1;
	int		n4;
	int		n6;
	
	int		a1;
	
	slice_t *	s;
	perm_t *	p;

	char	finalexpr[BUFSIZ] = "";
	char	varexpr[BUFSIZ] = "";
	int		reducefunc;
	int		p2pmode=0;
	
	DPRINT("In BuildGetVar\n");

	GetGraphVar(GRAPH_REDUCE, (int *)&reducefunc);

	if ( v->v_internal )
	{
		n4 = NODE(g, "GETVAR");
		t1 = SCONST(g, "CONST", v->v_var);
	}
	else
	{
		if ( (s = v->v_slice) != (slice_t *)NULL )
		{
		    char sliceexpr[BUFSIZ]="|slice(";
		    for ( n = 0 ; n < s->s_rank ; n++ )
		    {
				sprintf(sliceexpr, "%s%d,%d", sliceexpr, s->s_low[n], s->s_high[n]);
				if (n<s->s_rank-1)
				    strcat(sliceexpr, ",");
		    }
			strcat(sliceexpr, ")");
			sprintf(varexpr, "%s%s|", sliceexpr, v->v_var);
		}
		else
			strcpy(varexpr, v->v_var);

		/* check if reduce function is on
		** Minh 09/06/2009
		*/
		if (reducefunc != EXPR_REDUCE_NONE)
		{
	    	char reduceexpr[BUFSIZ]="";
			double lower_bound;
			double upper_bound;
			int histogram_numbins;
			double histogram_start;
			double histogram_end;

			switch (reducefunc) {
		    case EXPR_SUM:
				strcpy(reduceexpr, "`sum`");
				break;
		    case EXPR_PRODUCT:
				strcpy(reduceexpr, "`product`");
				break;
		    case EXPR_AVERAGE:
				strcpy(reduceexpr, "`average`");
				break;
		    case EXPR_MIN:
				strcpy(reduceexpr, "`min`");
				break;
		    case EXPR_MAX:
				strcpy(reduceexpr, "`max`");
				break;
		    case EXPR_STDEV:
				strcpy(reduceexpr, "`stdev`");
				break;
		    case EXPR_COUNTELEM:
				GetGraphVar(GRAPH_LOWER_BOUND, (double *)&lower_bound);
				GetGraphVar(GRAPH_UPPER_BOUND, (double *)&upper_bound);
				sprintf(reduceexpr, "`countelem(%lf,%lf)`", lower_bound, upper_bound);
				break;
		    case EXPR_HISTOGRAM:
				GetGraphVar(GRAPH_HISTOGRAM_NUMBINS, (int *)&histogram_numbins);
				GetGraphVar(GRAPH_HISTOGRAM_START, (double *)&histogram_start);
				GetGraphVar(GRAPH_HISTOGRAM_END, (double *)&histogram_end);
				sprintf(reduceexpr, "`histogram(%d,%lf,%lf)`", histogram_numbins, histogram_start, histogram_end);
				break;
			case EXPR_HASH_BOB:
				sprintf(reduceexpr, "`hash(%s)`", extra->hashinfo);
				break;
			default:
				SetError("Unknown reduction function %d", reducefunc);
				return;
			}

		    sprintf(finalexpr, "%s%s", reduceexpr, varexpr);
		}

        // if there is p2p mode turn on, attach blockmap info
		else if (p2pmode)
		{
			char p2pexpr[BUFSIZ]="";
			sprintf(p2pexpr, "`ptop(%s-%s)`", SymLookup(v->v_isym), extra->blockmapinfo);
			sprintf(finalexpr, "%s%s", p2pexpr, varexpr);
		}
		else
			strcpy(finalexpr, varexpr);

		t1 = SCONST(g, "CONST", finalexpr);
		n4 = NODE(g, "DBGGETVAR");
	}

	*inp = NODE(g, "PIT");
	EDGE(g, v->v_pid, *inp, 1);
//	EDGE(g, v->v_pid(pid), *inp, 1);

    /* MINH TODO : NEED TO REVIEW THIS BIT OF CODE */
    /* Minh add here to implement blockmap and parallel vs parallel
     * 19/05/2009
	 * Donny TODO: Probably we need to synchronize
     */
    EDGE(g, v->v_pid, *inp, 0);
//    EDGE(g, v->v_pid(pid), *inp, 0);

	EDGE(g, *inp, t1, 0);
	// Const for asserid
	a1 = ICONST(g, "CONST", iAssertID);
	EDGE(g, *inp, a1, 0);

	EDGE(g, *inp, n4, 0);
	EDGE(g, t1, n4, 1);	

	// Const for asserid
	EDGE(g, a1, n4 , 2);

	if ( v->v_perm != (list_t *)NULL )
	{
		n6 = NODE(g, "PERMUTE");
		EDGE(g, n4, n6, 0);

		SetPerm(v->v_perm);

		n = 1;

		while ( (p = GetPerm(v->v_perm)) != (perm_t *)NULL )
		{
			t1 = ICONST(g, "CONST", p->p_to);

			EDGE(g, n4, t1, 0);
			EDGE(g, t1, n6, n++);
		}

		n4 = n6;
	}

	*out = n4;
}

int _validate_block_map(graph_t *g, g_block_map_t* bm, var_t *v)
{
	int rank = bm->m_rank;
	int* size = (int*)Alloc(sizeof(int)*rank);
	int i;
	
	DPRINT("In _validate_block_map\n");
	
	for(i=0; i<rank; i++)
		size[i] = bm->m_data[i];

	if (_get_hash_block_size(rank, bm->m_data, bm->m_distribute, bm->m_blockcyclic_val, bm->m_procs, size)!=1) {
		char err_message[BUFSIZ];
		sprintf(err_message, "BlockMap %s decomposition is not feasiable with the given array shape", SymLookup(bm->m_sym));
		SetError(err_message);
		Free(size);
		return 0;
	}

	Free(size);
	return 1;
}

// andrewg fixed implicit calls to free on 8/10/10
int  _determine_hash_info(char* hashinfo, graph_t *g, g_block_map_t* bm1, g_block_map_t* bm2, int **data_size)
{
	int rank;
	int *final_size;
	int i;
	char buf[100];
	
	DPRINT("In _determine_hash_info\n");

	// no bockmap used, just hash the whole array
	if (bm1==(g_block_map_t*)NULL && bm2==(g_block_map_t*)NULL)
	{
		strcpy(hashinfo, "");
		return 0;
	}

	rank =  bm1!=(g_block_map_t*)NULL? bm1->m_rank : bm2->m_rank;
	final_size = (int*)Alloc(sizeof(int)*rank);

	for(i=0; i<rank; i++)
		final_size[i] = gcd(data_size[0][i], data_size[1][i]);

	// now put the data into hashinfo
	for(i=0; i<rank; i++) {
		sprintf(buf, "%d", final_size[i]);
		strcat(hashinfo, buf);
		if(i<rank-1)
			strcat(hashinfo, ",");
	}

	Free(final_size);

	return 0;
}

int _find_hash_info(char *hashinfo, graph_t *g, expr_t *e, rec_set_t *rec_set)
{
	expr_elt *      ep;
	g_block_map_t * bm1 = (g_block_map_t *)NULL; // Minh
	g_block_map_t * bm2 = (g_block_map_t *)NULL; // Minh
	int *data_size[2] = {NULL, NULL};
	int found = 0;
	int i, res;
	
	DPRINT("In _find_hash_info\n");

	RESET(e);
	while ( !EMPTY(e) )
	{
		ep = TOP(e)	;
		if ( ep->e_op == EXPR_BLOCKMAP )
		{
			if (found == 0 ) {
				if ( (bm1 = EvaluateBlockMap(e, rec_set, CMD_MODE_DEFERRED)) == (g_block_map_t *)NULL )
					return -1;

				data_size[0] = (int*)Alloc(sizeof(int)*bm1->m_rank);

				if (_get_hash_block_size(bm1->m_rank, bm1->m_data, bm1->m_distribute, bm1->m_blockcyclic_val, bm1->m_procs, data_size[0])!=1) {
					char err_message[BUFSIZ];
					sprintf(err_message, "BlockMap %s decomposition is not feasiable with the given array shape", SymLookup(bm1->m_sym));
					SetError(err_message);
		    if(data_size[0])    Free(data_size[0]);
		    if(data_size[1])    Free(data_size[1]);
					return -1;
				}
			}
			else {
				if ( (bm2 = EvaluateBlockMap(e, rec_set, CMD_MODE_DEFERRED)) == (g_block_map_t *)NULL )
					return -1;

				data_size[1] = (int*)Alloc(sizeof(int)*bm2->m_rank);
				if (_get_hash_block_size(bm2->m_rank, bm2->m_data, bm2->m_distribute, bm2->m_blockcyclic_val, bm2->m_procs, data_size[1])!=1) {
					char err_message[BUFSIZ];
					sprintf(err_message, "BlockMap %s decomposition is not feasiable with the given array shape", SymLookup(bm2->m_sym));
					SetError(err_message);
		    if(data_size[0])    Free(data_size[0]);
		    if(data_size[1])    Free(data_size[1]);
					return -1;
				}
				break;
			}
			found ++;
		}
		else
			ep = POP(e);
	}
	res = _determine_hash_info(hashinfo, g, bm1, bm2, data_size);

	for(i=0; i<2; i++)
		Free(data_size[i]);

	return res;
}

int
_identify_p2p_receiver_procset(graph_t *g, var_t *v1, var_t *v2)
{
	graph_inv * gi;
	int numprocs1, numprocs2;

	gi = FindGraphInv(g, SymLookup(v1->v_isym));
	numprocs1 = gi->gi_nproc;
	gi = FindGraphInv(g, SymLookup(v2->v_isym));
	numprocs2 = gi->gi_nproc;

	return numprocs1>numprocs2? 1:0;
}

/* Minh add this function to implement block map
 * 15/05/2009
 */
int
BuildBlockMap(graph_t *g, g_block_map_t *bm, var_t *v, extrainfo *extra, int blockmap_required)
{
	int     n1=0;
	int     n2=0;
	int     n3=0;
	int     t1;
	int     t2;
	int     t3;
	graph_inv * gi;

	int		reducefunc;
	char	mapname[BUFSIZ]="";
	
	DPRINT("In BuildBlockMap\n");

	gi = FindGraphInv(g, SymLookup(v->v_isym));

	GetGraphVar(GRAPH_REDUCE, (int *)&reducefunc);
	if (
		reducefunc != EXPR_REDUCE_NONE &&
		(
			// All known hash functions should be or'ed here
			reducefunc == EXPR_HASH_BOB
		)
	)
		sprintf(mapname, "%s#%s(%s)", extra->hashinfo, SymLookup(bm->m_sym), v->v_var);
	else
		sprintf(mapname, "%s(%s)", SymLookup(bm->m_sym), v->v_var);

	if (blockmap_required)
	{
		n1 = SCONST(g, "CONST", mapname);

		t3 = NODE(g, "TYPE_SET");
		n3 = NODE(g, "BLOCKMAP_SET");
		EDGE(g, n1, n3, 0);
		EDGE(g, t3, n3, 1);

//		if ( v->v_procset != (dbgprocset_t *)NULL && v->v_procset->s_indiv_proc >= 0)
		if ( v->v_procset != (dbgprocset_t *)NULL && v->v_indiv >= 0)
		{
			n2 = ICONST(g, "CONST", 1);
			EDGE(g, n2, n3, 2);
	
//			BuildGetVar(g, &t1, &t2, v, v->v_procset->s_indiv_proc, extra);
			BuildGetVar(g, &t1, &t2, v, v->v_indiv, extra);

			EDGE(g, t2, n3, 3);
			EDGE(g, t2, n1, 0);
			EDGE(g, t2, n2, 0);
			EDGE(g, t2, t3, 0);

			return n3;
		}

		n2 = ICONST(g, "CONST", bm->m_actual->v_procset->s_nproc);
		EDGE(g, n2, n3, 2);

		BuildGetVarSet(g, &t1, &t2, v, 0, gi->gi_nproc, extra); //TODO: gi->gi_nproc is not used
		//BuildGetVarSet_Sync(g, &t1, &t2, v, 0, gi->gi_nproc, extra, blockmap_required);

		EDGE(g, t2, n3, 3);

		EDGE(g, t2, n1, 0);
		EDGE(g, t2, n2, 0);
		EDGE(g, t2, t3, 0);
	}
	else
	{
		/*
		n3 = NODE(g, "PIEQ");
		n2 = ICONST(g, "CONST", 0);
		n1 = NODE(g, "EXIT");
		*/

//		if ( v->v_procset != (dbgprocset_t *)NULL && v->v_procset->s_indiv_proc >= 0)
		if ( v->v_procset != (dbgprocset_t *)NULL && v->v_indiv >= 0)
		{

//			BuildGetVar(g, &t1, &t2, v, v->v_procset->s_indiv_proc, extra);
			BuildGetVar(g, &t1, &t2, v, v->v_indiv, extra);

			EDGE(g, t2, n2, 0);
			EDGE(g, t2, n3, 1);
			EDGE(g, t2, n3, 2);
			EDGE(g, n2, n3, 3);
			EDGE(g, n3, n1, 0);

			return n3;
		}

		BuildGetVarSet(g, &t1, &t2, v, 0, gi->gi_nproc, extra);
		//BuildGetVarSet_Sync(g, &t1, &t2, v, 0, gi->gi_nproc, extra, blockmap_required);

		/*
		EDGE(g, t2, n2, 0);
		EDGE(g, t2, n3, 0);
		EDGE(g, t2, n3, 1);
		EDGE(g, n2, n3, 2);
		EDGE(g, n3, n1, 0);
		*/
	}

	return n3;
}

/* Minh adds this function for more reduce functions
 */
int
BuildReduce(graph_t *g, var_t *v, int *inp, extrainfo *extra)
{
	int	n1;
	int     n2;
	int     n3;
	int     t2;
	int	reducefunc;
	char	name[BUFSIZ]="";

	graph_inv * gi;
	
	DPRINT("In BuildReduce\n");

	gi = FindGraphInv(g, SymLookup(v->v_isym));

	// now need to work out the reduce function
	GetGraphVar(GRAPH_REDUCE, (int *)&reducefunc);
	if (reducefunc != EXPR_REDUCE_NONE)
	{
		switch (reducefunc)
		{
		case EXPR_SUM:
			sprintf(name, "`sum`%s", v->v_var);
			break;
		case EXPR_PRODUCT:
			sprintf(name, "`product`%s", v->v_var);
			break;
		case EXPR_AVERAGE:
			sprintf(name, "`average`%s", v->v_var);
			break;
		case EXPR_MIN:
			sprintf(name, "`min`%s", v->v_var);
			break;
		case EXPR_MAX:
			sprintf(name, "`max`%s", v->v_var);
			break;
		case EXPR_STDEV:
			sprintf(name, "`stdev`%s", v->v_var);
			break;
		case EXPR_COUNTELEM:
			sprintf(name, "`count`%s", v->v_var);
			break;
		/*
		case EXPR_HISTOGRAM:
			sprintf(name, "`histogram`%s", v->v_var);
			break;
		*/
		default:
			sprintf(name, "%s", v->v_var);
			break;
		}
	}

	n1 = SCONST(g, "CONST", name);

//	if ( v->v_procset != (dbgprocset_t *)NULL && v->v_procset->s_indiv_proc >= 0)
	if ( v->v_procset != (dbgprocset_t *)NULL && v->v_indiv >= 0)
	{
		n3 = NODE(g, "REDUCE");
		n2 = ICONST(g, "CONST", 1);
		EDGE(g, n1, n3, 0);
		EDGE(g, n2, n3, 1);

//		BuildGetVar(g, inp, &t2, v, v->v_procset->s_indiv_proc, NULL);
		BuildGetVar(g, inp, &t2, v, v->v_indiv, NULL);

		EDGE(g, t2, n1, 0);
		EDGE(g, t2, n2, 0);
		EDGE(g, t2, n3, 2);

		return n3;
	}

	n3 = NODE(g, "REDUCE_SET");
	EDGE(g, n1, n3, 0);

//	n2 = ICONST(g, "CONST", gi->gi_nproc);
	n2 = ICONST(g, "CONST", v->v_procset->s_nproc);
	EDGE(g, n2, n3, 1);

	BuildGetVarSet(g, inp, &t2, v, 0, gi->gi_nproc, extra);

	EDGE(g, t2, n1, 0);
	EDGE(g, t2, n2, 0);
	EDGE(g, t2, n3, 2);

	return n3;
}

/* this node is used to display compared results from p2p operation
 * Minh 29/06/2011
 */

int
BuildDisplayP2P(graph_t *g, int id, int receiver_data, int proc_set1, int proc_set2)
{
	int		n1;
	int		n2;
	int		n3;
	int		t1;
	int		t2;
	int		t3;

	n1 = NODE(g, "DISPLAY_P2P");
	t3 = ICONST(g, "CONST", id);
	EDGE(g, receiver_data, t3, 0);
	EDGE(g, t3, n1, 0);
	EDGE(g, receiver_data, n1, 1);

	n2 = NODE(g, "DBGCLOSESOCKET_SET");

	t1 = ICONST(g, "CONST", 0);
	EDGE(g, n1, t1, 0);
	n3 = NODE(g, "PIEQ");
	EDGE(g, n1, n3, 0);
	EDGE(g, t1, n3, 1);
	EDGE(g, n1, n3, 2);

	// connect the 2 PROC_SET nodes to CLOSESOCKET
	EDGE(g, proc_set1, n2, 0);
	EDGE(g, proc_set2, n2, 1);
	EDGE(g, n3, n2, 2);

	t2 = NODE(g, "EXIT");
	EDGE(g, n2, t2, 0);

	return n1;
}

int
BuildCmp(graph_t *g, int id, int data1, int data2, int cmpop)
{
	int		n1;
	int		n3;
	int		t1;
	int		t2;
	int		t3;
	int		t4;
	int		t6;
	int		log;
	char *	  lf;
	
	DPRINT("In BuildCmp\n");

	GetGraphVar(GRAPH_LOGGING, (int *)&log);
	GetGraphVar(GRAPH_LOGFILE, &lf);

	n1 = NODE(g, "COMPARE");
	t1 = ICONST(g, "CONST", id);
	EDGE(g, data1, t1, 0);
	EDGE(g, t1, n1, 0);
	EDGE(g, data1, n1, 1);
	EDGE(g, data2, n1, 2);
	t1 = ICONST(g, "CONST", cmpop);
	EDGE(g, data1, t1, 0);
	EDGE(g, t1, n1, 3);

	if ( log )
	{
		t1 = SCONST(g, "CONST", "compare");
		EDGE(g, n1, t1, 0);
		t2 = SCONST(g, "CONST", lf);
		EDGE(g, n1, t2, 0);
		t4 = NODE(g, "LOG");
		EDGE(g, n1, t4, 0);
		EDGE(g, t1, t4, 1);
		EDGE(g, t2, t4, 2);
	}

	t1 = ICONST(g, "CONST", 0);
	EDGE(g, n1, t1, 0);
	n3 = NODE(g, "PIEQ");
	EDGE(g, n1, n3, 0);
	EDGE(g, t1, n3, 1);
	EDGE(g, n1, n3, 2);

	t6 = NODE(g, "EXIT");
	EDGE(g, n3, t6, 0);

	if ( log )
	{
		t1 = SCONST(g, "CONST", "exiting");
		EDGE(g, n3, t1, 0);
		t2 = SCONST(g, "CONST", lf);
		EDGE(g, n3, t2, 0);
		t3 = NODE(g, "LOG");
		EDGE(g, n3, t3, 0);
		EDGE(g, t1, t3, 1);
		EDGE(g, t2, t3, 2);
	}

	return n1;

#ifdef to_fix
	if ( a->a_force )
	{
		int	f1;
		int	f2;
		int	f3;
		int	f4;
		int	f5;
		int	f6;
		int	f7;
		int	f8;

		f1 = ICONST(g, "CONST", 0);
		EDGE(g, n2, f1);
		f2 = NODE(g, "PIEQ");
		EDGE(g, f1, f2);
		EDGE(g, n2, f2);
		EDGE(g, a->a_v1->v_proc, f2);

		f3 = ICONST(g, "CONST", 0);
		EDGE(g, n2, f3);
		f4 = SCONST(g, "CONST", a->a_v1->v_var);
		EDGE(g, n2, f4);
		f5 = NODE(g, "PIEQ");
		EDGE(g, f3, f5);
		EDGE(g, n2, f5);
		EDGE(g, f4, f5);

		f6 = ICONST(g, "CONST", 0);
		EDGE(g, n2, f6);
		f7 = NODE(g, "PIEQ");
		EDGE(g, f6, f7);
		EDGE(g, n2, f7);
		EDGE(g, data2, f7);

		f8 = NODE(g, "DBGSETVAR");
		EDGE(g, f2, f8);
		EDGE(g, f5, f8);
		EDGE(g, f7, f8);

		/*
		** Result of force connected to "MERGE".
		*/

		if
		(
			a->a_dfmode == DF_SYNC 
			||
			(
				a->a_dfmode == DF_DEFAULT 
				&&
				!async
			)
		)
			EDGE(g, f8, or1);
	}
#endif /* to_fix */
}

/*
** Build an expression graph.
*/
int
BuildExpr(graph_t *g, int *inp, int *out, list_t *res, expr_t *e, rec_set_t *rec_set)
{
	extrainfo extra;
	
	DPRINT("In BuildExpr\n");

	if ( e->e_lval == 0 )
	{
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
int
build_expr(graph_t *g, int *inp, int *out, list_t *vlist, expr_t *e, rec_set_t *rec_set, func_t *f, extrainfo *extra)
{
	int		out1;
	int		out2;
	int		out3;
	int		inp1;
	int		inp2;
	int		inp3;
	int		reducefunc;
	list_t *	vl;
	var_t *		v;
	arg_t *		a;
	expr_elt *	ep;
	graph_inv * gi;

	DPRINT("In build_expr\n");
	
	ep = POP(e);
	
	DPRINT("build_expr: Popped expr_op %s\n", getExprOpStr(ep->e_op));

	switch ( ep->e_op )
	{
	case EXPR_OPCOND:
		vl = NewList();

		if ( build_expr(g, &inp1, &out1, vl, e, rec_set, f, extra) < 0 )
			return -1;

		AppendList(vlist, vl);
		DestroyList(vl, (void (*)())NULL);

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

		if ( build_expr(g, &inp2, &out2, vl, e, rec_set, f, extra) < 0 )
			return -1;

		AppendList(vlist, vl);
		DestroyList(vl, (void (*)())NULL);

		/* drop through */

	case EXPR_OPPOS:
	case EXPR_OPNEG:
		vl = NewList();

		if ( build_expr(g, &inp3, &out3, vl, e, rec_set, f, extra) < 0 )
			return -1;

		if ( (*out = BuildOp(g, ep->e_op, out3, inp2, out2, inp1, out1)) < 0 )
			return -1;

		*inp = inp3;

		AppendList(vlist, vl);
		DestroyList(vl, (void (*)())NULL);

		return 0;

	case EXPR_OPSUBSCR:
		PUSH(e);

		/* drop through */

	case EXPR_VAR:
		if ( (v = ReadVarDecl(e, rec_set)) == (var_t *)NULL )
			return -1;

		AddToList(vlist, (void *)v);

		BuildInvoke(g, v);
		BuildMPWait(g, v);
		// Minh add here for reduce functions
		GetGraphVar(GRAPH_REDUCE, (int *)&reducefunc);
		if (
			reducefunc != EXPR_REDUCE_NONE &&
			reducefunc != EXPR_HASH_BOB
			)
			(*out) = BuildReduce(g, v, inp, extra);
		else {
			if ( v->v_procset != (dbgprocset_t *)NULL && v->v_indiv >= 0) {
				BuildGetVar(g, inp, out, v, v->v_indiv, extra);
			}
//			if ( v->v_procset != (dbgprocset_t *)NULL && v->v_procset->s_indiv_proc >= 0) {
//				BuildGetVar(g, inp, out, v, v->v_procset->s_indiv_proc, extra);
//			}
			else
			{
				gi = FindGraphInv(g, SymLookup(v->v_isym));
				BuildGetVarSet(g, inp, out, v, 0, gi->gi_nproc, extra);
			}
		}
		return 0;

	case EXPR_INT:
		*out = ICONST(g, "CONST", ep->e_int);
		*inp = *out;
		return 0;

	case EXPR_FLOAT:
		*out = FCONST(g, "CONST", ep->e_float);
		*inp = *out;
		return 0;
		
	case EXPR_DSYM:
		SetError("Must specify location information in declarative mode");
		return -1;

	case EXPR_FUNC:
		vl = NewList();

		if ( build_func(g, ep->e_sym, inp, out, vl, e, rec_set, f, extra) < 0 )
			return -1;

		AppendList(vlist, vl);
		DestroyList(vl, (void (*)())NULL);
		return 0;

	case EXPR_SYM:
		if ( f != (func_t *)NULL )
		{
			if ( FindArg(&a, ep->e_sym, f) < 0 )
				return -1;

			if ( a != (arg_t *)NULL )
			{
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
int
build_func(graph_t *g, hentry_t *sym, int *res_inp, int *res_out, list_t *res_vlist, expr_t *e, rec_set_t *rec_set, func_t *f, extrainfo *extra)
{
	int		n;
	int		inp;
	int		out;
	arg_t *		a;
	func_t *	fp;
	list_t *	aarg;
	list_t *	sarg;
	list_t *	vlist;
	expr_elt *	ep;
	
	DPRINT("In build_func\n");

	if ( (fp = FindFunc(sym)) == (func_t *)NULL )
	{
		SetError("Function \"%s\" not defined", SymLookup(sym));
		return -1;
	}

	ep = POP(e);
	
	/*
	** We first expect to find a list start token.
	*/
	if ( ep->e_op != EXPR_LIST_START )
	{
		SetError("Internal error: expected EXPR_LIST_START");
		return -1;
	}
	
	ep = POP(e);

	/*
	** Evaluate the arguments for the function. The number of
	** arguments follows...
	*/

	if ( ep->e_op != EXPR_INT )
	{
		SetError("Internal error: expected EXPR_INT");
		return -1;
	}

	n = ep->e_int;

	aarg = NewList();

	while ( n-- > 0 )
	{
		ep = POP(e);

		if ( ep->e_op != EXPR_LIST )
		{
			SetError("Internal error: expected EXPR_LIST");
			DestroyList(aarg, DestroyArg);
			return -1;
		}

		vlist = NewList();

		if ( build_expr(g, &inp, &out, vlist, e, rec_set, f, extra) < 0 )
		{
			DestroyList(aarg, DestroyArg);
			return -1;
		}

		/*
		** Save the argument for when the function is
		** evaluated.
		*/

		a = NewArg(ARG_GRAPH, inp, out, vlist);
		AddFirst(aarg, (void *)a);
	}

	if ( fp->f_builtin != (int (*)(list_t *, val_t **))NULL )
	{
		SetError("Can't use builtin function in delarative mode");
		DestroyList(aarg, DestroyArg);
		return -1;
	}

	sarg = fp->f_actual;
	fp->f_actual = aarg;

	/*
	** Apply function to its arguments.
	*/
	RESET(fp->f_def);

	vlist = NewList();

	if ( build_expr(g, &inp, &out, vlist, fp->f_def, rec_set, fp, extra) < 0 )
	{
		DestroyList(aarg, DestroyArg);
		return -1;
	}

	/*
	** Restore args.
	*/
	fp->f_actual = sarg;
	DestroyList(aarg, DestroyArg);

	*res_inp = inp;
	*res_out = out;
	AppendList(res_vlist, vlist);
	DestroyList(vlist, (void (*)())NULL);

	return 0;
}

/*
** Build unary, binary or tertiary operator.
*/
int
BuildOp(graph_t *g, expr_op op, int o1, int i2, int o2, int i3, int o3)
{
	int	n;
	int	n1;
	int	n2;
	int	n3;
	
	DPRINT("In BuildOp\n");

	switch ( op )
	{
	case EXPR_OPCOND:
		n1 = ICONST(g, "CONST", 0);
		EDGE(g, o1, n1, 0);
		EDGE(g, o1, i2, 0);
		EDGE(g, o1, i3, 0);

		n2 = NODE(g, "PIEQ");
		EDGE(g, n1, n2, 0);
		EDGE(g, o1, n2, 1);
		EDGE(g, o2, n2, 2);

		n3 = NODE(g, "PINE");
		EDGE(g, n1, n3, 0);
		EDGE(g, o1, n3, 1);
		EDGE(g, o3, n3, 2);

		n = NODE(g, "MERGE");
		EDGE(g, n2, n, 0);
		EDGE(g, n3, n, 1);

		break;

	case EXPR_OPADD:
		n = NODE(g, "ADD");
		break;

	case EXPR_OPSUB:
		n = NODE(g, "SUB");
		break;

	case EXPR_OPMUL:
		n = NODE(g, "MUL");
		break;

	case EXPR_OPDIV:
		n = NODE(g, "DIV");
		break;

	case EXPR_OPREM:
		n = NODE(g, "REM");
		break;

	case EXPR_OPEQUAL:
		n = NODE(g, "EQUAL");
		break;

	case EXPR_OPNOTEQUAL:
		n = NODE(g, "NEQ");
		break;

	case EXPR_OPLEQ:
		n = NODE(g, "LEQ");
		break;

	case EXPR_OPGEQ:
		n = NODE(g, "GEQ");
		break;

	case EXPR_OPLT:
		n = NODE(g, "LT");
		break;

	case EXPR_OPGT:
		n = NODE(g, "GT");
		break;

	case EXPR_OPAND:
		n = NODE(g, "AND");
		break;

	case EXPR_OPOR:
		n = NODE(g, "OR");
		break;

	case EXPR_OPPOS:
		break;

	case EXPR_OPNEG:
		n = NODE(g, "NEG");
		break;

	default:
		SetError("Invalid operation");
		return -1;
	}

	switch ( op )
	{
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
		EDGE(g, o1, n, 0);
		EDGE(g, o1, i2, 0);
		EDGE(g, o2, n, 1);
		break;

	case EXPR_OPNEG:
		EDGE(g, o1, n, 0);
		break;

	case EXPR_OPPOS:
		n = o1;
		break;
	
	default:
		break;
	}

	return n;
}

void
BuildMerge(graph_t *g, var_t *v, int from, int async, int force)
{
	int		r;
	graph_inv *	gi;
	
	DPRINT("In BuildMerge\n");

	gi = FindGraphInv(g, SymLookup(v->v_isym));

//	if ( v->v_procset != (dbgprocset_t *)NULL && v->v_procset->s_indiv_proc >= 0)
	if ( v->v_procset != (dbgprocset_t *)NULL && v->v_indiv >= 0)
	{
		r = BuildMergeProc(g, gi, v, v->v_indiv, from, async, force); 
#ifdef DEBUG
		DebugPrintGraph(g);
#endif
		BuildSync(g, v, v->v_indiv, r);
#ifdef DEBUG
		DebugPrintGraph(g);
#endif
//		r = BuildMergeProc(g, gi, v, v->v_procset->s_indiv_proc, from, async, force); 
//		BuildSync(g, v, v->v_procset->s_indiv_proc, r);
/* old code
		r = BuildMergeProc(g, gi, v, v->v_proc->p_pnum, from, async, force);
		BuildSync(g, v, v->v_proc->p_pnum, r);
*/
	}
	else
	{
		// We use proc low_pid (0) as the "base".
		r = BuildMergeProc(g, gi, v, 0, from, async, force);
		BuildSync(g, v, 0, r);
	}
}

int
BuildMergeProc(graph_t *g, graph_inv *gi, var_t *v, int pid, int from, int async, int force)
{
	int		t1;
	int		t2;
	graph_procset *	gp;

	gp = gi->gi_procset;
	
	DPRINT("In BuildMergeProc\n");

	/*
	** Skip unused procs.
	*/
	if ( !gp->gp_builtinv )
		return -1;

	if ( !async )
	{
		t1 = ICONST(g, "CONST", 0);
		EDGE(g, from, t1, 0);

#ifdef to_fix
		if ( !force )
#endif /* to_fix */
			t2 = NODE(g, "PINE");
#ifdef to_fix
		else
			t2 = NODE(g, "PILT");
#endif /* to_fix */

		EDGE(g, from, t2, 0);
		EDGE(g, t1, t2, 1);
		EDGE(g, v->v_pid, t2, 2);
	}
	else
	{
		t2 = NODE(g, "PIT");
		EDGE(g, from, t2, 0);
		EDGE(g, v->v_pid, t2, 1);
	}

	return t2;
}

/*
** Build graph for variable assignment. 
**  lval is the lval string
**  einp is the node number of the input to the expression graph
**  eout is the node number of the output of the expression graph
**  vlist is a list of variables used in the expression graph
*/
int
BuildAssign(graph_t *g, char *lval, int einp, int eout, list_t *vlist)
//TODO: review this function.
{
	int	n1;
	int	n2;
	int	n3;
	int	pid=0;
	var_t *	v;
	var_t *	last = (var_t *)NULL;
	
	DPRINT("In BuildAssign\n");

	n1 = SCONST(g, "CONST", lval);
	n2 = NODE(g, "ASSIGN");
	EDGE(g, eout, n1, 0);
	EDGE(g, n1, n2, 0);
	EDGE(g, eout, n2, 1);

	if ( EmptyList(vlist) )
	{
		SetError("Must specify breakpoint in assignment");
		return -1;
	}

	SetList(vlist);

	while ( (v = (var_t *)GetListElement(vlist)) != (var_t *)NULL )
	{
		pid = v->v_procset->s_indiv_proc;

		n3 = NODE(g, "PIT");
		EDGE(g, n2, n3, 0);
		EDGE(g, v->v_pid, n3, 1);

		BuildSync(g, v, pid, n3);

		last = v;
	}

	/*
	** Connect a (any) proc to the input of the expression graph.
	*/
	EDGE(g, last->v_pid, einp, 0);

	return 0;
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
void
BuildSync(graph_t *g, var_t *v, int pid, int from)
{
	graph_inv *	gi;
	graph_procset *	gp;
	
	DPRINT("In BuildSync\n");

	gi = FindGraphInv(g, SymLookup(v->v_isym));

	gp = gi->gi_procset;

	/*
	** Skip unused procs.
	*/
	if ( !gp->gp_builtinv )
		return;

	if ( v->v_sync < 0 )
	{
		v->v_sync = NODE(g, "SYNC");
		EDGE(g, v->v_sync, PROC_MERGE_NODE(gp), -1);
	}

	EDGE(g, from, v->v_sync, -1);
/* old code
	if ( v->v_sync(pid) < 0 )
	{
		v->v_sync(pid) = NODE(g, "SYNC");
		EDGE(g, v->v_sync(pid), PROC_MERGE_NODE(gp), -1);
	}

	EDGE(g, from, v->v_sync(pid), -1);
*/
}

/*
** Evaluate and build an assert.
*/
int
BuildAssert(graph_t *g, int id, expr_t *e, rec_set_t *rec_set, list_t *vlist)
{
	int		i1;
	int		i2;
	int		o1;
	int		o2;
	int		cmp1=0;
	int		cmp2=0;
	int		cmp;
	int		cmpop;  // Minh add here for different type of assertions
	var_t *		v;
	var_t *		tmp_v = NULL;
	int		async;
	int		force;
	dfmode_t	dfmode;
	int		reducefunc;
	int		p2pmode=0;
	extrainfo	extra;

	// initialise the values in extra
	strcpy(extra.hashinfo, "");
	strcpy(extra.blockmapinfo, "");

	list_t *	v1 = (list_t *)NULL;
	list_t *	v2 = (list_t *)NULL;

	g_block_map_t * bm = (g_block_map_t *)NULL; // Minh
	g_block_map_t * bm2 = (g_block_map_t *)NULL; // Minh

	expr_elt *	ep;
	
	iAssertID = id;
	
	DPRINT("In BuildAssert for id %d\n", iAssertID);
	
	if ( e->e_lval == 0 )
		return -1;
	
	GetGraphVar(GRAPH_REDUCE, (int *)&reducefunc);
	if (
		reducefunc != EXPR_REDUCE_NONE &&
		(
			// known hash functions are or'ed here
			reducefunc == EXPR_HASH_BOB
		)
	)
	{
		DPRINT("BuildAssert: Reduce function is set!\n");
		int precision_lower, precision_upper;
		char tmp[BUFSIZ]="";
		if (_find_hash_info(extra.hashinfo, g, e, rec_set) == -1)
			return -1;
		GetGraphVar(GRAPH_HASH_LOWER, (int *)&precision_lower);
		GetGraphVar(GRAPH_HASH_UPPER, (int *)&precision_upper);
		DPRINT("BuildAssert: hashinfo is %s\n", extra.hashinfo);
		DPRINT("BuildAssert: Reduce function is %s\n", getExprOpStr(reducefunc));
		DPRINT("BuildAssert: lower precision is %d\n", precision_lower);
		DPRINT("BuildAssert: upper precision is %d\n", precision_upper);
		// now pass the hashinfo into the BlockMap node
		sprintf(tmp, "%s#%d,%d,%d", extra.hashinfo, reducefunc, precision_lower, precision_upper);
		strcpy(extra.hashinfo, tmp);
	}

	GetGraphVar(GRAPH_P2PMODE, (int *)&p2pmode);
	RESET(e);

	ep = POP(e);

	cmpop = ep->e_op;
	
	DPRINT("BuildAssert: Comparison operator is %s\n", getExprOpStr(ep->e_op));

	ep = TOP(e);
	
	DPRINT("BuildAssert: Building lval.\n");
	DPRINT("BuildAssert: The starting op for lval is %s\n", getExprOpStr(ep->e_op));

	switch ( ep->e_op )
	{
	case EXPR_BLOCKMAP:
		DPRINT("BuildAssert: Evaluating blockmap for lval\n");
		if ( (bm = EvaluateBlockMap(e, rec_set, CMD_MODE_DEFERRED)) == (g_block_map_t *)NULL )
			return -1;
		break;

	// Minh adds these to allow assert against constants
	// Just int and float for now
	case EXPR_INT:
		DPRINT("BuildAssert: Found const INT for lval\n");
		o1 = AIFCONST(g, "CONST", ep->e_int, AIF_INTEGER);
		i1 = o1;
		ep = POP(e);
		break;

	case EXPR_FLOAT:
		DPRINT("BuildAssert: Found const FLOAT for lval\n");
		o1 = AIFCONST(g, "CONST", ep->e_float, AIF_FLOATING);
		i1 = o1;
		ep = POP(e);
		break;

	default:
		DPRINT("BuildAssert: Building expression for lval\n");
		v1 = NewList();

		if ( build_expr(g, &i1, &o1, v1, e, rec_set, (func_t *)NULL, &extra) < 0 )
			return -1;

		if ( EmptyList(v1) )
		{
			SetError("Must specify breakpoint in the lval of the assertion!");
			return -1;
		}

		break;
	}

	ep = TOP(e);
	
	DPRINT("BuildAssert: Building rval.\n");
	DPRINT("BuildAssert: The starting op for rval is %s\n", getExprOpStr(ep->e_op));

	switch ( ep->e_op )
	{
	case EXPR_BLOCKMAP:
		DPRINT("BuildAssert: Evaluating blockmap for rval\n");
		if ( bm != (g_block_map_t *)NULL )
		{
			tmp_v = bm->m_actual;
			if ( (bm2 = EvaluateBlockMap(e, rec_set, CMD_MODE_DEFERRED)) == (g_block_map_t *)NULL )
			{
				DestroyList(v1, (void (*)())NULL);
				return -1;
			}
			break;
		}

		if ( (bm = EvaluateBlockMap(e, rec_set, CMD_MODE_DEFERRED)) == (g_block_map_t *)NULL )
		{
			DestroyList(v1, (void (*)())NULL);
			return -1;
		}

		break;

	case EXPR_INT:
		DPRINT("BuildAssert: Found const INT for rval\n");
		o2 = AIFCONST(g, "CONST", ep->e_int, AIF_INTEGER);
		i2 = o2;
		ep = POP(e);
		break;

	case EXPR_FLOAT:
		DPRINT("BuildAssert: Found const FLOAT for rval\n");
		o2 = AIFCONST(g, "CONST", ep->e_float, AIF_FLOATING);
		i2 = o2;
		ep = POP(e);
		break;

	default:
		DPRINT("BuildAssert: Building expression for rval\n");
		v2 = NewList();

		if ( build_expr(g, &i2, &o2, v2, e, rec_set, (func_t *)NULL, &extra) < 0 )
		{
			if( bm != (g_block_map_t *)NULL ) // Minh
				DestroyBlockMap(bm);
			else
				DestroyList(v1, (void (*)())NULL);

			return -1;
		}
		if ( EmptyList(v2) )
		{
			SetError("Must specify breakpoint in the rval of the assertion!");
			return -1;
		}
		break;
	}


	// Original: if ( v1 == (list_t *)NULL )
	/* Minh modify here to implement blockmap
	 * 19/05/2009
	 */
	if ( v1 == (list_t *)NULL && v2 != (list_t *)NULL )
	{
		cmp1 = o2;

		/*
		** Connect first (any) proc to input of expression graph.
		*/
		SetList(v2);
		v = (var_t *)GetListElement(v2);
//		if (v->v_procset != (dbgprocset_t *)NULL && v->v_procset->s_indiv_proc >= 0) {
		if (v->v_procset != (dbgprocset_t *)NULL && v->v_indiv >= 0) {
			EDGE(g, v->v_pid, i2, 0);

			/*
			** set v_value to output of expression
			*/
			v->v_value = o2;
		}

		AppendList(vlist, v2);
		DestroyList(v2, (void (*)())NULL);

		// check if there is constant
		if ( bm == (g_block_map_t *)NULL ) {
			EDGE(g, cmp1, i1, 0);
			cmp2 = o1;
		}
	}
	// Original: else if ( v2 == (list_t *)NULL )
	// Minh
	else if ( v2 == (list_t *)NULL && v1 != (list_t *)NULL )
	{
		cmp1 = o1;

		/*
		** Connect first (any) proc to input of expression graph.
		*/
		SetList(v1);
		v = (var_t *)GetListElement(v1);
//		if (v->v_procset != (dbgprocset_t *)NULL && v->v_procset->s_indiv_proc >= 0) {
		if (v->v_procset != (dbgprocset_t *)NULL && v->v_indiv >= 0) {
			EDGE(g, v->v_pid, i1, 0);
			/*
			** set v_value to output of expression
			*/
			v->v_value = o1;
		}

		AppendList(vlist, v1);
		DestroyList(v1, (void (*)())NULL);

		// check if there is constant
		if ( bm == (g_block_map_t *)NULL ) {
			EDGE(g, cmp1, i2, 0);
			cmp2 = o2;
		}
	}
	// Original: else
	// Minh
	else if (v2 != (list_t *)NULL && v2 != (list_t *)NULL)	
	{
		cmp1 = o1;
		cmp2 = o2;

		/*
		** Connect first (any) proc to input of expression graphs.
		*/
		SetList(v1);
		v = (var_t *)GetListElement(v1);
		if (v->v_procset != (dbgprocset_t *)NULL && v->v_procset->s_indiv_proc >= 0) {
			EDGE(g, v->v_pid, i1, 0);

			/*
			** set v_value to output of expression
			*/
			v->v_value = o1;
		}

		SetList(v2);
		v = (var_t *)GetListElement(v2);
		if (v->v_procset != (dbgprocset_t *)NULL && v->v_procset->s_indiv_proc >= 0) {
			EDGE(g, v->v_pid, i2, 0);

			/*
			** set v_value to output of expression
			*/
			v->v_value = o2;
		}

		AppendList(vlist, v1);
		AppendList(vlist, v2);
		DestroyList(v1, (void (*)())NULL);
		DestroyList(v2, (void (*)())NULL);
	}

	// Minh
	if ( (bm != (g_block_map_t *)NULL) && (bm2 != (g_block_map_t *)NULL) )
	{
		if (p2pmode)
			// set up the global var other_procset
			other_procset = bm2->m_actual->v_procset;

		AddToList(vlist, (void *)tmp_v);

		BuildInvoke(g, tmp_v);
		BuildMPWait(g, tmp_v);

		if (p2pmode)
			// set up the global var other_procset
			other_procset = tmp_v->v_procset;

		AddToList(vlist, (void *)bm2->m_actual);

		BuildInvoke(g, bm2->m_actual);
		BuildMPWait(g, bm2->m_actual);

		// check if the blockmap function is pplication for this program
		if (bm != (g_block_map_t *)NULL) {
			if (_validate_block_map(g, bm, tmp_v)==0)
				return -1;
		}
		if (bm2 != (g_block_map_t *)NULL) {
			if (_validate_block_map(g, bm2, bm2->m_actual)==0)
				return -1;
		}

		if (p2pmode)
		{
			int numprocs, blockmap_required;
			char tmpstr[BUFSIZ]="";
			double graph_lower, graph_upper;
			graph_inv * gi;

			gi = FindGraphInv(g, SymLookup(tmp_v->v_isym));
			numprocs = gi->gi_nproc;
			BlockMapToString(bm, tmp_v, numprocs, extra.blockmapinfo);
			strcat(extra.blockmapinfo, "-");

			gi = FindGraphInv(g, SymLookup(bm2->m_actual->v_isym));
			numprocs = gi->gi_nproc;
			BlockMapToString(bm2, bm2->m_actual, numprocs, tmpstr);
			strcat(extra.blockmapinfo, tmpstr);

			GetGraphVar(GRAPH_LOWER, &graph_lower);
			GetGraphVar(GRAPH_LOWER, &graph_upper);

			blockmap_required = _identify_p2p_receiver_procset(g, tmp_v, bm2->m_actual);
			if (blockmap_required)
				strcpy(tmpstr, SymLookup(bm2->m_actual->v_isym));
			else
				strcpy(tmpstr, SymLookup(tmp_v->v_isym));
			sprintf(tmpstr, "%s-%s-%f,%f", tmpstr, extra.blockmapinfo, graph_lower, graph_upper);
			strcpy(extra.blockmapinfo, tmpstr);

			cmp1 = BuildBlockMap(g, bm, tmp_v, &extra, blockmap_required);
			if (cmp1 == -1) {
				return -1;
			}
			cmp2 = BuildBlockMap(g, bm2, bm2->m_actual, &extra, !blockmap_required);
			if (cmp2 == -1) {
				//DestroyBlockMap(bm2);
				return -1;
			}
		}
		else
		{
			cmp1 = BuildBlockMap(g, bm, tmp_v, &extra, 1);
			if (cmp1 == -1) {
			//DestroyBlockMap(bm); //Why destroy the blockmap definition??
				return -1;
			}
			cmp2 = BuildBlockMap(g, bm2, bm2->m_actual, &extra, 1);
			if (cmp2 == -1) {
				//DestroyBlockMap(bm2);
				return -1;
			}
		}
	}
	else if ( bm != (g_block_map_t *)NULL )
	{
		int blockmap_node;

		AddToList(vlist, (void *)bm->m_actual);

		BuildInvoke(g, bm->m_actual);
		BuildMPWait(g, bm->m_actual);

		blockmap_node = BuildBlockMap(g, bm, bm->m_actual, &extra, 1);
		if (blockmap_node == -1) {
			//DestroyBlockMap(bm);
			return -1;
		}
		if ( v2 == (list_t *)NULL && v1 == (list_t *)NULL ) {
			cmp1 = blockmap_node;
			cmp2 = o1;
			EDGE(g, cmp1, i1, 0);
		}
		else
			cmp2 = blockmap_node;
	}

	GetGraphVar(GRAPH_DFMODE, (int *)&dfmode);
	GetGraphVar(GRAPH_FORCE, (int *)&force);

	if (p2pmode && (bm != (g_block_map_t *)NULL) && (bm2 != (g_block_map_t *)NULL))
	{
		int receiver_cmp;
		int proc_set1, proc_set2;
		graph_inv * gi;

		// work out whic cmp is the receiver_cmp
		receiver_cmp = _identify_p2p_receiver_procset(g, tmp_v, bm2->m_actual);
		receiver_cmp = receiver_cmp? cmp1:cmp2;

		// need to get the PROC_SET for both programs
		gi = FindGraphInv(g, SymLookup(tmp_v->v_isym));
		proc_set1 = PROC_PROC_NODE(gi->gi_procset);

		gi = FindGraphInv(g, SymLookup(bm2->m_actual->v_isym));
		proc_set2 = PROC_PROC_NODE(gi->gi_procset);

		cmp = BuildDisplayP2P(g, id, receiver_cmp, proc_set1, proc_set2);
	}
	else
		cmp = BuildCmp(g, id, cmp1, cmp2, cmpop);


#ifdef DEBUG
	DebugPrintGraph(g);
#endif

	async = AsyncMode(g, vlist, dfmode);

	SetList(vlist);
	while ( (v = (var_t *)GetListElement(vlist)) != (var_t *)NULL )
	{
		//if ( async )
		//	cmp = v->v_value(v->v_proc->p_pnum);
			
		BuildMerge(g, v, cmp, async, force);
	}

	DPRINT("BuildAssert: Finished building assertion for assert id %d\n", iAssertID);
	
	return 0;
}

/*
** Determine mode of graph.
*/
int
AsyncMode(graph_t *g, list_t *vlist, dfmode_t dfmode)
{
	var_t *		v;
	graph_inv *	gi=NULL;
	
	DPRINT("In AsyncMode\n");

	if ( dfmode != DF_DEFAULT )
		return dfmode == DF_ASYNC;

	SetList(vlist);

	while ( (v = (var_t *)GetListElement(vlist)) != (var_t *)NULL )
	{

		if ( gi == (graph_inv *)NULL )
		{
			gi = FindGraphInv(g, SymLookup(v->v_isym));
			continue;
		}

		if ( gi != FindGraphInv(g, SymLookup(v->v_isym)) )
			return 0;
	}

	return 1;
}
