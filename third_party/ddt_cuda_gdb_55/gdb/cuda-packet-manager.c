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

#include <stdbool.h>
#include "defs.h"
#include "gdbthread.h"
#include "remote.h"
#include "remote-cuda.h"
#include "cuda-packet-manager.h"
#include "cuda-context.h"
#include "cuda-events.h"
#include "cuda-state.h"
#include "cuda-textures.h"
#include "cuda-utils.h"
#include "cuda-options.h"


#define PBUFSIZ 16384

struct {
  char *buf;
  long int buf_size;
} pktbuf;

void
alloc_cuda_packet_buffer (void)
{
  if (pktbuf.buf == NULL)
    {
      pktbuf.buf = xmalloc (PBUFSIZ);
      pktbuf.buf_size = PBUFSIZ;
    }
}

void
free_cuda_packet_buffer (void *unused)
{
  if (pktbuf.buf != NULL)
    {
      xfree (pktbuf.buf);
      pktbuf.buf = NULL;
      pktbuf.buf_size = 0;
    }
}

static char *
append_string (const char *src, char *dest, bool sep)
{
  char *p;

  if (dest + strlen (src) - pktbuf.buf >= pktbuf.buf_size)
    error (_("Exceed the size of cuda packet.\n"));

  sprintf (dest, "%s", src);
  p = strchr (dest, '\0');

  if (sep)
    {
      *p = ';';
      *(++p) = '\0';
    }
  return p;
}

static char *
append_bin (const gdb_byte *src, char *dest, int size, bool sep)
{
  char *p;

  if (dest + size * 2 - pktbuf.buf >= pktbuf.buf_size)
    error (_("Exceed the size of cuda packet.\n"));

  bin2hex (src, dest, size);
  p = strchr (dest, '\0');
  
  if (sep)
    {
      *p = ';';
      *(++p) = '\0';
    } 
  return p;
}

static char *
extract_string (char *src)
{
  return strtok (src, ";");
}

static char *
extract_bin (char *src, gdb_byte *dest, int size)
{
  char *p;

  p = extract_string (src);
  if (!p)
    error (_("The data in the cuda packet is not complete.\n")); 
  hex2bin (p, dest, size);
  return p;
}

CUDBGResult
cuda_remote_api_resume_device (uint32_t dev)
{
  char *p;
  CUDBGResult res;
  cuda_packet_type_t packet_type = RESUME_DEVICE;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), true);
  p = append_bin ((gdb_byte *) &dev, p, sizeof (dev), false);

  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);
  extract_bin (pktbuf.buf, (gdb_byte *) &res, sizeof (res));
  return res;
}

CUDBGResult
cuda_remote_api_suspend_device (uint32_t dev)
{
  char *p;
  CUDBGResult res;
  cuda_packet_type_t packet_type = SUSPEND_DEVICE;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), true);
  p = append_bin ((gdb_byte *) &dev, p, sizeof (dev), false);

  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);
  extract_bin (pktbuf.buf, (gdb_byte *) &res, sizeof (res));
  return res;
}

CUDBGResult
cuda_remote_api_single_step_warp (uint32_t dev, uint32_t sm, uint32_t wp, uint64_t *warp_mask)
{
  char *p;
  CUDBGResult res;
  cuda_packet_type_t packet_type = SINGLE_STEP_WARP;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), true);
  p = append_bin ((gdb_byte *) &dev, p, sizeof (dev), true);
  p = append_bin ((gdb_byte *) &sm, p, sizeof (sm), true);
  p = append_bin ((gdb_byte *) &wp, p, sizeof (wp), true);
  p = append_bin ((gdb_byte *) warp_mask, p, sizeof (*warp_mask), false);

  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);
  extract_bin (pktbuf.buf, (gdb_byte *) &res, sizeof (res));
  extract_bin (NULL, (gdb_byte *) warp_mask, sizeof (*warp_mask));
  return res;
}

CUDBGResult
cuda_remote_api_set_breakpoint (uint32_t dev, uint64_t addr)
{
  char *p;
  CUDBGResult res;
  cuda_packet_type_t packet_type = SET_BREAKPOINT;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), true);
  p = append_bin ((gdb_byte *) &dev, p, sizeof (dev), true);
  p = append_bin ((gdb_byte *) &addr, p, sizeof (addr), false);
  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);
  
  extract_bin (pktbuf.buf, (gdb_byte *) &res, sizeof (res));
  return res;
}

CUDBGResult
cuda_remote_api_unset_breakpoint (uint32_t dev, uint64_t addr)
{
  char *p;
  CUDBGResult res;
  cuda_packet_type_t packet_type = UNSET_BREAKPOINT;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), true);
  p = append_bin ((gdb_byte *) &dev, p, sizeof (dev), true);
  p = append_bin ((gdb_byte *) &addr, p, sizeof (addr), false);
  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);

  extract_bin (pktbuf.buf, (gdb_byte *) &res, sizeof (res));
  return res;
}

CUDBGResult
cuda_remote_api_get_host_addr_from_device_addr (uint32_t dev, uint64_t addr, uint64_t *hostaddr)
{
  char *p;
  CUDBGResult res;
  cuda_packet_type_t packet_type = GET_HOST_ADDR_FROM_DEVICE_ADDR;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), true);
  p = append_bin ((gdb_byte *) &dev, p, sizeof (dev), true);
  p = append_bin ((gdb_byte *) &addr, p, sizeof (addr), true);
  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);

  extract_bin (pktbuf.buf, (gdb_byte *) &res, sizeof (res));
  extract_bin (NULL, (gdb_byte *) hostaddr, sizeof (*hostaddr));
  return res;
}

CUDBGResult
cuda_remote_api_read_grid_id (uint32_t dev_id, uint32_t sm_id, uint32_t wp_id, uint64_t *grid_id)
{
  char *p;
  CUDBGResult res;
  cuda_packet_type_t packet_type = READ_GRID_ID;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), true);
  p = append_bin ((gdb_byte *) &dev_id, p, sizeof (dev_id), true);
  p = append_bin ((gdb_byte *) &sm_id, p, sizeof (sm_id), true);
  p = append_bin ((gdb_byte *) &wp_id, p, sizeof (wp_id), false);

  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);
  extract_bin (pktbuf.buf, (gdb_byte *) &res, sizeof (res));
  extract_bin (NULL, (gdb_byte *) grid_id, sizeof (*grid_id));
  return res;
}

CUDBGResult
cuda_remote_api_read_block_idx (uint32_t dev_id, uint32_t sm_id, uint32_t wp_id, CuDim3 *block_idx)
{
  char *p;
  CUDBGResult res;
  cuda_packet_type_t packet_type = READ_BLOCK_IDX;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), true);
  p = append_bin ((gdb_byte *) &dev_id, p, sizeof (dev_id), true);
  p = append_bin ((gdb_byte *) &sm_id, p, sizeof (sm_id), true);
  p = append_bin ((gdb_byte *) &wp_id, p, sizeof (wp_id), false);

  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);
  extract_bin (pktbuf.buf, (gdb_byte *) &res, sizeof (res));
  extract_bin (NULL, (gdb_byte *) block_idx, sizeof (*block_idx));
  return res;
}

