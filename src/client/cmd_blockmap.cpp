/*********************************************************************************\
 * Block Map command: using the HPF data distribution idea
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "client.h"
#include "useful.h"

/*
** Static global variables
*/
static cmdlist_t *BlockMapCmds;
static cmdlist_t *OldBlockCmds;
static g_block_map_t *CurrentBlockMap;
static val_t *CurrentBlockMapVal;

/********************************
 * Info class commands
 ********************************/

static int class_info_subcmd_blockmap_cmd(rec_set_t *rec_set, int argc,
                                          char **argv) {
    expr_t *e;
    val_t *v;
    g_block_map_t *m;

    if (argc > 1) {
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

        if (RVAL_TYPE(v) != rval_blockmap) {
            PrintOutError("No such decomposition \"%s\"\n", argv[1]);
            DestroyVal(v);
            return 0;
        }

        // assign the blockmap directly
        if ((m = RVAL_BLOCKMAP(v)) == NULL) {
            PrintOutError("Internal error in info decomposition cmd.\n");
            DestroyVal(v);
            return 0;
        }

        PrintBlockMap(Client.cli_out_stream, m);

        DestroyVal(v);

        return 1;
    }

    PrintOutMsg("Defined decompositions:\n");

    SetBlockMap();

    while ((m = GetBlockMap()) != (g_block_map_t *)NULL)
        PrintBlockMap(Client.cli_out_stream, m);

    return 1;
}

/********************************
 * Usage class commands
 ********************************/

static int class_usage_subcmd_dimension_cmd(rec_set_t *rec_set, int argc,
                                            char **argv) {
    PrintHelpEntry((char **)cmd_blockmap_alt_dimension_help);

    return 1;
}

static int class_usage_subcmd_distribute_cmd(rec_set_t *rec_set, int argc,
                                             char **argv) {
    PrintHelpEntry((char **)cmd_blockmap_alt_distribute_help);

    return 1;
}

static int class_usage_subcmd_proc_grid_cmd(rec_set_t *rec_set, int argc,
                                            char **argv) {
    PrintHelpEntry((char **)cmd_blockmap_alt_proc_grid_help);

    return 1;
}

static int class_usage_subcmd_dim_order_cmd(rec_set_t *rec_set, int argc,
                                            char **argv) {
    PrintHelpEntry((char **)cmd_blockmap_alt_dim_order_help);

    return 1;
}

/********************************
 * Regular commands
 ********************************/

static int cmd_blockmap_cmd(rec_set_t *rec_set, int argc, char **argv) {
    expr_t *e;
    val_t *v;

    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: decomposition $decomposition_name\n");
        return 0;
    }

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
            "A name must be specified for the decomposition. Names begin with "
            "a \'$\' character.\n");
        return 0;
    }

    switch (RVAL_ACTUAL_TYPE(v)) {
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

    if ((CurrentBlockMap = NewBlockMap(LVAL_SYM(v))) == (g_block_map_t *)NULL) {
        PrintError();
        RemoveVal(LVAL_SYM(v));
        return 0;
    }

    CurrentBlockMapVal = v;

    if (argc > 2) {
        if (strcmp(argv[2], "-debug") == 0)
            CurrentBlockMap->m_debug = 1;
        else {
            PrintOutError("Invalid decomposition switch \"%s\".\n", argv[2]);
            DestroyBlockMap(CurrentBlockMap);
            return 0;
        }
    }

    OldBlockCmds = SetCommands(BlockMapCmds);

    cli_inc_prompt();

    return 1;
}

/********************************
 * Alt commands
 ********************************/

static expr_t* join_and_parse(int argc, char** argv)
{
    std::string args;
    for (int i=1; i<argc; ++i)
        args += argv[i];
    return ParseExpr(args.c_str());
}

