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

#ifndef LIBCUDBGREQ_H
#define LIBCUDBGREQ_H 1

#include <stdlib.h>

#if defined(__STDC__)
#include <inttypes.h>
#include <stdbool.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32) && !defined(_WIN64)
/*Windows 32-bit */
#define PRIxPTR "I32x"
#endif

#if defined(_WIN64)
/*Windows 64-bit */
#define PRIxPTR "I64x"
#endif

#if defined(_WIN32)
/*Windows 32- and 64-bit */
#define PRIx64  "I64x"
#define PRId64  "I64d"
typedef unsigned char bool;
#undef false
#undef true
#define false 0
#define true  1
#endif

// packed alignment defines for struct etc
#if defined(_WIN32) // Windows 32- and 64-bit
#define START_PACKED_ALIGNMENT __pragma(pack(push,1))      // exact fit - no padding
#define PACKED_ALIGNMENT
#define END_PACKED_ALIGNMENT __pragma(pack(pop))           // back to whatever the previous packing mode was
#elif defined(__GNUC__) // GCC
#define START_PACKED_ALIGNMENT
#define PACKED_ALIGNMENT __attribute__ ((__packed__))
#define END_PACKED_ALIGNMENT
#else // all other compilers
#define START_PACKED_ALIGNMENT
#define PACKED_ALIGNMENT
#define END_PACKED_ALIGNMENT
#endif

/* NOTE:  The following structure is *very similar* to that in cudadebugger.h,
          however, it is not identical.  This must be treated as its own entity
          when it comes to backwards compatibility (only debug clients using RPCD
          and libcudbg will hit this). */
typedef enum {
    /* API Version Query */
    CUDBGAPIREQ_getAPI,
    CUDBGAPIREQ_getAPIVersion,

    /* Initialization */
    CUDBGAPIREQ_initialize,
    CUDBGAPIREQ_finalize,

    /* Device Execution Control */
    CUDBGAPIREQ_suspendDevice,
    CUDBGAPIREQ_resumeDevice,
    CUDBGAPIREQ_singleStepWarp,

    /* Breakpoints */
    CUDBGAPIREQ_setBreakpoint,
    CUDBGAPIREQ_unsetBreakpoint,

    /* Device State Inspection */
    CUDBGAPIREQ_readGridId50,
    CUDBGAPIREQ_readBlockIdx,
    CUDBGAPIREQ_readThreadIdx,
    CUDBGAPIREQ_readBrokenWarps,
    CUDBGAPIREQ_readValidWarps,
    CUDBGAPIREQ_readValidLanes,
    CUDBGAPIREQ_readActiveLanes,
    CUDBGAPIREQ_readPinnedMemory,
    CUDBGAPIREQ_readGlobalMemory,
    CUDBGAPIREQ_readTextureMemory,
    CUDBGAPIREQ_readCodeMemory,
    CUDBGAPIREQ_readConstMemory,
    CUDBGAPIREQ_readParamMemory,
    CUDBGAPIREQ_readSharedMemory,
    CUDBGAPIREQ_readLocalMemory,
    CUDBGAPIREQ_readRegister,
    CUDBGAPIREQ_readPC,
    CUDBGAPIREQ_readVirtualPC,
    CUDBGAPIREQ_readLaneStatus,
    CUDBGAPIREQ_readLaneException,
    CUDBGAPIREQ_readCallDepth,
    CUDBGAPIREQ_readReturnAddress,
    CUDBGAPIREQ_readVirtualReturnAddress,

    /* Device State Alteration */
    CUDBGAPIREQ_writePinnedMemory,
    CUDBGAPIREQ_writeGlobalMemory,
    CUDBGAPIREQ_writeParamMemory,
    CUDBGAPIREQ_writeSharedMemory,
    CUDBGAPIREQ_writeLocalMemory,
    CUDBGAPIREQ_writeRegister,

    /* Grid Properties */
    CUDBGAPIREQ_getGridDim,
    CUDBGAPIREQ_getBlockDim,
    CUDBGAPIREQ_getTID,
    CUDBGAPIREQ_getElfImage,

    /* Device Properties */
    CUDBGAPIREQ_getDeviceType,
    CUDBGAPIREQ_getSmType,
    CUDBGAPIREQ_getNumDevices,
    CUDBGAPIREQ_getNumSMs,
    CUDBGAPIREQ_getNumWarps,
    CUDBGAPIREQ_getNumLanes,
    CUDBGAPIREQ_getNumRegisters,
    CUDBGAPIREQ_getGridAttribute,
    CUDBGAPIREQ_getGridAttributes,

    /* DWARF */
    CUDBGAPIREQ_getPhysicalRegister40,
    CUDBGAPIREQ_disassemble,
    CUDBGAPIREQ_isDeviceCodeAddress,
    CUDBGAPIREQ_lookupDeviceCodeSymbol,

    /* Events */
    CUDBGAPIREQ_setNotifyNewEventCallback,
    CUDBGAPIREQ_acknowledgeEvents42,
    CUDBGAPIREQ_getNextEvent42,

    /* 4.1 Extensions */
    CUDBGAPIREQ_getHostAddrFromDeviceAddr,
    CUDBGAPIREQ_readSyscallCallDepth,

    /* 4.2 Extensions */
    CUDBGAPIREQ_readTextureMemoryBindless,

    /* 5.0 Extensions */
    CUDBGAPIREQ_clearAttachState,
    CUDBGAPIREQ_memcheckReadErrorAddress,
    CUDBGAPIREQ_getNextSyncEvent50,
    CUDBGAPIREQ_getNextAsyncEvent50,
    CUDBGAPIREQ_acknowledgeSyncEvents,
    CUDBGAPIREQ_requestCleanupOnDetach,
    CUDBGAPIREQ_initializeAttachStub,
    CUDBGAPIREQ_getGridStatus50,

    /* 5.5 Extensions */
    CUDBGAPIREQ_getGridInfo,
    CUDBGAPIREQ_getNextSyncEvent,
    CUDBGAPIREQ_getNextAsyncEvent,
    CUDBGAPIREQ_readGridId,
    CUDBGAPIREQ_getGridStatus,
    CUDBGAPIREQ_setKernelLaunchNotificationMode,
    CUDBGAPIREQ_getDevicePCIBusInfo,
} CUDBGAPIREQ_t;

