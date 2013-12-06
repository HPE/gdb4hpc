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

#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
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

#include <cuda-utils.h>
#include <libcudbg.h>
#include <libcudbgipc.h>
#include <cudadebugger.h>

/*Forward declarations */
static void cudbgipc_trace(char *fmt, ...);

/*Globals */
CUDBGIPC_t commOut;
CUDBGIPC_t commIn;
CUDBGIPC_t commCB;
extern pthread_t cudagdbMainThreadHandle;

static CUDBGResult
cudbgipcCreate(CUDBGIPC_t *ipc, int from, int to, int flags)
{
    snprintf(ipc->name, sizeof (ipc->name), "%s/pipe.%d.%d", 
             cuda_gdb_session_get_dir (), from, to);

    /* If the inferior hasn't been properly set up for cuda
       debugging yet, the fifo should not exist (it is stale).
       Unlink it, and carry on. */
    if (access(ipc->name, F_OK) == 0) {
        if (!cuda_inferior_in_debug_mode()) {
            cudbgipc_trace("Found stale fifo (%s), unlinking...\n", ipc->name);
            if (unlink(ipc->name) && errno != ENOENT)
                return CUDBG_ERROR_COMMUNICATION_FAILURE;
        }
    }

    if ((flags & O_WRONLY) == O_WRONLY) {
        if (access(ipc->name, F_OK) == -1)
            return CUDBG_ERROR_UNINITIALIZED;
    }
    else if (mkfifo(ipc->name, S_IRGRP | S_IWGRP | S_IRUSR | S_IWUSR) && errno != EEXIST) {
        cudbgipc_trace("Failed to create fifo (from=%u, to=%u, file=%s, errno=%d)",
                       from, to, ipc->name, errno);
        return CUDBG_ERROR_COMMUNICATION_FAILURE;
    }

    if ((ipc->fd = open(ipc->name, flags)) == -1) {
        cudbgipc_trace("Pipe opening failure (from=%u, to=%u, flags=%x, file=%s, errno=%d)",
                       ipc->from, ipc->to, flags, ipc->name, errno);
        return CUDBG_ERROR_COMMUNICATION_FAILURE;
    }

    if ((flags & O_WRONLY) == O_WRONLY) {
        /* If opening for write, unlink it instantly */
        if (unlink(ipc->name) && errno != ENOENT) {
            cudbgipc_trace("Cannot unlink fifo (from=%u, to=%u, file=%s, errno=%d)",
                           ipc->from, ipc->to, ipc->name, errno);
            return CUDBG_ERROR_COMMUNICATION_FAILURE;
        }
    }

    /* Initialize message */
    ipc->dataSize = sizeof(ipc->dataSize);
    ipc->data     = malloc(sizeof(ipc->dataSize));
    memset(ipc->data, 0, ipc->dataSize);

    /* Indicate successful initialization */
    ipc->from        = from;
    ipc->to          = to;
    ipc->initialized = true;

    return CUDBG_SUCCESS;
}

static CUDBGResult
cudbgipcDestroy(CUDBGIPC_t *ipc)
{
    gdb_assert (ipc->name);

    if (close(ipc->fd) == -1) {
        cudbgipc_trace("Failed to close ipc (from=%u, to=%u, errno=%u)",
                       ipc->from, ipc->to, errno);
        return CUDBG_ERROR_COMMUNICATION_FAILURE;
    }

    /* not an error if file does not exist */
    if (unlink(ipc->name) && errno != ENOENT) {
        cudbgipc_trace("Cannot unlink fifo (from=%u, to=%u, file=%s, errno=%d)",
                       ipc->from, ipc->to, ipc->name, errno);
        return CUDBG_ERROR_COMMUNICATION_FAILURE;
    }

    bzero(ipc->name, sizeof (ipc->name));
    free(ipc->data);
    ipc->from = 0;
    ipc->to = 0;
    ipc->initialized = false;

    return CUDBG_SUCCESS;
}

CUDBGResult
cudbgipcInitializeCommIn(void)
{
    CUDBGResult res;

    res = cudbgipcCreate(&commIn,
                         LIBCUDBG_PIPE_ENDPOINT_RPCD,
                         LIBCUDBG_PIPE_ENDPOINT_DEBUG_CLIENT,
                         O_RDONLY | O_NONBLOCK);
    if (res != CUDBG_SUCCESS)
        return res;

    cudbgipc_trace("initialized commIn (from = %d, to = %d)", commIn.from, commIn.to);
    return CUDBG_SUCCESS;
}

