/*
 * NVIDIA CUDA Debugger CUDA-GDB Copyright (C) 2007-2012 NVIDIA Corporation
 * Written by CUDA-GDB team at NVIDIA <cudatools@nvidia.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CUDA_UTILS_H
#define CUDA_UTILS_H 1

#include "cuda-defs.h"

/* Utility functions for cuda-gdb */
#define CUDA_GDB_TMP_BUF_SIZE 1024
#define __S__(s)                #s
#define _STRING_(s)       __S__(s)

/* Initialize everything */
void cuda_utils_initialize (void);

/* Get the gdb temporary directory path */
extern const char* cuda_gdb_tmpdir_getdir (void);

/* Create a directory */
int cuda_gdb_dir_create (const char *dir_name, uint32_t permissions,
                         bool override_umask, bool *dir_exists);

/* Clean up files in a directory */
void cuda_gdb_dir_cleanup_files (char* dirpath);

/* cuda debugging clock, incremented at each resume/wait cycle */
cuda_clock_t cuda_clock (void);
void         cuda_clock_increment (void);

#endif