CUDBGResult
cuda_remote_api_read_thread_idx (uint32_t dev_id, uint32_t sm_id, uint32_t wp_id,
                                 uint32_t ln_id, CuDim3 *thread_idx)
{
  char *p;
  CUDBGResult res;
  cuda_packet_type_t packet_type = READ_THREAD_IDX;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), true);
  p = append_bin ((gdb_byte *) &dev_id, p, sizeof (dev_id), true);
  p = append_bin ((gdb_byte *) &sm_id, p, sizeof (sm_id), true);
  p = append_bin ((gdb_byte *) &wp_id, p, sizeof (wp_id), true);
  p = append_bin ((gdb_byte *) &ln_id, p, sizeof (ln_id), false);

  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);
  extract_bin (pktbuf.buf, (gdb_byte *) &res, sizeof (res));
  extract_bin (NULL, (gdb_byte *) thread_idx, sizeof (*thread_idx));
  return res;
}

CUDBGResult
cuda_remote_api_read_broken_warps (uint32_t dev, uint32_t sm, uint64_t *broken_warps_mask)
{
  char *p;
  CUDBGResult res;
  cuda_packet_type_t packet_type = READ_BROKEN_WARPS;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), true);
  p = append_bin ((gdb_byte *) &dev, p, sizeof (dev), true);
  p = append_bin ((gdb_byte *) &sm, p, sizeof (sm), false);

  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);
  extract_bin (pktbuf.buf, (gdb_byte *) &res, sizeof (res));
  extract_bin (NULL, (gdb_byte *) broken_warps_mask, sizeof (*broken_warps_mask));
  return res;
}

CUDBGResult
cuda_remote_api_read_valid_warps (uint32_t dev_id, uint32_t sm_id, uint64_t *valid_warps_mask)
{
  char *p;
  CUDBGResult res;
  cuda_packet_type_t packet_type = READ_VALID_WARPS;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), true);
  p = append_bin ((gdb_byte *) &dev_id, p, sizeof (dev_id), true);
  p = append_bin ((gdb_byte *) &sm_id, p, sizeof (sm_id), false); 

  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);	
  extract_bin (pktbuf.buf, (gdb_byte *) &res, sizeof (res));
  extract_bin (NULL, (gdb_byte *) valid_warps_mask, sizeof (*valid_warps_mask));
  return res;
}

CUDBGResult
cuda_remote_api_read_valid_lanes (uint32_t dev_id, uint32_t sm_id,
                                  uint32_t wp_id, uint32_t *valid_lanes_mask)
{
  char *p;
  CUDBGResult res;
  cuda_packet_type_t packet_type = READ_VALID_LANES;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), true);
  p = append_bin ((gdb_byte *) &dev_id, p, sizeof (dev_id), true);
  p = append_bin ((gdb_byte *) &sm_id, p, sizeof (sm_id), true);
  p = append_bin ((gdb_byte *) &wp_id, p, sizeof (wp_id), false);

  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);
  extract_bin (pktbuf.buf, (gdb_byte *) &res, sizeof (res));
  extract_bin (NULL, (gdb_byte *) valid_lanes_mask, sizeof (*valid_lanes_mask));
  return res;
}

CUDBGResult
cuda_remote_api_read_active_lanes (uint32_t dev_id, uint32_t sm_id, uint32_t wp_id, uint32_t *active_lanes)
{
  char *p;
  CUDBGResult res;
  cuda_packet_type_t packet_type = READ_ACTIVE_LANES;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), true);
  p = append_bin ((gdb_byte *) &dev_id, p, sizeof (dev_id), true);
  p = append_bin ((gdb_byte *) &sm_id, p, sizeof (sm_id), true);
  p = append_bin ((gdb_byte *) &wp_id, p, sizeof (wp_id), false);

  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);
  extract_bin (pktbuf.buf, (gdb_byte *) &res, sizeof (res));
  extract_bin (NULL, (gdb_byte *) active_lanes, sizeof (*active_lanes));
  return res;
}

CUDBGResult
cuda_remote_api_read_code_memory (uint32_t dev, uint64_t addr, void *buf, uint32_t sz)
{
  char *p;
  CUDBGResult res;
  cuda_packet_type_t packet_type = READ_CODE_MEMORY;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), true);
  p = append_bin ((gdb_byte *) &dev, p, sizeof (dev), true);
  p = append_bin ((gdb_byte *) &addr, p, sizeof (addr), true);
  p = append_bin ((gdb_byte *) &sz, p, sizeof (sz), false);

  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);

  extract_bin (pktbuf.buf, (gdb_byte *) &res, sizeof (res));
  extract_bin (NULL, (gdb_byte *) buf, sz);
  return res;
}

CUDBGResult
cuda_remote_api_read_const_memory (uint32_t dev, uint64_t addr, void *buf, uint32_t sz)
{
  char *p;
  CUDBGResult res;
  cuda_packet_type_t packet_type = READ_CONST_MEMORY;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), true);
  p = append_bin ((gdb_byte *) &dev, p, sizeof (dev), true);
  p = append_bin ((gdb_byte *) &addr, p, sizeof (addr), true);
  p = append_bin ((gdb_byte *) &sz, p, sizeof (sz), false);

  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);

  extract_bin (pktbuf.buf, (gdb_byte *) &res, sizeof (res));
  extract_bin (NULL, (gdb_byte *) buf, sz);
  return res;
}

CUDBGResult
cuda_remote_api_read_global_memory (uint32_t dev, uint32_t sm, uint32_t wp, uint32_t ln,
                                    uint64_t addr, void *buf, uint32_t sz)
{
  char *p;
  CUDBGResult res;
  cuda_packet_type_t packet_type = READ_GLOBAL_MEMORY;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), true);
  p = append_bin ((gdb_byte *) &dev, p, sizeof (dev), true);
  p = append_bin ((gdb_byte *) &sm, p, sizeof (sm), true);
  p = append_bin ((gdb_byte *) &wp, p, sizeof (wp), true);
  p = append_bin ((gdb_byte *) &ln, p, sizeof (ln), true);
  p = append_bin ((gdb_byte *) &addr, p, sizeof (addr), true);
  p = append_bin ((gdb_byte *) &sz, p, sizeof (sz), false);

  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);

  extract_bin (pktbuf.buf, (gdb_byte *) &res, sizeof (res));
  extract_bin (NULL, (gdb_byte *) buf, sz);
  return res;
}

CUDBGResult
cuda_remote_api_read_pinned_memory (uint64_t addr, void *buf, uint32_t sz)
{
  char *p;
  CUDBGResult res;
  cuda_packet_type_t packet_type = READ_PINNED_MEMORY;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), true);
  p = append_bin ((gdb_byte *) &addr, p, sizeof (addr), true);
  p = append_bin ((gdb_byte *) &sz, p, sizeof (sz), false);

  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);

  extract_bin (pktbuf.buf, (gdb_byte *) &res, sizeof (res));
  extract_bin (NULL, (gdb_byte *) buf, sz);
  return res;
}

