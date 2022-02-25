/*********************************************************************************\
 * Graph commands.
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

#include <arpa/inet.h>
#include <math.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "bitset.h"
#include "client.h"
#include "df.h"
#include "useful.h"

#define GRAPH_TABLE_SIZE 100 /* number that can be active */

/*
** Static global variables
*/
static list_t *GraphList; /* List of defined graphs */
static cmdlist_t *GraphCmds;
static cmdlist_t *OldCmds;
static val_t *GraphVal;
static graph_t *GraphPtr;

/*
** static function prototypes
*/
static void GraphCleanup(int);
static int DisplayData(AIF *data);

/********************************
 * Info class commands
 ********************************/

static int class_info_subcmd_diff_cmd(rec_set_t *rec_set, int argc,
                                      char **argv) {
    graph_t *g;
    graph_t::graph_diff *gd;
    ct_val cv;
    const char *s;
    int num = 0, req = 0;
    char *gname;

    if (SizeOfList(GraphList) < 1) {
        PrintOutError("There are no asertion scripts defined.\n");
        return 1;
    }

    if (argc < 2) {
        PrintOutMsg("Saved difference values:\n");
        // loop through each graphptr
        SetList(GraphList);
        while ((g = (graph_t *)GetListElement(GraphList)) != (graph_t *)NULL) {
            PrintOutMsg("Script $%s\n", g->name().c_str());
            SetList(g->diffsList());
            while ((gd = (graph_t::graph_diff *)GetListElement(g->diffsList())) !=
                   (graph_t::graph_diff *)NULL) {
                PrintOutMsg("%d. %s\n", ++num, gd->gd_name.c_str());
            }
        }
        return 1;
    }

    // Figure out the type of the argument
    s = argv[1];
    switch (ScanString(&cv, &s)) {
        case CT_INT:
            req = cv.cv_int;
            // find the requested comparison result the slow way
            SetList(GraphList);
            while ((g = (graph_t *)GetListElement(GraphList)) !=
                   (graph_t *)NULL) {
                SetList(g->diffsList());
                while ((gd = (graph_t::graph_diff *)GetListElement(g->diffsList())) !=
                       (graph_t::graph_diff *)NULL) {
                    if (++num == req) {
                        PrintOutMsg("Script $%s %s\n", g->name().c_str(),
                                    gd->gd_name.c_str());
                        return DisplayData(gd->gd_val);
                    }
                }
            }
            // shouldn't get here
            PrintOutError(
                "Invalid difference value number. Try \"help info diff\" for "
                "more information.\n");
            return 0;

        case CT_DSYM:
            gname = SymLookup(cv.cv_sym);
            // loop through and find the graph associated with this name
            SetList(GraphList);
            while ((g = (graph_t *)GetListElement(GraphList)) !=
                   (graph_t *)NULL) {
                if (!strcmp(g->name().c_str(), gname)) break;
            }
            // Make sure we found the graph
            if (g == NULL) {
                PrintOutError("Provided script %s not found.\n", gname);
                return 0;
            }
            // Check to see if a AssertID was provided
            if (argc > 2) {
                s = argv[2];
                switch (ScanString(&cv, &s)) {
                    case CT_INT:
                        // Make sure to subtract one since we use offset 0
                        // internally
                        req = cv.cv_int - 1;
                        SetList(g->diffsList());
                        while ((gd = (graph_t::graph_diff *)GetListElement(g->diffsList())) !=
                               (graph_t::graph_diff *)NULL) {
                            if (gd->gd_assert_id == req) {
                                PrintOutMsg("Script $%s %s\n", g->name().c_str(),
                                            gd->gd_name.c_str());
                                if (!DisplayData(gd->gd_val)) {
                                    return 0;
                                }
                            }
                        }
                        // done
                        return 1;

                    default:
                        PrintOutError(
                            "Encountered unknown argument. Expected AssertID "
                            "number (i.e. 1)\n");
                        return 0;
                }
            }
            // no AssertID, so lets just print out every difference associated
            // with this graph
            SetList(g->diffsList());
            while ((gd = (graph_t::graph_diff *)GetListElement(g->diffsList())) !=
                   (graph_t::graph_diff *)NULL) {
                PrintOutMsg("Script $%s %s\n", g->name().c_str(), gd->gd_name.c_str());
                if (!DisplayData(gd->gd_val)) {
                    return 0;
                }
            }
            // done
            return 1;

        default:
            PrintOutError(
                "Encountered unknown argument. Expected $script_name or "
                "difference number (i.e. 1)\n");
            return 0;
    }

    // Shouldn't get here
    return 0;
}

