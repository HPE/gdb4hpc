/*********************************************************************************\
 * Block Map routines.
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
 *********************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

#include "aif.h"
#include "bitset.h"
#include "blockmap_util.h"
#include "client_settings.h"
#include "defs.h"
#include "fds.h"
#include "gram.h"
#include "gvar.h"
#include "useful.h"

#define BLOCKMAP_TABLE_SIZE 7 /* 2^7 = 128 = number that can be active */

static htable_t *_block_map_table = (htable_t *)NULL;
static list_t *_block_map_list = (list_t *)NULL;
// int	EvalBlockMapFunc(int *, g_block_map_t *, func_t *, int);

/*
** Find a map.
*/
g_block_map_t *FindBlockMap(char *blockmap) {
    hentry_t *h;

    if ((h = HashSearch(_block_map_table, blockmap)) != (hentry_t *)NULL)
        return (g_block_map_t *)h->h_data;

    return (g_block_map_t *)NULL;
}

int CheckBlockMap(hentry_t *sym) {
    g_block_map_t *m = FindBlockMap(SymLookup(sym));

    return m != (g_block_map_t *)NULL;
}

void SetBlockMap(void) {
    if (_block_map_list != (list_t *)NULL) SetList(_block_map_list);
}

g_block_map_t *GetBlockMap(void) {
    if (_block_map_list == (list_t *)NULL) return (g_block_map_t *)NULL;

    return (g_block_map_t *)GetListElement(_block_map_list);
}

// TODO: When I wrote this (andrewg) I am still not 100% confident on how
// blockmap works. So this function should be made more versatile at some point.
int VerifyBlockMap(hentry_t *sym) {
    g_block_map_t *m;
    int i, count, star_dims;

    if (sym == NULL) {
        SetError("Attempted to lookup null decomposition sym.");
        return 0;
    }

    if ((m = FindBlockMap(SymLookup(sym))) == NULL) {
        SetError("Could not find a decomposition named %s.", SymLookup(sym));
        return 0;
    }

    // Check data definition
    if (m->m_data == (int *)NULL) {
        SetError(
            "Must provide a dimension definition before ending a "
            "decomposition.");
        return 0;
    }

    // Check distribute definition
    if (m->m_distribute == (int *)NULL) {
        SetError(
            "Must provide a distribute definition before ending a "
            "decomposition.");
        return 0;
    }

    // Check grid definition
    if (m->m_procs == (int *)NULL) {
        SetError(
            "Must provide a proc_grid definition before ending a "
            "decomposition.");
        return 0;
    }

    // We need to count the number of non-distributed dimensions to ensure they
    // match between each definition.
    for (i = 0, count = 0; i < m->m_rank; ++i) {
        if (m->m_distribute[i] == CT_STAR) ++count;
    }

    star_dims = count;

    // Check to make sure the grid definition matches
    for (i = 0, count = 0; i < m->m_rank; ++i) {
        if (m->m_procs[i] == 1) ++count;
    }

    if (star_dims != count) {
        SetError(
            "'*' allocation mismatch between distribute and proc_grid "
            "definition.");
        return 0;
    }

    // check order definition
    if (m->m_dim_order == (int *)NULL) {
        // If this is a 1-D distribution we can assume the dimension ordering
        if (m->m_rank - star_dims == 1) {
            // note here that count now refers to the number of "numbers"
            // written to make sure there isn't an issue in the logic
            count = 0;
            m->m_dim_order = (int *)Alloc(sizeof(int) * m->m_rank);
            for (i = 0; i < m->m_rank; ++i) {
                if (m->m_distribute[i] == CT_STAR) {
                    m->m_dim_order[i] = CT_STAR;
                } else {
                    m->m_dim_order[i] = 1;
                    ++count;
                }
            }
            if (count != 1) {
                // This should never happen
                SetError(
                    "'*' allocation mismatch between distribute and dim_order "
                    "definitions.");
                return 0;
            }
        } else {
            SetError(
                "Must provide a dim_order definition before ending a "
                "decomposition.");
            return 0;
        }
    } else {
        // Check to make sure the order definition matches
        for (i = 0, count = 0; i < m->m_rank; ++i) {
            if (m->m_dim_order[i] == CT_STAR) ++count;
        }

        if (star_dims != count) {
            SetError(
                "'*' allocation mismatch between distribute and dim_order "
                "definition.");
            return 0;
        }
    }

    return 1;
}

/*
** Create a new map.
*/
g_block_map_t *NewBlockMap(hentry_t *sym) {
    g_block_map_t *m;

    m = (g_block_map_t *)Alloc(sizeof(g_block_map_t));
    m->m_sym = sym;
    m->m_distribute = (int *)NULL;
    m->m_data = (int *)NULL;
    m->m_procs = (int *)NULL;
    m->m_blockcyclic_val = (int *)NULL;
    m->m_dim_order = (int *)NULL;
    m->m_rank = 0;
    m->m_builtin = (int (*)(list_t *))NULL;
    m->m_current = 0;
    m->m_debug = 0;

    if (sym == (hentry_t *)NULL) return m;

    if (_block_map_list == (list_t *)NULL) _block_map_list = NewList();

    if (_block_map_table == (htable_t *)NULL)
        _block_map_table = HashCreate(BLOCKMAP_TABLE_SIZE);

    if (HashInsert(_block_map_table, SymLookup(sym), (void *)m) ==
        (hentry_t *)NULL) {
        SetError("Too many decompositions defined");
        return (g_block_map_t *)NULL;
        ;
    }

    AddToList(_block_map_list, (void *)m);

    return m;
}