CUDBGResult
cuda_remote_api_read_param_memory (uint32_t dev, uint32_t sm, uint32_t wp, uint64_t addr,
                                   void *buf, uint32_t sz)
{
  char *p;
  CUDBGResult res;
  cuda_packet_type_t packet_type = READ_PARAM_MEMORY;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), true);
  p = append_bin ((gdb_byte *) &dev, p, sizeof (dev), true);
  p = append_bin ((gdb_byte *) &sm, p, sizeof (sm), true);
  p = append_bin ((gdb_byte *) &wp, p, sizeof (wp), true);
  p = append_bin ((gdb_byte *) &addr, p, sizeof (addr), true);
  p = append_bin ((gdb_byte *) &sz, p, sizeof (sz), false);

  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);

  extract_bin (pktbuf.buf, (gdb_byte *) &res, sizeof (res));
  extract_bin (NULL, (gdb_byte *) buf, sz);
  return res;
}

CUDBGResult
cuda_remote_api_read_shared_memory (uint32_t dev, uint32_t sm, uint32_t wp, uint64_t addr,
                                    void *buf, uint32_t sz)
{
  char *p;
  CUDBGResult res;
  cuda_packet_type_t packet_type = READ_SHARED_MEMORY;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), true);
  p = append_bin ((gdb_byte *) &dev, p, sizeof (dev), true);
  p = append_bin ((gdb_byte *) &sm, p, sizeof (sm), true);
  p = append_bin ((gdb_byte *) &wp, p, sizeof (wp), true);
  p = append_bin ((gdb_byte *) &addr, p, sizeof (addr), true);
  p = append_bin ((gdb_byte *) &sz, p, sizeof (sz), false);

  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);

  extract_bin (pktbuf.buf, (gdb_byte *) &res, sizeof (res));
  extract_bin (NULL, (gdb_byte *) buf, sz);
  return res;
}

CUDBGResult
cuda_remote_api_read_texture_memory (uint32_t dev, uint32_t sm, uint32_t wp, uint32_t id,
                                     uint32_t dim, uint32_t *coords, void *buf, uint32_t sz)
{
  char *p;
  CUDBGResult res;
  cuda_packet_type_t packet_type = READ_TEXTURE_MEMORY;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), true);
  p = append_bin ((gdb_byte *) &dev, p, sizeof (dev), true);
  p = append_bin ((gdb_byte *) &sm, p, sizeof (sm), true);
  p = append_bin ((gdb_byte *) &wp, p, sizeof (wp), true);
  p = append_bin ((gdb_byte *) &id, p, sizeof (id), true);
  p = append_bin ((gdb_byte *) &dim, p, sizeof (dim), true);
  p = append_bin ((gdb_byte *) coords, p, sizeof (*coords) * TEXTURE_DIM_MAX, true);
  p = append_bin ((gdb_byte *) &sz, p, sizeof (sz), false);

  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);

  extract_bin (pktbuf.buf, (gdb_byte *) &res, sizeof (res));
  extract_bin (NULL, (gdb_byte *) buf, sz);
  return res;
}

CUDBGResult
cuda_remote_api_read_texture_memory_bindless (uint32_t dev, uint32_t sm, uint32_t wp, uint32_t tex_symtab_index, 
                                              uint32_t dim, uint32_t *coords, void *buf, uint32_t sz)
{
  char *p;
  CUDBGResult res;
  cuda_packet_type_t packet_type = READ_TEXTURE_MEMORY_BINDLESS;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), true);
  p = append_bin ((gdb_byte *) &dev, p, sizeof (dev), true);
  p = append_bin ((gdb_byte *) &sm, p, sizeof (sm), true);
  p = append_bin ((gdb_byte *) &wp, p, sizeof (wp), true);
  p = append_bin ((gdb_byte *) &tex_symtab_index, p, sizeof (tex_symtab_index), true);
  p = append_bin ((gdb_byte *) &dim, p, sizeof (dim), true);
  p = append_bin ((gdb_byte *) coords, p, sizeof (*coords) * TEXTURE_DIM_MAX, true);
  p = append_bin ((gdb_byte *) &sz, p, sizeof (sz), false);

  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);

  extract_bin (pktbuf.buf, (gdb_byte *) &res, sizeof (res));
  extract_bin (NULL, (gdb_byte *) buf, sz);
  return res;
}

CUDBGResult
cuda_remote_api_read_local_memory (uint32_t dev, uint32_t sm, uint32_t wp, uint32_t ln,
                                   uint64_t addr, void *buf, uint32_t sz)
{
  char *p;
  CUDBGResult res;
  cuda_packet_type_t packet_type = READ_LOCAL_MEMORY;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), true);
  p = append_bin ((gdb_byte *) &dev, p, sizeof (dev), true);
  p = append_bin ((gdb_byte *) &sm, p, sizeof (sm), true);
  p = append_bin ((gdb_byte *) &wp, p, sizeof (wp), true);
  p = append_bin ((gdb_byte *) &ln, p, sizeof (ln), true);
  p = append_bin ((gdb_byte *) &addr, p, sizeof (addr), true);
  p = append_bin ((gdb_byte *) &sz, p, sizeof (sz), false);

  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);

  extract_bin (pktbuf.buf, (gdb_byte *) &res, sizeof (res));
  extract_bin (NULL, (gdb_byte *) buf, sz);
  return res;
}

CUDBGResult
cuda_remote_api_read_register (uint32_t dev, uint32_t sm, uint32_t wp,
                               uint32_t ln, int regno, uint32_t *value)
{
  char *p;
  CUDBGResult res;
  cuda_packet_type_t packet_type = READ_REGISTER;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), true);
  p = append_bin ((gdb_byte *) &dev, p, sizeof (dev), true);
  p = append_bin ((gdb_byte *) &sm, p, sizeof (sm), true);
  p = append_bin ((gdb_byte *) &wp, p, sizeof (wp), true);
  p = append_bin ((gdb_byte *) &ln, p, sizeof (ln), true);
  p = append_bin ((gdb_byte *) &regno, p, sizeof (regno), false);

  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);

  extract_bin (pktbuf.buf, (gdb_byte *) &res, sizeof (res));
  extract_bin (NULL, (gdb_byte *) value, sizeof (*value));
  return res;
}

CUDBGResult
cuda_remote_api_read_pc (uint32_t dev, uint32_t sm, uint32_t wp,
                         uint32_t ln, uint64_t *pc)
{
  char *p;
  CUDBGResult res;
  cuda_packet_type_t packet_type = READ_PC;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), true);
  p = append_bin ((gdb_byte *) &dev, p, sizeof (dev), true);
  p = append_bin ((gdb_byte *) &sm, p, sizeof (sm), true);
  p = append_bin ((gdb_byte *) &wp, p, sizeof (wp), true);
  p = append_bin ((gdb_byte *) &ln, p, sizeof (ln), false);

  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);

  extract_bin (pktbuf.buf, (gdb_byte *) &res, sizeof (res));
  extract_bin (NULL, (gdb_byte *) pc, sizeof (*pc));
  return res;
}

CUDBGResult
cuda_remote_api_read_virtual_pc (uint32_t dev, uint32_t sm, uint32_t wp, 
                                 uint32_t ln, uint64_t *virtual_pc)
{
  char *p;
  CUDBGResult res;
  cuda_packet_type_t packet_type = READ_VIRTUAL_PC;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), true);
  p = append_bin ((gdb_byte *) &dev, p, sizeof (dev), true);
  p = append_bin ((gdb_byte *) &sm, p, sizeof (sm), true);
  p = append_bin ((gdb_byte *) &wp, p, sizeof (wp), true);
  p = append_bin ((gdb_byte *) &ln, p, sizeof (ln), false);

  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);

  extract_bin (pktbuf.buf, (gdb_byte *) &res, sizeof (res));
  extract_bin (NULL, (gdb_byte *) virtual_pc, sizeof (*virtual_pc));
  return res;
}

