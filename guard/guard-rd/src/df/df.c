/*
** Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
** See COPYRIGHT in top-level directory.
**
** Dataflow interpreter.
**
*/

#ifdef HAVE_CONFIG_H
#include	<config.h>
#endif /* HAVE_CONFIG_H */

#include	<unistd.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<stdarg.h>
#include	<string.h>
#include	<signal.h>

#include	"defs.h"
#include	"client.h"
#include	"useful.h"
#include	"dbgbp.h"
#include	"df.h"
#include	"dbg_client.h"
#include	"dbg_set_async.h"
#include	"gram.h"
#include	"bitset.h"

static int			_is_running;
static list_t *		_evhandlers;
static list_t *		_iohandlers;
static graph_t *	_active_graph = NULL;

static void	_continue(invoke_t *);
static void	_halt(invoke_t *, int);
static void _delete_bp(invoke_t *);
void		DFDisplayMsg(char *);

/*
** Initialise the dataflow interpreter.
*/
void
InitDF(void)
{
	InitNodeDef();
	_is_running = 0;
}

/*
** Finalise the dataflow interpreter.
*/
void
FinishDF(void)
{
	_is_running = 0;
	FinishNodeDef();
}

/*
** Set the graph ready to be run and establish the appropriate
** event handlers to deal with events during the graph operation.
*/
void
StartDF(graph_t *g)
{

	ResetGraph(g);
	
	// ensure only one graph is active at a time
	if (_active_graph != NULL)
	{
		DFErrMsg("ERROR: Graph %s is already active! Stop the active graph or wait for completion.\n", _active_graph->g_valname);
		return;
	}
	
	/*
	** Set event and i/o handlers
	*/
	_evhandlers = NewList();
	_iohandlers = NewList();

	invoke_t *invoke;
	SetList(g->g_invoke_list);
	while ( (invoke = (invoke_t *)GetListElement(g->g_invoke_list)) != (invoke_t *)NULL )
	{
		// Save the old callback
		AddToList(_evhandlers, (void *)((dbginvoke_t *)invoke->i_iexec)->d_clnt_ev_callback);
		// Set the new callback
		DbgHandleEvent(invoke->i_iexec, g->g_dbgevent);
	}

	_active_graph = g;
	_active_graph->g_isactive = 1;
	_is_running = 1;

	DFMsg("*** Starting execution of application ***");
	
	// If XML mode is enabled, we need to update the status information to the 
	// xml output. We always do this when starting or stopping a graph.
	if (Client.XML_mode)
	{
		// used for checking status
		invoke_group *		ig;
		dbgprocset_t *		ps;
		dbg_over_set_t *	over_set;
		char *				bitset_str;
		char *				set_str;
		
		// Need to print graph start for every invoke
		SetList(g->g_invoke_list);
		
		while ( (invoke = (invoke_t *)GetListElement(g->g_invoke_list)) != (invoke_t *)NULL )
		{
			SetInvokeGroup(invoke);
			
			while ((ig = GetInvokeGroup(invoke)) != (invoke_group *)NULL)
			{
				ps = FindProcSet(SymLookup(ig->ig_sym));
				over_set = GetOverlaySetForProcSet(ps);
				if (over_set == NULL)
				{
					// cannot generate xml output for this invoke group, continue
					continue;
				}
				
				// First create the bitset string
				if (over_set->s_offset >= 0 && over_set->s_len > 0)
				{
					bitset_str = bitset_to_set_by_offset(over_set->s_set_bs, over_set->s_offset, over_set->s_len);
				} else
				{
					bitset_str = bitset_to_set(over_set->s_set_bs);
				}
				
				// Next create the full set string
				Asprintf(&set_str, "%s%s", over_set->s_setname, bitset_str);
					
				// Done with the bitset_str
				Free(bitset_str);
				
				// We want to create a new special status message indicating that
				// we are starting a graph.
				XMLPrintStartGraphMsg(over_set->s_setname, set_str);
				
				// Done with the set_str
				Free(set_str);
			}
		}
	}
	
	// if PERFLOG is turned on, we need to create a string to the location to 
	// create the log file.
#ifdef PERFLOG
	char *envDir;
	char logfile[PATH_MAX];
	
	if ((envDir = getenv(DBG_LOG_ENV_VAR)) == (char *)NULL)
	{
		// user didn't set the environment variable, so lets
		// read the HOME variable
		if ((envDir = getenv("HOME")) == (char *)NULL)
		{
			// couldn't read HOME, so lets default to tmp
			envDir = "/tmp";
		}
	}
	
	// create the fullpath string to the log file using PATH_MAX plus a null term
	snprintf(logfile, PATH_MAX+1, "%s/ccdb_perf_df.log", envDir);
	
	DPERFLOG(CreateTimerLog(logfile));
	DPERFLOG(SetTimerStart("Graph total"));
#endif /* PERFLOG */
}