/*
** Free a map.
*/
void DestroyBlockMap(g_block_map_t *m) {
    hentry_t *h;

    if (m == (g_block_map_t *)NULL) return;

    if ((h = HashSearch(_block_map_table, SymLookup(m->m_sym))) !=
        (hentry_t *)NULL)
        HashRemove(_block_map_table, h);

    RemoveFromList(_block_map_list, (void *)m);

    if (m->m_distribute != (int *)NULL) Free(m->m_distribute);
    if (m->m_data != (int *)NULL) Free(m->m_data);
    if (m->m_procs != (int *)NULL) Free(m->m_procs);
    if (m->m_blockcyclic_val != (int *)NULL) Free(m->m_blockcyclic_val);
    if (m->m_dim_order != (int *)NULL) Free(m->m_dim_order);

    Free(m);
}

char *_find_result_type(g_block_map_t *m, char *type, const char *hashinfo,
                        int *min, int *max, int *d_min, int *d_max, int num_procs) {
    char *basetype;
    char *tok;
    char *result;
    int hashmode = 0, p2pmode = 0;
    int rank, i;
    char tmpstr[BUFSIZ];
    int arraytype;

    basetype = FDSBaseType(type);
    if (basetype == NULL) basetype = type;

    switch (FDSType(basetype)) {
        case AIF_HASHED_SIGNATURE:
            hashmode = 1;
            // TODO: Is it okay to assume that a AIF_HASHED_SIGNATURE should
            // always use c-type arrays???
            arraytype = AIF_ARRAY;
            break;

        case AIF_FTN_INTEGER:
        case AIF_FTN_REAL:
        case AIF_FTN_COMPLEX:
        case AIF_FTN_LOGICAL:
        case AIF_FTN_CHARACTER:
            // fortran types should build a fortran array
            arraytype = AIF_FTN_ARRAY;
            break;

        default:
            // everything else should build a c array
            arraytype = AIF_ARRAY;
            break;
    }

    GetGraphVar(GRAPH_P2PMODE, (int *)&p2pmode);
    if (hashmode) {
        rank = m->m_rank;

        if (rank == 1) {
            min[0] = 0;
            max[0] = num_procs - 1;
        } else if (rank > 1) {
            // parse hashinfo to get result array dimensions
            strcpy(tmpstr, hashinfo);
            tok = strtok(tmpstr, ",");
            for (i = 0; i < rank; i++) {
                min[i] = 0;
                max[i] = m->m_data[i] / atoi(tok) - 1;
                tok = strtok(NULL, ",");
            }
        }
    } else if (p2pmode) {
        // just a 1D array of results
        rank = 1;
        min[0] = 0;
        // need to work out the number of result elements for P2P
        max[0] = (num_procs * (d_max[0] - d_min[0] + 1)) - 1;  // assume just 1 for now
    } else {
        rank = m->m_rank;
        for (i = 0; i < rank; i++) {
            min[i] = 0;
            max[i] = m->m_data[i] - 1;
        }
    }

    if (arraytype == AIF_FTN_ARRAY)  // revert the order of max array
    {
        int j;
        int *tmpmax = (int *)Alloc(sizeof(int) * rank);

        for (i = 0; i < rank; i++) tmpmax[i] = max[i];

        j = rank - 1;
        for (i = 0; i < rank; i++) {
            max[i] = tmpmax[j];
            j--;
        }
        Free(tmpmax);
    }

    result = FDSMultiRankArray(min, max, rank, basetype, arraytype);

    return result;
}