CUDBGResult
cuda_remote_api_read_lane_exception (uint32_t dev, uint32_t sm, uint32_t wp, uint32_t ln,
                                     CUDBGException_t *exception)
{
  char *p;
  CUDBGResult res;
  cuda_packet_type_t packet_type = READ_LANE_EXCEPTION;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), true);
  p = append_bin ((gdb_byte *) &dev, p, sizeof (dev), true);
  p = append_bin ((gdb_byte *) &sm, p, sizeof (sm), true);
  p = append_bin ((gdb_byte *) &wp, p, sizeof (wp), true);
  p = append_bin ((gdb_byte *) &ln, p, sizeof (ln), false);

  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);

  extract_bin (pktbuf.buf, (gdb_byte *) &res, sizeof (res));
  extract_bin (NULL, (gdb_byte *) exception, sizeof (*exception));
  return res;
}

CUDBGResult
cuda_remote_api_read_call_depth (uint32_t dev, uint32_t sm, uint32_t wp,
                                 uint32_t ln, uint32_t *call_depth)
{
  char *p;
  CUDBGResult res;
  cuda_packet_type_t packet_type = READ_CALL_DEPTH;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), true);
  p = append_bin ((gdb_byte *) &dev, p, sizeof (dev), true);
  p = append_bin ((gdb_byte *) &sm, p, sizeof (sm), true);
  p = append_bin ((gdb_byte *) &wp, p, sizeof (wp), true);
  p = append_bin ((gdb_byte *) &ln, p, sizeof (ln), false);

  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);

  extract_bin (pktbuf.buf, (gdb_byte *) &res, sizeof (res));
  extract_bin (NULL, (gdb_byte *) call_depth, sizeof (*call_depth));
  return res;
}

CUDBGResult
cuda_remote_api_read_syscall_call_depth (uint32_t dev, uint32_t sm, uint32_t wp,
                                         uint32_t ln, uint32_t *syscall_call_depth)
{
  char *p;
  CUDBGResult res;
  cuda_packet_type_t packet_type = READ_SYSCALL_CALL_DEPTH;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), true);
  p = append_bin ((gdb_byte *) &dev, p, sizeof (dev), true);
  p = append_bin ((gdb_byte *) &sm, p, sizeof (sm), true);
  p = append_bin ((gdb_byte *) &wp, p, sizeof (wp), true);
  p = append_bin ((gdb_byte *) &ln, p, sizeof (ln), false);

  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);

  extract_bin (pktbuf.buf, (gdb_byte *) &res, sizeof (res));
  extract_bin (NULL, (gdb_byte *) syscall_call_depth, sizeof (*syscall_call_depth));
  return res;
}

CUDBGResult
cuda_remote_api_read_virtual_return_address (uint32_t dev, uint32_t sm, uint32_t wp,
                                             uint32_t ln, uint32_t level, uint64_t *ra)
{
  char *p;
  CUDBGResult res;
  cuda_packet_type_t packet_type = READ_VIRTUAL_RETURN_ADDRESS;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), true);
  p = append_bin ((gdb_byte *) &dev, p, sizeof (dev), true);
  p = append_bin ((gdb_byte *) &sm, p, sizeof (sm), true);
  p = append_bin ((gdb_byte *) &wp, p, sizeof (wp), true);
  p = append_bin ((gdb_byte *) &ln, p, sizeof (ln), true);
  p = append_bin ((gdb_byte *) &level, p, sizeof (level), false);

  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);

  extract_bin (pktbuf.buf, (gdb_byte *) &res, sizeof (res));
  extract_bin (NULL, (gdb_byte *) ra, sizeof (*ra));
  return res;
}

CUDBGResult
cuda_remote_api_write_global_memory (uint32_t dev, uint32_t sm, uint32_t wp, uint32_t ln,
                                     uint64_t addr, const void *buf, uint32_t sz)
{
  char *p;
  CUDBGResult res;
  cuda_packet_type_t packet_type = WRITE_GLOBAL_MEMORY;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), true);
  p = append_bin ((gdb_byte *) &dev, p, sizeof (dev), true);
  p = append_bin ((gdb_byte *) &sm, p, sizeof (sm), true);
  p = append_bin ((gdb_byte *) &wp, p, sizeof (wp), true);
  p = append_bin ((gdb_byte *) &ln, p, sizeof (ln), true);
  p = append_bin ((gdb_byte *) &addr, p, sizeof (addr), true);
  p = append_bin ((gdb_byte *) &sz, p, sizeof (sz), true);
  p = append_bin ((gdb_byte *) buf, p, sz, false);

  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);
  extract_bin (pktbuf.buf, (gdb_byte *) &res, sizeof (res));
  return res;
}

CUDBGResult 
cuda_remote_api_write_pinned_memory (uint64_t addr, const void *buf, uint32_t sz)
{
  char *p;
  CUDBGResult res;
  cuda_packet_type_t packet_type = WRITE_PINNED_MEMORY;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), true);
  p = append_bin ((gdb_byte *) &addr, p, sizeof (addr), true);
  p = append_bin ((gdb_byte *) &sz, p, sizeof (sz), true);
  p = append_bin ((gdb_byte *) buf, p, sz, false);

  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);

  extract_bin (pktbuf.buf, (gdb_byte *) &res, sizeof (res));
  return res;
}

CUDBGResult 
cuda_remote_api_write_param_memory (uint32_t dev, uint32_t sm, uint32_t wp, uint64_t addr,
                                    const void *buf, uint32_t sz)
{
  char *p;
  CUDBGResult res;
  cuda_packet_type_t packet_type = WRITE_PARAM_MEMORY;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), true);
  p = append_bin ((gdb_byte *) &dev, p, sizeof (dev), true);
  p = append_bin ((gdb_byte *) &sm, p, sizeof (sm), true);
  p = append_bin ((gdb_byte *) &wp, p, sizeof (wp), true);
  p = append_bin ((gdb_byte *) &addr, p, sizeof (addr), true);
  p = append_bin ((gdb_byte *) &sz, p, sizeof (sz), true);
  p = append_bin ((gdb_byte *) buf, p, sz, false);

  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);
  extract_bin (pktbuf.buf, (gdb_byte *) &res, sizeof (res));
  return res;
}

CUDBGResult 
cuda_remote_api_write_shared_memory (uint32_t dev, uint32_t sm, uint32_t wp, uint64_t addr,
                                     const void *buf, uint32_t sz)
{
  char *p;
  CUDBGResult res;
  cuda_packet_type_t packet_type = WRITE_SHARED_MEMORY;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), true);
  p = append_bin ((gdb_byte *) &dev, p, sizeof (dev), true);
  p = append_bin ((gdb_byte *) &sm, p, sizeof (sm), true);
  p = append_bin ((gdb_byte *) &wp, p, sizeof (wp), true);
  p = append_bin ((gdb_byte *) &addr, p, sizeof (addr), true);
  p = append_bin ((gdb_byte *) &sz, p, sizeof (sz), true);
  p = append_bin ((gdb_byte *) buf, p, sz, false);

  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);
  extract_bin (pktbuf.buf, (gdb_byte *) &res, sizeof (res));
  return res;
}

