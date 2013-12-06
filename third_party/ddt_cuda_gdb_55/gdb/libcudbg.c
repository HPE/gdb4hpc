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

#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>

#ifdef GDBSERVER
#include <server.h>
#include <cuda-tdep-server.h>
#else
#include <defs.h>
#include <gdb_assert.h>
#include <cuda-options.h>
#include <cuda-tdep.h>
#endif

#include <libcudbg.h>
#include <libcudbgipc.h>
#include <cudadebugger.h>

/*Forward declarations */
static void cudbg_trace(char *fmt, ...);

/*Globals */
CUDBGNotifyNewEventCallback cudbgDebugClientCallback = NULL;
pthread_t callbackEventThreadHandle;
pthread_t cudagdbMainThreadHandle;
static bool cudbgPreInitComplete = false;

static void *
cudbgCallbackHandler(void *arg)
{
    CUDBGCBMSG_t data;
    CUDBGResult res;
    CUDBGEventCallbackData cbData;
    sigset_t sigset;

    /* SIGCHLD signals must be caught by the main thread */
    sigemptyset (&sigset);
    sigaddset (&sigset, SIGCHLD);
    sigprocmask (SIG_BLOCK, &sigset, NULL);

    for (;;) {
        res = cudbgipcCBWaitForData(&data, sizeof data);
        if (res != CUDBG_SUCCESS) {
            cudbg_trace ("failure while waiting for callback data! (res = %d)", res);
            break;
        }
        if (data.terminate) {
            cudbg_trace ("Callback handler thread received termination data.\n");
            break;
        }
        if (cudbgDebugClientCallback)
          {
            cbData.tid = data.tid;
            cbData.timeout = data.timeout;
            cudbgDebugClientCallback(&cbData);
          }
    }

    return NULL;
}

CUDBGResult
cudbgGetAPIVersion(uint32_t *major, uint32_t *minor, uint32_t *rev)
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_getAPIVersion;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;
    if (res == CUDBG_SUCCESS) {
        *major = ipcres->apiData.result.major;
        *minor = ipcres->apiData.result.minor;
        *rev   = ipcres->apiData.result.rev;
        cudbg_trace ("queried application API version (%d.%d.%d)", *major, *minor, *rev);
    }
    else
        cudbg_trace ("failed to query application API version");

    return res;
}

static CUDBGResult
cudbgPreInitialize(void)
{
    CUDBGResult res;
    int ret;

    if (!cudbgPreInitComplete) {
        ret = cuda_gdb_session_create ();
        if (ret)
            return CUDBG_ERROR_COMMUNICATION_FAILURE;

        res = cudbgipcInitializeCommIn();
        if (res != CUDBG_SUCCESS)
            return CUDBG_ERROR_COMMUNICATION_FAILURE;

        cudagdbMainThreadHandle = pthread_self();
        if (pthread_create(&callbackEventThreadHandle, NULL, cudbgCallbackHandler, NULL))
            return CUDBG_ERROR_COMMUNICATION_FAILURE;
    }
    cudbgPreInitComplete = true;

    return CUDBG_SUCCESS;
}

static CUDBGResult
cudbgInitialize(void)
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult initRes, versionRes;
    uint32_t major, minor, rev;

    cudbg_trace ("initializing...");

    initRes = cudbgPreInitialize();
    if (initRes != CUDBG_SUCCESS) {
        cudbg_trace ("pre initialization failed (res=%d)", initRes);
        return initRes;
    }

    cudbg_trace ("pre initialization successful");

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.client_major = CUDBG_API_VERSION_MAJOR;
    ipcreq.client_minor = CUDBG_API_VERSION_MINOR;
    ipcreq.client_rev   = CUDBG_API_VERSION_REVISION;
    ipcreq.kind = CUDBGAPIREQ_initialize;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    initRes = ipcres->result;
    cudbg_trace ("full initialization completed (res=%d)", initRes);

    versionRes = cudbgGetAPIVersion(&major, &minor, &rev);
    if (versionRes == CUDBG_SUCCESS) {
        if (CUDBG_API_VERSION_REVISION > rev) {
            cudbg_trace ("libcudbg version (%d.%d.%d) is too new "
                         "for application version (%d.%d.%d)",
                         CUDBG_API_VERSION_MAJOR,
                         CUDBG_API_VERSION_MINOR,
                         CUDBG_API_VERSION_REVISION,
                         major, minor, rev);
            return CUDBG_ERROR_INCOMPATIBLE_API;
        }
    }

    return initRes;
}

static CUDBGResult
cudbgInitializeAttachStub ()
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_initializeAttachStub;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    return res;
}

static CUDBGResult
cudbgPostFinalize(void)
{
    CUDBGResult res;

    if (pthread_join(callbackEventThreadHandle, NULL)) {
        cudbg_trace ("post finalize error joining with callback thread\n");
        return CUDBG_ERROR_INTERNAL;
    }

    res = cudbgipcFinalize();
    if (res != CUDBG_SUCCESS) {
        cudbg_trace ("post finalize error finalizing ipc (res = %d)\n", res);
        return res;
    }

    cuda_gdb_session_destroy ();

    cudbgPreInitComplete = false;
    cudbgDebugClientCallback = NULL;

    return CUDBG_SUCCESS;
}


static CUDBGResult
cudbgFinalize(void)
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res, postRes;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_finalize;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    postRes = cudbgPostFinalize();
    if (postRes != CUDBG_SUCCESS) {
        cudbg_trace ("post finalize failed (res=%d)", postRes);
        return postRes;
    }

    return res;
}

static CUDBGResult
cudbgSuspendDevice(uint32_t dev)
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_suspendDevice;
    ipcreq.apiData.request.dev = dev;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    return res;
}

static CUDBGResult
cudbgResumeDevice(uint32_t dev)
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_resumeDevice;
    ipcreq.apiData.request.dev = dev;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    return res;
}

static CUDBGResult
cudbgSingleStepWarp(uint32_t dev, uint32_t sm, uint32_t wp, uint64_t *steppedWarpMask)
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_singleStepWarp;
    ipcreq.apiData.request.dev = dev;
    ipcreq.apiData.request.sm  = sm;
    ipcreq.apiData.request.wp  = wp;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    // hijacking the 'value' field to avoid breaking backward compatibility
    *steppedWarpMask = ipcres->apiData.result.value;

    return res;
}


static CUDBGResult
cudbgSetBreakpoint(uint32_t dev, uint64_t addr)
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_setBreakpoint;
    ipcreq.apiData.request.dev  = dev;
    ipcreq.apiData.request.addr = addr;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    return res;
}

