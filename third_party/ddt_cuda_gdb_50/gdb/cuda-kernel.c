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

#include <string.h>

#include "defs.h"
#include "frame.h"
#include "gdb_assert.h"

#include "cuda-api.h"
#include "cuda-asm.h"
#include "cuda-context.h"
#include "cuda-iterator.h"
#include "cuda-modules.h"
#include "cuda-options.h"
#include "cuda-state.h"
#include "cuda-tdep.h"

/* counter for the CUDA kernel ids */
static uint64_t next_kernel_id = 0;

uint64_t
cuda_latest_launched_kernel_id (void)
{
  return next_kernel_id - 1;
}

/******************************************************************************
 *
 *                                   Kernel
 *
 *****************************************************************************/

struct kernel_st {
  uint64_t        id;              /* unique kernel id per GDB session */
  uint32_t        dev_id;          /* device where the kernel was launched */
  uint32_t        grid_id;         /* unique kernel id per device */
  char           *name;            /* name of the kernel if available */
  uint64_t        virt_code_base;  /* virtual address of the kernel entry point */
  module_t        module;          /* CUmodule handle of the kernel */
  bool            launched;        /* Has the kernel been seen on the hw? */
  bool            present;         /* Is kernel currently on the hw? */
  CuDim3          grid_dim;        /* The grid dimensions of the kernel. */
  CuDim3          block_dim;       /* The block dimensions of the kernel. */
  char            dimensions[128]; /* A string repr. of the kernel dimensions. */
  CUDBGKernelType type;            /* The kernel type: system or application. */
  disasm_cache_t  disasm_cache;    /* the cached disassembled instructions */
  kernel_t        next;            /* next kernel on the same device */
};

static kernel_t
kernel_new (uint32_t dev_id, uint64_t grid_id, uint64_t virt_code_base,
            char *name, module_t module, CuDim3 grid_dim, CuDim3 block_dim,
            CUDBGKernelType type)
{
  kernel_t        kernel;
  uint32_t        name_len;
  char           *name_copy;

  kernel = xmalloc (sizeof *kernel);

  name_len  = strlen (name);
  name_copy = xmalloc (name_len + 1);
  memcpy (name_copy, name, name_len + 1);

  kernel->id             = next_kernel_id++;
  kernel->dev_id         = dev_id;
  kernel->grid_id        = grid_id;
  kernel->virt_code_base = virt_code_base;
  kernel->name           = name_copy;
  kernel->module         = module;
  kernel->grid_dim       = grid_dim;
  kernel->block_dim      = block_dim;
  kernel->type           = type;
  kernel->disasm_cache   = disasm_cache_create ();
  kernel->next           = NULL;

  snprintf (kernel->dimensions, sizeof (kernel->dimensions), "<<<(%d,%d,%d),(%d,%d,%d)>>>",
            grid_dim.x, grid_dim.y, grid_dim.z, block_dim.x, block_dim.y, block_dim.z);

  kernel->launched = false;

  if (cuda_options_show_kernel_events ())
    printf_unfiltered (_("[Launch of CUDA Kernel %"PRIu64" (%s%s) on Device %u]\n"),
                       kernel->id, kernel->name, kernel->dimensions, dev_id);

  return kernel;
}

static void
kernel_delete (kernel_t kernel)
{
  gdb_assert (kernel);

  if (cuda_options_show_kernel_events ())
    printf_unfiltered (_("[Termination of CUDA Kernel %"PRIu64" (%s%s) on Device %u]\n"),
                      kernel->id, kernel->name, kernel->dimensions, kernel->dev_id);

  disasm_cache_destroy (kernel->disasm_cache);
  xfree (kernel->name);
  xfree (kernel);
}

uint64_t
kernel_get_id (kernel_t kernel)
{
  gdb_assert (kernel);
  return kernel->id;
}

const char *
kernel_get_name (kernel_t kernel)
{
  gdb_assert (kernel);
  return kernel->name;
}

uint32_t
kernel_get_grid_id (kernel_t kernel)
{
  gdb_assert (kernel);
  return kernel->grid_id;
}

uint64_t
kernel_get_virt_code_base (kernel_t kernel)
{
  gdb_assert (kernel);
  return kernel->virt_code_base;
}

context_t
kernel_get_context (kernel_t kernel)
{
  gdb_assert (kernel);
  return module_get_context (kernel->module);
}