static AIF *ApplyBlockMap_old(g_block_map_t *m, char *type, int nd, AIF **data,
                   const char *hashinfo) {
    int i, j;

    int *d_min = NULL;
    int *d_max = NULL;
    int d_rank = 0;  // local rank can differ from global rank
    int *min = NULL;
    int *max = NULL;
    int *hash_vals = NULL;

    int process_index, p2pmode = 0;
    int expect_num_elems = 1;
    int actual_num_elems = 0;

    char *r_ptr;
    char *d_ptr;

    AIF *res = NULL;
    AIF *tmpres;
    AIFIndex *dix;

    char *basetype;
    int hashmode = 0;
    int tmp_var;

    basetype = FDSBaseType(type);
    if (basetype == NULL) basetype = type;

    GetGraphVar(GRAPH_P2PMODE, (int *)&p2pmode);
    hashmode = (FDSType(basetype) == AIF_HASHED_SIGNATURE) ? 1 : 0;

    /*#ifdef DEBUG
            // print out all the block "chunks"
            for (i=0; i<nd; ++i)
            {
                    fprintf(out_stream, "\nblock %d is:\n", i);
                    AIFPrint(out_stream, 0, data[i]);
                    fprintf(out_stream, "\n");
                    fflush(out_stream);
            }
    #endif*/

    /*
     * now need to determine result array type by going through
     * the data in AIF data array
     * FIXME: Here we assume that all data is the same. However it could be
     *         the case where this is not true. We should make a check to ensure
     *         that all of the block chunks have the same fds type. I am
     * thinking of a simple loop that does a strcmp on data[0] to every other
     * element up to nd. If one differs we have a problem.
     * FIXME: In the future this might not be true though. Such is the case for
     *        AMR codes..
     */

    d_rank = FDSArrayRank(type);

    switch (FDSType(type)) {
        case AIF_ARRAY:
        case AIF_FTN_ARRAY:
            FDSArrayBounds(type, d_rank, &d_min, &d_max, NULL);
            break;

        default:
            break;
    }

    min = (int *)Alloc(m->m_rank * sizeof(int));
    max = (int *)Alloc(m->m_rank * sizeof(int));

    if (hashmode) {
        char tmpstr[BUFSIZ];
        char *tok;

        hash_vals = (int *)Alloc(m->m_rank * sizeof(int));
        strcpy(tmpstr, hashinfo);
        // parse hashinfo to get result array dimensions
        tok = strtok(tmpstr, ",");
        for (i = 0; i < m->m_rank; i++) {
            hash_vals[i] = atoi(tok);
            tok = strtok(NULL, ",");
        }
    }

    // It is sort of confusing to not use a temporary pointer for type, but
    // since we only free type after this point it technically is valid.
    type = _find_result_type(m, type, hashinfo, min, max, d_min, d_max, nd);

    // now check if expect number of elements is the same with the actual number
    // of elements
    bool isDegenerate = true;
    for (i = 0; i < m->m_rank; ++i) {
        if (m->m_distribute[i] != CT_STAR)
            isDegenerate = false;
    }
       
    if (p2pmode)
        expect_num_elems = max[0] - min[0] + 1;
    else {
        for (i = 0; i < m->m_rank; i++) expect_num_elems *= max[i] - min[i] + 1;
    }

    for (i = 0; i < nd; i++) {
        if (isDegenerate)
            actual_num_elems = 0;

        switch (FDSType(AIF_FORMAT(data[i]))) {
            case AIF_ARRAY:
            case AIF_FTN_ARRAY:
                actual_num_elems += AIFArraySize(data[i]);
                break;

            default:
                actual_num_elems++;
                break;
        }

        if (isDegenerate && expect_num_elems != actual_num_elems)
            break;
    }
    if (expect_num_elems != actual_num_elems) {
        SetError(
            "Wrong number of elements to map %d vs %d. Please check "
            "decomposition function '%s'",
            expect_num_elems, actual_num_elems, SymLookup(m->m_sym));
        if (type != (char *)NULL) Free(type);
        if (min != (int *)NULL) Free(min);
        if (max != (int *)NULL) Free(max);
        return (AIF *)NULL;
    }

    // If m->m_rank does not match d_rank, which is the case when a global
    // array has been redefined in fortran, the d_min and d_max values from
    // above are invalid.
    if (d_rank != m->m_rank) {
        if (d_min != NULL) Free(d_min);
        if (d_max != NULL) Free(d_max);
        // FIXME: d_min is never used currently. This is a flaw in the entire
        //         design.
        // UPDATE: Its now used by a Monash patch, but its not being used
        // properly
        //         since we want to know the min value of the global data array
        //         and thats not being passed in.
        // UPDATE2: But the above might not matter. If this is only operating on
        //          blockmap data definitions, the user doesn't provide a min
        //          value. They only provide the "number" of elements.
        d_min = (int *)Alloc(m->m_rank * sizeof(int));
        d_max = (int *)Alloc(m->m_rank * sizeof(int));

        // here we know we have the correct number of elements, but the
        // dimension is wrong. We need to interpret this based on the expected
        // distribution.
        for (i = 0; i < m->m_rank; ++i) {
            switch (m->m_distribute[i]) {
                case CT_STAR:
                    d_min[i] = 0;
                    d_max[i] = m->m_data[i]-1;
                    break;
                case CT_BLOCK:
                case CT_CYCLIC:
                    // Is this really the case??
                    d_min[i] = 0;
                    // Determine the block size. This is global data size
                    // divided by the number of ranks in the proc_grid
                    // FIXME: This doesn't handle remainder problems...
                    d_max[i] = m->m_data[i] / m->m_procs[i] - 1;
                    if (d_max[i] < 0)
                        d_max[i] = 0;	
                    break;
                case CT_BLOCK_CYCLIC:
                    // FIXME: Not implemented yet.
                    // break;
                default:
                    SetError(
                        "local data from rank doesn't match decomposition "
                        "dimension and our automated translation failed. "
                        "Please file a bug report.");
                    if (type != (char *)NULL) Free(type);
                    if (min != (int *)NULL) Free(min);
                    if (max != (int *)NULL) Free(max);
                    if (d_max != (int *)NULL) Free(d_max);
                    return (AIF *)NULL;
            }
        }

        switch (FDSType(AIF_FORMAT(data[0]))) {
            case AIF_FTN_ARRAY:
                // For fortran we need to transpose the above calculation since
                // we do our work with c-type arrays.
                // TODO: If the min value is supported
                j = m->m_rank - 1;
                for (i = 0; i < j; ++i) {
                    tmp_var = d_max[j];
                    d_max[j--] = d_max[i];
                    d_max[i] = tmp_var;
                }
            default:
                // do nothing.
                break;
        }
    }

    switch (FDSType(type)) {
        case AIF_ARRAY:
            m->m_ix = FDSArrayIndexInit(type);
            break;

        case AIF_FTN_ARRAY:
            m->m_ix = FDSFtnArrayIndexInit(type);
            break;

        default:
            SetError("Result type of decomposition is not an array");
            // Memory Leak
            if (type != (char *)NULL) Free(type);
            if (min != (int *)NULL) Free(min);
            if (max != (int *)NULL) Free(max);
            return (AIF *)NULL;
    }

    if (nd <= 0) {
        SetError("Bad number of processes: %d", nd);
        // Memory Leak
        if (type != (char *)NULL) Free(type);
        if (min != (int *)NULL) Free(min);
        if (max != (int *)NULL) Free(max);
        return (AIF *)NULL;
    }

    m->m_current = 1;

    tmpres = MakeAIF(strdup(type), NULL);
    Free(type);  // Memory Leak: Need to free the type string.

    if (p2pmode || m->m_rank == 1) {
        for (i = 0; i < nd; i++) {
            int num_elem = 1;
            process_index = i;

            switch (FDSType(AIF_FORMAT(data[process_index]))) {
                case AIF_ARRAY:
                case AIF_FTN_ARRAY:
                    dix = AIFArrayIndexInit(data[process_index]);
                    num_elem = dix->i_nel;
                    AIFArrayIndexFree(dix);
                    break;

                default:
                    break;
            }

            d_ptr = AIF_DATA(data[process_index]);
            r_ptr = AIF_DATA(tmpres) + i * num_elem * m->m_ix->i_bsize;
            // Copy data into result
            memcpy(r_ptr, d_ptr, m->m_ix->i_bsize * num_elem);
        }
        res = tmpres;
    } else if (m->m_rank > 1) {
        int *global_data = NULL;
        int *local_data = NULL;
        int *blockcyclic_val = NULL;
        int *block_size = NULL;
        int *proc_grid = NULL;
        int *dim_order = NULL;
        int *order_map = NULL;

        block_size = (int *)Alloc(m->m_rank * sizeof(int));
        global_data = (int *)Alloc(m->m_rank * sizeof(int));
        local_data = (int *)Alloc(m->m_rank * sizeof(int));
        blockcyclic_val = (int *)Alloc(m->m_rank * sizeof(int));
        proc_grid = (int *)Alloc(m->m_rank * sizeof(int));
        dim_order = (int *)Alloc(m->m_rank * sizeof(int));
        order_map = (int *)Alloc(m->m_rank * sizeof(int));

        switch (FDSType(AIF_FORMAT(data[0]))) {
            case AIF_ARRAY:
                if (hashmode) {
                    for (i = 0; i < m->m_rank; i++) {
                        global_data[i] = m->m_data[i] / hash_vals[i];
                        local_data[i] = d_max[i] - d_min[i] + 1;
                        blockcyclic_val[i] =
                            m->m_blockcyclic_val[i] / hash_vals[i];
                        proc_grid[i] = m->m_procs[i];
                        dim_order[i] = m->m_dim_order[i];
                    }
                } else {
                    for (i = 0; i < m->m_rank; i++) {
                        global_data[i] = m->m_data[i];
                        local_data[i] = d_max[i] - d_min[i] + 1;
                        blockcyclic_val[i] = m->m_blockcyclic_val[i];
                        proc_grid[i] = m->m_procs[i];
                        dim_order[i] = m->m_dim_order[i];
                    }
                }

                _get_block_size(m->m_rank, local_data, m->m_distribute,
                                blockcyclic_val, proc_grid, block_size);

                goto finish_blockmap;

            case AIF_FTN_ARRAY:
                // the difference between a fortran array and a c array is
                // that we need to reverse the order for the blockmap definition
                // data since the matrix is a transpose.
                if (hashmode) {
                    j = 0;
                    for (i = m->m_rank - 1; 0 <= i; --i) {
                        global_data[j] = m->m_data[i] / hash_vals[i];
                        blockcyclic_val[j] =
                            m->m_blockcyclic_val[i] / hash_vals[i];
                        local_data[i] = d_max[i] - d_min[i] + 1;
                        ++j;
                    }
                } else {
                    j = 0;
                    for (i = m->m_rank - 1; 0 <= i; --i) {
                        global_data[j] = m->m_data[i];
                        blockcyclic_val[j] = m->m_blockcyclic_val[i];
                        local_data[i] = d_max[i] - d_min[i] + 1;
                        ++j;
                    }
                }

                // we also need to reverse the distribute and procs array
                {
                    int *distribute_trans = NULL;
                    distribute_trans = (int *)Alloc(m->m_rank * sizeof(int));

                    j = 0;
                    for (i = m->m_rank - 1; 0 <= i; --i) {
                        distribute_trans[j] = m->m_distribute[i];
                        proc_grid[j] = m->m_procs[i];
                        dim_order[j] = m->m_dim_order[i];
                        ++j;
                    }
                    _get_block_size(m->m_rank, local_data, distribute_trans,
                                    blockcyclic_val, proc_grid, block_size);
                    Free(distribute_trans);
                }

            finish_blockmap:
                _get_order_map(m->m_rank, dim_order, order_map);

                /*#ifdef DEBUG
                        fprintf(out_stream, "\nproc_grid is: ");
                        for (i=0; i<m->m_rank; i++)
                        {
                                fprintf(out_stream, "%d ", proc_grid[i]);
                        }
                        fprintf(out_stream, "\n");
                        fprintf(out_stream, "\ndim_order is: ");
                        for (i=0; i<m->m_rank; i++)
                        {
                                fprintf(out_stream, "%d ", dim_order[i]);
                        }
                        fprintf(out_stream, "\n");
                        fprintf(out_stream, "\norder_map is: ");
                        for (i=0; i<m->m_rank; i++)
                        {
                                fprintf(out_stream, "%d ", order_map[i]);
                        }
                        fprintf(out_stream, "\n");
                        fprintf(out_stream, "\nblock_size is: ");
                        for (i=0; i<m->m_rank; i++)
                        {
                                fprintf(out_stream, "%d ", block_size[i]);
                        }
                        fprintf(out_stream, "\n");
                #endif*/

                /* now need to determine result array type by going through
                 * the data in AIF data array
                 */
                for (i = 0; i < nd; i++) {
                    int num_blocks, num_copies;
                    int *local_start_points = NULL;
                    int *global_start_points = NULL;
                    int *pid = NULL;

                    num_blocks =
                        _get_num_blocks(m->m_rank, local_data, block_size);
                    num_copies = product(block_size, m->m_rank);
                    local_start_points = (int *)Alloc(num_blocks * sizeof(int));
                    global_start_points =
                        (int *)Alloc(num_blocks * sizeof(int));
                    pid = (int *)Alloc(m->m_rank * sizeof(int));

                    _get_pid_grid(m->m_rank, i, proc_grid, order_map, pid);

                    _get_local_indices(m->m_rank, num_blocks, block_size,
                                       local_data, local_start_points);

                    _get_global_indices(m->m_rank, num_blocks, block_size, pid,
                                        order_map, proc_grid, global_data,
                                        local_data, global_start_points);

                    /*#ifdef DEBUG
                            int index;

                            fprintf(out_stream, "\nnum_blocks - num_copies -
                    pindex: %d - %d - %d\n", num_blocks, num_copies, i);
                            fprintf(out_stream, "\npid is: ");
                            for (index=0; index<m->m_rank; index++)
                            {
                                    fprintf(out_stream, "%d ", pid[index]);
                            }
                            fprintf(out_stream, "\n");
                            fprintf(out_stream, "\nlocal_indices is: ");
                            for (index=0; index<num_blocks; index++)
                            {
                                    fprintf(out_stream, "%d ",
                    local_start_points[index]);
                            }
                            fprintf(out_stream, "\n");
                            fprintf(out_stream, "\nglobal_indices is: ");
                            for (index=0; index<num_blocks; index++)
                            {
                                    fprintf(out_stream, "%d ",
                    global_start_points[index]);
                            }
                            fprintf(out_stream, "\n");

                    #endif*/

                    for (j = 0; j < num_blocks; j++) {
                        d_ptr = AIF_DATA(data[i]) +
                                (local_start_points[j] * m->m_ix->i_bsize);
                        r_ptr = AIF_DATA(tmpres) +
                                (global_start_points[j] * m->m_ix->i_bsize);
                        memcpy(r_ptr, d_ptr, m->m_ix->i_bsize * num_copies);
                    }
                    Free(local_start_points);   // Memory leak: free the
                                                // start_points
                    Free(global_start_points);  // Memory leak: free the
                                                // start_points
                    Free(pid);  // Memory leak: free the start_points
                }
                break;

            default:
                for (i = 0; i < m->m_rank; i++) {
                    global_data[i] = m->m_data[i];
                    blockcyclic_val[i] = m->m_blockcyclic_val[i];
                    proc_grid[i] = m->m_procs[i];
                    dim_order[i] = m->m_dim_order[i];
                }

                _get_order_map(m->m_rank, dim_order, order_map);

                for (i = 0; i < nd; i++) {
                    int global_start_point, k;
                    int *pid = NULL;

                    pid = (int *)Alloc(m->m_rank * sizeof(int));

                    _get_pid_grid(m->m_rank, i, proc_grid, order_map, pid);
                    // convert pid into global index
                    global_start_point = pid[m->m_rank - 1];
                    for (j = 0; j < m->m_rank - 1; j++) {
                        int tmp2 = pid[j];
                        for (k = j + 1; k < m->m_rank; k++) {
                            tmp2 *= global_data[k];
                        }
                        global_start_point += tmp2;
                    }

                    d_ptr = AIF_DATA(data[i]);
                    r_ptr = AIF_DATA(tmpres) +
                            global_start_point * m->m_ix->i_bsize;

                    // Copy data into result
                    memcpy(r_ptr, d_ptr, m->m_ix->i_bsize);

                    Free(pid);
                }
                break;
        }

        Free(block_size);
        Free(blockcyclic_val);
        Free(global_data);
        Free(proc_grid);
        Free(dim_order);
        Free(order_map);

        res = tmpres;
    }

    AIFArrayIndexFree(m->m_ix);
    m->m_current = 0;

    if (d_min != NULL) Free(d_min);
    if (d_max != NULL) Free(d_max);
    if (min != NULL) Free(min);
    if (max != NULL) Free(max);
    if (hash_vals != NULL) Free(hash_vals);

    return res;
}