CUDBGResult 
cuda_remote_api_write_local_memory (uint32_t dev, uint32_t sm, uint32_t wp, uint32_t ln,
                                    uint64_t addr, const void *buf, uint32_t sz)
{
  char *p;
  CUDBGResult res;
  cuda_packet_type_t packet_type = WRITE_LOCAL_MEMORY;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), true);
  p = append_bin ((gdb_byte *) &dev, p, sizeof (dev), true);
  p = append_bin ((gdb_byte *) &sm, p, sizeof (sm), true);
  p = append_bin ((gdb_byte *) &wp, p, sizeof (wp), true);
  p = append_bin ((gdb_byte *) &ln, p, sizeof (ln), true);
  p = append_bin ((gdb_byte *) &addr, p, sizeof (addr), true);
  p = append_bin ((gdb_byte *) &sz, p, sizeof (sz), true);
  p = append_bin ((gdb_byte *) buf, p, sz, false);

  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);
  extract_bin (pktbuf.buf, (gdb_byte *) &res, sizeof (res));
  return res;
}

CUDBGResult
cuda_remote_api_write_register (uint32_t dev, uint32_t sm, uint32_t wp,
                                uint32_t ln, int regno, uint32_t value)
{
  char *p;
  CUDBGResult res;
  cuda_packet_type_t packet_type = WRITE_REGISTER;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), true);
  p = append_bin ((gdb_byte *) &dev, p, sizeof (dev), true);
  p = append_bin ((gdb_byte *) &sm, p, sizeof (sm), true);
  p = append_bin ((gdb_byte *) &wp, p, sizeof (wp), true);
  p = append_bin ((gdb_byte *) &ln, p, sizeof (ln), true);
  p = append_bin ((gdb_byte *) &regno, p, sizeof (regno), true);
  p = append_bin ((gdb_byte *) &value, p, sizeof (value), value);

  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);
  extract_bin (pktbuf.buf, (gdb_byte *) &res, sizeof (res));
  return res;
}

CUDBGResult
cuda_remote_api_is_device_code_address (uint64_t addr, bool *is_device_address)
{
  char *p;
  CUDBGResult res;
  cuda_packet_type_t packet_type = IS_DEVICE_CODE_ADDRESS;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), true);
  p = append_bin ((gdb_byte *) &addr, p, sizeof (addr), false);
  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);

  extract_bin (pktbuf.buf, (gdb_byte *) &res, sizeof (res));
  extract_bin (NULL, (gdb_byte *) is_device_address, sizeof (*is_device_address));
  return res;
}

CUDBGResult
cuda_remote_api_disassemble (uint32_t dev, uint64_t addr, uint32_t *inst_size, 
                             char *buf, uint32_t buf_size)
{
  char *p;
  CUDBGResult res; 
  cuda_packet_type_t packet_type = DISASSEMBLE;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), true);
  p = append_bin ((gdb_byte *) &dev, p, sizeof (dev), true);
  p = append_bin ((gdb_byte *) &addr, p, sizeof (addr), true);
  p = append_bin ((gdb_byte *) &buf_size, p, sizeof (buf_size), false);

  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);

  extract_bin (pktbuf.buf, (gdb_byte *) &res, sizeof (res));
  extract_bin (NULL, (gdb_byte *) inst_size, sizeof (inst_size));
  extract_bin (NULL, (gdb_byte *) buf, buf_size);
  return res;
}

CUDBGResult 
cuda_remote_api_memcheck_read_error_address (uint32_t dev, uint32_t sm, uint32_t wp, uint32_t ln,
                                             uint64_t *address, ptxStorageKind *storage)
{
  char *p;
  CUDBGResult res;
  cuda_packet_type_t packet_type = MEMCHECK_READ_ERROR_ADDRESS;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), true);
  p = append_bin ((gdb_byte *) &dev, p, sizeof (dev), true);
  p = append_bin ((gdb_byte *) &sm, p, sizeof (sm), true);
  p = append_bin ((gdb_byte *) &wp, p, sizeof (wp), true);
  p = append_bin ((gdb_byte *) &ln, p, sizeof (ln), false);

  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);

  extract_bin (pktbuf.buf, (gdb_byte *) &res, sizeof (res));
  extract_bin (NULL, (gdb_byte *) address, sizeof (*address));
  extract_bin (NULL, (gdb_byte *) storage, sizeof (*storage));
  return res;
}

CUDBGResult
cuda_remote_api_get_grid_status (uint32_t dev, uint64_t grid_id, CUDBGGridStatus *status)
{
  char *p;
  CUDBGResult res;
  cuda_packet_type_t packet_type = GET_GRID_STATUS;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), true);
  p = append_bin ((gdb_byte *) &dev, p, sizeof (dev), true);
  p = append_bin ((gdb_byte *) &grid_id, p, sizeof (grid_id), false);

  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);

  extract_bin (pktbuf.buf, (gdb_byte *) &res, sizeof (res));
  extract_bin (NULL, (gdb_byte *) status, sizeof (*status));
  return res;
}

CUDBGResult
cuda_remote_api_get_grid_info (uint32_t dev, uint64_t grid_id, CUDBGGridInfo *info)
{
  char *p;
  CUDBGResult res;
  cuda_packet_type_t packet_type = GET_GRID_INFO;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), true);
  p = append_bin ((gdb_byte *) &dev, p, sizeof (dev), true);
  p = append_bin ((gdb_byte *) &grid_id, p, sizeof (grid_id), false);

  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);

  extract_bin (pktbuf.buf, (gdb_byte *) &res, sizeof (res));
  extract_bin (NULL, (gdb_byte *) info, sizeof (*info));
  return res;
}

bool
cuda_remote_notification_pending (void)
{
  char *p;
  bool ret_val;
  cuda_packet_type_t packet_type = NOTIFICATION_PENDING;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), false);
  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);

  extract_bin (pktbuf.buf, (gdb_byte *) &ret_val, sizeof (ret_val));
  return ret_val;
}

bool
cuda_remote_notification_received (void)
{
  char *p;
  bool ret_val;
  cuda_packet_type_t packet_type = NOTIFICATION_RECEIVED;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), false);
  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);

  extract_bin (pktbuf.buf, (gdb_byte *) &ret_val, sizeof (ret_val));
  return ret_val;
}

bool
cuda_remote_notification_aliased_event (void)
{
  char *p;
  bool ret_val;
  cuda_packet_type_t packet_type = NOTIFICATION_ALIASED_EVENT;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), false);
  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);

  extract_bin (pktbuf.buf, (gdb_byte *) &ret_val, sizeof (ret_val));
  return ret_val;
}

void
cuda_remote_notification_analyze (void)
{
  char *p;
  cuda_packet_type_t packet_type = NOTIFICATION_ANALYZE;
  struct thread_info *tp = inferior_thread ();

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), true);
  p = append_bin ((gdb_byte *) &(tp->trap_expected), p, sizeof (tp->trap_expected), false);
  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);
}