static int cmd_blockmap_alt_dimension_cmd(rec_set_t *rec_set, int argc,
                                          char **argv) {
    expr_t *ex;
    expr_elt *ep;
    int rank = 0;
    expr_elt **rank_data;
    int _rankbuf_size =
        10;  // Default to 10 entries, usually there will not be more than this
    int i;

    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: dimension 64,64,64\n");
        return 0;
    }

    if (CurrentBlockMap->m_data != (int *)NULL) {
        PrintOutError("Dimension already defined.\n");
        if (!cli_query("Re-define dimension?")) return 1;
    }

    if ((ex = join_and_parse(argc,argv)) == (expr_t *)NULL) {
        PrintError();
        return 0;
    }

    RESET(ex);

    // allocate the rank_data buffer. This is used to hold onto the EXPR_INT's
    // that correspond to the dimension size
    rank_data = (expr_elt **)Alloc(_rankbuf_size * sizeof(expr_elt *));

    do {
        ep = POP(ex);

        // Want to allow for at least 2 new entries, usually we index into
        // rank_data with rank - 1 so rank + 1 gives us 2 slots we need 2
        // entries because there is no comma for the final expr.
        if (rank + 1 > _rankbuf_size) {
            // need to resize the rank_data array
            _rankbuf_size += 10;
            rank_data = (expr_elt **)Resize((void *)rank_data,
                                            _rankbuf_size * sizeof(expr_elt *));
        }

        switch (ep->e_op) {
            case EXPR_OPCOMMA:
                // When we encounter a comma, we increment the rank counter.
                // Note that we always expect 1 less comma than the actual rank
                ++rank;
                break;

            case EXPR_INT:
                // If ex is empty, then we are at the last entry in the
                // expression tree and are done Note that we do not expect to
                // find a comma for the last entry, so we need to increment the
                // rank before saving the ep.
                if (EMPTY(ex)) {
                    ++rank;
                }

                // offset by 1 because of the comma, it should have already
                // incremented the rank counter or we are at the last entry and
                // already incremented the rank counter above.
                rank_data[rank - 1] = ep;
                break;

            default:
                // Unknown EXPR_OP
                PrintOutError("Syntax error: %s has no meaning.\n",
                              getExprOpStr(ep->e_op));
                PrintOutError("Expected integer.\n");
                FreeExpr(ex);
                Free(rank_data);
                return 0;
        }
    } while (!EMPTY(ex));

    // Set the rank value inside the blockmap definition if is has not been set
    // yet
    if (CurrentBlockMap->m_rank == 0) {
        CurrentBlockMap->m_rank = rank;
    } else {
        // rank has already been set, ensure that what the user provided us
        // matches
        if (CurrentBlockMap->m_rank != rank) {
            PrintOutError(
                "Rank mismatch between previous "
                "dimension/distribute/proc_grid/dim_order definition!\n");
            if (!cli_query("Override warning?")) {
                FreeExpr(ex);
                Free(rank_data);
                return 0;
            }
        }
    }

    // Allocate the data array inside the blockmap definition
    CurrentBlockMap->m_data = (int *)Alloc(sizeof(int) * rank);

    // Loop over the rank_data array and set the global blockmap data entries
    // Remember: Reverse order notation! The entries in the rank_data array are
    // backwards!!
    for (i = 0; i < rank; ++i) {
        CurrentBlockMap->m_data[i] = (rank_data[(rank - 1) - i])->e_int;
    }

    FreeExpr(ex);
    Free(rank_data);

    return 1;
}