module_t
kernel_get_module (kernel_t kernel)
{
  gdb_assert (kernel);
  return kernel->module;
}

uint32_t
kernel_get_dev_id (kernel_t kernel)
{
  gdb_assert (kernel);
  return kernel->dev_id;
}

CuDim3
kernel_get_grid_dim (kernel_t kernel)
{
  gdb_assert (kernel);
  return kernel->grid_dim;
}

CuDim3
kernel_get_block_dim (kernel_t kernel)
{
  gdb_assert (kernel);
  return kernel->block_dim;
}

const char*
kernel_get_dimensions (kernel_t kernel)
{
  gdb_assert (kernel);
  return kernel->dimensions;
}

CUDBGKernelType
kernel_get_type (kernel_t kernel)
{
  gdb_assert (kernel);
  return kernel->type;
}

bool
kernel_has_launched (kernel_t kernel)
{
  gdb_assert (kernel);
  return kernel->launched;
}

bool
kernel_is_present (kernel_t kernel)
{
  gdb_assert (kernel);
  return kernel->present;
}

uint32_t
kernel_compute_sms_mask (kernel_t kernel)
{
  cuda_coords_t current;
  cuda_coords_t filter;
  cuda_iterator itr;
  uint32_t      sms_mask;

  gdb_assert (kernel);

  filter        = CUDA_WILDCARD_COORDS;
  filter.dev    = kernel->dev_id;
  filter.gridId = kernel->grid_id;
  itr = cuda_iterator_create (CUDA_ITERATOR_TYPE_WARPS, &filter, CUDA_SELECT_VALID);

  sms_mask = 0U;
  for (cuda_iterator_start (itr); !cuda_iterator_end (itr); cuda_iterator_next (itr))
    {
      current = cuda_iterator_get_current (itr);
      sms_mask |= 1U << current.sm;
    }

  return sms_mask;
}

const char*
kernel_disassemble (kernel_t kernel, uint64_t pc, uint32_t *inst_size)
{
  gdb_assert (kernel);
  gdb_assert (inst_size);

  return disasm_cache_find_instruction (kernel->disasm_cache, pc, inst_size);
}

void
kernel_load_elf_images (kernel_t kernel)
{
  context_t context;
  module_t  module;

  gdb_assert (kernel);

  module    = kernel_get_module (kernel);
  context   = module_get_context (module);
  set_current_context (context);
}

void
kernel_print (kernel_t kernel)
{
  gdb_assert (kernel);

  fprintf (stderr, "    Kernel %"PRIu64":\n", kernel->id);
  fprintf (stderr, "        name        : %s\n", kernel->name);
  fprintf (stderr, "        device id   : %u\n", kernel->dev_id);
  fprintf (stderr, "        grid id     : %d\n", kernel->grid_id);
  fprintf (stderr, "        module id   : 0x%"PRIx64"\n", module_get_id (kernel->module));
  fprintf (stderr, "        entry point : 0x%"PRIx64"\n", kernel->virt_code_base);
  fprintf (stderr, "        dimensions  : %s\n", kernel->dimensions);
  fprintf (stderr, "        launched    : %s\n", kernel->launched ? "yes" : "no");
  fprintf (stderr, "        present     : %s\n", kernel->present ? "yes" : "no");
  fprintf (stderr, "        next        : 0x%"PRIx64"\n", (uint64_t)(uintptr_t)kernel->next);
  fflush (stderr);
}


/******************************************************************************
 *
 *                                   Kernels
 *
 *****************************************************************************/

struct kernels_st {
  uint32_t dev_id;               /* the parent device */
  uint32_t num_present_kernels;  /* number of kernels currently on the device */
  kernel_t head;                 /* the head of the list of kernels */
};

kernels_t
kernels_new (uint32_t dev_id)
{
  kernels_t kernels;

  kernels = xmalloc (sizeof *kernels);
  kernels->dev_id              = dev_id;
  kernels->num_present_kernels = 0;
  kernels->head                = NULL;

  return kernels;
}

void
kernels_delete (kernels_t kernels)
{
  kernel_t kernel, next_kernel;

  gdb_assert (kernels);

  kernel = kernels->head;
  while (kernel)
    {
      next_kernel = kernel->next;
      kernel_delete (kernel);
      kernel = next_kernel;
    }

  xfree (kernels);
}

uint32_t
kernels_get_dev_id (kernels_t kernels)
{
  gdb_assert (kernels);
  return kernels->dev_id;
}

