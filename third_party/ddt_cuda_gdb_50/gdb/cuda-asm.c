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
#include "cuda-elf-image.h"
#include "cuda-modules.h"
#include "cuda-state.h"
#include "cuda-tdep.h"
#include "cuda-options.h"

/******************************************************************************
 *
 *                        One PC-Instruction Mapping
 *
 *****************************************************************************/

typedef struct inst_st      *inst_t;

struct inst_st {
  uint64_t      pc;      /* the PC of the disassembled instruction */
  char         *text;    /* the dissassembled instruction */
  uint32_t      size;    /* size of the instruction in bytes */
  inst_t next;           /* the next element in the list */
};

static inst_t
inst_create (uint64_t pc, const char *text, inst_t next)
{
  inst_t inst;
  int len;

  gdb_assert (text);

  len = strlen (text);
  inst = xmalloc (sizeof *inst);
  inst->text= xmalloc (len + 1);

  inst->pc   = pc;
  inst->text= strncpy (inst->text, text, len + 1);
  inst->next = next;
  inst->size = 0;

  return inst;
}

static void
inst_destroy (inst_t inst)
{
  xfree (inst->text);
  xfree (inst);
}


/******************************************************************************
 *
 *                             Disassembly Cache
 *
 *****************************************************************************/

struct disasm_cache_st {
  uint64_t   start_addr;        /* address of the first cached instruction */
  uint64_t   end_addr;          /* address of the last cached instruction */
  inst_t     head;              /* head of the list of instructions */
};

disasm_cache_t
disasm_cache_create (void)
{
  disasm_cache_t disasm_cache;

  disasm_cache = xmalloc (sizeof *disasm_cache);

  disasm_cache->start_addr = 0;
  disasm_cache->end_addr   = 0;
  disasm_cache->head       = NULL;

  return disasm_cache;
}

void
disasm_cache_flush (disasm_cache_t disasm_cache)
{
  inst_t inst = disasm_cache->head;
  inst_t next_inst;

  while (inst)
    {
      next_inst = inst->next;
      inst_destroy (inst);
      inst = next_inst;
    }

  disasm_cache->start_addr = 0;
  disasm_cache->end_addr   = 0;
  disasm_cache->head       = NULL;
}

void
disasm_cache_destroy (disasm_cache_t disasm_cache)
{
  disasm_cache_flush (disasm_cache);

  xfree (disasm_cache);
}

const char *
disasm_cache_find_instruction (disasm_cache_t disasm_cache,
                               uint64_t pc, uint32_t *inst_size)
{
  inst_t inst = NULL, prev_inst = NULL;
  kernel_t kernel;
  module_t module;
  elf_image_t elf_img;
  struct objfile *objfile;
  uint32_t devId;
  uint64_t ofst = 0, entry_pc = 0;
  FILE *sass;
  char command[1024], line[1024], header[1024];
  char *filename;
  const char *kernel_name;
  char *kernel_base_name;
  char *text;
  CORE_ADDR pc1, pc2;
  char buf[512];
  uint32_t dev_id, sm_id, wp_id;
  int len = 0;

  if (!cuda_focus_is_device ())
    return NULL;

  /* update the cache using the ELF image if needed */
  if (cuda_options_disassemble_from_elf_image () &&
      (pc < disasm_cache->start_addr ||
       pc > disasm_cache->end_addr ||
       !disasm_cache->head))
  {
    /* flush the cache and free the associated memory */
    disasm_cache_flush (disasm_cache);

    /* collect all the necessary data */
    cuda_coords_get_current_physical (&dev_id, &sm_id, &wp_id, NULL);
    kernel      = warp_get_kernel (dev_id, sm_id, wp_id);
    module      = kernel_get_module (kernel);
    elf_img     = module_get_elf_image (module);
    objfile     = cuda_elf_image_get_objfile (elf_img);
    filename    = objfile->name;
    entry_pc    = kernel_get_virt_code_base (kernel);
    kernel_name = cuda_find_kernel_name_from_pc (entry_pc, false);

    /* generate the dissassembled code using cuobjdump if available */
    snprintf (command, sizeof (command), "cuobjdump --dump-sass %s", filename);
    sass = popen (command, "r");

    if (!sass)
      return NULL;

    /* discard the kernel arguments if specified */
    kernel_base_name = strdup (kernel_name);
    kernel_base_name = strtok (kernel_base_name, "(");

    /* find the wanted function in the cuobjdump output */
    snprintf (header, sizeof (header), "\t\tFunction : %s\n", kernel_base_name);
    while (fgets (line, sizeof (line), sass) != NULL)
      if (strncmp (line, header, strlen (header)) == 0)
        break;
    xfree (kernel_base_name);

    /* parse the sass output and insert each instruction individually */
    while (fgets (line, sizeof (line), sass) != NULL)
      {
        /* remember the previous instruction for convenience */
        prev_inst = disasm_cache->head;

        /* read the offset and stop reading if cannot find it */
        if (sscanf (line, "\t/*%"PRIx64"*/", &ofst) == 0)
          break;

        /* find the location of the instruction (after the second \t) */
        for (text = line + 1; *text!= '\t'; ++text);
        ++text;

        /* discard the "\n" and possible ";" at the end of the line */
        for (len = strlen (text); len && strchr("\n ;", text[len - 1]); --len);
        text[len] = 0;

        /* add the instruction to the cache at the found offset */
        disasm_cache->head = inst_create (entry_pc + ofst, text, disasm_cache->head);

        /* update the size of the previous instruction */
        if (prev_inst)
          prev_inst->size = entry_pc + ofst - prev_inst->pc;

        /* update the start and end address of the cached instructions */
        if (entry_pc + ofst < disasm_cache->start_addr)
          disasm_cache->start_addr = entry_pc + ofst;
        if (entry_pc + ofst > disasm_cache->end_addr)
          disasm_cache->end_addr = entry_pc + ofst;
      }

    /* update the instruction size of the last instruction */
    if (disasm_cache->head)
      {
        pc1 = get_pc_function_start (pc);
        pc2 = get_pc_function_start (pc+4);
        disasm_cache->head->size = (pc1 != 0 && pc2 != 0 && pc1 == pc2) ? 8 : 4;
      }

    /* close the sass file */
    pclose (sass);
  }

  /* find the cached disassembled instruction. Force re-reading if
     disassembling from the device memory.  */
  if (cuda_options_disassemble_from_elf_image ())
    for (inst = disasm_cache->head; inst; inst = inst->next)
      if (inst->pc == pc)
        break;

  /* Query the API if the disassembly failed */
  if (!inst && cuda_initialized)
    {
      buf[0] = 0;
      devId = cuda_current_device ();
      cuda_api_disassemble (devId, pc, inst_size, buf, sizeof (buf));
      if (buf[0] != 0)
        {
          /* Store the result in the cache */
          disasm_cache->head = inst_create (pc, buf, disasm_cache->head);
          disasm_cache->head->size = *inst_size;
          return disasm_cache->head->text;
        }
    }

  /* return the instruction or NULL if not found */
  if (inst)
    {
      *inst_size = inst->size;
      return inst->text;
    }
  else
    {
      *inst_size = 4;
      return NULL;
    }
}

