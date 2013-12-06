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


#include "cuda-regmap.h"
#include "gdb_assert.h"
#include "obstack.h"
#include "cuda-coords.h"
#include "cuda-state.h"

/*
   The PTX to SASS register map table is made of a series of entries,
   one per function. Each function entry is made of a list of register
   mappings, from a PTX register to a SASS register. The table size is
   saved in the first 32 bits.

     | fct name | number of entries |
       | idx | ptx_reg | sass_reg | start | end |
       | idx | ptx_reg | sass_reg | start | end |
       ...
       | idx | ptx_reg | sass_reg | start | end |
     | fct name | number of entries |
       | idx | ptx_reg | sass_reg | start | end |
       ...
     ...

   A PTX reg is mapped to one more SASS registers. If a PTX register
   is mapped to more than one SASS register, multiple entries are
   required and the 'idx' field is incremented by 1 for each one of
   them. The 'start' and 'end' addresses indicate the physical address
   between which the mapping is valid.

   The 8 high bits of a sass_reg are the register class (see cudadebugger.h).
   The low 24 bits are either the register index, or the offset in local
   memory, or the stack pointer register index and the offset.
 */

/* Raw value decoding */
#define REGMAP_CLASS(x)   (x >> 24)
#define REGMAP_REG(x)     (x & 0xffffff)
#define REGMAP_OFST(x)    (x & 0xffffff)
#define REGMAP_SP_REG(x)  ((x >> 16) & 0xff)
#define REGMAP_SP_OFST(x) (x & 0xffff)

struct regmap_st {
  struct {
    const char   *func_name;   /* the kernel name */
    const char   *reg_name;    /* the PTX register name */
    uint64_t      addr;        /* the kernel-relative PC address */
  } input;
  struct {
    uint32_t      num_entries;                        /* # entries in the other fields */
    uint32_t      max_location_index;                 /* max loc index across all addrs */
    uint32_t      location_index[REGMAP_MAX_ENTRIES]; /* location index for raw value */
    uint32_t      raw_value[REGMAP_MAX_ENTRIES];      /* see REGMAP_* macros above */
  } output;
};

typedef union {
  char        *byte;
  uint32_t    *func_num_entries;
  uint32_t    *table_size;
  uint32_t    *idx;
  char        *ptx_reg;
  uint32_t    *sass_reg;
  uint32_t    *start;
  uint32_t    *end;
} regmap_ptr_t;

typedef struct {
  char        *start;
  uint32_t     size;
} regmap_table_t;

typedef struct {
  char        *name;
  uint32_t     num_entries;
  char        *start;
  uint32_t     size;
} regmap_func_t;

typedef struct {
  uint32_t     idx;
  char        *ptx_reg;
  uint32_t     value;
  uint32_t     start_addr;
  uint32_t     end_addr;
  char         *next;
} regmap_reg_t;


static struct regmap_st cuda_regmap_st;
regmap_t cuda_regmap = &cuda_regmap_st;

/****************************************************************************

                               ACCESSOR ROUTINES

 ****************************************************************************/

regmap_t
regmap_get_search_result (void)
{
  return cuda_regmap;
}

const char *
regmap_get_func_name (regmap_t regmap)
{
  gdb_assert (regmap);
  gdb_assert (regmap->input.func_name);

  return regmap->input.func_name;
}

const char *
regmap_get_reg_name (regmap_t regmap)
{
  gdb_assert (regmap);
  gdb_assert (regmap->input.reg_name);

  return regmap->input.reg_name;
}

uint64_t
regmap_get_addr (regmap_t regmap)
{
  gdb_assert (regmap);

  return regmap->input.addr;
}

uint32_t
regmap_get_num_entries (regmap_t regmap)
{
  gdb_assert (regmap);

  return regmap->output.num_entries;
}

uint32_t
regmap_get_location_index (regmap_t regmap, uint32_t idx)
{
  gdb_assert (regmap);
  gdb_assert (idx < REGMAP_MAX_ENTRIES);
  gdb_assert (idx < regmap->output.num_entries);

  return regmap->output.location_index[idx];
}

CUDBGRegClass
regmap_get_class (regmap_t regmap, uint32_t idx)
{
  gdb_assert (regmap);
  gdb_assert (idx < REGMAP_MAX_ENTRIES);
  gdb_assert (idx < regmap->output.num_entries);

  return REGMAP_CLASS (regmap->output.raw_value[idx]);
}

