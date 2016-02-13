/*
 * Copyright 2007-2014 NVIDIA Corporation.  All rights reserved.
 *
 * NOTICE TO LICENSEE:
 *
 * This source code and/or documentation ("Licensed Deliverables") are
 * subject to NVIDIA intellectual property rights under U.S. and
 * international Copyright laws.
 *
 * These Licensed Deliverables contained herein is PROPRIETARY and
 * CONFIDENTIAL to NVIDIA and is being provided under the terms and
 * conditions of a form of NVIDIA software license agreement by and
 * between NVIDIA and Licensee ("License Agreement") or electronically
 * accepted by Licensee.  Notwithstanding any terms or conditions to
 * the contrary in the License Agreement, reproduction or disclosure
 * of the Licensed Deliverables to any third party without the express
 * written consent of NVIDIA is prohibited.
 *
 * NOTWITHSTANDING ANY TERMS OR CONDITIONS TO THE CONTRARY IN THE
 * LICENSE AGREEMENT, NVIDIA MAKES NO REPRESENTATION ABOUT THE
 * SUITABILITY OF THESE LICENSED DELIVERABLES FOR ANY PURPOSE.  IT IS
 * PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY OF ANY KIND.
 * NVIDIA DISCLAIMS ALL WARRANTIES WITH REGARD TO THESE LICENSED
 * DELIVERABLES, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY,
 * NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE.
 * NOTWITHSTANDING ANY TERMS OR CONDITIONS TO THE CONTRARY IN THE
 * LICENSE AGREEMENT, IN NO EVENT SHALL NVIDIA BE LIABLE FOR ANY
 * SPECIAL, INDIRECT, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, OR ANY
 * DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
 * OF THESE LICENSED DELIVERABLES.
 *
 * U.S. Government End Users.  These Licensed Deliverables are a
 * "commercial item" as that term is defined at 48 C.F.R. 2.101 (OCT
 * 1995), consisting of "commercial computer software" and "commercial
 * computer software documentation" as such terms are used in 48
 * C.F.R. 12.212 (SEPT 1995) and is provided to the U.S. Government
 * only as a commercial end item.  Consistent with 48 C.F.R.12.212 and
 * 48 C.F.R. 227.7202-1 through 227.7202-4 (JUNE 1995), all
 * U.S. Government End Users acquire the Licensed Deliverables with
 * only those rights set forth herein.
 *
 * Any use of the Licensed Deliverables in individual and commercial
 * software must include, in the user documentation and internal
 * comments to the code, the above Disclaimer and U.S. Government End
 * Users Notice.
 */

#ifndef __CUDACOREDUMP_H__
#define __CUDACOREDUMP_H__
#include <cuda_stdint.h>

typedef struct {
    uint64_t devName;           /* index into the string table */
    uint64_t devType;           /* index into the string table */
    uint64_t smType;            /* index into the string table */
    uint32_t devId;
    uint32_t pciBusId;
    uint32_t pciDevId;
    uint32_t numSMs;
    uint32_t numWarpsPerSM;
    uint32_t numLanesPerWarp;
    uint32_t numRegsPerLane;
    uint32_t numPredicatesPrLane;
    uint32_t smMajor;
    uint32_t smMinor;
    uint32_t instructionSize;   /* instruction size in bytes */
    uint32_t status;
} CudbgDeviceTableEntry;

typedef struct {
    uint64_t gridId64;           /* 64-bit grid ID */
    uint64_t contextId;
    uint64_t function;
    uint64_t functionEntry;
    uint64_t moduleHandle;
    uint64_t parentGridId64;
    uint64_t paramsOffset;
    uint32_t kernelType;
    uint32_t origin;
    uint32_t gridStatus;
    uint32_t numRegs;
    uint32_t gridDimX;
    uint32_t gridDimY;
    uint32_t gridDimZ;
    uint32_t blockDimX;
    uint32_t blockDimY;
    uint32_t blockDimZ;
    uint32_t attrLaunchBlocking;
    uint32_t attrHostTid;
} CudbgGridTableEntry;

typedef struct {
    uint32_t smId;
    uint32_t pad;
} CudbgSmTableEntry;

typedef struct {
    uint64_t gridId64;         /* 64-bit grid ID */
    uint32_t blockIdxX;
    uint32_t blockIdxY;
    uint32_t blockIdxZ;
    uint32_t pad;
} CudbgCTATableEntry;

