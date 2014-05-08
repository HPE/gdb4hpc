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

#include "defs.h"
#include "breakpoint.h"
#include "gdb_assert.h"
#include "objfiles.h"
#include "source.h"

#include "cuda-defs.h"
#include "cuda-elf-image.h"
#include "cuda-options.h"
#include "cuda-tdep.h"


/******************************************************************************
 *
 *                                   Module
 *
 *****************************************************************************/

struct module_st {
  uint64_t    module_id;            /* the CUmodule handle */
  context_t   context;              /* the parent context state */
  elf_image_t elf_image;            /* the ELF image object for the module */
  module_t    next;                 /* next module in the list */
};

module_t
module_new (context_t context, uint64_t module_id, void *elf_image, uint64_t elf_image_size)
{
  module_t module;

  module = xmalloc (sizeof *module);
  module->context    = context;
  module->module_id  = module_id;
  module->elf_image  = cuda_elf_image_new (elf_image, elf_image_size, module);
  module->next       = NULL;

  return module;
}

void
module_delete (module_t module)
{
  gdb_assert (module);

  cuda_elf_image_delete (module->elf_image);

  xfree (module);
}

void
module_print (module_t module)
{
  gdb_assert (module);

  cuda_trace ("      module_id 0x%"PRIx64, module->module_id);
}

context_t
module_get_context (module_t module)
{
  gdb_assert (module);

  return module->context;
}

uint64_t
module_get_id (module_t module)
{
  gdb_assert (module);

  return module->module_id;
}

elf_image_t
module_get_elf_image (module_t module)
{
  gdb_assert (module);

  return module->elf_image;
}


/******************************************************************************
 *
 *                                   Modules
 *
 *****************************************************************************/

struct modules_st {
  module_t    head;                 /* single-linked list of modules */
};

modules_t
modules_new (void)
{
  modules_t modules;

  modules = xmalloc (sizeof *modules);
  modules->head = NULL;

  return modules;
}

void
modules_delete (modules_t modules, kernels_t kernels)
{
  module_t module;
  module_t next_module;

  gdb_assert (modules);

  module = modules->head;
  while (module)
    {
      next_module = module->next;

      kernels_remove_kernels_for_module (kernels, module);

      module_delete (module);

      module = next_module;
    }
}

void
modules_add (modules_t modules, module_t module)
{
  gdb_assert (modules);
  gdb_assert (module);

  module->next  = modules->head;
  modules->head = module;
}

void
modules_print (modules_t modules)
{
  module_t module;

  gdb_assert (modules);

  for (module = modules->head; module; module = module->next)
    module_print (module);
}

void
modules_load_elf_images (modules_t modules)
{
  module_t    module;
  elf_image_t elf_image;

  for (module = modules->head; module; module = module->next)
    {
      elf_image = module_get_elf_image (module);
      if (!cuda_elf_image_is_loaded (elf_image))
        cuda_elf_image_load (elf_image);
    }
}

void
modules_unload_elf_images (modules_t modules)
{
  module_t    module;
  elf_image_t elf_image;

  for (module = modules->head; module; module = module->next)
    {
      elf_image = module_get_elf_image (module);
      if (cuda_elf_image_is_loaded (elf_image))
        cuda_elf_image_unload (elf_image);
    }
}

void
modules_resolve_breakpoints (modules_t modules, uint64_t context_id)
{
  module_t    module;
  elf_image_t elf_image;

  for (module = modules->head; module; module = module->next)
    {
      elf_image = module_get_elf_image (module);
      cuda_elf_image_resolve_breakpoints (elf_image);
    }
}

module_t
modules_find_module_by_id (modules_t modules, uint64_t module_id)
{
  module_t module;

  for (module = modules->head; module; module = module->next)
    if (module->module_id == module_id)
      return module;

  return NULL;
}

module_t
modules_find_module_by_address (modules_t modules, CORE_ADDR addr)
{
  module_t    module = NULL;
  elf_image_t elf_image;

  gdb_assert (modules);

  for (module = modules->head; module; module = module->next)
  {
    elf_image = module_get_elf_image (module);
    if (cuda_elf_image_contains_address (elf_image, addr))
      return module;
  }

  return NULL;
}
