/*********************************************************************************\
 * Block Map Utility routines.
 *
 * Copyright (c) 1996-2012 by Guardsoft Pty Ltd.
 *
 * © 2012-2013,2020 Cray Inc.
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
 *********************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aif.h"
#include "blockmap.h"
#include "defs.h"
#include "fds.h"
#include "gram.h"
#include "gvar.h"
#include "useful.h"

int _get_block_size(int rank, int *local_data, int *dist, int *blockcyclic_val,
                    int *proc_grid, int *block_size) {
    int i;

    for (i = 0; i < rank; i++) block_size[i] = 1;

    for (i = rank - 1; i >= 0; i--) {
        block_size[i] = local_data[i];
        if (dist[i] != CT_STAR) break;
    }

    return 1;
}

int _get_hash_block_size(int rank, int *global_data, int *dist,
                         int *blockcyclic_val, int *proc_grid,
                         int *block_size) {
    int i, count = 0;
    int checkCyclic = 0;
    int numprocs = 1;

    for (i = 0; i < rank; i++) {
        if (proc_grid[i] != CT_STAR) numprocs *= proc_grid[i];
    }

    if (product(global_data, rank) % numprocs != 0) return 0;

    for (i = 0; i < rank; i++) {
        if (dist[i] == CT_STAR)
            block_size[i] = global_data[i];
        else if (dist[i] == CT_CYCLIC) {
            block_size[i] = 1;
            checkCyclic = 1;
        } else if (dist[i] == CT_BLOCK_CYCLIC) {
            block_size[i] = blockcyclic_val[i];
            checkCyclic = 1;
        } else if (dist[i] == CT_BLOCK)
            count++;
    }
    if (count > 0) {
        int j = 0;
        for (i = 0; i < rank; i++) {
            if (dist[i] == CT_BLOCK)
                block_size[i] = global_data[i] / proc_grid[j];
            j++;
        }
    }
    if (!checkCyclic) {
        long int a = product(block_size, rank) * numprocs;
        long int b = product(global_data, rank);
        if (a > b || b % a > 0) return 0;
    } else {
        long int total = product(global_data, rank);
        long int sub_total = product(block_size, rank);
        if ((total / sub_total) % numprocs != 0) return 0;
    }

    return 1;
}

int _get_order_map(int rank, int *dim_order, int *order_map) {
    int i;

    for (i = 0; i < rank; i++) order_map[i] = CT_STAR;

    for (i = 0; i < rank; i++) {
        if (dim_order[i] != CT_STAR) {
            // dim_order is entered as 1..n, and in order to offset into the
            // array we need to subtract 1 from the entered value. I don't know
            // why this was done here and not earlier...
            order_map[dim_order[i] - 1] = i;
        }
    }

    return 0;
}

// work out pid given the pindex and proc_grid
int _get_pid_grid(int rank, int pindex, int *proc_grid, int *order_map,
                  int *pid) {
    int i;

    for (i = 0; i < rank; i++) pid[i] = 0;

    for (i = 0; i < rank; i++) {
        if (order_map[i] != CT_STAR) {
            pid[order_map[i]] = pindex % proc_grid[order_map[i]];
            pindex = pindex / proc_grid[order_map[i]];
        }
    }

    return 0;
}

int _get_num_blocks(int rank, int *local_data, int *block) {
    int num_blocks = 1;
    int i;

    // work out number of blocks for each process
    for (i = 0; i < rank; i++) num_blocks *= local_data[i] / block[i];

    return num_blocks;
}

// get local_indices for each block
int _get_local_indices(int rank, int num_blocks, int *block_size,
                       int *local_data, int *local_start_points) {
    int i, j, k;
    int **local_start_locs = NULL;

    local_start_locs = (int **)Alloc(num_blocks * sizeof(int *));
    for (i = 0; i < num_blocks; i++)
        local_start_locs[i] = (int *)Alloc(rank * sizeof(int));

    for (i = 0; i < rank; i++) local_start_locs[0][i] = 0;
    for (i = 1; i < num_blocks; i++) {
        for (j = 0; j < rank; j++)
            local_start_locs[i][j] = local_start_locs[i - 1][j];
        for (j = rank - 1; j >= 0; j--) {
            if ((local_start_locs[i][j] + block_size[j]) < local_data[j]) {
                local_start_locs[i][j] += block_size[j];
                break;
            } else
                local_start_locs[i][j] = local_start_locs[0][j];
        }
    }
    for (i = 0; i < num_blocks; i++) {
        local_start_points[i] = 0;
        for (j = 0; j < rank - 1; j++) {
            int tmp2 = local_start_locs[i][j];
            for (k = j + 1; k < rank; k++) {
                tmp2 *= local_data[k];
            }
            local_start_points[i] += tmp2;
        }
        local_start_points[i] += local_start_locs[i][rank - 1];
    }

    for (i = 0; i < num_blocks; i++) Free(local_start_locs[i]);
    Free(local_start_locs);

    return 1;
}

int _get_global_start_locs(int rank, int num_blocks, int *block_size, int *pid,
                           int *order_map, int *proc_grid, int *global_data,
                           int *local_data, int **global_start_locs) {
    int i, j;
    int *sub_pid = (int *)Alloc(sizeof(int) * rank);
    int *sub_proc_grid = (int *)Alloc(sizeof(int) * rank);
    int **block_locs = NULL;

    block_locs = (int **)Alloc(sizeof(int *) * num_blocks);
    for (i = 0; i < num_blocks; i++) {
        block_locs[i] = (int *)Alloc(sizeof(int) * rank);
    }

    for (i = 0; i < rank; i++) sub_proc_grid[i] = local_data[i] / block_size[i];

    for (j = 0; j < num_blocks; j++) {
        if (j == 0) {
            for (i = 0; i < rank; i++)
                block_locs[j][i] = pid[i] * sub_proc_grid[i];
        } else {
            int sub_index = j;

            for (i = rank - 1; i >= 0; i--) {
                if (sub_proc_grid[i] > 1) {
                    sub_pid[i] = sub_index % sub_proc_grid[i];
                    sub_index = sub_index / sub_proc_grid[i];
                } else
                    sub_pid[i] = 0;
            }
            // for each block of this pid, find block_locs
            for (i = 0; i < rank; i++)
                block_locs[j][i] = block_locs[0][i] + sub_pid[i];
        }
    }

    // now translate the block_locs to global_start_locs
    for (j = 0; j < num_blocks; j++) {
        for (i = 0; i < rank; i++)
            global_start_locs[j][i] = block_locs[j][i] * block_size[i];
    }

    for (i = 0; i < num_blocks; i++) {
        Free(block_locs[i]);
    }
    Free(block_locs);
    Free(sub_pid);
    Free(sub_proc_grid);

    return 0;
}

// get global indices of a block
int _get_global_indices(int rank, int num_blocks, int *block_size, int *pid,
                        int *order_map, int *proc_grid, int *global_data,
                        int *local_data, int *global_start_points) {
    int i, j, k;
    int **global_start_locs = NULL;

    global_start_locs = (int **)Alloc(sizeof(int *) * num_blocks);
    for (i = 0; i < num_blocks; i++) {
        global_start_locs[i] = (int *)Alloc(sizeof(int) * rank);
    }

    _get_global_start_locs(rank, num_blocks, block_size, pid, order_map,
                           proc_grid, global_data, local_data,
                           global_start_locs);

    // linearise global_start_locs
    for (i = 0; i < num_blocks; i++) {
        global_start_points[i] = 0;
        for (j = 0; j < rank - 1; j++) {
            int tmp2 = global_start_locs[i][j];
            for (k = j + 1; k < rank; k++) {
                tmp2 *= global_data[k];
            }
            global_start_points[i] += tmp2;
        }
        global_start_points[i] += global_start_locs[i][rank - 1];
    }

    for (i = 0; i < num_blocks; i++) {
        Free(global_start_locs[i]);
    }
    Free(global_start_locs);

    return 0;
}