static int class_info_subcmd_build_cmd(rec_set_t *rec_set, int argc,
                                       char **argv) {
    val_t *v;
    expr_t *e;
    graph_t *g;
    char *assert_cmd;
    int id = 1;

    if (argc < 2) {
        // display only summary info for all defined graphs
        SetList(GraphList);
        PrintOutMsg("\nDefined assertion scripts:\n");

        // loop through each graphptr
        while ((g = (graph_t *)GetListElement(GraphList)) != (graph_t *)NULL) {
            PrintOutMsg("Name: $%s\t\t\tTotal Assertions: %d\n", g->name().c_str(),
                        g->numAssertions());
        }

        PrintOutMsg(
            "\nTry \"info build $script_name\" for more information about a "
            "specific assertion script.\n\n");

        return 1;
    }

    // ignore all args except the first
    if ((e = ParseExpr(argv[1])) == (expr_t *)NULL) {
        PrintError();
        return 0;
    }

    if (EvalExpr(&v, e, rec_set) < 0) {
        PrintError();
        FreeExpr(e);
        return 0;
    }

    FreeExpr(e);

    if (RVAL_TYPE(v) != rval_graph) {
        PrintOutError("Unknown assertion script \"%s\".\n", argv[1]);
        return 0;
    }

    // assign the graph directly
    if ((g = RVAL_GRAPH(v)) == NULL) {
        PrintOutError("Internal error in info build cmd.\n");
        DestroyVal(v);
        return 0;
    }

    PrintOutMsg("Script $%s\n", g->name().c_str());

    // loop through the g_assert_cmd list
    SetList(g->assertsList());
    while ((assert_cmd = (char *)GetListElement(g->assertsList())) !=
           (char *)NULL) {
        PrintOutMsg("AssertID:%d  %s\n", id++, assert_cmd);
    }

    return 1;
}

static int class_info_subcmd_script_cmd(rec_set_t *rec_set, int argc,
                                        char **argv) {
    graph_t *g;

    if ((g = StatusDF()) == (graph_t *)NULL) {
        PrintOutMsg("There is currently no assertion script executing.\n");
    } else {
        PrintOutMsg("Active assertion script: %s\n", g->name().c_str());
        PrintOutMsg(
            "Completed Iterations: %d\n",
            (g->iterations() /
             g->numAssertions()));  // We want integer values only from division
        PrintOutMsg("Passed Cases: %d\n", g->numPasses());
        PrintOutMsg("Warned Cases: %d\n", g->numWarnings());
        PrintOutMsg("Failed Cases: %d\n", g->numFailures());
    }

    return 1;
}

/********************************
 * Set class commands
 ********************************/

static int class_set_subcmd_halt_cmd(rec_set_t *rec_set, int argc,
                                     char **argv) {
    const char *s;
    int halt_flag;
    ct_val cv;

    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: set halt [true|false]\n");
        return 0;
    }

    // only look at the first arg
    s = argv[1];

    switch (ScanString(&cv, &s)) {
        case CT_TRUE:
            halt_flag = 1;
            break;

        case CT_FALSE:
            halt_flag = 0;
            break;

        default:
            PrintOutError("Encountered something other than true or false.\n");
            PrintOutError("See \"help set halt\" for more information.\n");
            return 0;
    }

    SetGraphVar(GRAPH_HALT, VAR_INT, (void *)&halt_flag);

    return 1;
}

static int class_set_subcmd_stop_cmd(rec_set_t *rec_set, int argc,
                                     char **argv) {
    const char *s;
    int stop_flag;
    ct_val cv;

    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: set stop [true|false]\n");
        return 0;
    }

    // only look at the first arg
    s = argv[1];

    switch (ScanString(&cv, &s)) {
        case CT_TRUE:
            stop_flag = 1;
            break;

        case CT_FALSE:
            stop_flag = 0;
            break;

        default:
            PrintOutError("Encountered something other than true or false.\n");
            PrintOutError("See \"help set stop\" for more information.\n");
            return 0;
    }

    SetGraphVar(GRAPH_STOP, VAR_INT, (void *)&stop_flag);

    return 1;
}

