/*********************************************************************************\
 * Dataflow interpreter.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * © 2010-2019 Cray Inc.  All Rights Reserved.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
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
static list_t *_evhandlers;
static list_t *_iohandlers;
static graph_t *_active_graph = NULL;

static void _continue(dbginvoke_t *);
static void _halt(dbginvoke_t *, int);
static void _delete_bp(graph_t *, dbginvoke_t *);
void DFDisplayMsg(char *);

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
            _active_graph->g_valname);
        return;
    }

    // get the value of the dfblock, if this is true, we need to mask input
    GetGraphVar(GRAPH_DFBLOCK, &block);
    if (block) DbgMaskInput();

    ResetGraph(g);

    /*
    ** Set event and i/o handlers
    */
    _evhandlers = NewList();
    _iohandlers = NewList();

    SetList(g->g_di_list);
    while ((di = (dbginvoke_t *)GetListElement(g->g_di_list)) != NULL) {
        // Save the old callback
        AddToList(_evhandlers, (void *)di->d_clnt_ev_callback);
        // Set the new callback
        DbgHandleEvent(di, g->g_dbgevent);
    }

    _active_graph = g;
    _active_graph->g_isactive = 1;
    _is_running = 1;

    DFMsg("*** Starting execution of application ***");

    // If XML mode is enabled, we need to update the status information to the
    // xml output. We always do this when starting or stopping a graph.
    if (Client.XML_mode) {
        // used for checking status
        char *bitset_str;
        char *set_str;

        // Need to print graph start for every invoke
        SetList(g->g_di_list);

        while ((di = (dbginvoke_t *)GetListElement(g->g_di_list)) != NULL) {
            bitset_str = bitset_to_set(di->d_bitset_full);

            // Create the full set string
            Asprintf(&set_str, "%s%s", di->d_handle, bitset_str);

            // Done with the bitset_str
            Free(bitset_str);

            // We want to create a new special status message indicating that
            // we are starting a graph.
            XMLPrintStartGraphMsg(di->d_handle, set_str);

            // Done with the set_str
            Free(set_str);
        }
    }

    // if PERFLOG is turned on, we need to create a string to the location to
    // create the log file.