CUDBGResult
cudbgipcInitializeCommOut(void)
{
    CUDBGResult res;

    res = cudbgipcCreate(&commOut,
                         LIBCUDBG_PIPE_ENDPOINT_DEBUG_CLIENT,
                         LIBCUDBG_PIPE_ENDPOINT_RPCD,
                         O_WRONLY);
    if (res != CUDBG_SUCCESS)
        return res;

    cudbgipc_trace("initialized commOut (from = %d, to = %d)", commOut.from, commOut.to);
    return CUDBG_SUCCESS;
}

CUDBGResult
cudbgipcInitializeCommCB(void)
{
    CUDBGResult res;

    res = cudbgipcCreate(&commCB,
                         LIBCUDBG_PIPE_ENDPOINT_RPCD_CB,
                         LIBCUDBG_PIPE_ENDPOINT_DEBUG_CLIENT_CB,
                         O_RDONLY | O_NONBLOCK);
    if (res != CUDBG_SUCCESS)
        return res;

    cudbgipc_trace("initialized commCB (from = %d, to = %d)", commCB.from, commCB.to);

    return CUDBG_SUCCESS;
}

static CUDBGResult
cudbgipcPush(CUDBGIPC_t *out)
{
    int64_t writeCount = 0;
    uint64_t offset = 0;
    char *buf;

    gdb_assert (out);

    /* Push out the header (size) */
    memcpy(out->data, (char*)&out->dataSize, sizeof(out->dataSize));
    buf = out->data;

    /* Push out the data */
    for (offset = 0, writeCount = 0; offset < out->dataSize; offset += writeCount) {
        writeCount = write(out->fd, buf + offset, out->dataSize - offset);
        if (writeCount < 0) {
            /* Forward SIGINT received during syscall to main thread signal handler */
            if (errno == EINTR && pthread_self() != cudagdbMainThreadHandle)
              pthread_kill (cudagdbMainThreadHandle, SIGINT);

            if (errno != EAGAIN && errno != EINTR) {
                cudbgipc_trace("Fifo write error (from=%u, to=%u, out->datSize=%u, offset=%u, errno=%d)",
                               out->from, out->to, out->dataSize, offset, errno);
                return CUDBG_ERROR_COMMUNICATION_FAILURE;
            }
            writeCount = 0;
        }
    }

    memset(out->data, 0, sizeof(out->dataSize));
    out->dataSize = sizeof(out->dataSize);
    return CUDBG_SUCCESS;
}

static CUDBGResult
cudbgipcRead(CUDBGIPC_t *in, void *buf, uint32_t size)
{
    int64_t readCount = 0;
    uint64_t offset = 0;

    gdb_assert (in);

    for (offset = 0; offset < size; offset += readCount) {
        readCount = read(in->fd, (char*)buf + offset, size - offset);
        if (readCount == 0) {
            cudbgipc_trace("EOF reached");
            return CUDBG_ERROR_COMMUNICATION_FAILURE;
        }
        if (readCount < 0) {
            /* Forward SIGINT received during syscall to main thread signal handler */
            if (errno == EINTR && pthread_self() != cudagdbMainThreadHandle)
              pthread_kill (cudagdbMainThreadHandle, SIGINT);

            if (errno != EAGAIN && errno != EINTR) {
                cudbgipc_trace("Fifo read error (from=%u, to=%u, size=%u, offset=%u, errno=%d)",
                               in->from, in->to, size, offset, errno);
                return CUDBG_ERROR_COMMUNICATION_FAILURE;
            }
            readCount = 0;
        }
    }

    return CUDBG_SUCCESS;
}

static CUDBGResult
cudbgipcPull(CUDBGIPC_t *in)
{
    CUDBGResult res;

    /* Obtain the size */
    res = cudbgipcRead(in, &in->dataSize, sizeof in->dataSize);
    if (res != CUDBG_SUCCESS) {
        cudbgipc_trace("failed to read size (res=%d)", res);
        return res;
    }

    /* Allocate memory given the size */
    if ((in->data = realloc(in->data, in->dataSize)) == 0) {
        cudbgipc_trace("Memory reallocation failed (res=%d)", res);
        return CUDBG_ERROR_COMMUNICATION_FAILURE;
    }
    memset(in->data, 0, in->dataSize);

    /* Obtain the data */
    res = cudbgipcRead(in, in->data, in->dataSize - sizeof in->dataSize);
    if (res != CUDBG_SUCCESS) {
        cudbgipc_trace("failed to read data (res=%d)", res);
        return res;
    }

    return CUDBG_SUCCESS;
}