static int cmd_blockmap_alt_distribute_cmd(rec_set_t *rec_set, int argc,
                                           char **argv) {
    expr_t *ex;
    expr_elt *ep;
    int rank = 0;
    expr_elt **rank_dist;
    int _rankbuf_size =
        10;  // Default to 10 entries, usually there will not be more than this
    int i;

    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: distribute block,block,*\n");
        return 0;
    }

    if (CurrentBlockMap->m_distribute != (int *)NULL) {
        PrintOutError("Distribute already defined.\n");
        if (!cli_query("Re-define distribute?")) return 1;
    }

    if ((ex = join_and_parse(argc,argv)) == (expr_t *)NULL) {
        PrintError();
        return 0;
    }

    RESET(ex);

    // allocate the rank_dist buffer. This is used to hold onto the EXPR_INT's
    // that correspond to the dimension distribution
    rank_dist = (expr_elt **)Alloc(_rankbuf_size * sizeof(expr_elt *));

    do {
        ep = POP(ex);

        // Want to allow for at least 2 new entries, usually we index into
        // rank_dist with rank - 1 so rank + 1 gives us 2 slots we need 2
        // entries because there is no comma for the final expr.
        if (rank + 1 > _rankbuf_size) {
            // need to resize the rank_dist array
            _rankbuf_size += 10;
            rank_dist = (expr_elt **)Resize((void *)rank_dist,
                                            _rankbuf_size * sizeof(expr_elt *));
        }

        switch (ep->e_op) {
            case EXPR_OPCOMMA:
                // When we encounter a comma, we increment the rank counter.
                // Note that we always expect 1 less comma than the actual rank
                ++rank;
                break;

            case EXPR_SYM:
                // block or cyclic case

                // If ex is empty, then we are at the last entry in the
                // expression tree and are done Note that we do not expect to
                // find a comma for the last entry, so we need to increment the
                // rank before saving the ep.
                if (EMPTY(ex)) {
                    ++rank;
                }

                // Ensure the symbol corresponds to a known command token (i.e.
                // block, cyclic)
                switch (FindCmdTokenStr(ep->e_sym_str, NULL)) {
                    case CT_BLOCK:
                    case CT_CYCLIC:
                        // known symbol
                        rank_dist[rank - 1] = ep;
                        break;

                    default:
                        // Unknown symbol
                        PrintOutError("Syntax error: %s has no meaning.\n",
                                      ep->e_sym_str);
                        FreeExpr(ex);
                        Free(rank_dist);
                        return 0;
                }
                break;

            case EXPR_INT:
                // block-cyclic case

                // If ex is empty, then we are at the last entry in the
                // expression tree and are done Note that we do not expect to
                // find a comma for the last entry, so we need to increment the
                // rank before saving the ep.
                if (EMPTY(ex)) {
                    ++rank;
                }

                // offset by 1 because of the comma, it should have already
                // incremented the rank counter or we are at the last entry and
                // already incremented the rank counter above.
                rank_dist[rank - 1] = ep;
                break;

            case EXPR_STAR:
                // star (wildcard) case

                // If ex is empty, then we are at the last entry in the
                // expression tree and are done Note that we do not expect to
                // find a comma for the last entry, so we need to increment the
                // rank before saving the ep.
                if (EMPTY(ex)) {
                    ++rank;
                }

                // offset by 1 because of the comma, it should have already
                // incremented the rank counter or we are at the last entry and
                // already incremented the rank counter above.
                rank_dist[rank - 1] = ep;
                break;

            default:
                // Unknown EXPR_OP
                PrintOutError("Syntax error: %s has no meaning.\n",
                              getExprOpStr(ep->e_op));
                PrintOutError(
                    "Expected '*', \"block\", \"cyclic\", or integer.\n");
                FreeExpr(ex);
                Free(rank_dist);
                return 0;
        }
    } while (!EMPTY(ex));

    // Set the rank value inside the blockmap definition if is has not been set
    // yet
    if (CurrentBlockMap->m_rank == 0) {
        CurrentBlockMap->m_rank = rank;
    } else {
        // rank has already been set, ensure that what the user provided us
        // matches
        if (CurrentBlockMap->m_rank != rank) {
            PrintOutError(
                "Rank mismatch between previous "
                "dimension/distribute/proc_grid/dim_order definition!\n");
            if (!cli_query("Override warning?")) {
                FreeExpr(ex);
                Free(rank_dist);
                return 0;
            }
        }
    }

    // Allocate the distribute and blockcyclic_val array inside the blockmap
    // definition
    CurrentBlockMap->m_distribute = (int *)Alloc(sizeof(int) * rank);
    CurrentBlockMap->m_blockcyclic_val = (int *)Alloc(sizeof(int) * rank);

    // Loop over the rank_dist array and set the global blockmap distribute and
    // blockcyclic_val entries Remember: Reverse order notation! The entries in
    // the rank_dist array are backwards!!
    for (i = 0; i < rank; ++i) {
        // Set the ep pointer to the current entry in the rank_dist array
        ep = rank_dist[(rank - 1) - i];

        switch (ep->e_op) {
            case EXPR_SYM:
                // We already did a first pass to confirm the grammar, no need
                // for redundant checks here.
                CurrentBlockMap->m_distribute[i] =
                    FindCmdTokenStr(ep->e_sym_str, NULL);
                CurrentBlockMap->m_blockcyclic_val[i] = 0;
                break;

            case EXPR_INT:
                CurrentBlockMap->m_distribute[i] = CT_BLOCK_CYCLIC;
                CurrentBlockMap->m_blockcyclic_val[i] = ep->e_int;
                break;

            case EXPR_STAR:
                CurrentBlockMap->m_distribute[i] = CT_STAR;
                CurrentBlockMap->m_blockcyclic_val[i] = 0;
                break;

            default:
                // something really weird happened.
                PrintOutError("Internal error in distribute_cmd.\n");
                FreeExpr(ex);
                Free(rank_dist);
                Free(CurrentBlockMap->m_distribute);
                CurrentBlockMap->m_distribute = NULL;
                Free(CurrentBlockMap->m_blockcyclic_val);
                CurrentBlockMap->m_blockcyclic_val = NULL;
                return 0;
        }
    }

    FreeExpr(ex);
    Free(rank_dist);

    return 1;
}