static CUDBGResult
cudbgUnsetBreakpoint(uint32_t dev, uint64_t addr)
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_unsetBreakpoint;
    ipcreq.apiData.request.dev  = dev;
    ipcreq.apiData.request.addr = addr;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    return res;
}


static CUDBGResult
cudbgReadGridId(uint32_t dev, uint32_t sm, uint32_t wp, uint64_t *gridId64)
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_readGridId;
    ipcreq.apiData.request.dev = dev;
    ipcreq.apiData.request.sm  = sm;
    ipcreq.apiData.request.wp  = wp;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    *gridId64 = ipcres->apiData.result.gridId64;

    return res;
}

static CUDBGResult
cudbgReadBlockIdx(uint32_t dev, uint32_t sm, uint32_t wp, CuDim3 *blockIdx)
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_readBlockIdx;
    ipcreq.apiData.request.dev = dev;
    ipcreq.apiData.request.sm  = sm;
    ipcreq.apiData.request.wp  = wp;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    blockIdx->x = ipcres->apiData.result.blockIdx.x;
    blockIdx->y = ipcres->apiData.result.blockIdx.y;
    blockIdx->z = ipcres->apiData.result.blockIdx.z;

    return res;
}

static CUDBGResult
cudbgReadThreadIdx(uint32_t dev, uint32_t sm, uint32_t wp, uint32_t ln, CuDim3 *threadIdx)
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_readThreadIdx;
    ipcreq.apiData.request.dev = dev;
    ipcreq.apiData.request.sm  = sm;
    ipcreq.apiData.request.wp  = wp;
    ipcreq.apiData.request.ln  = ln;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    threadIdx->x = ipcres->apiData.result.threadIdx.x;
    threadIdx->y = ipcres->apiData.result.threadIdx.y;
    threadIdx->z = ipcres->apiData.result.threadIdx.z;

    return res;
}

static CUDBGResult
cudbgReadBrokenWarps(uint32_t dev, uint32_t sm, uint64_t *brokenWarpsMask)
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_readBrokenWarps;
    ipcreq.apiData.request.dev = dev;
    ipcreq.apiData.request.sm  = sm;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    *brokenWarpsMask = ipcres->apiData.result.brokenWarpsMask;

    return res;
}

static CUDBGResult
cudbgReadValidWarps(uint32_t dev, uint32_t sm, uint64_t *validWarpsMask)
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_readValidWarps;
    ipcreq.apiData.request.dev = dev;
    ipcreq.apiData.request.sm  = sm;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    *validWarpsMask = ipcres->apiData.result.validWarpsMask;

    return res;
}

static CUDBGResult
cudbgReadValidLanes(uint32_t dev, uint32_t sm, uint32_t wp, uint32_t *validLanesMask)
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_readValidLanes;
    ipcreq.apiData.request.dev = dev;
    ipcreq.apiData.request.sm  = sm;
    ipcreq.apiData.request.wp  = wp;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    *validLanesMask = ipcres->apiData.result.validLanesMask;

    return res;
}

static CUDBGResult
cudbgReadActiveLanes(uint32_t dev, uint32_t sm, uint32_t wp, uint32_t *activeLanesMask)
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_readActiveLanes;
    ipcreq.apiData.request.dev = dev;
    ipcreq.apiData.request.sm  = sm;
    ipcreq.apiData.request.wp  = wp;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    *activeLanesMask = ipcres->apiData.result.activeLanesMask;

    return res;
}

static CUDBGResult
cudbgReadPinnedMemory(uint64_t addr, void *buf, uint32_t sz)
{
    char *d, *p;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;
    
    memset(&ipcreq, 0, sizeof ipcreq);
    memset(buf, 0, sz);
    ipcreq.kind = CUDBGAPIREQ_readPinnedMemory;
    ipcreq.apiData.request.addr = addr;
    ipcreq.apiData.request.sz = sz;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_APPEND(buf, sz);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    p = d + sizeof ipcreq;
    memcpy (buf, p, sz);
  
    return res;
}

static CUDBGResult
cudbgReadGlobalMemory(uint32_t dev, uint32_t sm, uint32_t wp, uint32_t ln, uint64_t addr, void *buf, uint32_t sz)
{
    char *d, *p;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    memset(buf, 0, sz);
    ipcreq.kind = CUDBGAPIREQ_readGlobalMemory;
    ipcreq.apiData.request.dev = dev;
    ipcreq.apiData.request.sm  = sm;
    ipcreq.apiData.request.wp  = wp;
    ipcreq.apiData.request.ln  = ln;
    ipcreq.apiData.request.addr = addr;
    ipcreq.apiData.request.sz = sz;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_APPEND(buf, sz);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    p = d + sizeof ipcreq;
    memcpy (buf, p, sz);

    return res;
}

static CUDBGResult
cudbgReadTextureMemory(uint32_t dev, uint32_t sm, uint32_t wp, uint32_t id, uint32_t dim, uint32_t *coords, void *buf, uint32_t sz)
{
    char *d, *p;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    memset(buf, 0, sz);
    ipcreq.kind = CUDBGAPIREQ_readTextureMemory;
    ipcreq.apiData.request.dev = dev;
    ipcreq.apiData.request.sm  = sm;
    ipcreq.apiData.request.wp  = wp;
    ipcreq.apiData.request.dim = dim;
    ipcreq.apiData.request.texid = id;
    ipcreq.apiData.request.sz = sz;
    memcpy(ipcreq.apiData.request.coords, coords, dim * sizeof(*coords));

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_APPEND(buf, sz);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    p = d + sizeof ipcreq;
    memcpy (buf, p, sz);

    return res;
}

static CUDBGResult
cudbgReadTextureMemoryBindless(uint32_t dev, uint32_t sm, uint32_t wp, uint32_t texSymtabIndex, uint32_t dim, uint32_t *coords, void *buf, uint32_t sz)
{
    char *d, *p;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    memset(buf, 0, sz);
    ipcreq.kind = CUDBGAPIREQ_readTextureMemoryBindless;
    ipcreq.apiData.request.dev = dev;
    ipcreq.apiData.request.sm  = sm;
    ipcreq.apiData.request.wp  = wp;
    ipcreq.apiData.request.dim = dim;
    ipcreq.apiData.request.texid = texSymtabIndex;
    ipcreq.apiData.request.sz = sz;
    memcpy(ipcreq.apiData.request.coords, coords, dim * sizeof(*coords));

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_APPEND(buf, sz);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    p = d + sizeof ipcreq;
    memcpy (buf, p, sz);

    return res;
}