uint32_t
kernels_get_num_kernels (kernels_t kernels)
{
  kernel_t kernel;
  uint32_t num_kernels;

  gdb_assert (kernels);

  num_kernels = 0;
  for (kernel = kernels->head; kernel; kernel = kernel->next)
    ++num_kernels;

  return num_kernels;
}


uint32_t
kernels_get_num_present_kernels (kernels_t kernels)
{
  gdb_assert (kernels);
  return kernels->num_present_kernels;
}

void
kernels_print (kernels_t kernels)
{
  kernel_t kernel;

  gdb_assert (kernels);

  for (kernel = kernels->head; kernel; kernel = kernel->next)
    kernel_print (kernel);
}

void
kernels_start_kernel (kernels_t kernels, uint64_t grid_id,
                      uint64_t virt_code_base, uint64_t context_id,
                      uint64_t module_id, CuDim3 grid_dim,
                      CuDim3 block_dim, CUDBGKernelType type)
{
  uint32_t  dev_id;
  context_t context;
  modules_t modules;
  module_t  module;
  kernel_t  kernel;
  char     *kernel_name = NULL;

  gdb_assert (kernels);

  dev_id  = kernels->dev_id;
  context = device_find_context_by_id (dev_id, context_id);
  modules = context_get_modules (context);
  module  = modules_find_module_by_id (modules, module_id);

  set_current_context (context);
  kernel_name = cuda_find_kernel_name_from_pc (virt_code_base, true);

  kernel = kernel_new (dev_id, grid_id, virt_code_base, kernel_name, module,
                       grid_dim, block_dim, type);

  kernel->next  = kernels->head;
  kernels->head = kernel;
}

void
kernels_terminate_kernel (kernels_t kernels, kernel_t kernel)
{
  kernel_t  prev;
  kernel_t  ker;

  gdb_assert (kernels);
  gdb_assert (kernel);

  for (ker = kernels->head, prev = NULL;
       ker && ker != kernel;
       prev = ker, ker = ker->next)
    ;
  gdb_assert (ker);

  if (prev)
    prev->next = kernel->next;
  else
    kernels->head = kernel->next;

  kernel_delete (kernel);
}

kernel_t
kernels_find_kernel_by_grid_id (kernels_t kernels, uint64_t grid_id)
{
  kernel_t kernel;

  gdb_assert (kernels);

  for (kernel = kernels->head; kernel; kernel = kernel->next)
    if (kernel->grid_id == grid_id)
      return kernel;

  return NULL;
}

static kernel_t
kernels_find_kernel_by_module (kernels_t kernels, module_t module)
{
  kernel_t kernel;

  gdb_assert (kernels);

  for (kernel = kernels->head; kernel; kernel = kernel->next)
    if (kernel->module == module)
      return kernel;

  return NULL;
}

void
kernels_remove_kernels_for_module (kernels_t kernels, module_t module)
{
  kernel_t kernel;

  kernel = kernels_find_kernel_by_module (kernels, module);

  while (kernel != NULL)
    {
      kernels_terminate_kernel (kernels, kernel);

      kernel = kernels_find_kernel_by_module (kernels, module);
    }
}

void
kernels_update_kernels (kernels_t kernels)
{
  kernel_t      kernel;
  kernel_t      next_kernel;
  CUDBGGridStatus status;

  gdb_assert (kernels);

  kernels->num_present_kernels = 0;

  /* rediscover the kernels currently running on the hardware */
  kernel = kernels->head;
  while (kernel)
    {
      next_kernel = kernel->next;

      /* reset the kernel to not currently running */
      kernel->present = false;

      cuda_api_get_grid_status (kernel->dev_id, kernel->grid_id, &status);

      if (status == CUDBG_GRID_STATUS_ACTIVE || status == CUDBG_GRID_STATUS_SLEEPING)
        {
          kernel->launched = true;
          kernel->present = true;
          ++kernels->num_present_kernels;
        }
      else if (status == CUDBG_GRID_STATUS_TERMINATED ||
               status == CUDBG_GRID_STATUS_UNDETERMINED ||
               status == CUDBG_GRID_STATUS_INVALID)
        {
          kernel->present = false;
        }

      /* terminate the kernels that we had seen running at some point
         but are not here on the hardware anymore. */
      if (kernel->launched && !kernel->present)
          kernels_terminate_kernel (kernels, kernel);

      kernel = next_kernel;
    }
}