static int class_set_subcmd_dfblock_cmd(rec_set_t *rec_set, int argc,
                                        char **argv) {
    const char *s;
    int b;
    ct_val cv;

    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: set dfblock [on|off]\n");
        return 0;
    }

    // only look at the first arg
    s = argv[1];

    switch (ScanString(&cv, &s)) {
        case CT_TRUE:
            b = 1;
            break;
        case CT_FALSE:
            b = 0;
            break;
        default:
            PrintOutError("Encountered something other than on or off.\n");
            PrintOutError("See \"help set dfblock\" for more information.\n");
            return 0;
    }

    SetGraphVar(GRAPH_DFBLOCK, VAR_INT, (void *)&b);

    return 1;
}

/********************************
 * Show class commands
 ********************************/

static int class_show_subcmd_halt_cmd(rec_set_t *rec_set, int argc,
                                      char **argv) {
    int halt;

    GetGraphVar(GRAPH_HALT, &halt);

    PrintOutMsg("Assertion script halting mode: %s\n", halt ? "true" : "false");

    return 1;
}

static int class_show_subcmd_stop_cmd(rec_set_t *rec_set, int argc,
                                      char **argv) {
    int stop;

    GetGraphVar(GRAPH_STOP, &stop);

    PrintOutMsg("Assertion script stopping mode: %s\n",
                stop ? "true" : "false");

    return 1;
}

static int class_show_subcmd_dfblock_cmd(rec_set_t *rec_set, int argc,
                                         char **argv) {
    int block;

    GetGraphVar(GRAPH_DFBLOCK, &block);

    PrintOutMsg("Assertion script dataflow command blocking: %s\n",
                block ? "on" : "off");

    return 1;
}

/********************************
 * Maint class commands
 ********************************/

static int class_maint_subcmd_dump_cmd(rec_set_t *rec_set, int argc,
                                       char **argv) {
    val_t *v;
    expr_t *e;
    graph_t *g;
    FILE *file = Client.cli_out_stream;

    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: dump $script_name\n");
        PrintOutError(
            "Where $script_name is the name of the assertion script to "
            "dump.\n");
        return 0;
    }

    // Ignore all args besides the first
    if ((e = ParseExpr(argv[1])) == (expr_t *)NULL) {
        PrintError();
        return 0;
    }

    if (EvalExpr(&v, e, rec_set) < 0) {
        PrintError();
        FreeExpr(e);
        return 0;
    }

    FreeExpr(e);

    if (RVAL_TYPE(v) != rval_graph) {
        PrintOutError("Unknown assertion script \"%s\".\n", argv[1]);
        return 0;
    }

    // assign the graph directly
    if ((g = RVAL_GRAPH(v)) == NULL) {
        PrintOutError("Internal error in maint dump cmd.\n");
        DestroyVal(v);
        return 0;
    }

    if (argc == 3) {
        file = fopen(argv[2], "w");

        if (file == NULL) {
            PrintOutError("Failed to open '%s' for writing\n", argv[2]);
            return 0;
        }
    }

    g->print(file);

    if (file != NULL && file != Client.cli_out_stream) fclose(file);

    return 1;
}

/********************************
 * Usage class commands
 ********************************/

static int class_usage_subcmd_assert_cmd(rec_set_t *rec_set, int argc,
                                         char **argv) {
    PrintHelpEntry((char **)cmd_graph_alt_assert_help);

    return 1;
}

/********************************
 * Regular commands
 ********************************/