uint32_t
regmap_get_half_register (regmap_t regmap, uint32_t idx, bool *in_higher_16_bits)
{
  uint32_t raw_register = 0;

  gdb_assert (in_higher_16_bits);
  gdb_assert (regmap);
  gdb_assert (idx < REGMAP_MAX_ENTRIES);
  gdb_assert (idx < regmap->output.num_entries);
  gdb_assert (REGMAP_CLASS (regmap->output.raw_value[idx]) == REG_CLASS_REG_HALF);

  raw_register = REGMAP_REG (regmap->output.raw_value[idx]);
  *in_higher_16_bits = raw_register & 1;
  return raw_register > 1;
}

uint32_t
regmap_get_register (regmap_t regmap, uint32_t idx)
{
  gdb_assert (regmap);
  gdb_assert (idx < REGMAP_MAX_ENTRIES);
  gdb_assert (idx < regmap->output.num_entries);
  gdb_assert (REGMAP_CLASS (regmap->output.raw_value[idx]) == REG_CLASS_REG_FULL);

  return REGMAP_REG (regmap->output.raw_value[idx]);
}

uint32_t
regmap_get_sp_register (regmap_t regmap, uint32_t idx)
{
  gdb_assert (regmap);
  gdb_assert (idx < REGMAP_MAX_ENTRIES);
  gdb_assert (idx < regmap->output.num_entries);
  gdb_assert (REGMAP_CLASS (regmap->output.raw_value[idx]) == REG_CLASS_LMEM_REG_OFFSET);

  return REGMAP_SP_REG (regmap->output.raw_value[idx]);
}

uint32_t
regmap_get_sp_offset (regmap_t regmap, uint32_t idx)
{
  gdb_assert (regmap);
  gdb_assert (idx < REGMAP_MAX_ENTRIES);
  gdb_assert (idx < regmap->output.num_entries);
  gdb_assert (REGMAP_CLASS (regmap->output.raw_value[idx]) == REG_CLASS_LMEM_REG_OFFSET);

  return REGMAP_SP_OFST (regmap->output.raw_value[idx]);
}

uint32_t
regmap_get_offset (regmap_t regmap, uint32_t idx)
{
  gdb_assert (regmap);
  gdb_assert (idx < REGMAP_MAX_ENTRIES);
  gdb_assert (idx < regmap->output.num_entries);
  gdb_assert (REGMAP_CLASS (regmap->output.raw_value[idx]) == REG_CLASS_MEM_LOCAL);

  return REGMAP_OFST (regmap->output.raw_value[idx]);
}


/****************************************************************************

                              PROPERTY ROUTINES

 ****************************************************************************/

/* Determine if the value indicated by this register map is readable and
   writable.
 */
static void
regmap_find_access_permissions (regmap_t regmap, bool* read, bool *write)
{
  uint32_t num_chunks, chunk;
  uint32_t num_entries, i;
  uint32_t num_instances, expected_num_instances;

  gdb_assert (regmap);
  gdb_assert (write);
  gdb_assert (read);

  /* No entry means nothing to read or write */
  num_entries = regmap_get_num_entries (regmap);
  if (num_entries == 0)
    {
      *read = false;
      *write = false;
      return;
    }

  /* Compute the number of chunks */
  num_chunks = regmap->output.max_location_index + 1;
  gdb_assert (num_chunks <= REGMAP_MAX_LOCATION_INDEX + 1);

  /* Iterate over each chunk to determine the permissions. */
  *read = true;
  *write = true;
  expected_num_instances = ~0U;
  for (chunk = 0; chunk < num_chunks; ++chunk)
    {
      /* Count the number of instances for this chunk */
      num_instances = 0;
      for (i = 0; i < num_entries; ++i)
        if (regmap_get_location_index (regmap, i) == chunk)
          ++num_instances;

      /* Chunk 0, which always exists, is used as the reference */
      if (chunk == 0)
        expected_num_instances = num_instances;

      /* Not readable or writable if one chunk is missing */
      if (num_instances == 0)
        {
          *read = false;
          *write = false;
        }

      /* Writeable if same number of instances for all the chunks */
      if (num_instances != expected_num_instances)
        *write = false;
    }
}

bool
regmap_is_readable (regmap_t regmap)
{
  bool read = false;
  bool write = false;

  regmap_find_access_permissions (regmap, &read, &write);

  return read;
}