namespace {
// handle the data distribution in a single dimension
class indexer1D {
public:
    indexer1D(const g_block_map_t* m, int dimIdx, const indexer1D* prev);
    indexer1D() : m_dimIdx(0), m_len(0) {}
    void map(size_t idx, size_t& procIdx, size_t& dataIdx, size_t& repeat) const;
    size_t getTotalProcSize() const { return m_totalProcSize; }
    size_t getTotalDataSize() const { return m_dataScale * m_len / m_numProcs; }
    int getDimIdx() const { return m_dimIdx; }

private:
    int m_dimIdx;               // dimension represented
    int m_distribute;           // distribute scheme (from cmdtoken.h)
    size_t m_len;               // number of elements in this dimension
    size_t m_numProcs;          // number of process for distibuting 
    size_t m_blockCyclic;       // cycle count for the CT_BLOCK_CYCLIC

    size_t m_blockLen;          // number of elements in each block
    size_t m_totalProcSize;     // total processes from this and faster varying dims
    size_t m_dataScale;         // step in the data array for each subsequent element
    size_t m_procScale;         // step in the rank for each subsequent element
};

// define the distribution in single dimension
// \param m is the block map definition
// \param d is the index for this dimension, starting at 0
// \param prev if given is the distribution for the next faster dimension
indexer1D::indexer1D(const g_block_map_t* m, int d,
                               const indexer1D* prev) :
    m_dimIdx(d),
    m_distribute(m->m_distribute[d]),
    m_len(m->m_data[d]),
    m_numProcs(m->m_procs[d]),
    m_blockCyclic(m->m_blockcyclic_val[d]),
    m_blockLen(m_len / m_numProcs),
    m_totalProcSize(m_numProcs * (prev ? prev->getTotalProcSize() : 1)),
    m_dataScale(prev ? prev->getTotalDataSize() : 1),
    m_procScale(1)
{
    // the decomposition allows the distributed dimension to be reordered
    if (m_distribute != CT_STAR) {
        int dimOrder = m->m_dim_order[d];
        for (int i=0; i<m->m_rank; ++i) {
            if (m->m_distribute[i] != CT_STAR && m->m_dim_order[i] < dimOrder)
                m_procScale *= m->m_procs[i];
        }
    }
}

// map an index int the global array into the offsets into the
// array of process ranks and the data array from that rank
// \param idx is the index of the global array in this dimension,
//      it always has origin 0
// \param procIdx i/o is updated with the offset into the array of ranks
//      it is unchanged if this dimension is not decomposed
// \param repeat will contain on exit the number of elements in to row.
//      note that the result will only be used for the fastest varying
//      dimension because \a repeat elements will be contiguously stored
//      in the data result for that rank so they can be copied in one shot
void indexer1D::map(size_t idx, size_t& procIdx, size_t& dataIdx,
                         size_t& repeat) const {
    repeat = 1;
    size_t dproc = 0, ddata = 0;

    switch (m_distribute) {
    case CT_BLOCK:
        dproc = idx / m_blockLen;
        ddata = idx % m_blockLen;
        repeat = m_blockLen;
        break;
    case CT_CYCLIC:
        dproc = idx % m_numProcs;
        ddata = idx / m_numProcs;
        break;
    case CT_STAR:
        dproc = 0;
        ddata = idx;
        repeat = m_len;
        break;
    case CT_BLOCK_CYCLIC:
        dproc = (idx / m_blockCyclic) % m_numProcs;
        ddata =
            m_blockCyclic * (idx / (m_blockCyclic * m_numProcs)) +
            idx % m_blockCyclic;
        repeat = m_blockCyclic;
        break;
    default:  // checked at construction time
        break;
    }

    procIdx += dproc * m_procScale;
    dataIdx += ddata * m_dataScale;
}
}