static CUDBGResult
cudbgReadCodeMemory(uint32_t dev, uint64_t addr, void *buf, uint32_t sz)
{
    char *d, *p;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    memset(buf, 0, sz);
    ipcreq.kind = CUDBGAPIREQ_readCodeMemory;
    ipcreq.apiData.request.dev = dev;
    ipcreq.apiData.request.addr = addr;
    ipcreq.apiData.request.sz = sz;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_APPEND(buf, sz);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    p = d + sizeof ipcreq;
    memcpy (buf, p, sz);

    return res;
}

static CUDBGResult
cudbgReadConstMemory(uint32_t dev, uint64_t addr, void *buf, uint32_t sz)
{
    char *d, *p;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    memset(buf, 0, sz);
    ipcreq.kind = CUDBGAPIREQ_readConstMemory;
    ipcreq.apiData.request.dev = dev;
    ipcreq.apiData.request.addr = addr;
    ipcreq.apiData.request.sz = sz;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_APPEND(buf, sz);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    p = d + sizeof ipcreq;
    memcpy (buf, p, sz);

    return res;
}

static CUDBGResult
cudbgReadParamMemory(uint32_t dev, uint32_t sm, uint32_t wp, uint64_t addr, void *buf, uint32_t sz)
{
    char *d, *p;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    memset(buf, 0, sz);
    ipcreq.kind = CUDBGAPIREQ_readParamMemory;
    ipcreq.apiData.request.dev = dev;
    ipcreq.apiData.request.sm  = sm;
    ipcreq.apiData.request.wp  = wp;
    ipcreq.apiData.request.addr = addr;
    ipcreq.apiData.request.sz = sz;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_APPEND(buf, sz);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    p = d + sizeof ipcreq;
    memcpy (buf, p, sz);

    return res;
}

static CUDBGResult
cudbgReadSharedMemory(uint32_t dev, uint32_t sm, uint32_t wp, uint64_t addr, void *buf, uint32_t sz)
{
    char *d, *p;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    memset(buf, 0, sz);
    ipcreq.kind = CUDBGAPIREQ_readSharedMemory;
    ipcreq.apiData.request.dev = dev;
    ipcreq.apiData.request.sm  = sm;
    ipcreq.apiData.request.wp  = wp;
    ipcreq.apiData.request.addr = addr;
    ipcreq.apiData.request.sz = sz;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_APPEND(buf, sz);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    p = d + sizeof ipcreq;
    memcpy (buf, p, sz);

    return res;
}

static CUDBGResult
cudbgReadLocalMemory(uint32_t dev, uint32_t sm, uint32_t wp, uint32_t ln, uint64_t addr, void *buf, uint32_t sz)
{
    char *d, *p;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    memset(buf, 0, sz);
    ipcreq.kind = CUDBGAPIREQ_readLocalMemory;
    ipcreq.apiData.request.dev = dev;
    ipcreq.apiData.request.sm  = sm;
    ipcreq.apiData.request.wp  = wp;
    ipcreq.apiData.request.ln  = ln;
    ipcreq.apiData.request.addr = addr;
    ipcreq.apiData.request.sz = sz;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_APPEND(buf, sz);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    p = d + sizeof ipcreq;
    memcpy (buf, p, sz);

    return res;
}

static CUDBGResult
cudbgReadRegister(uint32_t dev, uint32_t sm, uint32_t wp, uint32_t ln, uint32_t regno, uint32_t *val)
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_readRegister;
    ipcreq.apiData.request.dev = dev;
    ipcreq.apiData.request.sm  = sm;
    ipcreq.apiData.request.wp  = wp;
    ipcreq.apiData.request.ln  = ln;
    ipcreq.apiData.request.regno = regno;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    *val = ipcres->apiData.result.val;

    return res;
}

static CUDBGResult
cudbgReadPC(uint32_t dev, uint32_t sm, uint32_t wp, uint32_t ln, uint64_t *pc)
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_readPC;
    ipcreq.apiData.request.dev = dev;
    ipcreq.apiData.request.sm  = sm;
    ipcreq.apiData.request.wp  = wp;
    ipcreq.apiData.request.ln  = ln;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    *pc = ipcres->apiData.result.pc;

    return res;
}

static CUDBGResult
cudbgReadVirtualPC(uint32_t dev, uint32_t sm, uint32_t wp, uint32_t ln, uint64_t *pc)
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_readVirtualPC;
    ipcreq.apiData.request.dev = dev;
    ipcreq.apiData.request.sm  = sm;
    ipcreq.apiData.request.wp  = wp;
    ipcreq.apiData.request.ln  = ln;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    *pc = ipcres->apiData.result.pc;

    return res;
}

static CUDBGResult
cudbgReadLaneStatus(uint32_t dev, uint32_t sm, uint32_t wp, uint32_t ln, bool *error)
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_readLaneStatus;
    ipcreq.apiData.request.dev = dev;
    ipcreq.apiData.request.sm  = sm;
    ipcreq.apiData.request.wp  = wp;
    ipcreq.apiData.request.ln  = ln;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    *error = ipcres->apiData.result.error;

    return res;
}

static CUDBGResult
cudbgReadLaneException(uint32_t dev, uint32_t sm, uint32_t wp, uint32_t ln, CUDBGException_t *exception)
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_readLaneException;
    ipcreq.apiData.request.dev = dev;
    ipcreq.apiData.request.sm  = sm;
    ipcreq.apiData.request.wp  = wp;
    ipcreq.apiData.request.ln  = ln;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    *exception = *(CUDBGException_t *)(void*)&ipcres->apiData.result.exception;

    return res;
}

static CUDBGResult
cudbgReadCallDepth(uint32_t dev, uint32_t sm, uint32_t wp, uint32_t ln, uint32_t *depth)
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_readCallDepth;
    ipcreq.apiData.request.dev = dev;
    ipcreq.apiData.request.sm  = sm;
    ipcreq.apiData.request.wp  = wp;
    ipcreq.apiData.request.ln  = ln;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    *depth = ipcres->apiData.result.depth;

    return res;
}

static CUDBGResult
cudbgReadSyscallCallDepth(uint32_t dev, uint32_t sm, uint32_t wp, uint32_t ln, uint32_t *depth)
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_readSyscallCallDepth;
    ipcreq.apiData.request.dev = dev;
    ipcreq.apiData.request.sm  = sm;
    ipcreq.apiData.request.wp  = wp;
    ipcreq.apiData.request.ln  = ln;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    *depth = ipcres->apiData.result.depth;

    return res;
}