/*
** Process an active node if available. Returns 1 if the graph is
** still active otherwise it returns 0 and raises the client event.
** The return value can be used to process the graph using a simple
** loop.
*/
int
DispatchDF(void)
{
	int			out_of_nodes = 0;
	node_t *	n = NULL;
	
	DPRINT("*** In DispatchDF\n");

	while (!out_of_nodes)
	{
	
	if
	(
		!_is_running
		||
		_active_graph == (graph_t *)NULL
		||
		!_active_graph->g_isactive
		||
		(
			(n = GetActiveNode(_active_graph)) == (node_t *)NULL 
			&&
			!ActiveEvents(_active_graph)
		)
	)
	{
		if ( _active_graph != (graph_t *)NULL )
		{
			_active_graph->g_isactive = 0;
			_is_running = 0;
		}

		return 0;
	}

	if
	(
		n != (node_t *)NULL 
		&&
		CheckIfQueueFull(_active_graph, n)
		&&
		CheckInputs(n)
	)
	{
		(void)FireNode(_active_graph, n);
	} else
		++out_of_nodes;
		
	}
	
	DPRINT("*** Done with DispatchDF\n");

	return 1;
}

/*
** Called when the active graph terminates and the event handler wishes to
** clean up the active graph.
*/
void
EndActiveDF(void)
{
	if (_active_graph == NULL)
		return;
	
	EndDF(_active_graph);
	_active_graph = (graph_t *)NULL;
}

