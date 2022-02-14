/*********************************************************************************\
 * Dataflow interpreter.
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

#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "bitset.h"
#include "client.h"
#include "dbg_client.h"
#include "dbgbp.h"
#include "defs.h"
#include "df.h"
#include "gram.h"
#include "useful.h"

static int _is_running;
static std::list<clnt_ev_callback_func> _evhandlers;
static graph_t *_active_graph = NULL;
static int old_print_elements;

static void _continue(dbginvoke_t *);
static void _halt(dbginvoke_t *, int);
static void _delete_bp(graph_t *, dbginvoke_t *);
void DFDisplayMsg(char *);

static rec_set_t* NewRecSetFromGraph(graph_t *g);

/*
** Initialise the dataflow interpreter.
*/
void InitDF(void) {
    InitNodeDef();
    _is_running = 0;
}

/*
** Finalise the dataflow interpreter.
*/
void FinishDF(void) {
    _is_running = 0;
    FinishNodeDef();
}

/*
** Set the graph ready to be run and establish the appropriate
** event handlers to deal with events during the graph operation.
*/
void StartDF(graph_t *g) {
    dbginvoke_t *di;
    int block = 0;

    // ensure only one graph is active at a time
    if (_active_graph != NULL) {
        DFErrMsg(
            "ERROR: Graph %s is already active! Stop the active graph or wait "
            "for completion.\n",
            _active_graph->name().c_str());
        return;
    }

    // get the value of the dfblock, if this is true, we need to mask input
    GetGraphVar(GRAPH_DFBLOCK, &block);
    if (block) DbgMaskInput();

    g->reset();

    /*
    ** Set event and i/o handlers
    */

    SetList(g->diList());
    while ((di = (dbginvoke_t *)GetListElement(g->diList())) != NULL) {
        // Save the old callback
        _evhandlers.push_back(di->d_clnt_ev_callback);
        // Set the new callback
        DbgHandleEvent(di, g->g_dbgevent);
    }

    _active_graph = g;
    _active_graph->setActive(true);
    _is_running = 1;

    // FIXME: this is an inefficient way of accomplishing this.
    // since the user can change print elements in the dbgsrv, make sure the
    // setting is 0 when the assertion script is active
    rec_set_t *rec_set = NewRecSetFromGraph(g);
    old_print_elements = Client.print_elements;
    ClientSetPrintElements(0, rec_set);
    FreeRecSet(rec_set);

    DFMsg("*** Starting execution of application ***");

    // If XML mode is enabled, we need to update the status information to the
    // xml output. We always do this when starting or stopping a graph.
    if (Client.XML_mode) {
        // used for checking status
        char *set_str;

        // Need to print graph start for every invoke
        SetList(g->diList());

        while ((di = (dbginvoke_t *)GetListElement(g->diList())) != NULL) {
            auto bitset_str = di->d_bitset_full->toSetString();

            // Create the full set string
            Asprintf(&set_str, "%s%s", di->d_handle, bitset_str.c_str());

            // We want to create a new special status message indicating that
            // we are starting a graph.
            XMLPrintStartGraphMsg(di->d_handle, set_str);

            // Done with the set_str
            Free(set_str);
        }
    }
}