static int cmd_build_cmd(rec_set_t *rec_set, int argc, char **argv) {
    val_t *v;
    expr_t *e;

    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: build $script_name\n");
        return 0;
    }

    // ignore all args except the first
    if ((e = ParseExpr(argv[1])) == (expr_t *)NULL) {
        PrintError();
        return 0;
    }

    if (EvalExpr(&v, e, rec_set) < 0) {
        PrintError();
        FreeExpr(e);
        return 0;
    }

    FreeExpr(e);

    if (LVAL_TYPE(v) != lval_internal) {
        PrintOutError(
            "A name must be specified for the assertion script. Names begin "
            "with a \'$\' character.\n");
        return 0;
    }

    switch (RVAL_ACTUAL_TYPE(v)) {
        case rval_graph:
            PrintOutError(
                "An assertion script has already been defined using this "
                "name.\n");
            return 0;
        case rval_proc:
            PrintOutError(
                "A process set has already been defined using this name.\n");
            return 0;
        case rval_blockmap:
            PrintOutError(
                "A decomposition has already been defined using this name.\n");
            return 0;
        case rval_aif:
            PrintOutError(
                "A debugger variable has already been defined using this "
                "name.\n");
            return 0;
        default:
            break;
    }

    GraphVal = v;
    // TODO use smart pointer
    GraphPtr = new graph_t;
    OldCmds = SetCommands(GraphCmds);

    cli_inc_prompt();

    /*
    ** Start building graph.
    */
    GraphPtr->setStartNode(GraphPtr->outConstStr(Client.proto));
    GraphPtr->setInitNode(GraphPtr->outNode("DBGINIT"));
    GraphPtr->outEdge(GraphPtr->startNode(), GraphPtr->initNode(), 0);

    return 1;
}

static int cmd_start_cmd(rec_set_t *rec_set, int argc, char **argv) {
    val_t *v;
    expr_t *e;
    graph_t *g;

    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: start $script_name\n");
        PrintOutError(
            "Where $script_name is the name of the assertion script to start "
            "executing.\n");
        return 0;
    }

    // Ignore all arguments except the first
    if ((e = ParseExpr(argv[1])) == (expr_t *)NULL) {
        PrintError();
        return 0;
    }

    if (EvalExpr(&v, e, rec_set) < 0) {
        PrintError();
        FreeExpr(e);
        return 0;
    }

    FreeExpr(e);

    if (RVAL_TYPE(v) != rval_graph) {
        PrintOutError("Unknown assertion script \"%s\".\n", argv[1]);
        DestroyVal(v);
        return 0;
    }

    // assign the graph directly
    if ((g = RVAL_GRAPH(v)) == NULL) {
        PrintOutError("Internal error in start cmd.\n");
        DestroyVal(v);
        return 0;
    }

    StartDF(g);

    return 1;
}

static int cmd_stop_cmd(rec_set_t *rec_set, int argc, char **argv) {
    graph_t *g;

    if ((g = StatusDF()) == (graph_t *)NULL) {
        PrintOutMsg("There is currently no assertion script executing.\n");
    } else {
        ExitDF();
    }

    return 1;
}

// FIXME: This function doesn't work anymore, its dead code.
#if 0
static int 
cmd_restart_cmd(rec_set_t *rec_set, int argc, char **argv)
{

	val_t *		v;
	expr_t *	e;
	graph_t *	g;
	char		buf[BUFSIZ];

	if ( argc < 2 )
	{
		PrintOutError("Not enough command arguments.\n");
		PrintOutError("Expected: graph $graph_name\n");
		PrintOutError("Where $graph_name is the name of the graph to restart.\n");
		return 0;
	}

	ArgsToBuf(&argv[1], argc-1, buf, BUFSIZ);

	if ( (e = ParseExpr(buf)) == (expr_t *)NULL )
	{
		PrintError();
		return 0;
	}

	if ( EvalExpr(&v, e, rec_set) < 0 )
	{
		PrintError();
		FreeExpr(e);
		return 0;
	}

	FreeExpr(e);

	if ( RVAL_TYPE(v) != rval_graph )
	{
		PrintOutError("Unknown graph \"%s\".\n", buf);
		DestroyVal(v);
		return 0;
	}
	
	// assign the graph directly
	if ((g = RVAL_GRAPH(v)) == NULL)
	{
		PrintOutError("Internal error in restart cmd.\n");
		DestroyVal(v);
		return 0;
	}

	/*
	** Kill any pending processes.
	*/

//TODO: fixme
/*	SetList(g->g_procs);

	while ( (p = (invoke_proc_t *)GetListElement(g->g_procs)) != (invoke_proc_t *)NULL )
		(void)DbgKill(p->p_exec);
*/
	StartDF(g, _graph_cleanup);

	while ( DispatchDF() )
		WaitForEvents(0);

	EndDF(g);

	return 1;
}
#endif

/********************************
 * Alt commands
 ********************************/