typedef enum {
    LIBCUDBG_PIPE_ENDPOINT_RPCD = 999,
    LIBCUDBG_PIPE_ENDPOINT_DEBUG_CLIENT,
    LIBCUDBG_PIPE_ENDPOINT_RPCD_CB,
    LIBCUDBG_PIPE_ENDPOINT_DEBUG_CLIENT_CB,
} libcudbg_pipe_endpoint_t;

START_PACKED_ALIGNMENT;

typedef struct PACKED_ALIGNMENT {
    uint32_t x;
    uint32_t y;
    uint32_t z;
} CUDBGDIM3_t;

typedef struct PACKED_ALIGNMENT CUDBGAPI_message50_st {
    uint32_t client_major;
    uint32_t client_minor;
    uint32_t client_rev;
    uint32_t kind;
    uint32_t result;
    struct PACKED_ALIGNMENT {
        struct PACKED_ALIGNMENT {
            uint32_t dev;
            uint32_t sm;
            uint32_t wp;
            uint32_t ln;
            uint32_t dim;
            uint32_t texid;
            uint32_t regno;
            uint32_t val;
            uint32_t numPairs;
            uint32_t relocated;
            uint32_t attr;
            uint32_t level;
            uint64_t addr;
            uint64_t pc;
            uint64_t sz;
            char reg[256];
            uint32_t coords[4];
        } request;
        struct PACKED_ALIGNMENT {
            uint32_t major;
            uint32_t minor;
            uint32_t rev;
            uint32_t gridId;
            uint32_t tid;
            uint32_t val;
            uint32_t numDevices;
            uint32_t numSMs;
            uint32_t numWarps;
            uint32_t numLanes;
            uint32_t numRegs;
            uint32_t numPhysRegs;
            uint32_t regClass;
            uint32_t symFound;
            uint32_t activeLanesMask;
            uint32_t validLanesMask;
            uint32_t instSize;
            uint32_t isDeviceAddress;
            uint32_t error;
            uint32_t exception;
            uint32_t depth;
            uint32_t found;
            uint64_t brokenWarpsMask;
            uint64_t validWarpsMask;
            uint64_t value;
            uint64_t symAddr;
            uint64_t pc;
            uint64_t ra;
            uint64_t size;
            CUDBGDIM3_t blockIdx;
            CUDBGDIM3_t threadIdx;
            CUDBGDIM3_t blockDim;
            CUDBGDIM3_t gridDim;
            struct PACKED_ALIGNMENT {
                uint32_t kind;
                union {
                    struct PACKED_ALIGNMENT {
                        uint64_t  relocatedElfImage;
                        uint64_t  nonRelocatedElfImage;
                        uint32_t  size32;
                        uint32_t  dev;
                        uint64_t  context;
                        uint64_t  module;
                        uint64_t  size;
                    } elfImageLoaded;
                    struct PACKED_ALIGNMENT {
                        uint32_t dev;
                        uint32_t gridId;
                        uint32_t tid;
                        uint64_t context;
                        uint64_t module;
                        uint64_t function;
                        uint64_t functionEntry;
                        CUDBGDIM3_t gridDim;
                        CUDBGDIM3_t blockDim;
                        uint32_t type;
                    } kernelReady;
                    struct PACKED_ALIGNMENT {
                        uint32_t dev;
                        uint32_t gridId;
                        uint32_t tid;
                        uint64_t context;
                        uint64_t module;
                        uint64_t function;
                        uint64_t functionEntry;
                    } kernelFinished;
                    struct PACKED_ALIGNMENT {
                        uint32_t dev;
                        uint32_t tid;
                        uint64_t context;
                    } contextPush;
                    struct PACKED_ALIGNMENT {
                        uint32_t dev;
                        uint32_t tid;
                        uint64_t context;
                    } contextPop;
                    struct PACKED_ALIGNMENT {
                        uint32_t dev;
                        uint32_t tid;
                        uint64_t context;
                    } contextCreate;
                    struct PACKED_ALIGNMENT {
                        uint32_t dev;
                        uint32_t tid;
                        uint64_t context;
                    } contextDestroy;
                    struct PACKED_ALIGNMENT {
                        CUDBGResult errorType;
                    } internalError;
                } cases;
            } event;
        } result;
    } apiData;
} CUDBGAPIMSG50_t;