bool
regmap_is_writable (regmap_t regmap)
{
  bool read = false;
  bool write = false;

  regmap_find_access_permissions (regmap, &read, &write);

  return write;
}

/****************************************************************************

                                 READ ROUTINES

 ****************************************************************************/

/* read one register entry and returns the address of the next entry */
static void
regmap_read_reg (regmap_ptr_t *p, regmap_reg_t *reg)
{
  gdb_assert (reg);

  reg->idx        = *p->idx++;
  reg->ptx_reg    = p->ptx_reg;
  p->byte        += strlen(p->byte) + 1;
  reg->value      = *p->sass_reg++;
  reg->start_addr = *p->start++;
  reg->end_addr   = *p->end++;
  reg->next       = p->byte;
}

/* read one function entry and returns the address of its first register entry */
static void
regmap_read_func (regmap_ptr_t *p, regmap_func_t *func)
{
  uint32_t i;
  regmap_ptr_t q;
  regmap_reg_t reg;

  gdb_assert (func);

  func->name        = p->byte;
  p->byte          += strlen (p->byte) + 1;
  func->num_entries = *p->func_num_entries++;
  func->start       = p->byte;

  /* Because the size of the register entries is variables, we must read all
     the register entries to compute the function entry size. Sigh. */
  q.byte = func->start;
  for (i = 0; i < func->num_entries; i++)
    regmap_read_reg (&q, &reg);
  func->size = q.byte - func->start;
}

/* read one table entry and returns the address of its first function entry */
static void
regmap_read_table (struct objfile *objfile, regmap_table_t *table)
{
  bfd      *abfd;
  asection *asection;
  static const char section_name[] = ".nv_debug_info_reg_sass";

  gdb_assert (objfile);
  gdb_assert (table);

  /* default value */
  table->start = NULL;
  table->size  = 0;

  /* find the proper section */
  abfd = objfile->obfd;
  asection = bfd_get_section_by_name (abfd, section_name);
  if (!asection)
    return;

  /* allocate space to read the section */
  table->size  = bfd_get_section_size (asection);
  table->start = obstack_alloc (&objfile->objfile_obstack, table->size);

  /* read the section */
  if (bfd_seek (abfd, asection->filepos, SEEK_SET) != 0 ||
      bfd_bread (table->start, table->size, abfd) != table->size)
    {
      obstack_free (&objfile->objfile_obstack, table->start);
      table->start = NULL;
      table->size  = 0;
    }
}

/****************************************************************************

                                 PRINT ROUTINES

 ****************************************************************************/

/* print a register entry */
static void
regmap_reg_print (regmap_ptr_t *p)
{
  regmap_reg_t reg;

  regmap_read_reg (p, &reg);
  fprintf (stderr, "\t%d (reg: %s) 0x%x, 0x%x, 0x%x\n",
           reg.idx, reg.ptx_reg, reg.value, reg.start_addr, reg.end_addr);
}

/* print a function entry */
static void
regmap_func_print (regmap_ptr_t *p)
{
  uint32_t i;
  regmap_func_t func;

  regmap_read_func (p, &func);

  fprintf (stderr, "Function: %s (%u entries)\n", func.name, func.num_entries);

  for (i = 0; i < func.num_entries; i++)
    regmap_reg_print (p);
}

/* print the whole regmap section */
void
regmap_table_print (struct objfile *objfile)
{
  regmap_ptr_t p;
  regmap_table_t table;

  gdb_assert (objfile);

  /* Print the table, one function at a time */
  regmap_read_table (objfile, &table);
  p.byte = table.start;
  while (p.byte < table.start + table.size)
    regmap_func_print (&p);

  gdb_assert (p.byte == table.start + table.size);

  /* Free the table */
  obstack_free (&objfile->objfile_obstack, table.start);
}

/* print the search regmap object */
void
regmap_print (regmap_t regmap)
{
  int i;

  fprintf (stderr, "Regmap: (Function \"%s\" PTX register \"%s\""
          " Address 0x%"PRIx64") -> (",
           regmap->input.func_name, regmap->input.reg_name, regmap->input.addr);

  for (i = 0; i < regmap->output.num_entries; ++i)
    fprintf (stderr, "idx %d raw 0x%x ", i, regmap->output.raw_value[i]);

  fprintf (stderr, ")\n");
}

/****************************************************************************

                                SEARCH ROUTINES

 ****************************************************************************/