static int cmd_graph_alt_assert_cmd(rec_set_t *rec_set, int argc, char **argv) {
    char *buf;
    list_t *vlist;
    expr_t *e;
    graph_t::ainfo_t *ai;

    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError(
            "Expected: assert $proc1::[var]@\"file\":[line] = "
            "$proc2::[var]@\"file\":[line]\n");
        return 0;
    }

    buf = ArgvStream(&argv[1]).createBuffer(0, argc - 1);

    if ((e = ParseExpr(buf)) == (expr_t *)NULL) {
        PrintError();
        Free(buf);
        return 0;
    }

    vlist = NewList();

    if (BuildAssert(GraphPtr, GraphPtr->numAssertions(), e, rec_set, vlist) < 0) {
        PrintError();
        FreeExpr(e);
        Free(buf);
        return 0;
    }

    /*
    ** Save vars so they are freed properly.
    */
    AppendList(GraphPtr->varsList(), vlist);
    DestroyList(vlist, NULL);

    // TODO: wrap this all up within the graph class itself

    if (!(ai = GraphPtr->NewAssertInfoEntry())) {
        PrintOutError("Error building assertion.\n");
        Free(buf);
        return 0;
    }

    GetGraphVar(GRAPH_LOWER, &ai->a_lower);
    GetGraphVar(GRAPH_UPPER, &ai->a_upper);
    GetGraphVar(GRAPH_RELATIVE, &ai->a_relative);
    GetGraphVar(GRAPH_STOP, &ai->a_stop);

    // Add the buffer for the assertion command string
    // Note that this does the cleanup later.
    AddToList(GraphPtr->assertsList(), (void *)buf);

    FreeExpr(e);  // Memory Leak: Free the expr

    return 1;
}

static int cmd_graph_alt_end_cmd(rec_set_t *rec_set, int argc, char **argv) {
    val_t *v;

    /*
    ** Finish off graph.
    */

    if (!GraphPtr->prepStartNode()) {
        PrintOutError("Build failed.\n");
        PrintError();
        GraphCleanup(0);
        return 0;
    }

    if (!GraphPtr->checkGraph()) {
        PrintOutError("Build failed.\n");
        GraphCleanup(0);
        return 0;
    }

    /*
    ** Take care of any unused procs.
    */
    BuildUnused(GraphPtr);

    /*
    ** Set the graph name. This corresponds to the GraphVal hashkey value.
    ** This is free'd by val functions and should not be free'd by us.
    ** Create graph variable.
    */

    if (auto name = SymLookup(LVAL_SYM(GraphVal))) {
        GraphPtr->setName(name);
    } else {
        PrintOutError("Build failed.\n");
        GraphCleanup(0);
        return 0;
    }

    v = NewVal(lval_none, rval_graph, GraphPtr);

    if (AssignVal(GraphVal, v, rec_set) < 0) PrintError();

    /*
    ** Save the GraphPtr into our internal list of graphs.
    */
    AddToList(GraphList, (void *)GraphPtr);

    PrintOutMsg("Assertion script $%s compiled.\n", GraphPtr->name().c_str());

    GraphCleanup(1);

    return 1;
}

/********************************
 * Helper functions
 ********************************/

static void GraphCleanup(int keep) {
    if (!keep) {
        if (GraphVal != (val_t *)NULL) {
            // Make sure the GraphVal is a lval_internal
            if (LVAL_TYPE(GraphVal) == lval_internal) {
                // Remove the sym
                RemoveVal(LVAL_SYM(GraphVal));
            }
            // Destroy the val
            DestroyVal(GraphVal);
        }
        if (GraphPtr != (graph_t *)NULL) {
            delete GraphPtr;
        }
    }

    // if the graph errors out, we free it. During exit,
    // we also try to free the graph a second time. To avoid
    // this, we must set the global vars to null.
    GraphVal = NULL;
    GraphPtr = NULL;

    cli_dec_prompt();

    (void)SetCommands(OldCmds);
}

static void graphnotfound(const char *cmd) { CmdNotFound(cmd); }

static void graphambig(const char *cmd) { AmbiguousAltCmd(GraphCmds, cmd); }

static void graphfailed(void) {
    // do nothing
    return;
}

/********************************
 * command init functions
 ********************************/