void
cuda_remote_notification_mark_consumed (void)
{
  char *p;
  cuda_packet_type_t packet_type = NOTIFICATION_MARK_CONSUMED;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), false);
  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);
}

void
cuda_remote_notification_consume_pending (void)
{
  char *p;
  cuda_packet_type_t packet_type = NOTIFICATION_CONSUME_PENDING;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), false);
  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);
}

bool
cuda_remote_query_events (cuda_event_kind_t cuda_event_kind)
{
  CUDBGResult res;
  CUDBGEvent event;
  bool ret = false;
  char client_object_file_path [CUDA_GDB_TMP_BUF_SIZE] = {0};
  char *p;
  cuda_packet_type_t packet_type;

  if (!cuda_initialized)
    return false;

  switch (cuda_event_kind)
    {
    case CUDA_EVENT_SYNC:
      {
        packet_type = QUERY_SYNC_EVENT;
        break;
      }
    case CUDA_EVENT_ASYNC:
      {
        packet_type = QUERY_ASYNC_EVENT;
        break;
      }
    default:
      {
        error (_("unknown cuda event kind.\n"));
        return false;
      }
    }
  
  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), false);
  
  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);

  p = extract_bin (pktbuf.buf, (gdb_byte *) &res, sizeof (res));
  if (res != CUDBG_SUCCESS && res != CUDBG_ERROR_NO_EVENT_AVAILABLE)
    error (_("Error: Failed to get the next %s CUDA event (error=%u)."), 
             cuda_event_kind == CUDA_EVENT_SYNC ? "sync" : "async", res);
  p = extract_string (NULL);

  while (strcmp ("NO_EVENT", p) != 0)
    {
       ret = true;
       if (strcmp ("ELF_IMAGE_LOADED", p) == 0)
         {
           uint64_t elf_image_size;
           char    *object_file_name;
           char    *server_object_file_path;

           event.kind = CUDBG_EVENT_ELF_IMAGE_LOADED;
           extract_bin (NULL, (gdb_byte *) &(event.cases.elfImageLoaded.dev),
                                     sizeof (event.cases.elfImageLoaded.dev));
           extract_bin (NULL, (gdb_byte *) &(event.cases.elfImageLoaded.context),
                                     sizeof (event.cases.elfImageLoaded.context));
           extract_bin (NULL, (gdb_byte *) &(event.cases.elfImageLoaded.module),
                                     sizeof (event.cases.elfImageLoaded.module));
           extract_bin (NULL, (gdb_byte *) &(event.cases.elfImageLoaded.size),
                                     sizeof (event.cases.elfImageLoaded.size));
           server_object_file_path = extract_string (NULL);
           object_file_name = strrchr (server_object_file_path, '/');
           snprintf (client_object_file_path, sizeof (client_object_file_path),
                     "%s%s", cuda_gdb_session_get_dir (), object_file_name);
           remote_file_get (server_object_file_path, client_object_file_path, 0);
           event.cases.elfImageLoaded.relocatedElfImage = client_object_file_path;
         }
       else if (strcmp ("KERNEL_READY", p) == 0)
         {
           event.kind = CUDBG_EVENT_KERNEL_READY;
           
           extract_bin (NULL, (gdb_byte *) &(event.cases.kernelReady.dev),
                                     sizeof (event.cases.kernelReady.dev));
           extract_bin (NULL, (gdb_byte *) &(event.cases.kernelReady.context),
                                     sizeof (event.cases.kernelReady.context));
           extract_bin (NULL, (gdb_byte *) &(event.cases.kernelReady.module),
                                     sizeof (event.cases.kernelReady.module));
           extract_bin (NULL, (gdb_byte *) &(event.cases.kernelReady.gridId64),
                                     sizeof (event.cases.kernelReady.gridId64));
           extract_bin (NULL, (gdb_byte *) &(event.cases.kernelReady.tid),
                                     sizeof (event.cases.kernelReady.tid));
           extract_bin (NULL, (gdb_byte *) &(event.cases.kernelReady.functionEntry),
                                     sizeof (event.cases.kernelReady.functionEntry));
           extract_bin (NULL, (gdb_byte *) &(event.cases.kernelReady.gridDim),
                                     sizeof (event.cases.kernelReady.gridDim));
           extract_bin (NULL, (gdb_byte *) &(event.cases.kernelReady.blockDim),
                                     sizeof (event.cases.kernelReady.blockDim));
           extract_bin (NULL, (gdb_byte *) &(event.cases.kernelReady.type),
                                     sizeof (event.cases.kernelReady.type));
           extract_bin (NULL, (gdb_byte *) &(event.cases.kernelReady.parentGridId),
                                     sizeof (event.cases.kernelReady.parentGridId));
           extract_bin (NULL, (gdb_byte *) &(event.cases.kernelReady.origin),
                                     sizeof (event.cases.kernelReady.origin));
         }
       else if (strcmp ("KERNEL_FINISHED", p) == 0)
         {
           event.kind = CUDBG_EVENT_KERNEL_FINISHED;

           extract_bin (NULL, (gdb_byte *) &(event.cases.kernelFinished.dev),
                                     sizeof (event.cases.kernelFinished.dev));
           extract_bin (NULL, (gdb_byte *) &(event.cases.kernelFinished.gridId64),
                                     sizeof (event.cases.kernelFinished.gridId64));
         }
       else if (strcmp ("CTX_PUSH", p) == 0)
         {
           event.kind = CUDBG_EVENT_CTX_PUSH;
           extract_bin (NULL, (gdb_byte *) &(event.cases.contextPush.dev),
                                     sizeof (event.cases.contextPush.dev));
           extract_bin (NULL, (gdb_byte *) &(event.cases.contextPush.context),
                                     sizeof (event.cases.contextPush.context));
           extract_bin (NULL, (gdb_byte *) &(event.cases.contextPush.tid),
                                     sizeof (event.cases.contextPush.tid));
         }
       else if (strcmp ("CTX_POP", p) == 0)
         {
           event.kind = CUDBG_EVENT_CTX_POP;
           extract_bin (NULL, (gdb_byte *) &(event.cases.contextPop.dev),
                                     sizeof (event.cases.contextPop.dev));
           extract_bin (NULL, (gdb_byte *) &(event.cases.contextPop.context),
                                     sizeof (event.cases.contextPop.context));
           extract_bin (NULL, (gdb_byte *) &(event.cases.contextPop.tid),
                                     sizeof (event.cases.contextPop.tid));
         }
       else if (strcmp ("CTX_CREATE", p) == 0)
         {
           event.kind = CUDBG_EVENT_CTX_CREATE;
           extract_bin (NULL, (gdb_byte *) &(event.cases.contextCreate.dev),
                                     sizeof (event.cases.contextCreate.dev));
           extract_bin (NULL, (gdb_byte *) &(event.cases.contextCreate.context),
                                     sizeof (event.cases.contextCreate.context));
           extract_bin (NULL, (gdb_byte *) &(event.cases.contextCreate.tid),
                                     sizeof (event.cases.contextCreate.tid));
         }
       else if (strcmp ("CTX_DESTROY", p) == 0)
         {
           event.kind = CUDBG_EVENT_CTX_DESTROY;
           extract_bin (NULL, (gdb_byte *) &(event.cases.contextDestroy.dev),
                                     sizeof (event.cases.contextDestroy.dev));
           extract_bin (NULL, (gdb_byte *) &(event.cases.contextDestroy.context),
                                     sizeof (event.cases.contextDestroy.context));
           extract_bin (NULL, (gdb_byte *) &(event.cases.contextDestroy.tid),
                                     sizeof (event.cases.contextDestroy.tid));
         }
       else if (strcmp ("INTERNAL_ERROR", p) == 0)
         {
           event.kind = CUDBG_EVENT_INTERNAL_ERROR;
           extract_bin (NULL, (gdb_byte *) &(event.cases.internalError.errorType),
                                     sizeof (event.cases.internalError.errorType));
         }
       else if (strcmp ("TIMEOUT", p) == 0)
         {
           event.kind = CUDBG_EVENT_TIMEOUT;
         }
       else if (strcmp ("ATTACH_COMPLETE", p) == 0)
         {
           event.kind = CUDBG_EVENT_ATTACH_COMPLETE;
         }
       else if (strcmp ("DETACH_COMPLETE", p) == 0)
         {
           event.kind = CUDBG_EVENT_DETACH_COMPLETE;
         }

       cuda_process_event (&event);
       p = append_string ("qnv.", pktbuf.buf, false);
       p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), false);     
       putpkt (pktbuf.buf);
       getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);

       p = extract_bin (pktbuf.buf, (gdb_byte *) &res, sizeof (res));
       if (res != CUDBG_SUCCESS && res != CUDBG_ERROR_NO_EVENT_AVAILABLE)
         error (_("Error: Failed to get the next %s CUDA event (error=%u)."), 
                cuda_event_kind == CUDA_EVENT_SYNC ? "sync" : "async", res);
       p = extract_string (NULL);
    }

  if (ret)
    cuda_event_post_process ();

  return ret;
}