/* Find register in the register entry pointed by p */
static void
regmap_reg_search (regmap_ptr_t *p, regmap_t regmap)
{
  regmap_reg_t reg;

  gdb_assert (p);
  gdb_assert (regmap);

  /* Read the register reg */
  regmap_read_reg (p, &reg);
  gdb_assert (p->byte == reg.next);
  gdb_assert (reg.idx <= REGMAP_MAX_LOCATION_INDEX);

  /* Discard this register reg if the register name does not match */
  if (strcmp (reg.ptx_reg, regmap->input.reg_name) != 0)
    return;

  /* Save the maximum location index encountered for this register name */
  if (regmap->output.max_location_index == ~0U ||
      reg.idx > regmap->output.max_location_index)
    regmap->output.max_location_index = reg.idx;

  /* Discard this register reg if the address if out of range */
  if (regmap->input.addr < reg.start_addr || regmap->input.addr > reg.end_addr)
    return;

  /* Save the found element in the regmap object */
  regmap->output.location_index[regmap->output.num_entries] = reg.idx;
  regmap->output.raw_value[regmap->output.num_entries] = reg.value;
  regmap->output.num_entries++;
}

/* Find register in the func entry pointed by p */
static void
regmap_func_search (regmap_ptr_t *p, regmap_t regmap)
{
  uint32_t i;
  regmap_func_t func;

  gdb_assert (p);
  gdb_assert (regmap);

  /* Read the function func */
  regmap_read_func (p, &func);

  /* Discard this function if the name does not match. Make sure to increment
     the pointer to the next function beforehand. */
  if (strcmp (func.name, regmap->input.func_name) != 0)
    {
      p->byte = func.start + func.size;
      return;
    }

  /* Find the register for this function */
  for (i = 0; i < func.num_entries; i++)
    regmap_reg_search (p, regmap);
}

/* entry point to map a PTX register */
regmap_t
regmap_table_search (struct objfile *objfile, const char *func_name,
                     const char *reg_name, uint64_t addr)
{
  uint32_t i, func_name_len;
  regmap_table_t table;
  regmap_ptr_t p;
  char *func_name_copy, *parentheses;

  gdb_assert (objfile);
  gdb_assert (func_name);
  gdb_assert (reg_name);

  /* Copy the function name to filter out the parameters, if any */
  func_name_len = strlen (func_name);
  func_name_copy = xmalloc (func_name_len + 1);
  memcpy (func_name_copy, func_name, func_name_len + 1);
  parentheses = strstr (func_name_copy, "(");
  if (parentheses)
    *parentheses = 0;

  /* Initialize the search */
  memset (cuda_regmap, 0, sizeof *cuda_regmap);
  cuda_regmap->input.func_name = func_name_copy;
  cuda_regmap->input.reg_name  = reg_name;
  cuda_regmap->input.addr      = addr;
  cuda_regmap->output.max_location_index = ~0U;

  /* Search in each function */
  regmap_read_table (objfile, &table);
  p.byte = table.start;
  while (p.byte < table.start + table.size)
    regmap_func_search (&p, cuda_regmap);

  /* Free the table */
  obstack_free (&objfile->objfile_obstack, table.start);

  /* Restore the name with the parentheses to free the memory now */
  xfree (func_name_copy);
  cuda_regmap->input.func_name = func_name;

  return cuda_regmap;
}


/****************************************************************************

                                     MISC

 ****************************************************************************/

/* See if reg is a properly encoded CUDA physical register.  Currently only
   used by the DWARF2 frame reader (see dwarf2-frame.c) to decode CFA
   instructions that take a ULEB128-encoded register as an argument.  More
   noteably, it neither overrides nor is tied to a gdbarch register method.

   NOTE:  This is the raw backend encoding of a physical register, inclusive
   of the reg class and reg # (not the ULEB128-encoded virtual PTX register
   name).
 */
int
cuda_decode_physical_register (uint64_t reg, int32_t*result)
{
  uint32_t dev_id   = cuda_current_device ();
  uint32_t num_regs = device_get_num_registers (dev_id);
  reg_t last_regnum = num_regs - 1;

  if (reg < last_regnum)
    {
      *result = (int32_t)reg;
      return 0;
    }

  if (REGMAP_CLASS (reg) == REG_CLASS_REG_FULL)
    {
      *result = (int32_t)REGMAP_REG (reg);
      return 0;
    }

  return -1;
}