static int cmd_blockmap_alt_proc_grid_cmd(rec_set_t *rec_set, int argc,
                                          char **argv) {
    expr_t *ex;
    expr_elt *ep;
    int rank = 0;
    expr_elt **rank_grid;
    int _rankbuf_size =
        10;  // Default to 10 entries, usually there will not be more than this
    int i;

    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: proc_grid 32,64,*\n");
        return 0;
    }

    if (CurrentBlockMap->m_procs != (int *)NULL) {
        PrintOutError("proc_grid already defined.\n");
        if (!cli_query("Re-define proc_grid?")) return 1;
    }

    if ((ex = join_and_parse(argc,argv)) == (expr_t *)NULL) {
        PrintError();
        return 0;
    }

    RESET(ex);

    // allocate the rank_grid buffer. This is used to hold onto the EXPR_INT's
    // that correspond to the dimension grid
    rank_grid = (expr_elt **)Alloc(_rankbuf_size * sizeof(expr_elt *));

    do {
        ep = POP(ex);

        // Want to allow for at least 2 new entries, usually we index into
        // rank_grid with rank - 1 so rank + 1 gives us 2 slots we need 2
        // entries because there is no comma for the final expr.
        if (rank + 1 > _rankbuf_size) {
            // need to resize the rank_grid array
            _rankbuf_size += 10;
            rank_grid = (expr_elt **)Resize((void *)rank_grid,
                                            _rankbuf_size * sizeof(expr_elt *));
        }

        switch (ep->e_op) {
            case EXPR_OPCOMMA:
                // When we encounter a comma, we increment the rank counter.
                // Note that we always expect 1 less comma than the actual rank
                ++rank;
                break;

            case EXPR_INT:
                // If ex is empty, then we are at the last entry in the
                // expression tree and are done Note that we do not expect to
                // find a comma for the last entry, so we need to increment the
                // rank before saving the ep.
                if (EMPTY(ex)) {
                    ++rank;
                }

                // offset by 1 because of the comma, it should have already
                // incremented the rank counter or we are at the last entry and
                // already incremented the rank counter above.
                rank_grid[rank - 1] = ep;
                break;

            case EXPR_STAR:
                // star (wildcard) case

                // If ex is empty, then we are at the last entry in the
                // expression tree and are done Note that we do not expect to
                // find a comma for the last entry, so we need to increment the
                // rank before saving the ep.
                if (EMPTY(ex)) {
                    ++rank;
                }

                // offset by 1 because of the comma, it should have already
                // incremented the rank counter or we are at the last entry and
                // already incremented the rank counter above.
                rank_grid[rank - 1] = ep;
                break;

            default:
                // Unknown EXPR_OP
                PrintOutError("Syntax error: %s has no meaning.\n",
                              getExprOpStr(ep->e_op));
                PrintOutError("Expected '*', or integer.\n");
                FreeExpr(ex);
                Free(rank_grid);
                return 0;
        }
    } while (!EMPTY(ex));

    // Set the rank value inside the blockmap definition if is has not been set
    // yet
    if (CurrentBlockMap->m_rank == 0) {
        CurrentBlockMap->m_rank = rank;
    } else {
        // rank has already been set, ensure that what the user provided us
        // matches
        if (CurrentBlockMap->m_rank != rank) {
            PrintOutError(
                "Rank mismatch between previous "
                "dimension/distribute/proc_grid/dim_order definition!\n");
            if (!cli_query("Override warning?")) {
                FreeExpr(ex);
                Free(rank_grid);
                return 0;
            }
        }
    }

    // Allocate the m_procs array inside the blockmap definition
    CurrentBlockMap->m_procs = (int *)Alloc(sizeof(int) * rank);

    // Loop over the rank_grid array and set the global blockmap dm_procs entry
    // Remember: Reverse order notation! The entries in the rank_grid array are
    // backwards!!
    for (i = 0; i < rank; ++i) {
        // Set the ep pointer to the current entry in the rank_grid array
        ep = rank_grid[(rank - 1) - i];

        switch (ep->e_op) {
            case EXPR_INT:
                CurrentBlockMap->m_procs[i] = ep->e_int;
                break;

            case EXPR_STAR:
                // 1 is the same as being non-distributed
                CurrentBlockMap->m_procs[i] = 1;
                break;

            default:
                // something really weird happened.
                PrintOutError("Internal error in proc_grid_cmd.\n");
                FreeExpr(ex);
                Free(rank_grid);
                Free(CurrentBlockMap->m_procs);
                CurrentBlockMap->m_procs = NULL;
                return 0;
        }
    }

    FreeExpr(ex);
    Free(rank_grid);

    return 1;
}