bool
cuda_remote_query_sync_events (void)
{
  return cuda_remote_query_events (CUDA_EVENT_SYNC);
}

bool
cuda_remote_query_async_events (void)
{
  return cuda_remote_query_events (CUDA_EVENT_ASYNC);
}

CUDBGResult
cuda_remote_api_acknowledge_sync_events (void)
{
  char *p;
  CUDBGResult res;
  cuda_packet_type_t packet_type = ACK_SYNC_EVENTS;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), false);
  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);
  extract_bin (pktbuf.buf, (gdb_byte *) &res, sizeof (res)); 
  return res;
}

void
cuda_remote_update_grid_id_in_sm (uint32_t dev, uint32_t sm)
{
  CUDBGResult res;
  char *p;
  uint32_t wp;
  uint64_t valid_warps_mask_c;
  uint64_t valid_warps_mask_s;
  uint32_t num_warps;
  uint64_t grid_id;
  cuda_packet_type_t packet_type = UPDATE_GRID_ID_IN_SM;

  valid_warps_mask_c = sm_get_valid_warps_mask (dev, sm);
  num_warps = device_get_num_warps (dev);
  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), true);
  p = append_bin ((gdb_byte *) &dev, p, sizeof (dev), true);
  p = append_bin ((gdb_byte *) &sm,  p, sizeof (sm), true);
  p = append_bin ((gdb_byte *) &num_warps, p, sizeof (num_warps), false);

  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);

  extract_bin (pktbuf.buf, (gdb_byte *) &valid_warps_mask_s, sizeof (valid_warps_mask_s));
  gdb_assert (valid_warps_mask_s == valid_warps_mask_c);
  for (wp = 0; wp < num_warps; wp++)
    {
      if (warp_is_valid (dev, sm, wp))
        {
          extract_bin (NULL, (gdb_byte *) &grid_id, sizeof (grid_id));
          warp_set_grid_id (dev, sm, wp, grid_id);
        }
    }
  extract_bin (NULL, (gdb_byte *) &res, sizeof (res));
  if (res != CUDBG_SUCCESS)
    error (_("Error: Failed to read the grid index (error=%u).\n"), res);
}

void
cuda_remote_update_block_idx_in_sm (uint32_t dev, uint32_t sm)
{
  CUDBGResult res;
  char *p;
  uint32_t wp;
  uint64_t valid_warps_mask_c;
  uint64_t valid_warps_mask_s;
  uint32_t num_warps;
  CuDim3 block_idx;
  cuda_packet_type_t packet_type = UPDATE_BLOCK_IDX_IN_SM;

  valid_warps_mask_c = sm_get_valid_warps_mask (dev, sm);
  num_warps = device_get_num_warps (dev);
  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), true);
  p = append_bin ((gdb_byte *) &dev, p, sizeof (dev), true);
  p = append_bin ((gdb_byte *) &sm,  p, sizeof (sm), true);
  p = append_bin ((gdb_byte *) &num_warps, p, sizeof (num_warps), false);

  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);

  extract_bin (pktbuf.buf, (gdb_byte *) &valid_warps_mask_s, sizeof (valid_warps_mask_s));
  gdb_assert (valid_warps_mask_s == valid_warps_mask_c);
  for (wp = 0; wp < num_warps; wp++)
    {
       if (warp_is_valid (dev, sm, wp))
         {
           extract_bin (NULL, (gdb_byte *) &block_idx, sizeof (block_idx));
           warp_set_block_idx (dev, sm, wp, &block_idx);
         }
    }
  extract_bin (NULL, (gdb_byte *) &res, sizeof (res));
  if (res != CUDBG_SUCCESS)
    error (_("Error: Failed to read the block index (error=%u).\n"), res);
}

void
cuda_remote_update_thread_idx_in_warp (uint32_t dev, uint32_t sm, uint32_t wp)
{
  CUDBGResult res;
  char *p;
  uint32_t ln;
  uint32_t valid_lanes_mask_c;
  uint32_t valid_lanes_mask_s;
  uint32_t num_lanes;
  CuDim3 thread_idx;
  cuda_packet_type_t packet_type = UPDATE_THREAD_IDX_IN_WARP;

  valid_lanes_mask_c = warp_get_valid_lanes_mask (dev, sm, wp);
  num_lanes = device_get_num_lanes (dev);
  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), true);
  p = append_bin ((gdb_byte *) &dev, p, sizeof (dev), true);
  p = append_bin ((gdb_byte *) &sm,  p, sizeof (sm), true);
  p = append_bin ((gdb_byte *) &wp,  p, sizeof (wp), true);
  p = append_bin ((gdb_byte *) &num_lanes, p, sizeof (num_lanes), false);

  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);

  extract_bin (pktbuf.buf, (gdb_byte *) &valid_lanes_mask_s, sizeof (valid_lanes_mask_s));
  gdb_assert (valid_lanes_mask_s == valid_lanes_mask_c);
  for (ln = 0; ln < num_lanes; ln++)
    {
       if (lane_is_valid (dev, sm, wp, ln))
         {
           extract_bin (NULL, (gdb_byte *) &thread_idx, sizeof (thread_idx));
           lane_set_thread_idx (dev, sm, wp, ln, &thread_idx);
         }
    }
  extract_bin (NULL, (gdb_byte *) &res, sizeof (res));
  if (res != CUDBG_SUCCESS)
    error (_("Error: Failed to read the thread index (error=%u).\n"), res);
}