void InitCmd_graph(void) {
    // Create a new list of graph ptrs
    GraphList = NewList();

    // info commands
    AddInfoCommand("diff", proc_ignore, 0,
                   class_info_subcmd_diff_cmd,
                   class_info_subcmd_diff_help,
                   "Display saved deltas from assertion script execution.");
    AddInfoCommand(
        "build", proc_ignore, 0, class_info_subcmd_build_cmd,
        class_info_subcmd_build_help, "Display built assertion scripts.");
    AddInfoCommand("script", proc_ignore, 0,
                   class_info_subcmd_script_cmd,
                   class_info_subcmd_script_help,
                   "Display active assertion script status.");

    // maint commands
    AddMaintCommand("dump", proc_ignore, 0,
                    class_maint_subcmd_dump_cmd,
                    class_maint_subcmd_dump_help,
                    "Dump an assertion script in digraph format.");

    // set commands
    AddSetCommand("halt", proc_ignore, 0, class_set_subcmd_halt_cmd,
                  class_set_subcmd_halt_help,
                  "Enable/disable halting of processes on termination.");
    AddSetCommand("stop", proc_ignore, 0, class_set_subcmd_stop_cmd,
                  class_set_subcmd_stop_help,
                  "Enable/disable stopping on assertion failure.");
    AddSetCommand(
        "dfblock", proc_ignore, 0, class_set_subcmd_dfblock_cmd,
        class_set_subcmd_dfblock_help, "Set dataflow command blocking.");

    // show commands
    AddShowCommand("halt", proc_ignore, 0,
                   class_show_subcmd_halt_cmd,
                   class_show_subcmd_halt_help,
                   "Show status of halting of processes on termination.");
    AddShowCommand("stop", proc_ignore, 0,
                   class_show_subcmd_stop_cmd,
                   class_show_subcmd_stop_help,
                   "Show status of stopping on assertion failure.");
    AddShowCommand(
        "dfblock", proc_ignore, 0, class_show_subcmd_dfblock_cmd,
        class_show_subcmd_dfblock_help, "Show dataflow command blocking.");

    // usage commands
    AddUsageCommand("assert", proc_ignore, 0,
                    class_usage_subcmd_assert_cmd,
                    class_usage_subcmd_assert_help,
                    "Display help about using build \"assert\" commands.");

    // regular commands
    AddCommand("build", NULL, proc_ignore, 0, cmd_build_cmd,
               cmd_build_help, "Build an assertion script.");
    AddCommand("start", NULL, proc_ignore, 0, cmd_start_cmd,
               cmd_start_help, "Start executing an assertion script.");
    AddCommand("stop", NULL, proc_ignore, 0, cmd_stop_cmd,
               cmd_stop_help, "Stop the currently executing assertion script.");
    /* FIXME: This is currently broken on cray systems
    AddCommand("restart", NULL, proc_ignore, 0, cmd_restart_cmd,
            cmd_restart_help, "Restart a dataflow graph.");
    */

    // graph alt commands
    GraphCmds = NewCommandList(graphnotfound, graphambig, graphfailed);

    AddAltCommand(GraphCmds, "assert", NULL, proc_ignore, 0,
                  cmd_graph_alt_assert_cmd,
                  cmd_graph_alt_assert_help, "Create an assertion.");
    AddAltCommand(GraphCmds, "end", NULL, proc_ignore, 0,
                  cmd_graph_alt_end_cmd, cmd_graph_alt_end_help,
                  "Finish building the current assertion script.");
    AddAltCommand(GraphCmds, "help", "h", proc_ignore, 0,
                  cmd_help_cmd, cmd_help_help,
                  "Display help information about build script mode commands.");
    AddAltCommand(GraphCmds, "set", NULL, proc_ignore, 1,
                  class_set_cmd, class_set_help,
                  "Set information about the debugger environment.");
    AddAltCommand(GraphCmds, "show", NULL, proc_ignore, 1,
                  class_show_cmd, class_show_help,
                  "Show information about the debugger environment.");
}

void FinishCmd_graph(void) {
    FinishGraphVars();
    GraphCleanup(0);
    DestroyCommandList(GraphCmds);
    DestroyList(GraphList, NULL);  // destroyVal supposedly cleans up the graph
}

/*
** Called to display the result of a comparison when the
** dataflow interpreter is executing.
*/
int DisplayData(AIF *data) {
    PrintOutLine("The difference: ");
    AIFPrint(Client.cli_out_stream, 0, data);
    PrintOutLine("\n");
    return 1;
}