/*
** Called when the graph terminates to clean up the state of the
** interpreter, halt any executing processes and restore event
** handlers.
*/
void
EndDF(graph_t *g)
{
	int		halt, id;
	invoke_t *invoke;
	
	DPERFLOG(SetTimerStopAndLog("Graph total"));
	
	// If XML mode is enabled, print out the final result tag with id equal to 0
	if (Client.XML_mode)
	{
		DFMsg(XML_RESULT, g->g_valname, 0, g->g_pass, g->g_warn, g->g_fail);
	}
	
	DFMsg("\n*** The interpreter has halted. ***");
	DFMsg("Assertion script $%s complete.", g->g_valname);
	DFMsg("Successful Assertion Set Iterations: %d", ((g->g_iterations - g->g_fail)/g->g_assertions));
	DFMsg("Total Passed Assertions: %d", g->g_pass);
	DFMsg("Total Warned Assertions: %d", g->g_warn);
	DFMsg("Total Failed Assertions: %d", g->g_fail);
	DFMsg("\nAssertion summary:");
	for (id=0; id < g->g_assertions; ++id)
	{
		// remember that the assert id we display to the user is the id offset plus one.
		DFMsg("AssertID %d:  Pass: %d Warn: %d Fail: %d", id+1, g->g_assert_pass[id], g->g_assert_warn[id], g->g_assert_fail[id]);
	}
	// Print a final newline
	DFMsg("***********************************\n");
	
	/*
	** Check if we need to continue processes. The process is always halted
	** so that we are able to delete the created breakpoints.
	*/
	GetGraphVar(GRAPH_HALT, &halt);
	
	if (halt)
		DFMsg("Current location:");

	/*
	** Remove any breakpoints set by the graph.
	*/
	SetList(g->g_invoke_list);
	while ( (invoke = (invoke_t *)GetListElement(g->g_invoke_list)) != (invoke_t *)NULL )
	{
		// In order to delete breakpoints, we need to ensure the process has halted first.
		_halt(invoke, halt);
		
		// FIXME: This shouldn't delete all the breakpoints. Rather it should
		// only delete the breakpoints created by the graph. Currently there is
		// no way to keep track of this and is a serious flaw.
		_delete_bp(invoke);
		RemoveAllBPSet(invoke->i_tree);
	}
	
	if (halt)
		DFMsg("\n");

	/*
	** Restore event and I/O handlers.
	*/
	SetList(_evhandlers);
	SetList(_iohandlers);

	SetList(g->g_invoke_list);
	while ( (invoke = (invoke_t *)GetListElement(g->g_invoke_list)) != (invoke_t *)NULL )
	{
		DbgHandleEvent(invoke->i_iexec, GetListElement(_evhandlers));
	}

	DestroyList(_evhandlers, (void (*)())NULL);
	DestroyList(_iohandlers, (void (*)())NULL);

	if (!halt)
	{
		SetList(g->g_invoke_list);

		while ( (invoke = (invoke_t *)GetListElement(g->g_invoke_list)) != (invoke_t *)NULL )
		{
			_continue(invoke);
		}
	}
	
	// If XML mode is enabled, we need to update the status information to the 
	// xml output. We always do this when starting or stopping a graph.
	if (Client.XML_mode)
	{
		// used for checking status
		invoke_group *		ig;
		dbgprocset_t *		ps;
		dbg_over_set_t *	over_set;
		rec_set_t *			rec_set;
		rep_set_t *			rep_set;
		group_reply_t *		reply;
		char *				bitset_str;
		char *				set_str;
		dbgstatus_t *		s;
		char *				status_block;
		
		// Need to check status of every invoke
		SetList(g->g_invoke_list);
		
		while ( (invoke = (invoke_t *)GetListElement(g->g_invoke_list)) != (invoke_t *)NULL )
		{
			SetInvokeGroup(invoke);
			
			while ((ig = GetInvokeGroup(invoke)) != (invoke_group *)NULL)
			{
				ps = FindProcSet(SymLookup(ig->ig_sym));
				over_set = GetOverlaySetForProcSet(ps);
				if (over_set == NULL)
				{
					// cannot generate xml output for this invoke group, continue
					continue;
				}
				
				rec_set = NewRecSetByOverSet(over_set, over_set->s_set_bs);
				
				// Query the new status from the backends, note that for this we
				// want to grab the location information
				rep_set = DbgStatus(rec_set, 1);
			
				ProcessErrorInRepSet(rec_set, rep_set);
			
				// Start a new status block
				status_block = XMLStartStatusBlock(rec_set->s_setname);
			
				// Process each status message and add it to the status block
				while((reply = GetNextReplyInSet(rep_set)) != (group_reply_t*)NULL)
				{
					// First create the bitset string
					if (over_set->s_offset >= 0 && over_set->s_len > 0)
					{
						bitset_str = bitset_to_set_by_offset(reply->rep_group, over_set->s_offset, over_set->s_len);
					} else
					{
						bitset_str = bitset_to_set(reply->rep_group);
					}
					
					// Next create the full set string
					Asprintf(&set_str, "%s%s", rec_set->s_setname, bitset_str);
					
					// Done with the bitset_str
					Free(bitset_str);
				
					// point at the status data
					s = EVENT_STATUS(reply->rep_event);
				
					// The XML status that is printed is based on the status value
					XMLAddStatus(&status_block, set_str, s->stat, s->stat_loc);
					
					// Done with the set_str
					Free(set_str);
				}
			
				// Print the entire status block, this will free the string
				XMLPrintStatusBlock(status_block);
				
				// Free the reply set
				FreeRepSet(rep_set);
			
				// Done with the rec_set
				FreeRecSet(rec_set);
			}
		}
	}
}