static CUDBGResult
cudbgipcWait(CUDBGIPC_t *in)
{
   fd_set readFDS;
   fd_set errFDS;
   int ret;

   if (!in->initialized)
       return CUDBG_ERROR_COMMUNICATION_FAILURE;

   /* wait for data to be available for reading */
   FD_ZERO(&readFDS);
   FD_ZERO(&errFDS);
   FD_SET(in->fd, &readFDS);
   FD_SET(in->fd, &errFDS);
   do {
       ret = select(in->fd + 1, &readFDS, NULL, &errFDS, NULL);

       /* Forward SIGINT received during syscall to main thread signal handler */
       if (ret < 0 && errno == EINTR && pthread_self() != cudagdbMainThreadHandle)
         pthread_kill (cudagdbMainThreadHandle, SIGINT);
   } while (ret == -1 && errno == EINTR);

   if (ret == -1) {
       cudbgipc_trace("Select error (from=%u, to=%u, errno=%u)", in->from, in->to, errno);
       return CUDBG_ERROR_COMMUNICATION_FAILURE;
   }

   if (FD_ISSET(in->fd, &errFDS)) {
       cudbgipc_trace("Select error on in->fd (from=%u, to=%u, errno=%u)", in->from, in->to, errno);
       return CUDBG_ERROR_COMMUNICATION_FAILURE;
   }

   return CUDBG_SUCCESS;
}

CUDBGResult
cudbgipcFinalize(void)
{
    CUDBGResult res;

    res = cudbgipcDestroy(&commOut);
    if (res != CUDBG_SUCCESS)
        return res;

    res = cudbgipcDestroy(&commIn);
    if (res != CUDBG_SUCCESS)
        return res;

    res = cudbgipcDestroy(&commCB);
    if (res != CUDBG_SUCCESS)
        return res;

    return CUDBG_SUCCESS;
}

CUDBGResult
cudbgipcAppend(void *d, uint32_t size)
{
    CUDBGResult res;
    uint32_t dataSize = 0;
    void *data = NULL;

    if (!commOut.initialized) {
        res = cudbgipcInitializeCommOut();
        if (res != CUDBG_SUCCESS)
            return res;
    }

    dataSize = commOut.dataSize + size;
    if ((data = realloc(commOut.data, dataSize)) == NULL)
        return CUDBG_ERROR_COMMUNICATION_FAILURE;

    memcpy(((char *)data) + commOut.dataSize, d, size);

    commOut.data = data;
    commOut.dataSize = dataSize;

    return CUDBG_SUCCESS;
}

CUDBGResult
cudbgipcRequest(void **d)
{
    CUDBGResult res;

    res = cudbgipcPush(&commOut);
    if (res != CUDBG_SUCCESS) {
        cudbgipc_trace("cudbgipcRequest push failed (res=%d)", res);
        return res;
    }

    res = cudbgipcWait(&commIn);
    if (res != CUDBG_SUCCESS) {
        cudbgipc_trace("cudbgipcRequest wait failed (res=%d)", res);
        return res;
    }

    res = cudbgipcPull(&commIn);
    if (res != CUDBG_SUCCESS) {
        cudbgipc_trace("cudbgipcRequest pull failed (res=%d)", res);
        return res;
    }

    *(uintptr_t **)d = (uintptr_t *)commIn.data;

    return CUDBG_SUCCESS;
}

CUDBGResult
cudbgipcCBWaitForData(void *d, uint32_t size)
{
    CUDBGResult res;

    if (!commCB.initialized) {
        res = cudbgipcInitializeCommCB();
        if (res != CUDBG_SUCCESS) {
            cudbgipc_trace("failed to initialize cb fifo (res=%d)", res);
            return res;
        }
    }

    res = cudbgipcWait(&commCB);
    if (res != CUDBG_SUCCESS) {
        cudbgipc_trace("CB wait for data failed (res=%d)", res);
        return res;
    }

    res = cudbgipcRead(&commCB, d, size);
    if (res != CUDBG_SUCCESS) {
        cudbgipc_trace("CB read data failed (res=%d)", res);
        return res;
    }

    return CUDBG_SUCCESS;
}

void cudbgipc_trace(char *fmt, ...)
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
  sprintf (msg->buf, "[CUDAGDB] libcudbg ipc");
  vsnprintf (msg->buf + strlen (msg->buf), sizeof (msg->buf), fmt, ap);
  msg->next = NULL;
  cuda_last_trace_msg = msg;
#else
  fprintf (stderr, "[CUDAGDB] libcudbg ipc");
  vfprintf (stderr, fmt, ap);
  fprintf (stderr, "\n");
  fflush (stderr);
#endif
}