// create a single unified AIF value for a decomposed data element
// \param m is the block map definition
// \param d_type is the datatype of the data elements
// \param nd is the number of results, i.e. ranks
// \param data is result from evalating each rank, in order
// \param hashinfo used for hash reduction of the result
AIF* ApplyBlockMap(g_block_map_t *m, char *d_type, int nd, AIF **data,
                   const char *hashinfo) {

    if (nd <= 0) {
        SetError("Bad number of processes: %d", nd);
        return nullptr;
    }

    // p2p and hash mode are indifferently supported at the moment.
    // Just let these fall back into the old implemnetation until
    // we can revisit them.
    char* basetype = FDSBaseType(d_type);
    if (!basetype) basetype = d_type;

    int p2pmode;
    GetGraphVar(GRAPH_P2PMODE, &p2pmode);

    if (p2pmode || (FDSType(basetype) == AIF_HASHED_SIGNATURE))
        return ApplyBlockMap_old(m, d_type, nd, data, hashinfo);

    int d_rank = FDSArrayRank(d_type);
    int* d_min = nullptr, *d_max = nullptr;
    bool isFortran = false;

    // get the array limits - compute d_min and d_max for each dimension
    switch (FDSType(d_type)) {
        case AIF_FTN_ARRAY:
            isFortran = true;   // intentionally falling through
        case AIF_ARRAY:
            FDSArrayBounds(d_type, d_rank, &d_min, &d_max, nullptr);
            break;

        default:
            break;
    }

    // get the result type - note that this call maps all min[i] to 0 and
    // reverses the order for fortan.  (though it doesn't matter because
    // min and max are unused - we get dimension directly from the block map
    int rank = m->m_rank;
    std::vector<int>  min(rank);
    std::vector<int>  max(rank);
    char* type = _find_result_type(m, d_type, hashinfo, &min[0], &max[0], d_min, d_max, nd);

    if (d_min)
        Free(d_min);
    if (d_max)
        Free(d_max);
    d_min = d_max = nullptr;


    // the decomposition is handled indendently in each dimension, so we create
    // a little class object to handle the decomposition in a single dimension.
    // We want the list ordered from slowest chaning to fastest changing
    std::vector<indexer1D> dimMaps(rank);
    for (int i=rank; i--; ) {
        indexer1D d(m, (isFortran ? rank-1-i : i),
                         i+1==rank ? nullptr : &dimMaps[i+1]);
        dimMaps[i] = d;
    }

    // check that the actual number of ranks matches the decomposition.
    // totalProcs == 1 is the special case where the data is not decomposed;
    // the blockmap is just potentially redefining the shape of distributed data
    size_t totalProcs = dimMaps[0].getTotalProcSize();
    if (totalProcs > 1 && int(totalProcs) != nd ) {
        SetError(
            "Wrong number of PEs to map %d vs %d. Please check "
            "decomposition function '%s'",
            int(totalProcs), nd, SymLookup(m->m_sym));
        if (type) Free(type);
        return nullptr;

    }

    // check that the data sizes match up
    size_t expect_num_elems = totalProcs * dimMaps[0].getTotalDataSize();
    size_t actual_num_elems = 0;

    for (int i = 0; i < nd; i++) {
        if (totalProcs == 1)
            actual_num_elems = 0;

        switch (FDSType(AIF_FORMAT(data[i]))) {
            case AIF_ARRAY:
            case AIF_FTN_ARRAY:
                actual_num_elems += AIFArraySize(data[i]);
                break;

            default:
                actual_num_elems++;
                break;
        }

        if (totalProcs == 1 && expect_num_elems != actual_num_elems)
            break;
    }

    if (expect_num_elems != actual_num_elems) {
        SetError(
            "Wrong number of elements to map %zu vs %zu. Please check "
            "decomposition function '%s'",
            expect_num_elems, actual_num_elems, SymLookup(m->m_sym));
        if (type) Free(type);
        return nullptr;
    }


    // Looks good, create the result
    AIF* res = MakeAIF(strdup(type), NULL);
    size_t bsize = FDSTypeSize(basetype);

    // And copy in the values
    std::vector<size_t> globalIdx(rank, size_t(0));
    std::vector<size_t> globalLen(m->m_data, m->m_data+rank);

    size_t repeat = 1;
    for (size_t globalCnt = 0; globalCnt < expect_num_elems; globalCnt += repeat ) {
        size_t procIdx = 0, dataIdx = 0;
        for (const auto& dd : dimMaps) {
            dd.map( globalIdx[dd.getDimIdx()], procIdx, dataIdx, repeat);
        }

        void * g_ptr = AIF_DATA(res) + globalCnt * bsize;
        void * d_ptr = AIF_DATA(data[procIdx]) + dataIdx * bsize;
        memcpy(g_ptr, d_ptr, bsize * repeat);

        size_t inc = repeat;
        for (int i=rank; i--; ) {
            int d = dimMaps[i].getDimIdx();
            if ((globalIdx[d] += inc) < globalLen[d])
                break;
            globalIdx[d] = 0;
            inc = 1;
        }
    }

    Free(type);
    return res;
}