/*
** Check if there is currently a active graph that needs handling. This can
** be used in conjunction with a check events function to determine if the
** df dispatch function needs to be called.
*/
int 
ActiveDF(void)
{
	return (_active_graph != (graph_t *)NULL);
}

/*
** Tell the interpreter to exit immediately.
*/
void
ExitDF(void)
{
	_is_running = 0;
}

/*
** Return the active graph.
*/
graph_t *
StatusDF(void)
{
	if( _is_running && (_active_graph != (graph_t *)NULL))
	{
		return _active_graph;
	} else
	{
		return (graph_t *)NULL;
	}
}

/*
** Display an error message identifying the offending node. The DFDisplayMsg() 
** routine must be suppled by the caller in order to actually display the 
** message. This routine will depend on the particular environment being used.
*/
void
DFError(node_t *n, char *err, ...)
{
	int	l;
	char *	p;
	va_list	ap;
	char	buf[BUFSIZ];

	p = buf;
	l = BUFSIZ;

	snprintf(p, l, "Error at node %s(%d): ", n->n_def->nd_name, n->n_id);
	p += strlen(buf);
	l -= strlen(buf);

	va_start(ap, err);
	vsnprintf(p, l, err, ap);
	va_end(ap);

	DFDisplayMsg(buf);

	ExitDF();
}

/*
** Display an general error message. DFDisplayMsg() must be supplied as
** for DFError().
*/
void
DFErrMsg(char *fmt, ...)
{
	va_list	ap;
	char *	buf;

	va_start(ap, fmt);
	vasprintf(&buf, fmt, ap);
	DFDisplayMsg(buf);
	va_end(ap);
	Free(buf);
}

/*
** Display an informative message. DFDisplayMsg() must be supplied as
** for DFError().
*/
void
DFMsg(char *fmt, ...)
{
	va_list	ap;
	char *	buf;

	va_start(ap, fmt);
	vasprintf(&buf, fmt, ap);
	DFDisplayMsg(buf);
	va_end(ap);
	Free(buf);
}

/*
** Version of DFDisplayMsg() that uses the debugger stdout.
*/
void
DFDisplayMsg(char *msg)
{
	PrintOutLine("%s\n", msg);
}

/*
** Internal helper routine to delete breakpoints.
*/
static void
_delete_bp(invoke_t *i)
{
	invoke_group *		ig;
	dbgprocset_t *		ps;
	dbg_over_set_t *	over_set;
	rec_set_t *			rec_set;
	bp_set_t *			bpset;
	int					bpset_id;
	bp_id_set_t *		id_set;
	rec_set_t *			bp_rec_set;
	rep_set_t *			rep_set;
	
	SetInvokeGroup(i);
	
	while ((ig = GetInvokeGroup(i)) != (invoke_group *)NULL)
	{
		ps = FindProcSet(SymLookup(ig->ig_sym));
		over_set = GetOverlaySetForProcSet(ps);
		if (over_set == NULL)
		{
			DFErrMsg("ERROR: _delete_bp() cannot get Overlay Set for [%s]!", SymLookup(ig->ig_sym));
			return;
		}
		
		rec_set = NewRecSetByOverSet(over_set, over_set->s_set_bs);
		
		SetBPSetHead(i->i_tree);
		
		while ((bpset = GetNextBPSet(i->i_tree, &bpset_id)) != (bp_set_t *)NULL)
		{
			SetList(bpset->b_bp_id_list);
			while ((id_set = (bp_id_set_t *)GetListElement(bpset->b_bp_id_list)) != (bp_id_set_t *)NULL)
			{
				bp_rec_set = NewRecSet( rec_set->s_setname, 
										rec_set->s_invoke_handler,
										rec_set->s_dbginvoke_handler,
										rec_set->s_proto_handler,
										id_set->b_group);
				if ((rep_set = DbgDeleteBreak(bp_rec_set, id_set->b_bp_id)) != NULL)
				{
					ProcessErrorInRepSet(bp_rec_set, rep_set);
					DFErrMsg("ERROR: Could not delete graph breakpoint.");
					FreeRepSet(rep_set);
					FreeRecSet(bp_rec_set);
					continue;
				}
				FreeRecSet(bp_rec_set);
			}
		}
		
		FreeRecSet(rec_set);
	}
}

