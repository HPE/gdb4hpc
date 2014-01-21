/*********************************************************************************\
 * Block Map Utility routines.
 *
 * Copyright (c) 1996-2012 by Guardsoft Pty Ltd.
 *
 * © 2012-2013 Cray Inc.  All Rights Reserved.
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


#ifndef _GBLOCKMAP_UTIL_H
#define _GBLOCKMAP_UTIL_H


//#include	"gram.h"

int _get_block_size(int rank, int *local_data, int *dist, int *blockcyclic_val, int *proc_grid, int *block_size);
int _get_hash_block_size(int rank, int *global_data, int *dist, int *blockcyclic_val, int *proc_grid, int *block_size);
int _get_order_map(int rank, int *dim_order, int *order_map);
int _get_pid_grid(int rank, int pindex, int *proc_grid, int *order_map, int *pid);
int _get_num_blocks(int rank, int *local_data, int *block);

int _get_local_indices(int rank, int num_blocks, int *block_size, int *local_data, int *local_start_points);
int _get_global_start_locs(int rank, int num_blocks, int *block_size, int *pid, int *order_map,
						int *proc_grid, int *global_data, int *local_data, int **global_start_locs);
int _get_global_indices(int rank, int num_blocks, int *block_size, int *pid, int *order_map,
						int *proc_grid, int *global_data, int *local_data, int *global_start_points);

#endif /* !_GBLOCKMAP_UTIL_H */