static CUDBGResult
cudbgReadReturnAddress(uint32_t dev, uint32_t sm, uint32_t wp, uint32_t ln, uint32_t level, uint64_t *ra)
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_readReturnAddress;
    ipcreq.apiData.request.dev = dev;
    ipcreq.apiData.request.sm  = sm;
    ipcreq.apiData.request.wp  = wp;
    ipcreq.apiData.request.ln  = ln;
    ipcreq.apiData.request.level = level;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    *ra = ipcres->apiData.result.ra;

    return res;
}

static CUDBGResult
cudbgReadVirtualReturnAddress(uint32_t dev, uint32_t sm, uint32_t wp, uint32_t ln, uint32_t level, uint64_t *ra)
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_readVirtualReturnAddress;
    ipcreq.apiData.request.dev = dev;
    ipcreq.apiData.request.sm  = sm;
    ipcreq.apiData.request.wp  = wp;
    ipcreq.apiData.request.ln  = ln;
    ipcreq.apiData.request.level = level;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    *ra = ipcres->apiData.result.ra;

    return res;
}


static CUDBGResult
cudbgWritePinnedMemory(uint64_t addr, const void *buf, uint32_t sz)
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_writePinnedMemory;
    ipcreq.apiData.request.addr = addr;
    ipcreq.apiData.request.sz = sz;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_APPEND((void *)buf, sz);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    return res;
}

static CUDBGResult
cudbgWriteGlobalMemory(uint32_t dev, uint32_t sm, uint32_t wp, uint32_t ln, uint64_t addr, const void *buf, uint32_t sz)
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_writeGlobalMemory;
    ipcreq.apiData.request.dev = dev;
    ipcreq.apiData.request.sm  = sm;
    ipcreq.apiData.request.wp  = wp;
    ipcreq.apiData.request.ln  = ln;
    ipcreq.apiData.request.addr = addr;
    ipcreq.apiData.request.sz = sz;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_APPEND((void *)buf, sz);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    return res;
}

static CUDBGResult
cudbgWriteParamMemory(uint32_t dev, uint32_t sm, uint32_t wp, uint64_t addr, const void *buf, uint32_t sz)
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_writeParamMemory;
    ipcreq.apiData.request.dev = dev;
    ipcreq.apiData.request.sm  = sm;
    ipcreq.apiData.request.wp  = wp;
    ipcreq.apiData.request.addr = addr;
    ipcreq.apiData.request.sz = sz;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_APPEND((void *)buf, sz);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    return res;
}

static CUDBGResult
cudbgWriteSharedMemory(uint32_t dev, uint32_t sm, uint32_t wp, uint64_t addr, const void *buf, uint32_t sz)
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_writeSharedMemory;
    ipcreq.apiData.request.dev = dev;
    ipcreq.apiData.request.sm  = sm;
    ipcreq.apiData.request.wp  = wp;
    ipcreq.apiData.request.addr = addr;
    ipcreq.apiData.request.sz = sz;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_APPEND((void *)buf, sz);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    return res;
}

static CUDBGResult
cudbgWriteLocalMemory(uint32_t dev, uint32_t sm, uint32_t wp, uint32_t ln, uint64_t addr, const void *buf, uint32_t sz)
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_writeLocalMemory;
    ipcreq.apiData.request.dev = dev;
    ipcreq.apiData.request.sm  = sm;
    ipcreq.apiData.request.wp  = wp;
    ipcreq.apiData.request.ln  = ln;
    ipcreq.apiData.request.addr = addr;
    ipcreq.apiData.request.sz = sz;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_APPEND((void *)buf, sz);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    return res;
}

static CUDBGResult
cudbgWriteRegister(uint32_t dev, uint32_t sm, uint32_t wp, uint32_t ln, uint32_t regno, uint32_t val)
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_writeRegister;
    ipcreq.apiData.request.dev = dev;
    ipcreq.apiData.request.sm  = sm;
    ipcreq.apiData.request.wp  = wp;
    ipcreq.apiData.request.ln  = ln;
    ipcreq.apiData.request.regno = regno;
    ipcreq.apiData.request.val = val;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    return res;
}


static CUDBGResult
cudbgGetGridDim(uint32_t dev, uint32_t sm, uint32_t wp, CuDim3 *gridDim)
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_getGridDim;
    ipcreq.apiData.request.dev = dev;
    ipcreq.apiData.request.sm  = sm;
    ipcreq.apiData.request.wp  = wp;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    gridDim->x = ipcres->apiData.result.gridDim.x;
    gridDim->y = ipcres->apiData.result.gridDim.y;
    gridDim->z = ipcres->apiData.result.gridDim.z;

    return res;
}

static CUDBGResult
cudbgGetBlockDim(uint32_t dev, uint32_t sm, uint32_t wp, CuDim3 *blockDim)
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_getBlockDim;
    ipcreq.apiData.request.dev = dev;
    ipcreq.apiData.request.sm  = sm;
    ipcreq.apiData.request.wp  = wp;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    blockDim->x = ipcres->apiData.result.blockDim.x;
    blockDim->y = ipcres->apiData.result.blockDim.y;
    blockDim->z = ipcres->apiData.result.blockDim.z;

    return res;
}

static CUDBGResult
cudbgGetTID(uint32_t dev, uint32_t sm, uint32_t wp, uint32_t *tid)
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_getTID;
    ipcreq.apiData.request.dev = dev;
    ipcreq.apiData.request.sm  = sm;
    ipcreq.apiData.request.wp  = wp;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    *tid = ipcres->apiData.result.tid;

    return res;
}

typedef struct cudbgElfImage_st {
    void *image;
    struct cudbgElfImage_st *next;
} cudbgElfImage_t;

static void *
cudbgNewElfImage(void *image, uint32_t size)
{
    void *newImage = NULL; 
    if (!(newImage = xmalloc(size)))
        return NULL;
    memcpy(newImage, image, size);
    return newImage;
}

static CUDBGResult
cudbgGetElfImage(uint32_t dev, uint32_t sm, uint32_t wp, bool relocated, void **elfImage, uint64_t *size)
{
    char *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_getElfImage;
    ipcreq.apiData.request.dev = dev;
    ipcreq.apiData.request.sm  = sm;
    ipcreq.apiData.request.wp  = wp;
    ipcreq.apiData.request.relocated = relocated;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    *size = ipcres->apiData.result.size;
    *elfImage = cudbgNewElfImage(d + sizeof ipcreq, *size);

    if (!elfImage)
        return CUDBG_ERROR_INTERNAL;

    return res;
}