// for now remove EvalMapFunc, when need, copy from map.c

void PrintBlockMap(FILE *fp, g_block_map_t *m) {
    int i;

    PrintToFd(fp, "$%s:\n", SymLookup(m->m_sym));

    if (m->m_rank == 0) {
        SetError("Invalid decomposition.");
        return;
    }

    PrintToFd(fp, "\tdimension ");

    if (m->m_data == (int *)NULL) {
        SetError("Invalid dimension definition for decomposition.");
        return;
    } else {
        // want to stop before last index
        for (i = 0; i < (m->m_rank - 1); i++) {
            PrintToFd(fp, "%d,", m->m_data[i]);
        }
        // now print the last index
        PrintToFd(fp, "%d\n", m->m_data[m->m_rank - 1]);
    }

    PrintToFd(fp, "\tdistribute ");

    if (m->m_distribute == (int *)NULL) {
        SetError("Invalid distribute definition for decomposition.");
        return;
    } else {
        // want to stop before last index
        for (i = 0; i < (m->m_rank - 1); i++) {
            switch (m->m_distribute[i]) {
                case CT_BLOCK:
                    PrintToFd(fp, "%s,", "block");
                    break;
                case CT_CYCLIC:
                    PrintToFd(fp, "%s,", "cyclic");
                    break;
                case CT_STAR:
                    PrintToFd(fp, "%s,", "*");
                    break;
                case CT_BLOCK_CYCLIC:
                    PrintToFd(fp, "%d,", m->m_blockcyclic_val[i]);
                    break;
                default:
                    SetError(
                        "Invalid distribute definition for decomposition.");
                    return;
            }
        }
        // need to print the last index
        switch (m->m_distribute[m->m_rank - 1]) {
            case CT_BLOCK:
                PrintToFd(fp, "%s\n", "block");
                break;
            case CT_CYCLIC:
                PrintToFd(fp, "%s\n", "cyclic");
                break;
            case CT_STAR:
                PrintToFd(fp, "%s\n", "*");
                break;
            case CT_BLOCK_CYCLIC:
                PrintToFd(fp, "%d\n", m->m_blockcyclic_val[m->m_rank - 1]);
                break;
            default:
                SetError("Invalid distribute definition for decomposition.");
                return;
        }
    }

    PrintToFd(fp, "\tproc_grid ");

    if (m->m_procs == (int *)NULL) {
        SetError("Invalid process grid definition for decomposition.");
        return;
    } else {
        // want to stop before last index
        for (i = 0; i < (m->m_rank - 1); i++) {
            if (m->m_procs[i] == 1)
                PrintToFd(fp, "%s,", "*");
            else
                PrintToFd(fp, "%d,", m->m_procs[i]);
        }
        // now print the last index
        if (m->m_procs[i] == 1)
            PrintToFd(fp, "%s\n", "*");
        else
            PrintToFd(fp, "%d\n", m->m_procs[m->m_rank - 1]);
    }

    PrintToFd(fp, "\tdim_order ");

    if (m->m_dim_order == (int *)NULL) {
        SetError("Invalid dimension order definition for decomposition.");
        return;
    } else {
        // want to stop before last index
        for (i = 0; i < (m->m_rank - 1); i++) {
            if (m->m_dim_order[i] == CT_STAR)
                PrintToFd(fp, "%s,", "*");
            else
                PrintToFd(fp, "%d,", m->m_dim_order[i]);
        }
        // now print the last index
        if (m->m_dim_order[i] == CT_STAR)
            PrintToFd(fp, "%s\n", "*");
        else
            PrintToFd(fp, "%d\n", m->m_dim_order[m->m_rank - 1]);
    }

    PrintToFd(fp, "\n");
}