/*
** Internal helper routine to halt a set.
*/
static void
_halt(invoke_t *i, int haltmode)
{
	invoke_group *		ig;
	dbgprocset_t *		ps;
	dbg_over_set_t *	over_set;
	rec_set_t *			rec_set;
	rep_set_t *			rep_set;
	group_reply_t *		reply;
	dbgloc_t *			halt_loc;
	char *				set_info;
	
	SetInvokeGroup(i);
	
	while ((ig = GetInvokeGroup(i)) != (invoke_group *)NULL)
	{
		ps = FindProcSet(SymLookup(ig->ig_sym));
		over_set = GetOverlaySetForProcSet(ps);
		if (over_set == NULL)
		{
			DFErrMsg("ERROR: _halt() cannot get Overlay Set for [%s]!", SymLookup(ig->ig_sym));
			return;
		}
		
		rec_set = NewRecSetByOverSet(over_set, over_set->s_set_bs);
		
		rep_set = DbgInterrupt(rec_set);
		
		// We only want to alert the user that the application has halted if 
		// haltmode is true
		if (haltmode)
		{
			//process error
			ProcessErrorInRepSet(rec_set, rep_set);
			
			//process location
			
			//only print out the info if we have less than 3 replies!!!
			// FIXME: Let this be user defined
			if (GetNumberOfReply(rep_set) > 3)
			{
				set_info = bitset_to_set(rec_set->s_receiver_bs);
				
				DFMsg("%s%s: Application halted at multiple locations.",
					rec_set->s_setname,
					set_info);
			} else
			{
				while((reply = GetNextReplyInSet(rep_set)) != (group_reply_t *)NULL)
				{
					set_info = bitset_to_set(reply->rep_group);
					// point at the location data
					halt_loc = EVENT_LOC(reply->rep_event);
				
					if (halt_loc->loc_line > 0)
					{
						DFMsg("%s%s: Application halted in %s at %s:%d",
							rec_set->s_setname,
							set_info,
							halt_loc->loc_func,
							halt_loc->loc_file,
							halt_loc->loc_line);
					} else
					{
						DFMsg("%s%s: Application halted in %s",
							rec_set->s_setname,
							set_info,
							halt_loc->loc_func);
					}
				
					Free(set_info);
				}
			}
		}
		
		FreeRecSet(rec_set);
		FreeRepSet(rep_set);
	}
}

/*
** Internal helper routine to continue a set.
*/
static void
_continue(invoke_t *i)
{
	invoke_group *		ig;
	dbgprocset_t *		ps;
	dbg_over_set_t *	over_set;
	rec_set_t *			rec_set;
	rep_set_t *			rep_set;
	
	SetInvokeGroup(i);
	
	while ((ig = GetInvokeGroup(i)) != (invoke_group *)NULL)
	{
		ps = FindProcSet(SymLookup(ig->ig_sym));
		over_set = GetOverlaySetForProcSet(ps);
		if (over_set == NULL)
		{
			DFErrMsg("ERROR: _continue() cannot get Overlay Set for [%s]!", SymLookup(ig->ig_sym));
			return;
		}
		
		rec_set = NewRecSetByOverSet(over_set, over_set->s_set_bs);
		
		if ((rep_set = DbgGo(rec_set)) != NULL)
		{
			ProcessErrorInRepSet(rec_set, rep_set);
			FreeRecSet(rec_set);
			FreeRepSet(rep_set);
			return;
		}
		
		FreeRecSet(rec_set);
	}
}

