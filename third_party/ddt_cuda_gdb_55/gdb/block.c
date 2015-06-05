/* Block-related functions for the GNU debugger, GDB.

   Modified by Allinea Software.
   
   Copyright (C) Allinea Software Ltd. 2015
   Copyright (C) 2003, 2007, 2008, 2009, 2010 Free Software Foundation, Inc.

   This file is part of GDB.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/*
 * NVIDIA CUDA Debugger CUDA-GDB Modified by Allinea Software.
 * NVIDIA CUDA Debugger CUDA-GDB 
 * NVIDIA CUDA Debugger CUDA-GDB Copyright (C) Allinea Software Ltd. 2015
 * NVIDIA CUDA Debugger CUDA-GDB Copyright (C) 2007-2013 NVIDIA Corporation
 * Modified from the original GDB file referenced above by the CUDA-GDB 
 * team at NVIDIA <cudatools@nvidia.com>.
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
#include "block.h"
#include "symtab.h"
#include "symfile.h"
#include "gdb_obstack.h"
#include "cp-support.h"
#include "addrmap.h"

/* This is used by struct block to store namespace-related info for
   C++ files, namely using declarations and the current namespace in
   scope.  */

struct block_namespace_info
{
  const char *scope;
  struct using_direct *using;
};

static void block_initialize_namespace (struct block *block,
					struct obstack *obstack);

/* Return Nonzero if block a is lexically nested within block b,
   or if a and b have the same pc range.
   Return zero otherwise. */

int
contained_in (const struct block *a, const struct block *b)
{
  if (!a || !b)
    return 0;

  do
    {
      if (a == b)
	return 1;
      /* If A is a function block, then A cannot be contained in B,
         except if A was inlined.  */
      if (BLOCK_FUNCTION (a) != NULL && !block_inlined_p (a))
        return 0;
      a = BLOCK_SUPERBLOCK (a);
    }
  while (a != NULL);

  return 0;
}


/* Return the symbol for the function which contains a specified
   lexical block, described by a struct block BL.  The return value
   will not be an inlined function; the containing function will be
   returned instead.  */

struct symbol *
block_linkage_function (const struct block *bl)
{
  if (!bl) return NULL;

  while ((BLOCK_FUNCTION (bl) == NULL || block_inlined_p (bl))
	 && BLOCK_SUPERBLOCK (bl) != NULL)
    bl = BLOCK_SUPERBLOCK (bl);

  return BLOCK_FUNCTION (bl);
}

/* Return one if BL represents an inlined function.  */

int
block_inlined_p (const struct block *bl)
{
  return BLOCK_FUNCTION (bl) != NULL && SYMBOL_INLINED (BLOCK_FUNCTION (bl));
}

/* Return the blockvector immediately containing the innermost lexical
   block containing the specified pc value and section, or 0 if there
   is none.  PBLOCK is a pointer to the block.  If PBLOCK is NULL, we
   don't pass this information back to the caller.  */

struct blockvector *
blockvector_for_pc_sect (CORE_ADDR pc, struct obj_section *section,
			 struct block **pblock, struct symtab *symtab)
{
  struct block *b, *innermost;
  int bot, top, half;
  struct blockvector *bl;

  if (symtab == 0)		/* if no symtab specified by caller */
    {
      /* First search all symtabs for one whose file contains our pc */
      symtab = find_pc_sect_symtab (pc, section);
      if (symtab == 0)
	return 0;
    }

  bl = BLOCKVECTOR (symtab);

  /* Then search that symtab for the smallest block that wins.  */

  /* If we have an addrmap mapping code addresses to blocks, then use
     that.  */
  if (BLOCKVECTOR_MAP (bl))
    {
      b = addrmap_find (BLOCKVECTOR_MAP (bl), pc);
      if (b)
        {
          if (pblock)
            *pblock = b;
          return bl;
        }
      else
        return 0;
    }


  /* Otherwise, use binary search to find the last block that starts
     before PC.  */
  bot = 0;
  top = BLOCKVECTOR_NBLOCKS (bl);

  while (top - bot > 1)
    {
      half = (top - bot + 1) >> 1;
      b = BLOCKVECTOR_BLOCK (bl, bot + half);
      if (BLOCK_START (b) <= pc)
	bot += half;
      else
	top = bot + half;
    }

  /* CUDA - bug fix */
  /* Now search backward for the innermost block that starts before PC and ends
     after PC. The block are sorted by start addr, but not by end addr. The
     original version was returning one arbitrary block of all the blocks that
     contain the PC. The CUDA version makes sure we return the innermost one. */
  innermost = NULL;
  for (; bot >= 0; --bot)
    {
      b = BLOCKVECTOR_BLOCK (bl, bot);
      if (BLOCK_END (b) <= pc)
        continue;
      if (!innermost)
        innermost = b;
      if (BLOCK_END (b) < BLOCK_END (innermost))
        innermost = b;
      /* If this block's superblock is the current 'innermost' block, then this
         block must be made the new 'innermost'.  lookup_symbol_block will traverse
         backwards from this block, through each superblock. */
      if (BLOCK_SUPERBLOCK (b) == innermost)
        innermost = b;
    }