void
cuda_remote_initialize (CUDBGResult *get_debugger_api_res, CUDBGResult *set_callback_api_res,
                        CUDBGResult *initialize_api_res, bool *cuda_initialized,
                        bool *cuda_debugging_enabled, bool *driver_is_compatible)
{
  char *p;
  cuda_packet_type_t packet_type = INITIALIZE_TARGET;
  bool preemption          = cuda_options_software_preemption ();
  bool memcheck            = cuda_options_memcheck ();
  bool launch_blocking     = cuda_options_launch_blocking ();

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type,     p, sizeof (packet_type), true);
  p = append_bin ((gdb_byte *) &preemption,      p, sizeof (preemption), true);
  p = append_bin ((gdb_byte *) &memcheck,        p, sizeof (memcheck), true);
  p = append_bin ((gdb_byte *) &launch_blocking, p, sizeof (launch_blocking), false);

  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);

  extract_bin (pktbuf.buf, (gdb_byte *) get_debugger_api_res, sizeof (*get_debugger_api_res));
  extract_bin (NULL, (gdb_byte *) set_callback_api_res, sizeof (*set_callback_api_res));
  extract_bin (NULL, (gdb_byte *) initialize_api_res, sizeof (*initialize_api_res));
  extract_bin (NULL, (gdb_byte *) cuda_initialized, sizeof (*cuda_initialized));
  extract_bin (NULL, (gdb_byte *) cuda_debugging_enabled, sizeof (*cuda_debugging_enabled));
  extract_bin (NULL, (gdb_byte *) driver_is_compatible, sizeof (*driver_is_compatible));
}

CUDBGResult
cuda_remote_api_get_num_devices (uint32_t *dev)
{
  char *p;
  CUDBGResult res;
  cuda_packet_type_t packet_type = GET_NUM_DEVICES;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), false);
  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);

  extract_bin (pktbuf.buf, (gdb_byte *) &res, sizeof (res));
  extract_bin (NULL, (gdb_byte *) dev, sizeof (*dev));
  return res;
}

void
cuda_remote_query_device_spec (uint32_t dev_id, uint32_t *num_sms, uint32_t *num_warps,
                               uint32_t *num_lanes, uint32_t *num_registers,
                               char **dev_type, char **sm_type)
{
  char *p;
  CUDBGResult res;
  cuda_packet_type_t packet_type = QUERY_DEVICE_SPEC;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), true);
  p = append_bin ((gdb_byte *) &dev_id, p, sizeof (uint32_t), false);

  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);

  extract_bin (pktbuf.buf, (gdb_byte *) &res, sizeof (res));
  if (res != CUDBG_SUCCESS)
    error (_("Error: Failed to read device specification (error=%u).\n"), res);  
  extract_bin (NULL, (gdb_byte *) num_sms, sizeof (num_sms));
  extract_bin (NULL, (gdb_byte *) num_warps, sizeof (num_warps));
  extract_bin (NULL, (gdb_byte *) num_lanes, sizeof (num_lanes));
  extract_bin (NULL, (gdb_byte *) num_registers, sizeof (num_registers));
  *dev_type = extract_string (NULL);
  *sm_type  = extract_string (NULL);
}

bool
cuda_remote_check_pending_sigint (void)
{
  char *p;
  bool ret_val;
  cuda_packet_type_t packet_type = CHECK_PENDING_SIGINT;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), false);

  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);

  extract_bin (pktbuf.buf, (gdb_byte *) &ret_val, sizeof (ret_val));
  return ret_val;
}

CUDBGResult
cuda_remote_api_initialize (void)
{
  char *p;
  CUDBGResult res;
  cuda_packet_type_t packet_type = API_INITIALIZE;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), true);

  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);

  extract_bin (pktbuf.buf, (gdb_byte *) &res, sizeof (res));
  return res;
}

CUDBGResult
cuda_remote_api_finalize (void)
{
  char *p;
  CUDBGResult res;
  cuda_packet_type_t packet_type = API_FINALIZE;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), true);

  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);

  extract_bin (pktbuf.buf, (gdb_byte *) &res, sizeof (res));
  return res;
}

CUDBGResult
cuda_remote_api_clear_attach_state (void)
{
  char *p;
  CUDBGResult res;
  cuda_packet_type_t packet_type = CLEAR_ATTACH_STATE;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), false);

  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);
  extract_bin (pktbuf.buf, (gdb_byte *) &res, sizeof (res));
  return res;
}

CUDBGResult
cuda_remote_api_request_cleanup_on_detach (void)
{
  char *p;
  CUDBGResult res;
  cuda_packet_type_t packet_type = REQUEST_CLEANUP_ON_DETACH;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), false);

  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);
  extract_bin (pktbuf.buf, (gdb_byte *) &res, sizeof (res));
  return res;
}

CUDBGResult
cuda_remote_api_set_kernel_launch_notification_mode (CUDBGKernelLaunchNotifyMode mode)
{
  char *p;
  CUDBGResult res;
  cuda_packet_type_t packet_type = SET_ASYNC_LAUNCH_NOTIFICATIONS;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), true);
  p = append_bin ((gdb_byte *) &mode, p, sizeof (mode), false);

  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);
  extract_bin (pktbuf.buf, (gdb_byte *) &res, sizeof (res));

  return res;
}

void
cuda_remote_set_option ()
{
  char *p;
  bool general_trace       = cuda_options_debug_general ();
  bool libcudbg_trace      = cuda_options_debug_libcudbg ();
  bool notifications_trace = cuda_options_debug_notifications ();
  bool notify_youngest     = cuda_options_notify_youngest ();

  cuda_packet_type_t packet_type = SET_OPTION;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type,         p, sizeof (cuda_packet_type_t), true);
  p = append_bin ((gdb_byte *) &general_trace,       p, sizeof (general_trace), true);
  p = append_bin ((gdb_byte *) &libcudbg_trace,      p, sizeof (libcudbg_trace), true);
  p = append_bin ((gdb_byte *) &notifications_trace, p, sizeof (notifications_trace), true);
  p = append_bin ((gdb_byte *) &notify_youngest,     p, sizeof (notify_youngest), false);

  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);
}

void
cuda_remote_query_trace_message ()
{
  char *p;
  cuda_packet_type_t packet_type = QUERY_TRACE_MESSAGE;

  if (!cuda_options_debug_general () &&
      !cuda_options_debug_libcudbg () &&
      !cuda_options_debug_notifications ())
    return;

  p = append_string ("qnv.", pktbuf.buf, false);
  p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), false);

  putpkt (pktbuf.buf);
  getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);
  p = extract_string (pktbuf.buf);
  while (strcmp ("NO_TRACE_MESSAGE", p) != 0)
    {
      fprintf (stderr, "%s\n", p);

      p = append_string ("qnv.", pktbuf.buf, false);
      p = append_bin ((gdb_byte *) &packet_type, p, sizeof (packet_type), false);
      putpkt (pktbuf.buf);
      getpkt (&pktbuf.buf, &pktbuf.buf_size, 1);
      p = extract_string (pktbuf.buf);
    }
  fflush (stderr);
}