static int cmd_blockmap_alt_dim_order_cmd(rec_set_t *rec_set, int argc,
                                          char **argv) {
    expr_t *ex;
    expr_elt *ep;
    int rank = 0;
    expr_elt **rank_order;
    int _rankbuf_size =
        10;  // Default to 10 entries, usually there will not be more than this
    int i;

    if (argc < 2) {
        PrintOutError("Not enough command arguments.\n");
        PrintOutError("Expected: dim_order 1,2,*\n");
        return 0;
    }

    if (CurrentBlockMap->m_dim_order != (int *)NULL) {
        PrintOutError("dim_order already defined.\n");
        if (!cli_query("Re-define dim_order?")) return 1;
    }

    if ((ex = join_and_parse(argc,argv)) == (expr_t *)NULL) {
        PrintError();
        return 0;
    }

    RESET(ex);

    // allocate the rank_order buffer. This is used to hold onto the EXPR_INT's
    // that correspond to the dimension order
    rank_order = (expr_elt **)Alloc(_rankbuf_size * sizeof(expr_elt *));

    do {
        ep = POP(ex);

        // Want to allow for at least 2 new entries, usually we index into
        // rank_order with rank - 1 so rank + 1 gives us 2 slots we need 2
        // entries because there is no comma for the final expr.
        if (rank + 1 > _rankbuf_size) {
            // need to resize the rank_order array
            _rankbuf_size += 10;
            rank_order = (expr_elt **)Resize(
                (void *)rank_order, _rankbuf_size * sizeof(expr_elt *));
        }

        switch (ep->e_op) {
            case EXPR_OPCOMMA:
                // When we encounter a comma, we increment the rank counter.
                // Note that we always expect 1 less comma than the actual rank
                ++rank;
                break;

            case EXPR_INT:
                // If ex is empty, then we are at the last entry in the
                // expression tree and are done Note that we do not expect to
                // find a comma for the last entry, so we need to increment the
                // rank before saving the ep.
                if (EMPTY(ex)) {
                    ++rank;
                }

                // offset by 1 because of the comma, it should have already
                // incremented the rank counter or we are at the last entry and
                // already incremented the rank counter above.
                rank_order[rank - 1] = ep;
                break;

            case EXPR_STAR:
                // star (wildcard) case

                // If ex is empty, then we are at the last entry in the
                // expression tree and are done Note that we do not expect to
                // find a comma for the last entry, so we need to increment the
                // rank before saving the ep.
                if (EMPTY(ex)) {
                    ++rank;
                }

                // offset by 1 because of the comma, it should have already
                // incremented the rank counter or we are at the last entry and
                // already incremented the rank counter above.
                rank_order[rank - 1] = ep;
                break;

            default:
                // Unknown EXPR_OP
                PrintOutError("Syntax error: %s has no meaning.\n",
                              getExprOpStr(ep->e_op));
                PrintOutError("Expected '*', or integer.\n");
                FreeExpr(ex);
                Free(rank_order);
                return 0;
        }
    } while (!EMPTY(ex));

    // Set the rank value inside the blockmap definition if is has not been set
    // yet
    if (CurrentBlockMap->m_rank == 0) {
        CurrentBlockMap->m_rank = rank;
    } else {
        // rank has already been set, ensure that what the user provided us
        // matches
        if (CurrentBlockMap->m_rank != rank) {
            PrintOutError(
                "Rank mismatch between previous "
                "dimension/distribute/proc_grid/dim_order definition!\n");
            if (!cli_query("Override warning?")) {
                FreeExpr(ex);
                Free(rank_order);
                return 0;
            }
        }
    }

    // Allocate the m_dim_order array inside the blockmap definition
    CurrentBlockMap->m_dim_order = (int *)Alloc(sizeof(int) * rank);

    // Loop over the rank_order array and set the global blockmap m_dim_order
    // entry Remember: Reverse order notation! The entries in the rank_order
    // array are backwards!!
    for (i = 0; i < rank; ++i) {
        // Set the ep pointer to the current entry in the rank_order array
        ep = rank_order[(rank - 1) - i];

        switch (ep->e_op) {
            case EXPR_INT:
                CurrentBlockMap->m_dim_order[i] = ep->e_int;
                break;

            case EXPR_STAR:
                // CT_STAR is the same as being non-distributed
                // FIXME: If the user actually has a distribution with 155
                // dimensions, this will break. However I don't see this
                // happening any time soon.
                CurrentBlockMap->m_dim_order[i] = CT_STAR;
                break;

            default:
                // something really weird happened.
                PrintOutError("Internal error in dim_order_cmd.\n");
                FreeExpr(ex);
                Free(rank_order);
                Free(CurrentBlockMap->m_dim_order);
                CurrentBlockMap->m_dim_order = NULL;
                return 0;
        }
    }

    FreeExpr(ex);
    Free(rank_order);

    return 1;
}