static CUDBGResult
cudbgGetGridAttribute(uint32_t dev, uint32_t sm, uint32_t wp, CUDBGAttribute attr, uint64_t *value)
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_getGridAttribute;
    ipcreq.apiData.request.dev = dev;
    ipcreq.apiData.request.sm  = sm;
    ipcreq.apiData.request.wp  = wp;
    ipcreq.apiData.request.attr = attr;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    *value = ipcres->apiData.result.value;

    return res;
}

static CUDBGResult
cudbgGetGridAttributes(uint32_t dev, uint32_t sm, uint32_t wp, CUDBGAttributeValuePair *pairs, uint32_t numPairs)
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_getGridAttributes;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    return res;
}


static CUDBGResult
cudbgGetDeviceType(uint32_t dev, char *buf, uint32_t sz)
{
    char *d, *p;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    memset(buf, 0, sz);
    ipcreq.kind = CUDBGAPIREQ_getDeviceType;
    ipcreq.apiData.request.dev = dev;
    ipcreq.apiData.request.sz = sz;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_APPEND((void *)buf, sz);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    p = d + sizeof ipcreq;
    memcpy (buf, p, sz);

    return res;
}

static CUDBGResult
cudbgGetSmType(uint32_t dev, char *buf, uint32_t sz)
{
    char *d, *p;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    memset(buf, 0, sz);
    ipcreq.kind = CUDBGAPIREQ_getSmType;
    ipcreq.apiData.request.dev = dev;
    ipcreq.apiData.request.sz = sz;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_APPEND((void *)buf, sz);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    p = d + sizeof ipcreq;
    memcpy (buf, p, sz);

    return res;
}

static CUDBGResult
cudbgGetNumDevices(uint32_t *numDev)
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_getNumDevices;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    *numDev = ipcres->apiData.result.numDevices;

    return res;
}

static CUDBGResult
cudbgGetNumSMs(uint32_t dev, uint32_t *numSMs)
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_getNumSMs;
    ipcreq.apiData.request.dev = dev;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    *numSMs = ipcres->apiData.result.numSMs;

    return res;
}

static CUDBGResult
cudbgGetNumWarps(uint32_t dev, uint32_t *numWarps)
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_getNumWarps;
    ipcreq.apiData.request.dev = dev;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    *numWarps = ipcres->apiData.result.numWarps;

    return res;
}

static CUDBGResult
cudbgGetNumLanes(uint32_t dev, uint32_t *numLanes)
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_getNumLanes;
    ipcreq.apiData.request.dev = dev;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    *numLanes = ipcres->apiData.result.numLanes;

    return res;
}

static CUDBGResult
cudbgGetNumRegisters(uint32_t dev, uint32_t *numRegs)
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_getNumRegisters;
    ipcreq.apiData.request.dev = dev;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    *numRegs = ipcres->apiData.result.numRegs;

    return res;
}

static CUDBGResult
cudbgDisassemble(uint32_t dev, uint64_t addr, uint32_t *instSize, char *buf, uint32_t sz)
{
    char *d, *p;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    memset(buf, 0, sz);
    ipcreq.kind = CUDBGAPIREQ_disassemble;
    ipcreq.apiData.request.dev = dev;
    ipcreq.apiData.request.addr = addr;
    ipcreq.apiData.request.sz = sz;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_APPEND((void *)buf, sz);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    *instSize = ipcres->apiData.result.instSize;

    if (*instSize > sz)
        return CUDBG_ERROR_INTERNAL;

    p = d + sizeof ipcreq;
    memcpy(buf, p, strlen((char *)p) + 1);

    return res;
}

static CUDBGResult
cudbgIsDeviceCodeAddress(uintptr_t addr, bool *isDeviceAddress)
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_isDeviceCodeAddress;
    ipcreq.apiData.request.addr = addr;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    *isDeviceAddress = ipcres->apiData.result.isDeviceAddress;

    return res;
}

static CUDBGResult
cudbgLookupDeviceCodeSymbol(char *symName, bool *symFound, uintptr_t *symAddr)
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    if (!symName)
        return CUDBG_ERROR_INVALID_ARGS;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_lookupDeviceCodeSymbol;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_APPEND((void *)symName, strlen(symName)+1);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    *symFound = ipcres->apiData.result.symFound;
    *symAddr = (uintptr_t)ipcres->apiData.result.symAddr;

    return res;
}

static CUDBGResult
cudbgSetNotifyNewEventCallback(CUDBGNotifyNewEventCallback callback)
{
    cudbgDebugClientCallback = callback;

    return CUDBG_SUCCESS;
}

static CUDBGResult
cudbgAcknowledgeSyncEvents(void)
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_acknowledgeSyncEvents;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    return res;
}

