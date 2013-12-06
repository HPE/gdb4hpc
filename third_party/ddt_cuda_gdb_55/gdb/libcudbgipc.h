/*
 * NVIDIA CUDA Debugger CUDA-GDB Copyright (C) 2007-2013 NVIDIA Corporation
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

#ifndef LIBCUDBGIPC_H
#define LIBCUDBGIPC_H 1

#include <stdarg.h>

#define CUDBG_IPC_APPEND(d, s)                     \
do {                                               \
  CUDBGResult r = cudbgipcAppend(d, s);            \
  if (r != CUDBG_SUCCESS) return r;                \
} while (0)

#define CUDBG_IPC_REQUEST(d)                       \
do {                                               \
  CUDBGResult r = cudbgipcRequest(d);              \
  if (r != CUDBG_SUCCESS) return r;                \
} while (0)

typedef struct CUDBGIPC_st {
    int from;
    int to;
    char name[256];
    int  fd;
    bool initialized;
    char *data;
    uint64_t dataSize;
} CUDBGIPC_t;

CUDBGResult cudbgipcAppend(void *d, uint32_t size);
CUDBGResult cudbgipcRequest(void **d);
CUDBGResult cudbgipcCBWaitForData(void *data, uint32_t size);
CUDBGResult cudbgipcInitializeCommIn(void);
CUDBGResult cudbgipcInitializeCommOut(void);
CUDBGResult cudbgipcInitializeCommCB(void);
CUDBGResult cudbgipcFinalize(void);

#endif
