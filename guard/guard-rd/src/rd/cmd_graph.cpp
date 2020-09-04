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

#include "ainfo.h"
#include "bitset.h"
#include "client.h"
#include "df.h"
#include "rd.h"
#include "rd_cmd.h"
#include "useful.h"

#define GRAPH_TABLE_SIZE 100 /* number that can be active */
#define DEFQLEN 5            /* default length of input queue */

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
static int SetStart(graph_t *, int);
static int CheckGraph(graph_t *);
static int OutNode(graph_t *, const char *);
static int OutConstInt(graph_t *, const char *, int);
static int OutConstFloat(graph_t *, const char *, double);
static int OutConstStr(graph_t *, const char *, const char *);
static int OutConstAIF(graph_t *, const char *, double, int);
static int OutConstBitSet(graph_t *, const char *, bitset *);
static void OutEdge(graph_t *, int, int, int);

/********************************
 * Info class commands
 ********************************/

static int class_info_subcmd_diff_cmd(rec_set_t *rec_set, int argc,
                                      char **argv) {
    char *disp = (char *)NULL;
    char *output = (char *)NULL;
    graph_t *g;
    graph_diff *gd;
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
            PrintOutMsg("Script $%s\n", g->g_valname);
            SetList(g->g_diff);
            while ((gd = (graph_diff *)GetListElement(g->g_diff)) !=
                   (graph_diff *)NULL) {
                PrintOutMsg("%d. %s\n", ++num, gd->gd_name);
            }
        }
        return 1;
    }

    // grab the current display setting
    GetGraphVar(GRAPH_DISPLAY, &disp);
    // grab the current output setting
    GetGraphVar(GRAPH_OUTPUT, &output);

    // Figure out the type of the argument
    s = argv[1];
    switch (ScanString(&cv, &s)) {
        case CT_INT:
            req = cv.cv_int;
            // find the requested comparison result the slow way
            SetList(GraphList);
            while ((g = (graph_t *)GetListElement(GraphList)) !=
                   (graph_t *)NULL) {
                SetList(g->g_diff);
                while ((gd = (graph_diff *)GetListElement(g->g_diff)) !=
                       (graph_diff *)NULL) {
                    if (++num == req) {
                        PrintOutMsg("Script $%s %s\n", g->g_valname,
                                    gd->gd_name);
                        return (
                            DisplayData(disp, output, gd->gd_name, gd->gd_val));
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
                if (!strcmp(g->g_valname, gname)) break;
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
                        SetList(g->g_diff);
                        while ((gd = (graph_diff *)GetListElement(g->g_diff)) !=
                               (graph_diff *)NULL) {
                            if (gd->gd_assert_id == req) {
                                PrintOutMsg("Script $%s %s\n", g->g_valname,
                                            gd->gd_name);
                                if (!DisplayData(disp, output, gd->gd_name,
                                                 gd->gd_val)) {
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
            SetList(g->g_diff);
            while ((gd = (graph_diff *)GetListElement(g->g_diff)) !=
                   (graph_diff *)NULL) {
                PrintOutMsg("Script $%s %s\n", g->g_valname, gd->gd_name);
                if (!DisplayData(disp, output, gd->gd_name, gd->gd_val)) {
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
            PrintOutMsg("Name: $%s\t\t\tTotal Assertions: %d\n", g->g_valname,
                        g->g_assertions);
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

    PrintOutMsg("Script $%s\n", g->g_valname);

    // loop through the g_assert_cmd list
    SetList(g->g_assert_cmds);
    while ((assert_cmd = (char *)GetListElement(g->g_assert_cmds)) !=
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
        PrintOutMsg("Active assertion script: %s\n", g->g_valname);
        PrintOutMsg(
            "Completed Iterations: %d\n",
            (g->g_iterations /
             g->g_assertions));  // We want integer values only from division
        PrintOutMsg("Passed Cases: %d\n", g->g_pass);
        PrintOutMsg("Warned Cases: %d\n", g->g_warn);
        PrintOutMsg("Failed Cases: %d\n", g->g_fail);
    }

    return 1;
}

/********************************
 * Set class commands
 ********************************/

static int class_set_subcmd_display_cmd(rec_set_t *rec_set, int argc,
                                        char **argv) {
    const char *s;
    ct_val cv;

    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: set display [text|aif|hdf|hdf5]\n");
        return 0;
    }

    // only look at the first arg
    s = argv[1];

    switch (ScanString(&cv, &s)) {
        case CT_TEXT:
        case CT_AIF:
        case CT_HDF:
        case CT_HDF5:
            break;
        default:
            PrintOutError(
                "Invalid command argument. See \"help set display\" for more "
                "information.\n");
            PrintOutError(
                "Argument must be one of the following: text, aif, hdf, "
                "hdf5\n");
            return 0;
    }

    SetGraphVar(GRAPH_DISPLAY, VAR_STRING, (void *)cv.cv_sym->h_key);

    return 1;
}

static int class_set_subcmd_output_cmd(rec_set_t *rec_set, int argc,
                                       char **argv) {
    const char *s;
    ct_val cv;

    if (argc < 2) {
        PrintOutError("Not enough command arguments. Filename expected.\n");
        PrintOutError("See \"help set output\" for more information.\n");
        return 0;
    }

    // only look at the first arg
    s = argv[1];

    if (ScanString(&cv, &s) != CT_STR) {
        PrintOutError(
            "Invalid filename. The filename provided must be enclosed within "
            "quotes.\n");
        PrintOutError("Example command: set output \"my.file\"\n");
        return 0;
    }

    SetGraphVar(GRAPH_OUTPUT, VAR_STRING, (void *)cv.cv_str);

    return 1;
}

static int class_set_subcmd_force_cmd(rec_set_t *rec_set, int argc,
                                      char **argv) {
    const char *s;
    int force_flag;
    ct_val cv;

    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: set force [on|off]\n");
        return 0;
    }

    // only look at the first arg
    s = argv[1];

    switch (ScanString(&cv, &s)) {
        case CT_TRUE:
            force_flag = 1;
            break;

        case CT_FALSE:
            force_flag = 0;
            break;

        default:
            PrintOutError("Encountered something other than on or off.\n");
            PrintOutError("See \"help set force\" for more information.\n");
            return 0;
    }

    SetGraphVar(GRAPH_FORCE, VAR_INT, (void *)&force_flag);

    return 1;
}

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

/*
** Implement the p2p comparison scheme
*/
static int class_set_subcmd_p2p_cmd(rec_set_t *rec_set, int argc, char **argv) {
    const char *s;
    int p2p_flag;
    ct_val cv;

    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: set p2p [true|false]\n");
        return 0;
    }

    // only look at the first arg
    s = argv[1];

    switch (ScanString(&cv, &s)) {
        case CT_TRUE:
            p2p_flag = 1;
            break;

        case CT_FALSE:
            p2p_flag = 0;
            break;

        default:
            PrintOutError("Encountered something other than true or false.\n");
            PrintOutError("See \"help set p2p\" for more information.\n");
            return 0;
    }

    SetGraphVar(GRAPH_P2PMODE, VAR_INT, (void *)&p2p_flag);

    return 1;
}

/*
** Implements the hashing interface.
*/
static int class_set_subcmd_reduce_cmd(rec_set_t *rec_set, int argc,
                                       char **argv) {
    const char *s;
    char *buf;
    ct_val cv;
    int reducefunc;

    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError(
            "Expected: set reduce "
            "[sum|product|average|hash|min|max|stdev|none] [args ...]\n");
        return 0;
    }

    buf = ArgsToBuf(&argv[1], argc - 1);

    s = buf;

    switch (ScanString(&cv, &s)) {
        case CT_SUM:
            reducefunc = EXPR_SUM;
            break;
        case CT_PRODUCT:
            reducefunc = EXPR_PRODUCT;
            break;
        case CT_AVERAGE:
            reducefunc = EXPR_AVERAGE;
            break;
        case CT_BOBHASH:
            reducefunc = EXPR_HASH_BOB;
            {
                int lower_precision = -1;
                int upper_precision = -1;

                // only need to get lower and upper precision if argc is over 2.
                if (2 < argc) {
                    // scan the lower precision
                    if (ScanString(&cv, &s) != CT_INT) {
                        PrintOutError(
                            "Encountered a non-integer value for the lower "
                            "precision argument.\n");
                        PrintOutError(
                            "Example must be a integer value. Example: 2\n");
                        Free(buf);
                        return 0;
                    }
                    lower_precision = cv.cv_int;

                    // scan the upper precision
                    if (ScanString(&cv, &s) != CT_INT) {
                        PrintOutError(
                            "Encountered a non-integer value for the upper "
                            "precision argument.\n");
                        PrintOutError(
                            "Example must be a integer value. Example: 2\n");
                        Free(buf);
                        return 0;
                    }
                    upper_precision = cv.cv_int;
                }

                SetGraphVar(GRAPH_HASH_LOWER, VAR_INT,
                            (void *)&lower_precision);
                SetGraphVar(GRAPH_HASH_UPPER, VAR_INT,
                            (void *)&upper_precision);
            }
            break;
        case CT_MIN:
            reducefunc = EXPR_MIN;
            break;
        case CT_MAX:
            reducefunc = EXPR_MAX;
            break;
        case CT_STDEV:
            reducefunc = EXPR_STDEV;
            break;
        case CT_COUNTELEM:
            reducefunc = EXPR_COUNTELEM;
            {
                double lower_bound = 0.0;
                double upper_bound = 0.0;

                // only need to get lower and upper bound if argc is over 2.
                if (2 < argc) {
                    // scan the lower bound
                    if (ScanString(&cv, &s) != CT_FLOAT) {
                        PrintOutError(
                            "Encountered a non-floating value for the lower "
                            "bound argument.\n");
                        PrintOutError(
                            "Example must be a floating value. Example: 2.1\n");
                        Free(buf);
                        return 0;
                    }
                    lower_bound = cv.cv_float;

                    // scan the upper bound
                    if (ScanString(&cv, &s) != CT_FLOAT) {
                        PrintOutError(
                            "Encountered a non-floating value for the upper "
                            "bound argument.\n");
                        PrintOutError(
                            "Example must be a floating value. Example: 2.1\n");
                        Free(buf);
                        return 0;
                    }
                    upper_bound = cv.cv_float;
                }

                SetGraphVar(GRAPH_LOWER_BOUND, VAR_DOUBLE,
                            (void *)&lower_bound);
                SetGraphVar(GRAPH_UPPER_BOUND, VAR_DOUBLE,
                            (void *)&upper_bound);
            }
            break;
        case CT_NONE:
            reducefunc = EXPR_REDUCE_NONE;
            break;
        default:
            PrintOutError(
                "Invalid command argument. See \"help set reduce\" for more "
                "information.\n");
            PrintOutError(
                "Argument must be one of the following: sum, product, average, "
                "hash, min, max, stdev, countelem, none\n");
            Free(buf);
            return 0;
    }

    // Done with buf
    Free(buf);

    SetGraphVar(GRAPH_REDUCE, VAR_INT, (void *)&reducefunc);

    return 1;
}

static int class_set_subcmd_logging_cmd(rec_set_t *rec_set, int argc,
                                        char **argv) {
    const char *s;
    int logging_flag;
    ct_val cv;

    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: set logging [on|off]\n");
        return 0;
    }

    // only look at the first arg
    s = argv[1];

    switch (ScanString(&cv, &s)) {
        case CT_TRUE:
            logging_flag = 1;
            break;
        case CT_FALSE:
            logging_flag = 0;
            break;
        default:
            PrintOutError("Encountered something other than on or off.\n");
            PrintOutError("See \"help set halt\" for more information.\n");
            return 0;
    }

    SetGraphVar(GRAPH_LOGGING, VAR_INT, (void *)&logging_flag);

    return 1;
}

static int class_set_subcmd_logfile_cmd(rec_set_t *rec_set, int argc,
                                        char **argv) {
    const char *s;
    ct_val cv;

    if (argc < 2) {
        PrintOutError("Not enough command arguments. Filename expected.\n");
        PrintOutError("See \"help set logfile\" for more information.\n");
        return 0;
    }

    // only look at the first arg
    s = argv[1];

    if (ScanString(&cv, &s) != CT_STR) {
        PrintOutError(
            "Invalid filename. The filename provided must be enclosed within "
            "quotes.\n");
        PrintOutError("Example command: set output \"my.file\"\n");
        return 0;
    }

    SetGraphVar(GRAPH_LOGFILE, VAR_STRING, (void *)cv.cv_str);

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

static int class_set_subcmd_dfmode_cmd(rec_set_t *rec_set, int argc,
                                       char **argv) {
    const char *s;
    ct_val cv;
    int i;

    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: set dfmode [default|sync|async]\n");
        return 0;
    }

    // only look at the first arg
    s = argv[1];

    switch (ScanString(&cv, &s)) {
        case CT_DEFAULT:
            i = DF_DEFAULT;
            break;
        case CT_SYNC:
            i = DF_SYNC;
            break;
        case CT_ASYNC:
            i = DF_ASYNC;
            break;
        default:
            PrintOutError(
                "Invalid command argument. See \"help set dfmode\" for more "
                "information.\n");
            PrintOutError(
                "Argument must be one of the following: default, sync, "
                "async\n");
            return 0;
    }

    SetGraphVar(GRAPH_DFMODE, VAR_INT, (void *)&i);

    return 1;
}

/********************************
 * Show class commands
 ********************************/

static int class_show_subcmd_display_cmd(rec_set_t *rec_set, int argc,
                                         char **argv) {
    char *s = NULL;

    GetGraphVar(GRAPH_DISPLAY, &s);

    if (s != NULL)
        PrintOutMsg("Display type: %s\n", s);
    else
        PrintOutMsg("Display type has not been not set.\n");

    return 1;
}

static int class_show_subcmd_reduce_cmd(rec_set_t *rec_set, int argc,
                                        char **argv) {
    int reducefunc;
    int lower_precision;
    int upper_precision;
    int histogram_numbins;
    double histogram_start;
    double histogram_end;
    double lower_bound;
    double upper_bound;

    GetGraphVar(GRAPH_REDUCE, (int *)&reducefunc);

    GetGraphVar(GRAPH_HASH_LOWER, (int *)&lower_precision);
    GetGraphVar(GRAPH_HASH_UPPER, (int *)&upper_precision);
    GetGraphVar(GRAPH_LOWER_BOUND, (double *)&lower_bound);
    GetGraphVar(GRAPH_UPPER_BOUND, (double *)&upper_bound);
    GetGraphVar(GRAPH_HISTOGRAM_NUMBINS, (int *)&histogram_numbins);
    GetGraphVar(GRAPH_HISTOGRAM_START, (double *)&histogram_start);
    GetGraphVar(GRAPH_HISTOGRAM_END, (double *)&histogram_end);

    switch (reducefunc) {
        case EXPR_SUM:
            PrintOutMsg("Reduce function: simple sum function\n");
            break;
        case EXPR_PRODUCT:
            PrintOutMsg("Reduce function: simple product function\n");
            break;
        case EXPR_AVERAGE:
            PrintOutMsg("Reduce function: simple average function\n");
            break;
        case EXPR_HASH_BOB:
            PrintOutMsg("Reduce function: hashing algorithm\n");
            PrintOutMsg("Lower precision: 10^-%d\n", lower_precision);
            PrintOutMsg("Upper precision: 10^-%d\n", upper_precision);
            break;
        case EXPR_MIN:
            PrintOutMsg("Reduce function: simple min function\n");
            break;
        case EXPR_MAX:
            PrintOutMsg("Reduce function: simple max function\n");
            break;
        case EXPR_STDEV:
            PrintOutMsg("Reduce function: standard deviation function\n");
            break;
        case EXPR_COUNTELEM:
            PrintOutMsg("Reduce function: simple element counting function\n");
            PrintOutMsg("Lower bound: %lf\n", lower_bound);
            PrintOutMsg("Upper bound: %lf\n", upper_bound);
            break;
        default:
            PrintOutMsg("Reduce function has not been set.\n");
    }

    return 1;
}

static int class_show_subcmd_output_cmd(rec_set_t *rec_set, int argc,
                                        char **argv) {
    char *s = NULL;

    GetGraphVar(GRAPH_OUTPUT, &s);

    if (s != NULL)
        PrintOutMsg("Output filename: %s\n", s);
    else
        PrintOutMsg("Output filename has not been not set.\n");

    return 1;
}

static int class_show_subcmd_force_cmd(rec_set_t *rec_set, int argc,
                                       char **argv) {
    int force;

    GetGraphVar(GRAPH_FORCE, &force);

    PrintOutMsg("Assertion forcing is %s.\n", force ? "on" : "off");

    return 1;
}

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

static int class_show_subcmd_logging_cmd(rec_set_t *rec_set, int argc,
                                         char **argv) {
    int halt;

    GetGraphVar(GRAPH_LOGGING, &halt);

    PrintOutMsg("Assertion script logging mode: %s\n", halt ? "on" : "off");

    return 1;
}

static int class_show_subcmd_logfile_cmd(rec_set_t *rec_set, int argc,
                                         char **argv) {
    char *s;

    GetGraphVar(GRAPH_LOGFILE, &s);

    if (*s != '\0')
        PrintOutMsg("Assertion script event logging filename: %s\n", s);
    else
        PrintOutMsg(
            "Assertion script event logging filename has not been set.\n");

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

static int class_show_subcmd_dfmode_cmd(rec_set_t *rec_set, int argc,
                                        char **argv) {
    int i;

    GetGraphVar(GRAPH_DFMODE, &i);

    switch (i) {
        case DF_DEFAULT:
            PrintOutMsg("Assertion script dataflow mode: default\n");
            break;
        case DF_SYNC:
            PrintOutMsg("Assertion script dataflow mode: sync\n");
            break;
        case DF_ASYNC:
            PrintOutMsg("Assertion script dataflow mode: async\n");
            break;
        default:
            PrintOutMsg("Assertion script dataflow mode: invalid\n");
            break;
    }

    return 1;
}

static int class_show_subcmd_p2p_cmd(rec_set_t *rec_set, int argc,
                                     char **argv) {
    int p2p;

    GetGraphVar(GRAPH_P2PMODE, &p2p);

    PrintOutMsg("P2P Comparison mode: %s\n", p2p ? "true" : "false");

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

    PrintGraph(file, g);

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

static int class_usage_subcmd_assign_cmd(rec_set_t *rec_set, int argc,
                                         char **argv) {
    PrintHelpEntry((char **)cmd_graph_alt_assign_help);

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
    //	GraphPtr = NewGraph(CompareData, OutLine, QueueLen); TODO: fixe me. how
    //to replace OutLine() ?
    GraphPtr = NewGraph(CompareData, NULL, DisplayP2P,
                        DEFQLEN);  // TODO: What does DEFQLEN even mean?
    OldCmds = SetCommands(GraphCmds);

    cli_inc_prompt();

    GraphPtr->g_startGraph =
        (void (*)(struct graph_t *, char *, int, void *))NULL;
    GraphPtr->g_endGraph = (void (*)(struct graph_t *))NULL;
    GraphPtr->g_checkGraph = CheckGraph;
    GraphPtr->g_setStartNode = SetStart;
    GraphPtr->g_outNode = OutNode;
    GraphPtr->g_outConstInt = OutConstInt;
    GraphPtr->g_outConstFloat = OutConstFloat;
    GraphPtr->g_outConstStr = OutConstStr;
    GraphPtr->g_outConstAIF = OutConstAIF;
    GraphPtr->g_outConstBitSet = OutConstBitSet;
    GraphPtr->g_outEdge = OutEdge;

    /*
    ** Start building graph.
    */
    GraphPtr->g_startnode = SCONST(GraphPtr, "CONST", Client.proto);
    GraphPtr->g_initnode = NODE(GraphPtr, "DBGINIT");
    EDGE(GraphPtr, GraphPtr->g_startnode, GraphPtr->g_initnode, 0);

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
    char *str;
    char *buf;
    list_t *vlist;
    expr_t *e;
    ainfo_t *ai;

    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError(
            "Expected: assert $proc1::[var]@\"file\":[line] = "
            "$proc2::[var]@\"file\":[line]\n");
        return 0;
    }

    buf = ArgsToBuf(&argv[1], argc - 1);

    if ((e = ParseExpr(buf)) == (expr_t *)NULL) {
        PrintError();
        Free(buf);
        return 0;
    }

    vlist = NewList();

    if (BuildAssert(GraphPtr, GraphPtr->g_assertions, e, rec_set, vlist) < 0) {
        PrintError();
        FreeExpr(e);
        Free(buf);
        return 0;
    }

    /*
    ** Save vars so they are freed properly.
    */
    AppendList(GraphPtr->g_vars, vlist);
    DestroyList(vlist, NULL);

    if ((ai = NewAssertInfoEntry(GraphPtr->g_assert_info_tab,
                                 GraphPtr->g_assertions)) == (ainfo_t *)NULL) {
        PrintOutError("Error building assertion.\n");
        Free(buf);
        return 0;
    }

    GetGraphVar(GRAPH_LOWER, &ai->a_lower);
    GetGraphVar(GRAPH_UPPER, &ai->a_upper);
    GetGraphVar(GRAPH_RELATIVE, &ai->a_relative);
    GetGraphVar(GRAPH_DISPLAY, &str);
    ai->a_display = strdup(str);
    GetGraphVar(GRAPH_OUTPUT, &str);
    ai->a_file = strdup(str);
    GetGraphVar(GRAPH_STOP, &ai->a_stop);

    // Add the buffer for the assertion command string
    // Note that this does the cleanup later.
    AddToList(GraphPtr->g_assert_cmds, (void *)buf);

    GraphPtr->g_assertions++;

    FreeExpr(e);  // Memory Leak: Free the expr

    return 1;
}

// FIXME: Disabled until BuildAssign is fixed in build.c
#if 0
static int 
cmd_graph_alt_assign_cmd(rec_set_t *rec_set, int argc, char **argv)
{
	int			inp;
	int			out;
	list_t *	vlist;
	expr_t *	e;
	char *		buf;
	int			force = 0;

	if ( argc < 3 )
	{
		PrintOutError("Not enough command arguments.\n");
		PrintOutError("Expected: assign [target var] [expr] [<null>|-force]\n");
		return 0;
	}

	if ( strcmp(argv[argc-1], "-force") == 0 )
	{
		force = 1;
		argc--;
	}

	/*
	** Check that destination parses correctly.
	*/
	if ( (e = ParseExpr(argv[1])) == (expr_t *)NULL )
	{
		PrintError();
		return 0;
	}

	FreeExpr(e);

	/*
	** Now parse source.
	*/
	buf = ArgsToBuf(&argv[2], argc-2);

	if ( (e = ParseExpr(buf)) == (expr_t *)NULL )
	{
		PrintError();
		Free(buf);
		return 0;
	}
	
	// done with buf
	Free(buf);

	vlist = NewList();

	if ( BuildExpr(GraphPtr, &inp, &out, vlist, e, rec_set) < 0 )
	{
		PrintError();
		FreeExpr(e);
		return 0;
	}

	FreeExpr(e);

	if ( EmptyList(vlist) )
	{
		PrintOutError("Must specify breakpoint in assignment.\n");
		return 0;
	}

	/*
	** Build assign graph.
	*/

	if ( BuildAssign(GraphPtr, argv[1], inp, out, vlist) < 0 )
	{
		PrintError();
		DestroyList(vlist, (void (*)())NULL);
		return 0;
	}

	/*
	** Save vars so they are freed properly.
	*/
	AppendList(GraphPtr->g_vars, vlist);
	DestroyList(vlist, NULL);

	return 1;
}
#endif

static int cmd_graph_alt_end_cmd(rec_set_t *rec_set, int argc, char **argv) {
    val_t *v;
    size_t assertbuf_size;

    /*
    ** Finish off graph.
    */
    if (GraphPtr->g_endGraph != (void (*)(struct graph_t *))NULL)
        GraphPtr->g_endGraph(GraphPtr);

    if (GraphPtr->g_setStartNode != (int (*)(struct graph_t *, int))NULL &&
        !GraphPtr->g_setStartNode(GraphPtr, GraphPtr->g_startnode)) {
        PrintOutError("Build failed.\n");
        GraphCleanup(0);
        return 0;
    }

    if (GraphPtr->g_checkGraph != (int (*)(struct graph_t *))NULL &&
        !GraphPtr->g_checkGraph(GraphPtr)) {
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

    GraphPtr->g_valname = SymLookup(LVAL_SYM(GraphVal));

    v = NewVal(lval_none, rval_graph, GraphPtr);

    if (AssignVal(GraphVal, v, rec_set) < 0) PrintError();

    /*
    ** Create the assertion pass/warn/fail result arrays
    */
    assertbuf_size = sizeof(int) * GraphPtr->g_assertions;
    GraphPtr->g_assert_pass = (int *)Alloc(assertbuf_size);
    memset(GraphPtr->g_assert_pass, 0, assertbuf_size);
    GraphPtr->g_assert_warn = (int *)Alloc(assertbuf_size);
    memset(GraphPtr->g_assert_warn, 0, assertbuf_size);
    GraphPtr->g_assert_fail = (int *)Alloc(assertbuf_size);
    memset(GraphPtr->g_assert_fail, 0, assertbuf_size);

    /*
    ** Save the GraphPtr into our internal list of graphs.
    */
    AddToList(GraphList, (void *)GraphPtr);

    PrintOutMsg("Assertion script $%s compiled.\n", GraphPtr->g_valname);

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
            DestroyGraph(GraphPtr);
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
    AddSetCommand(
        "display", proc_ignore, 0, class_set_subcmd_display_cmd,
        class_set_subcmd_display_help, "Set how to display output of assert.");
    AddSetCommand(
        "output", proc_ignore, 0, class_set_subcmd_output_cmd,
        class_set_subcmd_output_help, "Set name of output file for assert.");
    AddSetCommand("force", proc_ignore, 0,
                  class_set_subcmd_force_cmd,
                  class_set_subcmd_force_help,
                  "Enable/disable forcing of assertion values.");
    AddSetCommand("halt", proc_ignore, 0, class_set_subcmd_halt_cmd,
                  class_set_subcmd_halt_help,
                  "Enable/disable halting of processes on termination.");
    AddSetCommand("stop", proc_ignore, 0, class_set_subcmd_stop_cmd,
                  class_set_subcmd_stop_help,
                  "Enable/disable stopping on assertion failure.");
    AddSetCommand("logging", proc_ignore, 0,
                  class_set_subcmd_logging_cmd,
                  class_set_subcmd_logging_help, "Enable/disable logging.");
    AddSetCommand("logfile", proc_ignore, 0,
                  class_set_subcmd_logfile_cmd,
                  class_set_subcmd_logfile_help, "Set log file name.");
    AddSetCommand(
        "dfblock", proc_ignore, 0, class_set_subcmd_dfblock_cmd,
        class_set_subcmd_dfblock_help, "Set dataflow command blocking.");
    AddSetCommand("dfmode", proc_ignore, 0,
                  class_set_subcmd_dfmode_cmd,
                  class_set_subcmd_dfmode_help, "Set dataflow mode.");
    AddSetCommand("reduce", proc_ignore, 0,
                  class_set_subcmd_reduce_cmd,
                  class_set_subcmd_reduce_help, "Set reduce function.");

    /* FIXME: andrewg: Once P2P has been fully tested, we can add this to the
    *normal set
    **         commands. But for now only support this feature as experimental.
    */
    AddMaintSetCommand("p2p", proc_ignore, 0,
                       class_set_subcmd_p2p_cmd,
                       class_set_subcmd_p2p_help, "Set p2p mode.");

    // show commands
    AddShowCommand("display", proc_ignore, 0,
                   class_show_subcmd_display_cmd,
                   class_show_subcmd_display_help,
                   "Show how output of assert is displayed.");
    AddShowCommand(
        "output", proc_ignore, 0, class_show_subcmd_output_cmd,
        class_show_subcmd_output_help, "Show name of output file for assert.");
    AddShowCommand("force", proc_ignore, 0,
                   class_show_subcmd_force_cmd,
                   class_show_subcmd_force_help,
                   "Show status of forcing of assertion values.");
    AddShowCommand("halt", proc_ignore, 0,
                   class_show_subcmd_halt_cmd,
                   class_show_subcmd_halt_help,
                   "Show status of halting of processes on termination.");
    AddShowCommand("stop", proc_ignore, 0,
                   class_show_subcmd_stop_cmd,
                   class_show_subcmd_stop_help,
                   "Show status of stopping on assertion failure.");
    AddShowCommand("logging", proc_ignore, 0,
                   class_show_subcmd_logging_cmd,
                   class_show_subcmd_logging_help, "Show logging status.");
    AddShowCommand("logfile", proc_ignore, 0,
                   class_show_subcmd_logfile_cmd,
                   class_show_subcmd_logfile_help, "Show log file name.");
    AddShowCommand(
        "dfblock", proc_ignore, 0, class_show_subcmd_dfblock_cmd,
        class_show_subcmd_dfblock_help, "Show dataflow command blocking.");
    AddShowCommand("dfmode", proc_ignore, 0,
                   class_show_subcmd_dfmode_cmd,
                   class_show_subcmd_dfmode_help, "Show dataflow mode.");
    AddShowCommand("reduce", proc_ignore, 0,
                   class_show_subcmd_reduce_cmd,
                   class_show_subcmd_reduce_help, "Show reduce function.");

    /* FIXME: andrewg: Once P2P has been fully tested, we can add this to the
    *normal show
    **         commands. But for now only support this feature as experimental.
    */
    AddMaintShowCommand("p2p", proc_ignore, 0,
                        class_show_subcmd_p2p_cmd,
                        class_show_subcmd_p2p_help, "Show p2p mode.");

    // usage commands
    AddUsageCommand("assert", proc_ignore, 0,
                    class_usage_subcmd_assert_cmd,
                    class_usage_subcmd_assert_help,
                    "Display help about using build \"assert\" commands.");
    AddUsageCommand("assign", proc_ignore, 0,
                    class_usage_subcmd_assign_cmd,
                    class_usage_subcmd_assign_help,
                    "Display help about using build \"assign\" commands.");

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
    // FIXME: Disabled until BuildAssign is fixed in build.c
#if 0
	AddAltCommand(GraphCmds, "assign", NULL, proc_ignore, 0, 
		cmd_graph_alt_assign_cmd, cmd_graph_alt_assign_help, "Assign a value to a debugger variable.");
#endif
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

/**************************************
***************************************
** Support routines for BuildGraph().**
***************************************
**************************************/

static int SetStart(graph_t *g, int start) {
    token_t *t;
    node_t *n;

    /*
    ** Create dummy token.
    */

    if ((n = FindNode(g, start)) == (node_t *)NULL) {
        PrintOutError("No such start node \"%d\"\n", start);
        return 0;
    }

    /*
    ** Check if node has any inputs. Create one
    ** if not.
    */
    if (n->n_ninp == 0 && NewInput(g, n, 0, TK_ANY) < 0) {
        PrintError();
        return 0;
    }

    t = NewToken(g, TK_INT);
    t->t_int = start;

    /*
    ** Fake an token on first input of node.
    ** GenNodeInput() duplicates token, and adds
    ** node to active list.
    */

    GenNodeInput(g, n, &n->n_inputs[0], t);

    // DestroyToken(g, t);

    return 1;
}

static int CheckGraph(graph_t *g) {
    int np;

    if ((np = CheckNodes(g)) == 0) {
        PrintOutError("Errors encountered, cannot proceed.\n");
        return 0;
    }

    DPRINT("%d nodes successfully processed\n", np);

    return 1;
}

static int gennode(graph_t *g, const char *node, token_t *t) {
    nodedef_t *nd;

    if ((nd = FindNodeDef(node)) == (nodedef_t *)NULL) {
        PrintOutError("No such node \"%s\"\n", node);
        return -1;
    }

    (void)NewNode(g, g->g_nid, nd, t);

    return g->g_nid++;
}

static int OutNode(graph_t *g, const char *node) {
    return gennode(g, node, (token_t *)NULL);
}

static int OutConstInt(graph_t *g, const char *node, int val) {
    token_t *t;
    char buf[BUFSIZ];

    sprintf(buf, "%d", val);

    t = NewToken(g, TK_INT);
    t->t_int = val;
    t->t_name = strdup(buf);

    return gennode(g, node, t);
}

static int OutConstFloat(graph_t *g, const char *node, double val) {
    token_t *t;
    char buf[BUFSIZ];

    sprintf(buf, "%#g", val);

    t = NewToken(g, TK_FLOAT);
    t->t_float = val;
    t->t_name = strdup(buf);

    return gennode(g, node, t);
}

static int OutConstStr(graph_t *g, const char *node, const char *val) {
    token_t *t;

    t = NewToken(g, TK_STR);
    t->t_str = strdup(val);

    return gennode(g, node, t);
}

static int OutConstAIF(graph_t *g, const char *node, double val,
                       int type)  // just int for now
{
    token_t *t;
    char buf[BUFSIZ];

    t = NewToken(g, TK_DATA);

    if (type == AIF_INTEGER) {
        sprintf(buf, "%d", (int)val);
        t->t_data = IntToAIF((int)val);
    } else if (type == AIF_FLOATING) {
        sprintf(buf, "%f", val);
        t->t_data = IntToAIF(val);
    }

    t->t_name = strdup(buf);

    return gennode(g, node, t);
}

static int OutConstBitSet(graph_t *g, const char *node, bitset *val) {
    token_t *t;

    t = NewToken(g, TK_BIT_SET);
    t->t_bit_set = bitset_dup(val);

    return gennode(g, node, t);
}

static void OutEdge(graph_t *g, int from, int to, int port) {
    if (!Connect(g, from, to, port)) {
        PrintOutError("Could not connect node %d to %d:%d\n", from, to, port);
        PrintError();
    }
}