static CUDBGResult
cudbgGetNextEventCommon(CUDBGEvent *event, CUDBGAPIREQ_t kind)
{
    char *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = kind;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    /* Each field in the event structure must be copied individually. */
    event->kind = ipcres->apiData.result.event.kind;

    switch (event->kind) {
        case CUDBG_EVENT_ELF_IMAGE_LOADED:
            cudbg_trace ("elf image loaded event received");
            event->cases.elfImageLoaded.size = ipcres->apiData.result.event.cases.elfImageLoaded.size;
            event->cases.elfImageLoaded.dev = ipcres->apiData.result.event.cases.elfImageLoaded.dev;
            event->cases.elfImageLoaded.context = ipcres->apiData.result.event.cases.elfImageLoaded.context;
            event->cases.elfImageLoaded.module = ipcres->apiData.result.event.cases.elfImageLoaded.module;

            event->cases.elfImageLoaded.relocatedElfImage =
                cudbgNewElfImage(d + sizeof ipcreq, event->cases.elfImageLoaded.size);
            if (!event->cases.elfImageLoaded.relocatedElfImage)
                return CUDBG_ERROR_INTERNAL;
            break;
        case CUDBG_EVENT_KERNEL_READY:
            cudbg_trace ("kernel ready event received");
            event->cases.kernelReady.dev = ipcres->apiData.result.event.cases.kernelReady.dev;
            event->cases.kernelReady.gridId64 = ipcres->apiData.result.event.cases.kernelReady.gridId64;
            event->cases.kernelReady.tid = ipcres->apiData.result.event.cases.kernelReady.tid;
            event->cases.kernelReady.context = ipcres->apiData.result.event.cases.kernelReady.context;
            event->cases.kernelReady.module = ipcres->apiData.result.event.cases.kernelReady.module;
            event->cases.kernelReady.function = ipcres->apiData.result.event.cases.kernelReady.function;
            event->cases.kernelReady.functionEntry = ipcres->apiData.result.event.cases.kernelReady.functionEntry;
            event->cases.kernelReady.gridDim.x = ipcres->apiData.result.event.cases.kernelReady.gridDim.x;
            event->cases.kernelReady.gridDim.y = ipcres->apiData.result.event.cases.kernelReady.gridDim.y;
            event->cases.kernelReady.gridDim.z = ipcres->apiData.result.event.cases.kernelReady.gridDim.z;
            event->cases.kernelReady.blockDim.x = ipcres->apiData.result.event.cases.kernelReady.blockDim.x;
            event->cases.kernelReady.blockDim.y = ipcres->apiData.result.event.cases.kernelReady.blockDim.y;
            event->cases.kernelReady.blockDim.z = ipcres->apiData.result.event.cases.kernelReady.blockDim.z;
            event->cases.kernelReady.type = ipcres->apiData.result.event.cases.kernelReady.type;
            event->cases.kernelReady.parentGridId= ipcres->apiData.result.event.cases.kernelReady.parentGridId;
            event->cases.kernelReady.origin = ipcres->apiData.result.event.cases.kernelReady.origin;
            break;
        case CUDBG_EVENT_KERNEL_FINISHED:
            cudbg_trace ("kernel finished event received");
            event->cases.kernelFinished.dev = ipcres->apiData.result.event.cases.kernelFinished.dev;
            event->cases.kernelFinished.gridId64 = ipcres->apiData.result.event.cases.kernelFinished.gridId64;
            event->cases.kernelFinished.tid = ipcres->apiData.result.event.cases.kernelFinished.tid;
            event->cases.kernelFinished.context = ipcres->apiData.result.event.cases.kernelFinished.context;
            event->cases.kernelFinished.module = ipcres->apiData.result.event.cases.kernelFinished.module;
            event->cases.kernelFinished.function = ipcres->apiData.result.event.cases.kernelFinished.function;
            event->cases.kernelFinished.functionEntry = ipcres->apiData.result.event.cases.kernelFinished.functionEntry;
            break;
        case CUDBG_EVENT_INTERNAL_ERROR:
            cudbg_trace ("internal error event received");
            event->cases.internalError.errorType = ipcres->apiData.result.event.cases.internalError.errorType;
            break;
        case CUDBG_EVENT_CTX_PUSH:
            cudbg_trace ("ctx push event received");
            event->cases.contextPush.dev = ipcres->apiData.result.event.cases.contextPush.dev;
            event->cases.contextPush.tid = ipcres->apiData.result.event.cases.contextPush.tid;
            event->cases.contextPush.context = ipcres->apiData.result.event.cases.contextPush.context;
            break;
        case CUDBG_EVENT_CTX_POP:
            cudbg_trace ("ctx pop event received");
            event->cases.contextPop.dev = ipcres->apiData.result.event.cases.contextPop.dev;
            event->cases.contextPop.tid = ipcres->apiData.result.event.cases.contextPop.tid;
            event->cases.contextPop.context = ipcres->apiData.result.event.cases.contextPop.context;
            break;
        case CUDBG_EVENT_CTX_CREATE:
            cudbg_trace ("ctx create event received");
            event->cases.contextCreate.dev = ipcres->apiData.result.event.cases.contextCreate.dev;
            event->cases.contextCreate.tid = ipcres->apiData.result.event.cases.contextCreate.tid;
            event->cases.contextCreate.context = ipcres->apiData.result.event.cases.contextCreate.context;
            break;
        case CUDBG_EVENT_CTX_DESTROY:
            cudbg_trace ("ctx destroy event received");
            event->cases.contextDestroy.dev = ipcres->apiData.result.event.cases.contextDestroy.dev;
            event->cases.contextDestroy.tid = ipcres->apiData.result.event.cases.contextDestroy.tid;
            event->cases.contextDestroy.context = ipcres->apiData.result.event.cases.contextDestroy.context;
            break;
        case CUDBG_EVENT_TIMEOUT:
            cudbg_trace ("timeout event received");
            break;
        case CUDBG_EVENT_ATTACH_COMPLETE:
            cudbg_trace ("Finished collecting CUDA state for attaching to the app");
            break;
        case CUDBG_EVENT_DETACH_COMPLETE:
            cudbg_trace ("Finished detaching from the CUDA app");
            break;
        case CUDBG_EVENT_INVALID:
            cudbg_trace ("No valid event received");
            break;
        default:
            cudbg_trace ("Invalid event received (kind=%d)", event->kind);
            return CUDBG_ERROR_INTERNAL;
    }

    res = ipcres->result;

    return res;
}

static CUDBGResult
cudbgGetNextSyncEvent(CUDBGEvent *event)
{
    return cudbgGetNextEventCommon(event, CUDBGAPIREQ_getNextSyncEvent);
}

static CUDBGResult
cudbgGetNextAsyncEvent(CUDBGEvent *event)
{
    return cudbgGetNextEventCommon(event, CUDBGAPIREQ_getNextAsyncEvent);
}

static CUDBGResult
cudbgGetHostAddrFromDeviceAddr(uint32_t dev, uint64_t device_addr, uint64_t *host_addr)
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;
    
    if (!host_addr)
        return CUDBG_ERROR_INVALID_ARGS;
    
    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_getHostAddrFromDeviceAddr;
    ipcreq.apiData.request.dev = dev;
    ipcreq.apiData.request.addr = device_addr;
    
    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;
    
    res = ipcres->result;
    
    *host_addr = ipcres->apiData.result.ra;

    return res;
}

static CUDBGResult
cudbgClearAttachState(void)
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_clearAttachState;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    return res;
}

static CUDBGResult
cudbgRequestCleanupOnDetach(void)
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_requestCleanupOnDetach;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    return res;
}

static CUDBGResult
cudbgMemcheckReadErrorAddress(uint32_t dev, uint32_t sm, uint32_t wp, uint32_t ln, uint64_t *address, ptxStorageKind *storage)
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_memcheckReadErrorAddress;
    ipcreq.apiData.request.dev = dev;
    ipcreq.apiData.request.sm  = sm;
    ipcreq.apiData.request.wp  = wp;
    ipcreq.apiData.request.ln  = ln;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    *address = ipcres->apiData.result.ra;
    *storage = ipcres->apiData.result.regClass;
    return res;
}

