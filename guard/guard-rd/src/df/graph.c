/*
** Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
** See COPYRIGHT in top-level directory.
**
** Graph routines
**
*/

#ifdef HAVE_CONFIG_H
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#include	<stdio.h>
#include	<stdlib.h>

#include	"client.h"
#include	"defs.h"
#include	"bitset.h"
#include	"useful.h"
#include	"df.h"

graph_procset *	NewGraphProcSet(graph_t *, invoke_group *, int);

extern int		NodeEvent(dbgevent_t *, bitset*, bitset*, void *);

/*
** Create a new graph.
*/
graph_t *	
NewGraph(int (*compare)(graph_t *, int, char *, AIF *, char *, AIF *, int), void (*out)(char *, int, int, void *),
		int (*display_p2p)(graph_t *, int, char *, AIF *), int qlen)
{
	graph_t *	g;

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
	g->g_procs = NewList();
	g->g_invoke_list = NewList();
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

graph_inv *
NewGraphInv(graph_t *g, invoke_group *ig)
//TODO: add var_t in the signature
{
	int		c;
	graph_inv *	gi;

	gi = (graph_inv *)Alloc(sizeof(graph_inv));
	gi->gi_inv = ig;
	gi->gi_nproc = ig->ig_nproc;
	gi->gi_builtinv = 0;
	gi->gi_builtunused = 0;

	for ( c = 0 ; c < INVOKE_NODES ; c++ )
		gi->gi_node[c] = -1;

	gi->gi_procset = NewGraphProcSet(g, ig, ig->ig_nproc);

	HashInsert(g->g_inv, SymLookup(ig->ig_sym), (void *)gi);

	if(InList(g->g_invoke_list, ig->ig_invoke) == 0)
		AddToList(g->g_invoke_list, ig->ig_invoke);

	return gi;
}

graph_procset*
NewGraphProcSet(graph_t *g, invoke_group *ig, int nproc)
{
	int		d;
	graph_procset *	gp;

	gp = (graph_procset *)Alloc(sizeof(graph_procset));
	gp->gp_iproc = ig->ig_invoke;
	gp->gp_builtinv = 0;
	gp->gp_built = 0;
	gp->gp_files = (list_t *)NULL;

	gp->gp_builtinv_bs = bitset_new(nproc); //TODO: fix me
	gp->gp_built_bs = bitset_new(nproc);	//TODO: fix me 

	for ( d = 0 ; d < INVOKE_NODES ; d++ )
		gp->gp_node[d] = -1;

	return gp;
}

graph_diff *
NewGraphDiff(graph_t *g, int id, AIF *res, char *stat)
{
	graph_diff *	gd;
	char *			res_name = NULL;

	// create the graph_diff object
	gd = (graph_diff *)Alloc(sizeof(graph_diff));
	
	// create the name
	Asprintf(&res_name, "AssertID %d %s", id+1, stat);
	
	// set the struct members
	gd->gd_assert_id = id;
	gd->gd_name = res_name;
	gd->gd_val = res;
	
	// add it to the g_diff list
	AddToList(g->g_diff, (void *)gd);
	
	return gd;
}

void
AddActiveEvent(graph_t *g)
{
	g->g_activeEvents++;
}

void
RemoveActiveEvent(graph_t *g)
{
	g->g_activeEvents--;
}

int
ActiveEvents(graph_t *g)
{
	return g->g_activeEvents > 0;
}

graph_inv *
FindGraphInv(graph_t *g, char *inv)
{
	hentry_t *	h;

	if ( g->g_inv == (htable_t *)NULL )
		return (graph_inv *)NULL;

	if ( (h = HashSearch(g->g_inv, inv)) == (hentry_t *)NULL )
		return (graph_inv *)NULL;

	return (graph_inv *)h->h_data;
}

void
destroyGProcset(graph_procset *p)
{
	DestroyList(p->gp_files, DestroyFile);

	if(p->gp_builtinv_bs != NULL)
		bitset_free(p->gp_builtinv_bs);

	if(p->gp_built_bs != NULL)
		bitset_free(p->gp_built_bs);

	Free(p);
}

void
destroyInv(char *key, void *arg)
{
	graph_inv *	gi = (graph_inv *)arg;

	destroyGProcset(gi->gi_procset);
	
	Free(gi);
}

void
destroyGDiff(graph_diff *gd)
{
	if (gd->gd_name != (char *)NULL)
		Free(gd->gd_name);
	if (gd->gd_val != (AIF *)NULL)
		AIFFree(gd->gd_val);

	Free(gd);
}

/*
** Deallocate a graph.
*/
void		
DestroyGraph(graph_t *g)
{
	if ( g == (graph_t *)NULL )
		return;

	DestroyNodes(g->g_nodes);
	HashDestroy(g->g_inv, destroyInv);
	DestroyList(g->g_diff, destroyGDiff);
	DestroyList(g->g_procs, (void (*)())NULL);
	DestroyList(g->g_invoke_list, (void (*)())NULL);
	DestroyList(g->g_assigns, DestroyAssign);
	DestroyList(g->g_vars, DestroyVar);
	DestroyList(g->g_assert_cmds, Free);
	DestroyAssertInfo(g->g_assert_info_tab, g->g_assertions);
	
	if (g->g_assert_pass != NULL)
		Free(g->g_assert_pass);
	if (g->g_assert_warn != NULL)
		Free(g->g_assert_warn);
	if (g->g_assert_fail != NULL)
		Free(g->g_assert_fail);

	while ( g->g_tokens != (token_t *)NULL )
		DestroyToken(g, g->g_tokens);

	Free(g);
}

/*
** Reset a graph. This involves:
** - removing tokens from the inputs of all nodes
** - removing all nodes from the active list
** - recreating start token
*/
void		
ResetGraph(graph_t *g)
{
	node_t *	n;

	if ( g->g_reset )
		return;

	for ( n = g->g_nodes ; n != NULL ; n = n->n_next )
		ResetInputs(g, n);

	g->g_active = (node_t *)NULL;
	g->g_currActive = (node_t *)NULL;
	g->g_lastActive = (node_t *)NULL;
	g->g_activeEvents = 0;

	g->g_setStartNode(g, g->g_startnode);

	g->g_reset = 1;
}


void
DebugPrintGraph(graph_t* g)
{
	static int	print_index = 0;
	char		print_name[128];
	FILE * 		file = Client.cli_out_stream;

	sprintf(print_name, "graph-%d.dot", print_index++);
	file = fopen(print_name, "w");
	PrintGraph(file, g);
}
/*
** Print a graph.
*/
void
PrintGraph(FILE *fp, graph_t *g)
{
	int			i;
	node_t *	n;
	output_t *	o;
	token_t *	t;

	PrintToFd(fp, "digraph a {\n");

	for ( n = g->g_nodes ; n != (node_t *)NULL ; n = n->n_next )
	{
		PrintToFd(fp, "%d [", n->n_id);

		if ( (t = n->n_store) != (token_t *)NULL )
		{
			switch ( t->t_type )
			{
			case TK_STR:
				PrintToFd(fp, " label=\"%s\"", t->t_str);
				break;

			case TK_INT:
				PrintToFd(fp, " label=%d", t->t_int);
				break;

			case TK_BIT_SET:
				PrintToFd(fp, " label=\"%s\"", bitset_to_set(t->t_bit_set));
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
		}
		else
		{
			PrintToFd(fp, " label=\"%s(%d)\"", n->n_def->nd_name, n->n_id);
			PrintToFd(fp, " shape=box ]\n");
		}

		for ( i = 0 ; i < n->n_nout ; i++ )
		{
			o = &n->n_outputs[i];
			PrintToFd(fp, "%d -> %d\n", n->n_id, o->o_node->n_id);
		}
	}

	PrintToFd(fp, "}\n");
}