/*
** Process an active node if available. Returns 1 if the graph is
** still active otherwise it returns 0 and raises the client event.
** The return value can be used to process the graph using a simple
** loop.
*/
int DispatchDF(void) {
    int out_of_nodes = 0;
    node_t *n = NULL;

    DPRINT("*** In DispatchDF\n");

    while (!out_of_nodes) {
        if (!_is_running || _active_graph == (graph_t *)NULL ||
            !_active_graph->isActive() ||
            ((n = GetActiveNode(_active_graph)) == (node_t *)NULL &&
             !_active_graph->hasActiveEvents())) {
            if (_active_graph != (graph_t *)NULL) {
                _active_graph->setActive(false);
                _is_running = 0;
            }

            return 0;
        }

        if (n != (node_t *)NULL && CheckIfQueueFull(_active_graph, n) &&
            CheckInputs(n)) {
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
void EndActiveDF(void) {
    if (_active_graph == NULL) return;

    EndDF(_active_graph);
    _active_graph = (graph_t *)NULL;
}

/*
** Called when the graph terminates to clean up the state of the
** interpreter, halt any executing processes and restore event
** handlers.
*/
void EndDF(graph_t *g) {
    int halt, id;
    int block = 0;
    dbginvoke_t *di;

    DPERFLOG(SetTimerStopAndLog("Graph total"));

    // If XML mode is enabled, print out the final result tag with id equal to 0
    if (Client.XML_mode) {
        DFMsg(XML_RESULT, g->name().c_str(), 0, g->numPasses(), g->numWarnings(), g->numFailures());
    }

    DFMsg("\n*** The interpreter has halted. ***");
    DFMsg("Assertion script $%s complete.", g->name().c_str());
    DFMsg("Successful Assertion Set Iterations: %d",
          g->numAssertions() ? ((g->iterations() - g->numFailures()) / g->numAssertions())
                          : 0);
    DFMsg("Total Passed Assertions: %d", g->numPasses());
    DFMsg("Total Warned Assertions: %d", g->numWarnings());
    DFMsg("Total Failed Assertions: %d", g->numFailures());
    DFMsg("\nAssertion summary:");
    for (id = 0; id < g->numAssertions(); ++id) {
        // remember that the assert id we display to the user is the id offset
        // plus one.
        auto [pass, warn, fail] = g->getAssertResult(id);
        DFMsg("AssertID %d:  Pass: %d Warn: %d Fail: %d", id + 1, pass, warn, fail);
    }
    // Print a final newline
    DFMsg("***********************************\n");

    /*
    ** Check if we need to continue processes. The process is always halted
    ** so that we are able to delete the created breakpoints.
    */
    GetGraphVar(GRAPH_HALT, &halt);

    if (halt) DFMsg("Current location:");

    /*
    ** Remove any breakpoints set by the graph.
    */
    SetList(g->diList());
    while ((di = (dbginvoke_t *)GetListElement(g->diList())) != NULL) {
        // In order to delete breakpoints, we need to ensure the process has
        // halted first.
        _halt(di, halt);

        // delete the breakpoints set by graph
        _delete_bp(g, di);
    }

    if (halt) DFMsg("\n");

    /*
    ** Restore event and I/O handlers.
    */
    // XXX: Horrible half std::list half list_t dual iteration we're stuck with
    //      until g_di_list graduates to a std::list or similar...
    auto ev_it = _evhandlers.begin();

    SetList(g->diList());
    while ((di = (dbginvoke_t *)GetListElement(g->diList())) != NULL) {
        DbgHandleEvent(di, *ev_it++);
    }

    _evhandlers.clear();

    // FIXME: this is an inefficient way of accomplishing this.
    // set print elements back to Client setting
    rec_set_t *rec_set = NewRecSetFromGraph(g);
    ClientSetPrintElements(old_print_elements, rec_set);
    FreeRecSet(rec_set);

    if (!halt) {
        SetList(g->diList());

        while ((di = (dbginvoke_t *)GetListElement(g->diList())) != NULL) {
            _continue(di);
        }
    }

    // If XML mode is enabled, we need to update the status information to the
    // xml output. We always do this when starting or stopping a graph.
    if (Client.XML_mode) {
        // used for checking status
        rec_set_t *rec_set;
        rep_set_t *rep_set;
        char *set_str;
        dbgstatus_t *s;
        char *status_block;

        // Need to check status of every invoke
        SetList(g->diList());

        while ((di = (dbginvoke_t *)GetListElement(g->diList())) != NULL) {
            // create the rec_set
            rec_set = NewRecSet();
            DASSERT_NOT_NULL(di);
            AddReceiverToSet(rec_set, NewGroupReceiver(di, di->d_bitset_full.get()));

            // Query the new status from the backends, note that for this we
            // want to grab the location information
            rep_set = DbgStatus(rec_set, 1);

            ProcessErrorInRepSet(rep_set);

            // Start a new status block
            // Note that this is okay since we are only processing a single
            // di in the rec_set at a time.
            status_block = XMLStartStatusBlock(di->d_handle);

            // Process each status message and add it to the status block
            for (auto reply : rep_set) {
                // First create the bitset string
                auto bitset_str = reply->rep_group->toSetString();

                // Next create the full set string
                Asprintf(&set_str, "%s%s", di->d_handle, bitset_str.c_str());

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

    // get the value of the dfblock, if this is true, we need to unmask input
    // since we are done with the graph
    GetGraphVar(GRAPH_DFBLOCK, &block);
    if (block) DbgUnmaskInput();
}

/*
** Check if there is currently a active graph that needs handling. This can
** be used in conjunction with a check events function to determine if the
** df dispatch function needs to be called.
*/
int ActiveDF(void) { return (_active_graph != (graph_t *)NULL); }

/*
** Tell the interpreter to exit immediately.
*/
void ExitDF(void) { _is_running = 0; }

/*
** Return the active graph.
*/
graph_t *StatusDF(void) {
    if (_is_running && (_active_graph != (graph_t *)NULL)) {
        return _active_graph;
    } else {
        return (graph_t *)NULL;
    }
}

/*
** Display an error message identifying the offending node. The DFDisplayMsg()
** routine must be suppled by the caller in order to actually display the
** message. This routine will depend on the particular environment being used.
*/
void DFError(node_t *n, const char *err, ...) {
    int l;
    char *p;
    va_list ap;
    char buf[BUFSIZ];

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
void DFErrMsg(const char *fmt, ...) {
    va_list ap;
    char *buf;

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
void DFMsg(const char *fmt, ...) {
    va_list ap;
    char *buf;

    va_start(ap, fmt);
    vasprintf(&buf, fmt, ap);
    DFDisplayMsg(buf);
    va_end(ap);
    Free(buf);
}

/*
** Version of DFDisplayMsg() that uses the debugger stdout.
*/
void DFDisplayMsg(char *msg) { PrintOutLine("%s\n", msg); }

/*
** Internal helper routine to delete breakpoints.
*/
static void _delete_bp(graph_t *g, dbginvoke_t *di) {
    dbgbp_t *bp;
    group_reply_t *bp_reply;
    rec_set_t *rec_set;
    rep_set_t *rep_set;

    // create the rec_set
    rec_set = NewRecSet();
    DASSERT_NOT_NULL(di);
    AddReceiverToSet(rec_set, NewGroupReceiver(di, di->d_bitset_full.get()));

    SetList(g->replyList());
    while ((bp_reply = (group_reply_t *)GetListElement(g->replyList())) !=
           NULL) {
        if (bp_reply->rep_di != di) {
            continue;
        }
        bp = EVENT_BP(bp_reply->rep_event);

        // Check to see if this di has this breakpoint
        if (auto bpset = di->d_breakpoints.findByDbgBreakpoint(*bp)) {
            // The dbg_client layer handles all the of the details here
            if ((rep_set = DbgDeleteBreak(rec_set, bpset->id())) != NULL) {
                ProcessErrorInRepSet(rep_set);
                FreeRepSet(rep_set);
                DFErrMsg("ERROR: Could not delete graph breakpoint.");
            }
        }
    }

    FreeRecSet(rec_set);
}

/*
** Internal helper routine to halt a set.
*/
static void _halt(dbginvoke_t *di, int haltmode) {
    rec_set_t *rec_set;
    rep_set_t *rep_set;
    dbgloc_t *halt_loc;
    std::string set_info;

    // create the rec_set
    rec_set = NewRecSet();
    DASSERT_NOT_NULL(di);
    AddReceiverToSet(rec_set, NewGroupReceiver(di, di->d_bitset_full.get()));

    rep_set = DbgInterrupt(rec_set,true);

    // We only want to alert the user that the application has halted if
    // haltmode is true
    if (haltmode) {
        // process error
        ProcessErrorInRepSet(rep_set);

        // process location

        // only print out the info if we have less than 3 replies!!!
        // Note that this is okay since we are only processing a single di in
        // the rec_set.
        // FIXME: Let this be user defined
        if (GetNumberOfReply(rep_set) > 3) {
            set_info = di->d_bitset_full->toSetString();

            DFMsg("%s%s: Application halted at multiple locations.",
                  di->d_handle, set_info.c_str());
        } else {
            for (auto reply : rep_set) {
                set_info = reply->rep_group->toSetString();
                // point at the location data
                halt_loc = EVENT_LOC(reply->rep_event);

                if (halt_loc->loc_line > 0) {
                    DFMsg("%s%s: Application halted in %s at %s:%d",
                          reply->rep_di->d_handle, set_info.c_str(), halt_loc->loc_func,
                          halt_loc->loc_file, halt_loc->loc_line);
                } else {
                    DFMsg("%s%s: Application halted in %s",
                          reply->rep_di->d_handle, set_info.c_str(),
                          halt_loc->loc_func);
                }
            }
        }
    }

    FreeRecSet(rec_set);
    FreeRepSet(rep_set);
}

/*
** Internal helper routine to continue a set.
*/
static void _continue(dbginvoke_t *di) {
    rec_set_t *rec_set;
    rep_set_t *rep_set;

    // create the rec_set
    rec_set = NewRecSet();
    DASSERT_NOT_NULL(di);
    AddReceiverToSet(rec_set, NewGroupReceiver(di, di->d_bitset_full.get()));

    if ((rep_set = DbgGo(rec_set,true)) != NULL) {
        ProcessErrorInRepSet(rep_set);
        FreeRepSet(rep_set);
    }

    FreeRecSet(rec_set);
}

// creates a rec_set_t from a graph so we can send commands to it.
// the rec_set_t needs to be freed by the caller of this function.
static rec_set_t* NewRecSetFromGraph(graph_t *g) {
    rec_set_t* rec_set = NewRecSet();

    SetList(g->diList());

    for (dbginvoke_t *di = (dbginvoke_t *)GetListElement(g->diList());
        di != nullptr; di = (dbginvoke_t *)GetListElement(g->diList())) {
        group_recv_t *recv = NewGroupReceiver(di, di->d_bitset_full.get());
        rec_set->addReceiver(recv);
    }

    return rec_set;
}