static CUDBGResult
cudbgGetGridStatus(uint32_t dev, uint64_t gridId64, CUDBGGridStatus *status)
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_getGridStatus;
    ipcreq.apiData.request.dev = dev;
    ipcreq.apiData.request.gridId64 = gridId64;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    *status = (CUDBGGridStatus)ipcres->apiData.result.val;

    return res;
}

static CUDBGResult
cudbgGetGridInfo(uint32_t dev, uint64_t grid_id, CUDBGGridInfo *gridInfo)
{
    char *d, *p;
    CUDBGAPIMSG_t ipcreq, *ipcres;
    CUDBGResult res;

    memset(&ipcreq, 0, sizeof ipcreq);
    memset(gridInfo, 0, sizeof *gridInfo);
    ipcreq.kind = CUDBGAPIREQ_getGridInfo;
    ipcreq.apiData.request.dev = dev;
    ipcreq.apiData.request.gridId64 = grid_id;
    ipcreq.apiData.request.sz = sizeof *gridInfo;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_APPEND(gridInfo, sizeof *gridInfo);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    res = ipcres->result;

    p = d + sizeof ipcreq;
    memcpy (gridInfo, p, sizeof *gridInfo);

    return res;
}

static CUDBGResult
cudbgSetKernelLaunchNotificationMode (CUDBGKernelLaunchNotifyMode mode)
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_setKernelLaunchNotificationMode;
    ipcreq.apiData.request.val = mode;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_REQUEST((void *)&d);
    ipcres = (CUDBGAPIMSG_t *)d;

    return ipcres->result;

}

static CUDBGResult
cudbgGetDevicePCIBusInfo (uint32_t dev, uint32_t *pciBusId, uint32_t *pciDevId)
{
    void *d;
    CUDBGAPIMSG_t ipcreq, *ipcres;

    memset(&ipcreq, 0, sizeof ipcreq);
    ipcreq.kind = CUDBGAPIREQ_getDevicePCIBusInfo;
    ipcreq.apiData.request.dev = dev;

    CUDBG_IPC_APPEND(&ipcreq, sizeof ipcreq);
    CUDBG_IPC_REQUEST((void *)&d);

    ipcres = (CUDBGAPIMSG_t *)d;

    *pciBusId = ipcres->apiData.result.pciBusId;
    *pciDevId = ipcres->apiData.result.pciDevId;

    return ipcres->result;
}


/*Stubs (Unused functions) Assert if they are called */

static CUDBGResult
STUB_cudbgSetBreakpoint31(uint64_t addr)
{
    gdb_assert (0);
    return CUDBG_SUCCESS;
}

static CUDBGResult
STUB_cudbgUnsetBreakpoint31(uint64_t addr)
{
    gdb_assert (0);
    return CUDBG_SUCCESS;
}

static CUDBGResult
STUB_cudbgReadBlockIdx32(uint32_t dev, uint32_t sm, uint32_t wp, CuDim2 *blockIdx)
{
    gdb_assert (0);
    return CUDBG_SUCCESS;
}

static CUDBGResult
STUB_cudbgReadGlobalMemory31(uint32_t dev, uint64_t addr, void *buf, uint32_t sz)
{
    gdb_assert (0);
    return CUDBG_SUCCESS;
}

static CUDBGResult
STUB_cudbgWriteGlobalMemory31(uint32_t dev, uint64_t addr, const void *buf, uint32_t sz)
{
    gdb_assert (0);
    return CUDBG_SUCCESS;
}

static CUDBGResult
STUB_cudbgGetGridDim32(uint32_t dev, uint32_t sm, uint32_t wp, CuDim2 *gridDim)
{
    gdb_assert (0);
    return CUDBG_SUCCESS;
}

static CUDBGResult
STUB_cudbgGetPhysicalRegister30(uint64_t pc, char *reg, uint32_t *buf, uint32_t sz, uint32_t *numPhysRegs, CUDBGRegClass *regClass)
{
    gdb_assert (0);
    return CUDBG_SUCCESS;
}

static CUDBGResult
STUB_cudbgSetNotifyNewEventCallback31(CUDBGNotifyNewEventCallback31 callback, void *data)
{
    gdb_assert (0);
    return CUDBG_SUCCESS;
}

static CUDBGResult
STUB_cudbgGetNextEvent30(CUDBGEvent30 *event)
{
    gdb_assert (0);
    return CUDBG_SUCCESS;
}

static CUDBGResult
STUB_cudbgAcknowledgeEvent30(CUDBGEvent30 *event)
{
    gdb_assert (0);
    return CUDBG_SUCCESS;
}

static CUDBGResult
STUB_cudbgGetNextEvent32(CUDBGEvent32 *event)
{
    gdb_assert (0);
    return CUDBG_SUCCESS;
}

static CUDBGResult
STUB_cudbgReadCallDepth32(uint32_t dev, uint32_t sm, uint32_t wp, uint32_t *depth)
{
    gdb_assert (0);
    return CUDBG_SUCCESS;
}

static CUDBGResult
STUB_cudbgReadReturnAddress32(uint32_t dev, uint32_t sm, uint32_t wp, uint32_t level, uint64_t *ra)
{
    gdb_assert (0);
    return CUDBG_SUCCESS;
}

static CUDBGResult
STUB_cudbgReadVirtualReturnAddress32(uint32_t dev, uint32_t sm, uint32_t wp, uint32_t level, uint64_t *ra)
{
    gdb_assert (0);
    return CUDBG_SUCCESS;
}

static CUDBGResult
STUB_cudbgGetElfImage32(uint32_t dev, uint32_t sm, uint32_t wp, bool relocated, void **elfImage, uint32_t *size)
{
    gdb_assert (0);
    return CUDBG_SUCCESS;
}

static CUDBGResult
STUB_cudbgSingleStepWarp40(uint32_t dev, uint32_t sm, uint32_t wp)
{
    gdb_assert (0);
    return CUDBG_SUCCESS;
}

static CUDBGResult
STUB_cudbgSetNotifyNewEventCallback40(CUDBGNotifyNewEventCallback40 callback)
{
    gdb_assert (0);
    return CUDBG_SUCCESS;
}

static CUDBGResult
STUB_cudbgGetPhysicalRegister40(uint32_t dev, uint32_t sm, uint32_t wp, uint64_t pc, char *reg, uint32_t *buf, uint32_t sz, uint32_t *numPhysRegs, CUDBGRegClass *regClass)
{
    gdb_assert (0);
    return CUDBG_SUCCESS;
}

static CUDBGResult
STUB_cudbgGetNextEvent42(CUDBGEvent42 *event)
{
    gdb_assert (0);
    return CUDBG_SUCCESS;
}