void FinishBlockMap(void) {
    g_block_map_t *m;

    SetList(_block_map_list);

    while ((m = (g_block_map_t *)GetListElement(_block_map_list)) !=
           (g_block_map_t *)NULL)
        DestroyBlockMap(m);

    DestroyList(_block_map_list, (void (*)(void *))NULL);
    HashDestroy(_block_map_table, (void (*)(char *, void *))NULL);
}

/* This function turn the given blockmap function into a description string for
 * p2p comp purposes 21/06/2011 - Minh Dinh
 */
void BlockMapToString(g_block_map_t *m, var_t *v, int numprocs, char *str) {
    int i;
    char buf[100];

    sprintf(buf, "$%s,%d,%d,", v->v_pname, numprocs, m->m_rank);
    strcat(str, buf);
    for (i = 0; i < m->m_rank; i++) {
        sprintf(buf, "%d,", m->m_distribute[i]);
        strcat(str, buf);
    }

    for (i = 0; i < m->m_rank; i++) {
        sprintf(buf, "%d,", m->m_blockcyclic_val[i]);
        strcat(str, buf);
    }

    for (i = 0; i < m->m_rank; i++) {
        sprintf(buf, "%d,", m->m_data[i]);
        strcat(str, buf);
    }

    for (i = 0; i < m->m_rank; i++) {
        sprintf(buf, "%d,", m->m_procs[i]);
        strcat(str, buf);
    }

    for (i = 0; i < m->m_rank; i++) {
        sprintf(buf, "%d", m->m_dim_order[i]);
        strcat(str, buf);
        if (i < m->m_rank - 1)
            strcat(str, ",");
        else
            strcat(str, "");
    }

    return;
}