typedef struct {
    uint64_t errorPC;
    uint32_t warpId;
    uint32_t validLanesMask;
    uint32_t activeLanesMask;
    uint32_t isWarpBroken;     /* indicates if the warp is in
                                  brokenWarpsMask */
    uint32_t errorPCValid;
    uint32_t pad;
} CudbgWarpTableEntry;

typedef struct {
    uint64_t virtualPC;          /* virtualPC in the client's host VA */
    uint64_t physPC;             /* for gpudbgReadPC() */
    uint32_t ln;
    uint32_t threadIdxX;
    uint32_t threadIdxY;
    uint32_t threadIdxZ;
    uint32_t exception;          /* exception info for the lane */
    uint32_t callDepth;
    uint32_t syscallCallDepth;
    uint32_t ccRegister;
} CudbgThreadTableEntry;

typedef struct {
    uint64_t returnAddress;
    uint64_t virtualReturnAddress;
    uint32_t level;
    uint32_t pad;
} CudbgBacktraceTableEntry;

typedef struct {
    uint64_t moduleHandle;
} CudbgModuleTableEntry;

typedef struct {
    uint64_t contextId;
    uint64_t sharedWindowBase;
    uint64_t localWindowBase;
    uint64_t globalWindowBase;
    uint32_t deviceIdx;    /* index of an entry in the device table */
    uint32_t tid;          /* host thread id */
} CudbgContextTableEntry;

#ifndef SHT_LOUSER
#define SHT_LOUSER    0x80000000
#endif

typedef enum {
    CUDBG_SHT_MANAGED_MEM = SHT_LOUSER + 1,
    CUDBG_SHT_GLOBAL_MEM  = SHT_LOUSER + 2,
    CUDBG_SHT_LOCAL_MEM   = SHT_LOUSER + 3,
    CUDBG_SHT_SHARED_MEM  = SHT_LOUSER + 4,
    CUDBG_SHT_DEV_REGS    = SHT_LOUSER + 5,
    CUDBG_SHT_ELF_IMG     = SHT_LOUSER + 6,
    CUDBG_SHT_RELF_IMG    = SHT_LOUSER + 7,
    CUDBG_SHT_BT          = SHT_LOUSER + 8,
    CUDBG_SHT_DEV_TABLE   = SHT_LOUSER + 9,
    CUDBG_SHT_CTX_TABLE   = SHT_LOUSER + 10,
    CUDBG_SHT_SM_TABLE    = SHT_LOUSER + 11,
    CUDBG_SHT_GRID_TABLE  = SHT_LOUSER + 12,
    CUDBG_SHT_CTA_TABLE   = SHT_LOUSER + 13,
    CUDBG_SHT_WP_TABLE    = SHT_LOUSER + 14,
    CUDBG_SHT_LN_TABLE    = SHT_LOUSER + 15,
    CUDBG_SHT_MOD_TABLE   = SHT_LOUSER + 16,
    CUDBG_SHT_DEV_PRED    = SHT_LOUSER + 17,
    CUDBG_SHT_PARAM_MEM   = SHT_LOUSER + 18,
} CudbgSectionHeaderTypes;

/* Section names */
#define CUDBG_SHNAME_GLOBAL     ".cudbg.global"
#define CUDBG_SHNAME_LOCAL      ".cudbg.local"
#define CUDBG_SHNAME_SHARED     ".cudbg.shared"
#define CUDBG_SHNAME_REGS       ".cudbg.regs"
#define CUDBG_SHNAME_PARAM      ".cudbg.param"
#define CUDBG_SHNAME_PRED       ".cudbg.pred"
#define CUDBG_SHNAME_DEVTABLE   ".cudbg.devtbl"
#define CUDBG_SHNAME_CTXTABLE   ".cudbg.ctxtbl"
#define CUDBG_SHNAME_SMTABLE    ".cudbg.smtbl"
#define CUDBG_SHNAME_GRIDTABLE  ".cudbg.gridtbl"
#define CUDBG_SHNAME_CTATABLE   ".cudbg.ctatbl"
#define CUDBG_SHNAME_WPTABLE    ".cudbg.wptbl"
#define CUDBG_SHNAME_LNTABLE    ".cudbg.lntbl"
#define CUDBG_SHNAME_BT         ".cudbg.bt"
#define CUDBG_SHNAME_MODTABLE   ".cudbg.modtbl"
#define CUDBG_SHNAME_ELFIMG     ".cudbg.elfimg"
#define CUDBG_SHNAME_RELFIMG    ".cudbg.relfimg"

#endif