#ifdef PERFLOG
    char *envDir;
    char logfile[PATH_MAX];

    if ((envDir = getenv(DBG_LOG_ENV_VAR)) == (char *)NULL) {
        // user didn't set the environment variable, so lets
        // read the HOME variable
        if ((envDir = getenv("HOME")) == (char *)NULL) {
            // couldn't read HOME, so lets default to tmp
            envDir = "/tmp";
        }
    }

    // create the fullpath string to the log file using PATH_MAX plus a null
    // term
    snprintf(logfile, PATH_MAX + 1, "%s/ccdb_perf_df.log", envDir);

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
int DispatchDF(void) {
    int out_of_nodes = 0;
    node_t *n = NULL;

    DPRINT("*** In DispatchDF\n");

    while (!out_of_nodes) {
        if (!_is_running || _active_graph == (graph_t *)NULL ||
            !_active_graph->g_isactive ||
            ((n = GetActiveNode(_active_graph)) == (node_t *)NULL &&
             !ActiveEvents(_active_graph))) {
            if (_active_graph != (graph_t *)NULL) {
                _active_graph->g_isactive = 0;
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
        DFMsg(XML_RESULT, g->g_valname, 0, g->g_pass, g->g_warn, g->g_fail);
    }

    DFMsg("\n*** The interpreter has halted. ***");
    DFMsg("Assertion script $%s complete.", g->g_valname);
    DFMsg("Successful Assertion Set Iterations: %d",
          g->g_assertions ? ((g->g_iterations - g->g_fail) / g->g_assertions)
                          : 0);
    DFMsg("Total Passed Assertions: %d", g->g_pass);
    DFMsg("Total Warned Assertions: %d", g->g_warn);
    DFMsg("Total Failed Assertions: %d", g->g_fail);
    DFMsg("\nAssertion summary:");
    for (id = 0; id < g->g_assertions; ++id) {
        // remember that the assert id we display to the user is the id offset
        // plus one.
        DFMsg("AssertID %d:  Pass: %d Warn: %d Fail: %d", id + 1,
              g->g_assert_pass[id], g->g_assert_warn[id], g->g_assert_fail[id]);
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
    SetList(g->g_di_list);
    while ((di = (dbginvoke_t *)GetListElement(g->g_di_list)) != NULL) {
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
    SetList(_evhandlers);
    SetList(_iohandlers);

    SetList(g->g_di_list);
    while ((di = (dbginvoke_t *)GetListElement(g->g_di_list)) != NULL) {
        DbgHandleEvent(
            di, (int (*)(event_set *, bitset *))GetListElement(_evhandlers));
    }

    DestroyList(_evhandlers, NULL);
    DestroyList(_iohandlers, NULL);

    if (!halt) {
        SetList(g->g_di_list);

        while ((di = (dbginvoke_t *)GetListElement(g->g_di_list)) != NULL) {
            _continue(di);
        }
    }

    // If XML mode is enabled, we need to update the status information to the
    // xml output. We always do this when starting or stopping a graph.
    if (Client.XML_mode) {
        // used for checking status
        rec_set_t *rec_set;
        rep_set_t *rep_set;
        group_reply_t *reply;
        char *bitset_str;
        char *set_str;
        dbgstatus_t *s;
        char *status_block;

        // Need to check status of every invoke
        SetList(g->g_di_list);

        while ((di = (dbginvoke_t *)GetListElement(g->g_di_list)) != NULL) {
            // create the rec_set
            rec_set = NewRecSet();
            AddReceiverToSet(rec_set, NewGroupReceiver(di, di->d_bitset_full));

            // Query the new status from the backends, note that for this we
            // want to grab the location information
            rep_set = DbgStatus(rec_set, 1);

            ProcessErrorInRepSet(rep_set);

            // Start a new status block
            // Note that this is okay since we are only processing a single
            // di in the rec_set at a time.
            status_block = XMLStartStatusBlock(di->d_handle);

            // Process each status message and add it to the status block
            while ((reply = GetNextReplyInSet(rep_set)) != NULL) {
                // First create the bitset string
                bitset_str = bitset_to_set(reply->rep_group);

                // Next create the full set string
                Asprintf(&set_str, "%s%s", di->d_handle, bitset_str);

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
    bp_set_t *bpset;
    group_reply_t *bp_reply;
    rec_set_t *rec_set;
    rep_set_t *rep_set;

    // create the rec_set
    rec_set = NewRecSet();
    AddReceiverToSet(rec_set, NewGroupReceiver(di, di->d_bitset_full));

    SetList(g->g_bp_reply_list);
    while ((bp_reply = (group_reply_t *)GetListElement(g->g_bp_reply_list)) !=
           NULL) {
        if (bp_reply->rep_di != di) {
            continue;
        }
        bp = EVENT_BP(bp_reply->rep_event);

        // Check to see if this di has this breakpoint
        if ((bpset = FindBPSetForBP(di, bp)) != NULL) {
            // The dbg_client layer handles all the of the details here
            if ((rep_set = DbgDeleteBreak(rec_set, bpset->b_bp_id)) != NULL) {
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
    group_reply_t *reply;
    dbgloc_t *halt_loc;
    char *set_info;

    // create the rec_set
    rec_set = NewRecSet();
    AddReceiverToSet(rec_set, NewGroupReceiver(di, di->d_bitset_full));

    rep_set = DbgInterrupt(rec_set);

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
            set_info = bitset_to_set(di->d_bitset_full);

            DFMsg("%s%s: Application halted at multiple locations.",
                  di->d_handle, set_info);
        } else {
            while ((reply = GetNextReplyInSet(rep_set)) != NULL) {
                set_info = bitset_to_set(reply->rep_group);
                // point at the location data
                halt_loc = EVENT_LOC(reply->rep_event);

                if (halt_loc->loc_line > 0) {
                    DFMsg("%s%s: Application halted in %s at %s:%d",
                          reply->rep_di->d_handle, set_info, halt_loc->loc_func,
                          halt_loc->loc_file, halt_loc->loc_line);
                } else {
                    DFMsg("%s%s: Application halted in %s",
                          reply->rep_di->d_handle, set_info,
                          halt_loc->loc_func);
                }

                Free(set_info);
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
    AddReceiverToSet(rec_set, NewGroupReceiver(di, di->d_bitset_full));

    if ((rep_set = DbgGo(rec_set)) != NULL) {
        ProcessErrorInRepSet(rep_set);
        FreeRepSet(rep_set);
    }

    FreeRecSet(rec_set);
}