typedef struct PACKED_ALIGNMENT CUDBGAPI_message_st {
    uint32_t client_major;
    uint32_t client_minor;
    uint32_t client_rev;
    uint32_t kind;
    uint32_t result;
    struct PACKED_ALIGNMENT {
        struct PACKED_ALIGNMENT {
            uint32_t dev;
            uint32_t sm;
            uint32_t wp;
            uint32_t ln;
            uint32_t dim;
            uint32_t texid;
            uint32_t regno;
            uint32_t val;
            uint32_t numPairs;
            uint32_t relocated;
            uint32_t attr;
            uint32_t level;
            uint64_t addr;
            uint64_t pc;
            uint64_t sz;
            char reg[256];
            uint32_t coords[4];
            uint64_t gridId64;
        } request;
        struct PACKED_ALIGNMENT {
            uint32_t major;
            uint32_t minor;
            uint32_t rev;
            uint64_t gridId64;
            uint32_t tid;
            uint32_t val;
            uint32_t numDevices;
            uint32_t numSMs;
            uint32_t numWarps;
            uint32_t numLanes;
            uint32_t numRegs;
            uint32_t numPhysRegs;
            uint32_t regClass;
            uint32_t symFound;
            uint32_t activeLanesMask;
            uint32_t validLanesMask;
            uint32_t instSize;
            uint32_t isDeviceAddress;
            uint32_t error;
            uint32_t exception;
            uint32_t depth;
            uint32_t found;
            uint64_t brokenWarpsMask;
            uint64_t validWarpsMask;
            uint64_t value;
            uint64_t symAddr;
            uint64_t pc;
            uint64_t ra;
            uint64_t size;
            CUDBGDIM3_t blockIdx;
            CUDBGDIM3_t threadIdx;
            CUDBGDIM3_t blockDim;
            CUDBGDIM3_t gridDim;
            uint32_t pciBusId;
            uint32_t pciDevId;
            struct PACKED_ALIGNMENT {
                uint32_t kind;
                union {
                    struct PACKED_ALIGNMENT {
                        uint64_t  relocatedElfImage;
                        uint64_t  nonRelocatedElfImage;
                        uint32_t  size32;
                        uint32_t  dev;
                        uint64_t  context;
                        uint64_t  module;
                        uint64_t  size;
                    } elfImageLoaded;
                    struct PACKED_ALIGNMENT {
                        uint32_t dev;
                        uint64_t gridId64;
                        uint32_t tid;
                        uint64_t context;
                        uint64_t module;
                        uint64_t function;
                        uint64_t functionEntry;
                        CUDBGDIM3_t gridDim;
                        CUDBGDIM3_t blockDim;
                        uint32_t type;
                        uint64_t parentGridId;
                        CUDBGKernelOrigin origin;
                    } kernelReady;
                    struct PACKED_ALIGNMENT {
                        uint32_t dev;
                        uint64_t gridId64;
                        uint32_t tid;
                        uint64_t context;
                        uint64_t module;
                        uint64_t function;
                        uint64_t functionEntry;
                    } kernelFinished;
                    struct PACKED_ALIGNMENT {
                        uint32_t dev;
                        uint32_t tid;
                        uint64_t context;
                    } contextPush;
                    struct PACKED_ALIGNMENT {
                        uint32_t dev;
                        uint32_t tid;
                        uint64_t context;
                    } contextPop;
                    struct PACKED_ALIGNMENT {
                        uint32_t dev;
                        uint32_t tid;
                        uint64_t context;
                    } contextCreate;
                    struct PACKED_ALIGNMENT {
                        uint32_t dev;
                        uint32_t tid;
                        uint64_t context;
                    } contextDestroy;
                    struct PACKED_ALIGNMENT {
                        CUDBGResult errorType;
                    } internalError;
                } cases;
            } event;
        } result;
    } apiData;
} CUDBGAPIMSG_t;

typedef struct PACKED_ALIGNMENT {
    uint32_t tid;
    uint32_t terminate;
} CUDBGCBMSG40_t;

typedef struct PACKED_ALIGNMENT {
    uint32_t tid;
    uint32_t terminate;
    uint32_t timeout;
} CUDBGCBMSG_t;

END_PACKED_ALIGNMENT;

#ifdef __cplusplus
}
#endif

#endif