/*
** Extract and combine data using blockmap function.
*/
AIF *construct_blockmap_data(g_block_map_t *bm, dbgprocset_t *procset,
                             int array_size, rep_set_t *rep_set) {
    AIF *a = (AIF *)NULL;
    AIF **da;
    int i;
    dbg_over_set_t *over_set;

    if (rep_set == NULL) return NULL;

    // Make sure that there were no errors and we received rep_set, otherwise
    // print error.
    if (SetHasError(rep_set)) {
        PrintOutError("Cannot build blockmap due to error in reply!\n");
        return NULL;
    }
    if (GetNumberOfReply(rep_set) == 0) {
        PrintOutError("Cannot build blockmap due to empty reply!\n");
        return NULL;
    }
    // FIXME: Currently this only works with sets that have only 1 over_set
    if (procset->s_num_sets != 1) {
        PrintOutError(
            "Cannot build blockmap due to multiple handles in procset!\n");
        return NULL;
    }

    // FIXME: This only works with procsets having a single over_set, i.e.
    // DBGSET_INVOKE
    SetOverlaySetForProcSet(procset);
    over_set = GetNextOverlaySetForProcSet(procset);

    // Create an array of aif values
    da = (AIF **)Alloc(sizeof(AIF *) * array_size);

    // process rep_set, placing the AIF data into the array
    for (auto&& reply : rep_set) {
        // Now loop over every rank to see if its in this reply
        // FIXME: There must be a more efficient way to do this...
        for (i = 0; i < array_size; ++i) {
            // Check if this rank is in this reply
            if (bitset_test(reply->rep_group, (i + over_set->s_ps_offset))) {
                da[i] = EVENT_DATA(reply->rep_event)->joinAIF();
            }
        }
    }

    if ((a = ApplyBlockMap(bm, AIF_FORMAT(da[0]), array_size, da, "")) ==
        (AIF *)NULL) {
        PrintOutError("Cannot construct decomposition data: %s\n", GetError());
    }

    // Cleanup
    Free(da);

    return a;
}