  if (innermost)
    {
      if (pblock)
        *pblock = innermost;
      return bl;
    }

  return 0;
}

/* Return the blockvector immediately containing the innermost lexical block
   containing the specified pc value, or 0 if there is none.
   Backward compatibility, no section.  */

struct blockvector *
blockvector_for_pc (CORE_ADDR pc, struct block **pblock)
{
  return blockvector_for_pc_sect (pc, find_pc_mapped_section (pc),
				  pblock, NULL);
}

/* Return the innermost lexical block containing the specified pc value
   in the specified section, or 0 if there is none.  */

struct block *
block_for_pc_sect (CORE_ADDR pc, struct obj_section *section)
{
  struct blockvector *bl;
  struct block *b;

  bl = blockvector_for_pc_sect (pc, section, &b, NULL);
  if (bl)
    return b;
  return 0;
}

/* Return the innermost lexical block containing the specified pc value,
   or 0 if there is none.  Backward compatibility, no section.  */

struct block *
block_for_pc (CORE_ADDR pc)
{
  return block_for_pc_sect (pc, find_pc_mapped_section (pc));
}

/* Now come some functions designed to deal with C++ namespace issues.
   The accessors are safe to use even in the non-C++ case.  */

/* This returns the namespace that BLOCK is enclosed in, or "" if it
   isn't enclosed in a namespace at all.  This travels the chain of
   superblocks looking for a scope, if necessary.  */

const char *
block_scope (const struct block *block)
{
  for (; block != NULL; block = BLOCK_SUPERBLOCK (block))
    {
      if (BLOCK_NAMESPACE (block) != NULL
	  && BLOCK_NAMESPACE (block)->scope != NULL)
	return BLOCK_NAMESPACE (block)->scope;
    }

  return "";
}

/* Set BLOCK's scope member to SCOPE; if needed, allocate memory via
   OBSTACK.  (It won't make a copy of SCOPE, however, so that already
   has to be allocated correctly.)  */

void
block_set_scope (struct block *block, const char *scope,
		 struct obstack *obstack)
{
  block_initialize_namespace (block, obstack);

  BLOCK_NAMESPACE (block)->scope = scope;
}

/* This returns the using directives list associated with BLOCK, if
   any.  */

struct using_direct *
block_using (const struct block *block)
{
  if (block == NULL || BLOCK_NAMESPACE (block) == NULL)
    return NULL;
  else
    return BLOCK_NAMESPACE (block)->using;
}

/* Set BLOCK's using member to USING; if needed, allocate memory via
   OBSTACK.  (It won't make a copy of USING, however, so that already
   has to be allocated correctly.)  */

void
block_set_using (struct block *block,
		 struct using_direct *using,
		 struct obstack *obstack)
{
  block_initialize_namespace (block, obstack);

  BLOCK_NAMESPACE (block)->using = using;
}

/* If BLOCK_NAMESPACE (block) is NULL, allocate it via OBSTACK and
   ititialize its members to zero.  */

static void
block_initialize_namespace (struct block *block, struct obstack *obstack)
{
  if (BLOCK_NAMESPACE (block) == NULL)
    {
      BLOCK_NAMESPACE (block)
	= obstack_alloc (obstack, sizeof (struct block_namespace_info));
      BLOCK_NAMESPACE (block)->scope = NULL;
      BLOCK_NAMESPACE (block)->using = NULL;
    }
}

/* Return the static block associated to BLOCK.  Return NULL if block
   is NULL or if block is a global block.  */

const struct block *
block_static_block (const struct block *block)
{
  if (block == NULL || BLOCK_SUPERBLOCK (block) == NULL)
    return NULL;

  while (BLOCK_SUPERBLOCK (BLOCK_SUPERBLOCK (block)) != NULL)
    block = BLOCK_SUPERBLOCK (block);

  return block;
}

/* Return the static block associated to BLOCK.  Return NULL if block
   is NULL.  */

const struct block *
block_global_block (const struct block *block)
{
  if (block == NULL)
    return NULL;

  while (BLOCK_SUPERBLOCK (block) != NULL)
    block = BLOCK_SUPERBLOCK (block);

  return block;
}

/* Allocate a block on OBSTACK, and initialize its elements to
   zero/NULL.  This is useful for creating "dummy" blocks that don't
   correspond to actual source files.

   Warning: it sets the block's BLOCK_DICT to NULL, which isn't a
   valid value.  If you really don't want the block to have a
   dictionary, then you should subsequently set its BLOCK_DICT to
   dict_create_linear (obstack, NULL).  */

struct block *
allocate_block (struct obstack *obstack)
{
  struct block *bl = obstack_alloc (obstack, sizeof (struct block));

  BLOCK_START (bl) = 0;
  BLOCK_END (bl) = 0;
  BLOCK_FUNCTION (bl) = NULL;
  BLOCK_SUPERBLOCK (bl) = NULL;
  BLOCK_DICT (bl) = NULL;
  BLOCK_NAMESPACE (bl) = NULL;

  return bl;
}