static CUDBGResult
STUB_cudbgAcknowledgeEvents42(void)
{
    gdb_assert (0);
    return CUDBG_SUCCESS;
}

static CUDBGResult
STUB_cudbgGetNextSyncEvent50(CUDBGEvent50 *event)
{
    gdb_assert (0);
    return CUDBG_SUCCESS;
}

static CUDBGResult
STUB_cudbgGetNextAsyncEvent50(CUDBGEvent50 *event)
{
    gdb_assert (0);
    return CUDBG_SUCCESS;
}

static CUDBGResult
STUB_cudbgReadGridId50(uint32_t dev, uint32_t sm, uint32_t wp, uint32_t *gridId)
{
    gdb_assert (0);
    return CUDBG_SUCCESS;
}

static CUDBGResult
STUB_cudbgGetGridStatus50(uint32_t dev, uint32_t gridId, CUDBGGridStatus *status)
{
    gdb_assert (0);
    return CUDBG_SUCCESS;
}

static const struct CUDBGAPI_st cudbgCurrentApi = {
    /* Initialization */
    cudbgInitialize,
    cudbgFinalize,

    /* Device Execution Control */
    cudbgSuspendDevice,
    cudbgResumeDevice,
    STUB_cudbgSingleStepWarp40,

    /* Breakpoints */
    STUB_cudbgSetBreakpoint31,
    STUB_cudbgUnsetBreakpoint31,

    /* Device State Inspection */
    STUB_cudbgReadGridId50,
    STUB_cudbgReadBlockIdx32,
    cudbgReadThreadIdx,
    cudbgReadBrokenWarps,
    cudbgReadValidWarps,
    cudbgReadValidLanes,
    cudbgReadActiveLanes,
    cudbgReadCodeMemory,
    cudbgReadConstMemory,
    STUB_cudbgReadGlobalMemory31,
    cudbgReadParamMemory,
    cudbgReadSharedMemory,
    cudbgReadLocalMemory,
    cudbgReadRegister,
    cudbgReadPC,
    cudbgReadVirtualPC,
    cudbgReadLaneStatus,

    /* Device State Alteration */
    STUB_cudbgWriteGlobalMemory31,
    cudbgWriteParamMemory,
    cudbgWriteSharedMemory,
    cudbgWriteLocalMemory,
    cudbgWriteRegister,

    /* Grid Properties */
    STUB_cudbgGetGridDim32,
    cudbgGetBlockDim,
    cudbgGetTID,
    STUB_cudbgGetElfImage32,

    /* Device Properties */
    cudbgGetDeviceType,
    cudbgGetSmType,
    cudbgGetNumDevices,
    cudbgGetNumSMs,
    cudbgGetNumWarps,
    cudbgGetNumLanes,
    cudbgGetNumRegisters,

    /* DWARF */
    STUB_cudbgGetPhysicalRegister30,
    cudbgDisassemble,
    cudbgIsDeviceCodeAddress,
    cudbgLookupDeviceCodeSymbol,

    /* Events */
    STUB_cudbgSetNotifyNewEventCallback31,
    STUB_cudbgGetNextEvent30,
    STUB_cudbgAcknowledgeEvent30,

    /* 3.1 Extensions */
    cudbgGetGridAttribute,
    cudbgGetGridAttributes,
    STUB_cudbgGetPhysicalRegister40,
    cudbgReadLaneException,
    STUB_cudbgGetNextEvent32,
    STUB_cudbgAcknowledgeEvents42,

    /* 3.1 - ABI */
    STUB_cudbgReadCallDepth32,
    STUB_cudbgReadReturnAddress32,
    STUB_cudbgReadVirtualReturnAddress32,

    /* 3.2 Extensions */
    cudbgReadGlobalMemory,
    cudbgWriteGlobalMemory,
    cudbgReadPinnedMemory,
    cudbgWritePinnedMemory,
    cudbgSetBreakpoint,
    cudbgUnsetBreakpoint,
    STUB_cudbgSetNotifyNewEventCallback40,

    /* 4.0 Extensions */
    STUB_cudbgGetNextEvent42,
    cudbgReadTextureMemory,
    cudbgReadBlockIdx,
    cudbgGetGridDim,
    cudbgReadCallDepth,
    cudbgReadReturnAddress,
    cudbgReadVirtualReturnAddress,
    cudbgGetElfImage,

    /* 4.1 Extensions */
    cudbgGetHostAddrFromDeviceAddr,
    cudbgSingleStepWarp,
    cudbgSetNotifyNewEventCallback,
    cudbgReadSyscallCallDepth,

    /* 4.2 Extensions */
    cudbgReadTextureMemoryBindless,

    /* 5.0 Extensions */
    cudbgClearAttachState,
    STUB_cudbgGetNextSyncEvent50,
    cudbgMemcheckReadErrorAddress,
    cudbgAcknowledgeSyncEvents,
    STUB_cudbgGetNextAsyncEvent50,
    cudbgRequestCleanupOnDetach,
    cudbgInitializeAttachStub,
    STUB_cudbgGetGridStatus50,

    /* 5.5 Extensions */
    cudbgGetNextSyncEvent,
    cudbgGetNextAsyncEvent,
    cudbgGetGridInfo,
    cudbgReadGridId,
    cudbgGetGridStatus,
    cudbgSetKernelLaunchNotificationMode,
    cudbgGetDevicePCIBusInfo,
};

CUDBGResult
cudbgGetAPI(uint32_t major, uint32_t minor, uint32_t rev, CUDBGAPI *api)
{
    *api = &cudbgCurrentApi;
    return CUDBG_SUCCESS;
}

static void cudbg_trace(char *fmt, ...)
{
#ifdef GDBSERVER
  struct cuda_trace_msg *msg;
#endif
  va_list ap;

  if (!cuda_options_debug_libcudbg())
    return;

  va_start (ap, fmt);
#ifdef GDBSERVER
  msg = xmalloc (sizeof (*msg));
  if (!cuda_first_trace_msg)
    cuda_first_trace_msg = msg;
  else
    cuda_last_trace_msg->next = msg;
  sprintf (msg->buf, "[CUDAGDB] libcudbg ");
  vsnprintf (msg->buf + strlen (msg->buf), sizeof (msg->buf), fmt, ap);
  msg->next = NULL;
  cuda_last_trace_msg = msg;
#else
  fprintf (stderr, "[CUDAGDB] libcudbg ");
  vfprintf (stderr, fmt, ap);
  fprintf (stderr, "\n");
  fflush (stderr);
#endif
}