static int cmd_blockmap_alt_end_cmd(rec_set_t *rec_set, int argc, char **argv) {
    val_t *v;

    if (!VerifyBlockMap(CurrentBlockMap->m_sym)) {
        PrintError();
        if (cli_query("Exit script mode and destroy current decomposition?")) {
            DestroyBlockMap(CurrentBlockMap);
            cli_dec_prompt();
            SetCommands(OldBlockCmds);

            return 1;
        }
        return 0;
    }

    v = NewVal(lval_none, rval_blockmap, CurrentBlockMap);

    if (AssignVal(CurrentBlockMapVal, v, rec_set) < 0) PrintError();

    cli_dec_prompt();
    SetCommands(OldBlockCmds);

    return 1;
}

/********************************
 * Helper functions
 ********************************/

static void blockmapnotfound(const char *cmd) { CmdNotFound(cmd); }

static void blockmapambig(const char *cmd) {
    AmbiguousAltCmd(BlockMapCmds, cmd);
}

static void blockmapfailed(void) {
    // Do nothing
    return;
}

/********************************
 * command init functions
 ********************************/

void InitCmd_blockmap(void) {
    // info commands
    AddInfoCommand("decomposition", proc_ignore, 0,
                   class_info_subcmd_blockmap_cmd,
                   class_info_subcmd_blockmap_help,
                   "Display currently defined decomposition schemes.");

    // usage commands
    AddUsageCommand(
        "dimension", proc_ignore, 0,
        class_usage_subcmd_dimension_cmd,
        class_usage_subcmd_decomposition_help,
        "Display help about using decomposition \"dimension\" command.");
    AddUsageCommand(
        "distribute", proc_ignore, 0,
        class_usage_subcmd_distribute_cmd,
        class_usage_subcmd_decomposition_help,
        "Display help about using decomposition \"distribute\" command.");
    AddUsageCommand(
        "proc_grid", proc_ignore, 0,
        class_usage_subcmd_proc_grid_cmd,
        class_usage_subcmd_decomposition_help,
        "Display help about using decomposition \"proc_grid\" command.");
    AddUsageCommand(
        "dim_order", proc_ignore, 0,
        class_usage_subcmd_dim_order_cmd,
        class_usage_subcmd_decomposition_help,
        "Display help about using decomposition \"dim_order\" command.");

    // regular commands
    AddCommand("decomposition", NULL, proc_ignore, 0,
               cmd_blockmap_cmd, cmd_blockmap_help,
               "Define a decomposition scheme.");

    // blockmap alt commands
    BlockMapCmds =
        NewCommandList(blockmapnotfound, blockmapambig, blockmapfailed);

    AddAltCommand(BlockMapCmds, "dimension", NULL, proc_ignore, 0,
                  cmd_blockmap_alt_dimension_cmd,
                  cmd_blockmap_alt_dimension_help,
                  "Provide a dimension definition.");
    AddAltCommand(BlockMapCmds, "distribute", NULL, proc_ignore, 0,
                  cmd_blockmap_alt_distribute_cmd,
                  cmd_blockmap_alt_distribute_help,
                  "Provide a distribute definition.");
    AddAltCommand(BlockMapCmds, "proc_grid", NULL, proc_ignore, 0,
                  cmd_blockmap_alt_proc_grid_cmd,
                  cmd_blockmap_alt_proc_grid_help,
                  "Provide a process grid definition.");
    AddAltCommand(BlockMapCmds, "dim_order", NULL, proc_ignore, 0,
                  cmd_blockmap_alt_dim_order_cmd,
                  cmd_blockmap_alt_dim_order_help,
                  "Provide a dimension order definition.");
    AddAltCommand(BlockMapCmds, "end", NULL, proc_ignore, 0,
                  cmd_blockmap_alt_end_cmd,
                  cmd_blockmap_alt_end_help,
                  "Finish defining the current decomposition scheme.");
    AddAltCommand(
        BlockMapCmds, "help", "h", proc_ignore, 0, cmd_help_cmd,
        cmd_help_help,
        "Display help information about decomposition script mode commands.");
}

void FinishCmd_blockmap(void) {
    FinishBlockMap();
    DestroyCommandList(BlockMapCmds);
}
