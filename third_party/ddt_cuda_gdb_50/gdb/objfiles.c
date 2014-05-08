/* GDB routines for manipulating objfiles.

   Copyright (C) 1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2000, 2001,
   2002, 2003, 2004, 2007, 2008, 2009, 2010 Free Software Foundation, Inc.

   Contributed by Cygnus Support, using pieces from other GDB modules.

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

/* This file contains support routines for creating, manipulating, and
   destroying objfile structures. */

#include "defs.h"
#include "bfd.h"		/* Binary File Description */
#include "symtab.h"
#include "symfile.h"
#include "objfiles.h"
#include "gdb-stabs.h"
#include "target.h"
#include "bcache.h"
#include "mdebugread.h"
#include "expression.h"
#include "parser-defs.h"

#include "gdb_assert.h"
#include <sys/types.h>
#include "gdb_stat.h"
#include <fcntl.h>
#include "gdb_obstack.h"
#include "gdb_string.h"
#include "hashtab.h"

#include "breakpoint.h"
#include "block.h"
#include "dictionary.h"
#include "source.h"
#include "addrmap.h"
#include "arch-utils.h"
#include "exec.h"
#include "observer.h"
#include "complaints.h"
#include "psymtab.h"
#include "solist.h"

/* Prototypes for local functions */

#if defined(USE_MMALLOC) && defined(HAVE_MMAP)
#include <sys/mman.h>

#include "mmalloc.h"

#include "command.h"
#include "gdbcmd.h"
#include "psympriv.h"
#include "cp-support.h"
#include "splay-tree.h"
#include "macrotab.h"
#include "buildsym.h" // for HASHSIZE, required by stabsread.h
#include "stabsread.h"
#include "dwarf2.h"

struct mapped_file_header;
static void init_mapped_file_header (void *md, struct stat *exesbuf);

static int open_existing_mapped_file (char *filename, struct stat *exesbuf, int flags);

static int open_mapped_file (char *filename, struct stat *exesbuf, int flags);

static void *map_to_file (int, int);

static char *auxillary_symbol_file_directory = NULL;

#endif /* defined(USE_MMALLOC) && defined(HAVE_MMAP) */

static void objfile_alloc_data (struct objfile *objfile);
static void objfile_free_data (struct objfile *objfile);

/* Externally visible variables that are owned by this module.
   See declarations in objfile.h for more info. */

struct objfile *current_objfile;	/* For symbol file being read in */
struct objfile *rt_common_objfile;	/* For runtime common symbols */

struct objfile_pspace_info
{
  int objfiles_changed_p;
  struct obj_section **sections;
  int num_sections;
};

/* Per-program-space data key.  */
static const struct program_space_data *objfiles_pspace_data;

static void
objfiles_pspace_data_cleanup (struct program_space *pspace, void *arg)
{
  struct objfile_pspace_info *info;

  info = program_space_data (pspace, objfiles_pspace_data);
  if (info != NULL)
    {
      xfree (info->sections);
      xfree (info);
    }
}

/* Get the current svr4 data.  If none is found yet, add it now.  This
   function always returns a valid object.  */

static struct objfile_pspace_info *
get_objfile_pspace_data (struct program_space *pspace)
{
  struct objfile_pspace_info *info;

  info = program_space_data (pspace, objfiles_pspace_data);
  if (info == NULL)
    {
      info = XZALLOC (struct objfile_pspace_info);
      set_program_space_data (pspace, objfiles_pspace_data, info);
    }

  return info;
}

/* Records whether any objfiles appeared or disappeared since we last updated
   address to obj section map.  */

int mapped_symbol_files;	/* Try to use mapped symbol files */

/* Locate all mappable sections of a BFD file. 
   objfile_p_char is a char * to get it through
   bfd_map_over_sections; we cast it back to its proper type.  */

struct add_to_objfile_data
{
  struct objfile *objfile;
  int count;
  int in_place;
};

/* Called via bfd_map_over_sections to build up the section table that
   the objfile references.  The objfile contains pointers to the start
   of the table (objfile->sections) and to the first location after
   the end of the table (objfile->sections_end). */

static void
add_to_objfile_sections (struct bfd *abfd, struct bfd_section *asect,
			 void *objfile_p_char)
{
  struct add_to_objfile_data *data = (struct add_to_objfile_data *) objfile_p_char;
  struct objfile *objfile = (struct objfile *) data->objfile;
  struct obj_section section;
  flagword aflag;

  aflag = bfd_get_section_flags (abfd, asect);

  if (!(aflag & SEC_ALLOC))
    return;

  if (0 == bfd_section_size (abfd, asect))
    return;
  section.objfile = objfile;
  section.the_bfd_section = asect;
  section.ovly_mapped = 0;
  if (data->in_place)
    objfile->sections[data->count] = section;
  else
    obstack_grow (&objfile->objfile_obstack, (char *) &section, sizeof (section));
  data->count++;
}

/* Builds a section table for OBJFILE.
   Returns 0 if OK, 1 on error (in which case bfd_error contains the
   error).

   Note that while we are building the table, which goes into the
   psymbol obstack, we hijack the sections_end pointer to instead hold
   a count of the number of sections.  When bfd_map_over_sections
   returns, this count is used to compute the pointer to the end of
   the sections table, which then overwrites the count.

   Also note that the OFFSET and OVLY_MAPPED in each table entry
   are initialized to zero.

   Also note that if anything else writes to the psymbol obstack while
   we are building the table, we're pretty much hosed. */

int
build_objfile_section_table (struct objfile *objfile, int in_place)
{
  /* objfile->sections can be already set when reading a mapped symbol
     file.  I believe that we do need to rebuild the section table in
     this case (we rebuild other things derived from the bfd), but we
     can't free the old one (it's in the objfile_obstack).  So we just
     waste some memory.  */

  struct add_to_objfile_data data;
  data.objfile = objfile;
  data.count = 0;
  data.in_place = in_place;
  bfd_map_over_sections (objfile->obfd,
			 add_to_objfile_sections, (void *) &data);
  if (!in_place)
    objfile->sections = obstack_finish (&objfile->objfile_obstack);
  objfile->sections_end = objfile->sections + data.count;
  return (0);
}

/* Given a pointer to an initialized bfd (ABFD) and some flag bits
   allocate a new objfile struct, fill it in as best we can, link it
   into the list of all known objfiles, and return a pointer to the
   new objfile struct.

   The FLAGS word contains various bits (OBJF_*) that can be taken as
   requests for specific operations, like trying to open a mapped
   version of the objfile (OBJF_MAPPED).  Other bits like
   OBJF_SHARED are simply copied through to the new objfile flags
   member. */

/* NOTE: carlton/2003-02-04: This function is called with args NULL, 0
   by jv-lang.c, to create an artificial objfile used to hold
   information about dynamically-loaded Java classes.  Unfortunately,
   that branch of this function doesn't get tested very frequently, so
   it's prone to breakage.  (E.g. at one time the name was set to NULL
   in that situation, which broke a loop over all names in the dynamic
   library loader.)  If you change this function, please try to leave
   things in a consistent state even if abfd is NULL.  */

struct objfile_instance *
allocate_objfile (bfd *abfd, int flags)
{
  struct objfile *objfile = NULL;
  int restore_data = 0;

  if (mapped_symbol_files)
    flags |= OBJF_MAPPED;

#if defined(USE_MMALLOC) && defined(HAVE_MMAP)
  /* Only try to use a mapped symbol file if the underlying object file is a
     real file (and not, for example, the system-supplied DSO object).
   */
  if (abfd != NULL
      && access (bfd_get_filename (abfd), R_OK) == 0)
    {

      /* If we can support mapped symbol files, try to open/reopen the
         mapped file that corresponds to the file from which we wish to
         read symbols.  If the objfile is to be mapped, we must malloc
         the structure itself using the mmap version, and arrange that
         all memory allocation for the objfile uses the mmap routines.
         If we are reusing an existing mapped file, from which we get
         our objfile pointer, we have to make sure that we update the
         pointers to the alloc/free functions in the obstack, in case
         these functions have moved within the current gdb.  */

      int fd;
      struct stat statbuf;
      bfd_stat (abfd, &statbuf);
      fd = open_mapped_file (bfd_get_filename (abfd),
			     &statbuf,
			     flags);
      if (fd >= 0)
	{
	  void *md;
	  if ((md = map_to_file (fd, flags)) == NULL)
	    {
	      close (fd);
	    }
	  else if ((objfile = (struct objfile *) mmalloc_getkey (md, 0)) != NULL)
	    {
	      /* Update memory corruption handler function addresses. */
	      init_malloc (md);
	      objfile->md = md;
	      objfile->mmfd = fd;
	      obstack_chunkfun (&objfile->objfile_obstack, xmmalloc);
	      obstack_freefun (&objfile->objfile_obstack, xmfree);
	      if (objfile->demangled_names_hash)
		htab_set_functions_ex
		  (objfile->demangled_names_hash, htab_hash_string,
		   (int (*) (const void *, const void *)) streq, NULL,
		   objfile->md, xmcalloc, xmfree);

	      /* If already in objfile list, unlink it. */
	      objfile->separate_debug_objfile = NULL;
	      objfile->separate_debug_objfile_backlink = NULL;
	      objfile->separate_debug_objfile_link = NULL;
	      objfile->instances = NULL;
	    }
	  else
	    {	      
	      /* Set up to detect internal memory corruption.  MUST be
	         done before the first malloc.  See comments in
	         init_malloc() and mmcheck().  */

	      init_malloc (md);

	      init_mapped_file_header (md, &statbuf);
	      
	      objfile = (struct objfile *)
		xmmalloc (md, sizeof (struct objfile));
	      memset (objfile, 0, sizeof (struct objfile));
	      objfile->md = md;
	      objfile->mmfd = fd;
	      objfile->flags |= OBJF_MAPPED;
	      mmalloc_setkey (objfile->md, 0, objfile);
	      objfile->psymbol_cache = bcache_xmmalloc (objfile->md);
	      objfile->macro_cache = bcache_xmmalloc (objfile->md);
	      objfile->filename_cache = bcache_xmmalloc (objfile->md);
	      obstack_specify_allocation_with_arg (&objfile->objfile_obstack,
						   0, 0, xmmalloc, xmfree,
						   objfile->md);
	      terminate_minimal_symbol_table (objfile);
	      
	      /* Initialize the section indexes for this objfile, so that we can
		 later detect if they are used w/o being properly assigned to. */

	      objfile->sect_index_text = -1;
	      objfile->sect_index_data = -1;
	      objfile->sect_index_bss = -1;
	      objfile->sect_index_rodata = -1;

	      /* We don't yet have a C++-specific namespace symtab.  */

	      objfile->cp_namespace_symtab = NULL;
	    }
	}

      if ((flags & OBJF_MAPPED) && (objfile == NULL))
	{
	  warning ("symbol table for '%s' will not be mapped",
		   bfd_get_filename (abfd));
	  flags &= ~OBJF_MAPPED;
	}
    }
#else /* !defined(USE_MMALLOC) || !defined(HAVE_MMAP) */

  if (flags & OBJF_MAPPED)
    {
      warning ("mapped symbol tables are not supported on this machine; missing or broken mmap().");

      /* Turn off the global flag so we don't try to do mapped symbol tables
         any more, which shuts up gdb unless the user specifically gives the
         "mapped" keyword again. */

      mapped_symbol_files = 0;
      flags &= ~OBJF_MAPPED;
    }

#endif /* defined(USE_MMALLOC) && defined(HAVE_MMAP) */

  /* If we don't support mapped symbol files, didn't ask for the file to be
     mapped, or failed to open the mapped file for some reason, then revert
     back to an unmapped objfile. */

  if (objfile == NULL)
    {
      objfile = (struct objfile *) xzalloc (sizeof (struct objfile));
      objfile->psymbol_cache = bcache_xmmalloc (NULL);
      objfile->macro_cache = bcache_xmmalloc (NULL);
      objfile->filename_cache = bcache_xmmalloc (NULL);
      /* We could use obstack_specify_allocation here instead, but
	gdb_obstack.h specifies the alloc/dealloc functions.  */
      obstack_init (&objfile->objfile_obstack);
      flags &= ~OBJF_MAPPED;

      terminate_minimal_symbol_table (objfile);
      
      /* Initialize the section indexes for this objfile, so that we can
	 later detect if they are used w/o being properly assigned to. */

      objfile->sect_index_text = -1;
      objfile->sect_index_data = -1;
      objfile->sect_index_bss = -1;
      objfile->sect_index_rodata = -1;

      /* We don't yet have a C++-specific namespace symtab.  */

      objfile->cp_namespace_symtab = NULL;      
    }

  if (objfile->data == NULL)
    objfile_alloc_data (objfile);
  else
    restore_data = 1;

  /* Update the per-objfile information that comes from the bfd, ensuring
     that any data that is reference is saved in the per-objfile data
     region. */

  objfile->obfd = gdb_bfd_ref (abfd);
  if (objfile->name != NULL)
    {
      xmfree (objfile->md, objfile->name);
    }
  if (abfd != NULL)
    {
      /* Look up the gdbarch associated with the BFD.  */
      objfile->gdbarch = gdbarch_from_bfd (abfd);

      objfile->name = mstrsave (objfile->md, bfd_get_filename (abfd));
      objfile->mtime = bfd_get_mtime (abfd);

      /* Build section table.  */

      if (build_objfile_section_table (objfile, objfile->sections != NULL))
	{
	  error (_("Can't find the file sections in `%s': %s"),
		 objfile->name, bfd_errmsg (bfd_get_error ()));
	}
    }
  else
    {
      objfile->name = mstrsave (objfile->md, "<<anonymous objfile>>");
    }

  /* Save passed in flag bits. */
  objfile->flags |= flags & ~(OBJF_USERLOADED | OBJF_SHARED);

  if (restore_data)
    restore_objfile_data (objfile);
  
  return new_objfile_instance (objfile, flags & (OBJF_USERLOADED | OBJF_SHARED));
}

struct objfile_instance *
new_objfile_instance (struct objfile *objfile, int flags)
{
  struct objfile_instance *objfile_instance;
  struct objfile_instance **last_one;

  objfile_instance = (struct objfile_instance *) xzalloc (sizeof (struct objfile_instance));
  objfile_instance->objfile = objfile;
  objfile_instance->instance_link = objfile->instances;
  objfile->instances = objfile_instance;
  
  objfile_instance->instance_flags |= flags;
  
  objfile_instance->pspace = current_program_space;

  /* Add this file onto the tail of the linked list of other such files. */

  if (object_files == NULL)
    last_one = &object_files;    
  else
    {
      for (last_one = &object_files;
	   (*last_one)->next;
	   last_one = &(*last_one)->next);
    }
  objfile_instance->next = *last_one;
  *last_one = objfile_instance;

  /* Rebuild section map next time we need it.  */
  get_objfile_pspace_data (objfile_instance->pspace)->objfiles_changed_p = 1;

  if (objfile_instance->instance_link != NULL)
    {
      struct objfile *separate_debug_objfile = objfile->separate_debug_objfile;
      while (separate_debug_objfile)
	{
	  struct objfile_instance *separate_debug_objfile_instance = new_objfile_instance (separate_debug_objfile, 0);
	  put_objfile_before (separate_debug_objfile_instance, objfile_instance);
	  separate_debug_objfile = separate_debug_objfile->separate_debug_objfile_link;
	}
    }

  return objfile_instance;
}

/* Retrieve the gdbarch associated with OBJFILE.  */
struct gdbarch *
get_objfile_arch (struct objfile *objfile)
{
  return objfile->gdbarch;
}

/* Initialize entry point information for this objfile. */

void
init_entry_point_info (struct objfile *objfile)
{
  /* Save startup file's range of PC addresses to help blockframe.c
     decide where the bottom of the stack is.  */

  if (bfd_get_file_flags (objfile->obfd) & EXEC_P)
    {
      /* Executable file -- record its entry point so we'll recognize
         the startup file because it contains the entry point.  */
      objfile->ei.entry_point = bfd_get_start_address (objfile->obfd);
      objfile->ei.entry_point_p = 1;
    }
  else if (bfd_get_file_flags (objfile->obfd) & DYNAMIC
	   && bfd_get_start_address (objfile->obfd) != 0)
    {
      /* Some shared libraries may have entry points set and be
	 runnable.  There's no clear way to indicate this, so just check
	 for values other than zero.  */
      objfile->ei.entry_point = bfd_get_start_address (objfile->obfd);    
      objfile->ei.entry_point_p = 1;
    }
  else
    {
      /* Examination of non-executable.o files.  Short-circuit this stuff.  */
      objfile->ei.entry_point_p = 0;
    }
}

/* If there is a valid and known entry point, function fills *ENTRY_P with it
   and returns non-zero; otherwise it returns zero.  */

int
entry_point_address_query (CORE_ADDR *entry_p)
{
  struct gdbarch *gdbarch;
  CORE_ADDR entry_point;

  if (symfile_objfile->objfile == NULL || !symfile_objfile->objfile->ei.entry_point_p)
    return 0;

  gdbarch = get_objfile_arch (symfile_objfile->objfile);

  entry_point = symfile_objfile->objfile->ei.entry_point;

  /* Make certain that the address points at real code, and not a
     function descriptor.  */
  entry_point = gdbarch_convert_from_func_ptr_addr (gdbarch, entry_point,
						    &current_target);

  /* Remove any ISA markers, so that this matches entries in the
     symbol table.  */
  entry_point = gdbarch_addr_bits_remove (gdbarch, entry_point);

  *entry_p = entry_point;
  return 1;
}

/* Get current entry point address.  Call error if it is not known.  */

CORE_ADDR
entry_point_address (void)
{
  CORE_ADDR retval;

  if (!entry_point_address_query (&retval))
    error (_("Entry point address is not known."));

  return retval;
}

/* Create the terminating entry of OBJFILE's minimal symbol table.
   If OBJFILE->msymbols is zero, allocate a single entry from
   OBJFILE->objfile_obstack; otherwise, just initialize
   OBJFILE->msymbols[OBJFILE->minimal_symbol_count].  */
void
terminate_minimal_symbol_table (struct objfile *objfile)
{
  if (! objfile->msymbols)
    objfile->msymbols = ((struct minimal_symbol *)
                         obstack_alloc (&objfile->objfile_obstack,
                                        sizeof (objfile->msymbols[0])));

  {
    struct minimal_symbol *m
      = &objfile->msymbols[objfile->minimal_symbol_count];

    memset (m, 0, sizeof (*m));
    /* Don't rely on these enumeration values being 0's.  */
    MSYMBOL_TYPE (m) = mst_unknown;
    SYMBOL_INIT_LANGUAGE_SPECIFIC (m, language_unknown);
  }
}

/* Iterator on PARENT and every separate debug objfile of PARENT.
   The usage pattern is:
     for (objfile = parent;
          objfile;
          objfile = objfile_separate_debug_iterate (parent, objfile))
       ...
*/

struct objfile *
objfile_separate_debug_iterate (const struct objfile *parent,
                                const struct objfile *objfile)
{
  struct objfile *res;

  /* If any, return the first child.  */
  res = objfile->separate_debug_objfile;
  if (res)
    return res;

  /* Common case where there is no separate debug objfile.  */
  if (objfile == parent)
    return NULL;

  /* Return the brother if any.  Note that we don't iterate on brothers of
     the parents.  */
  res = objfile->separate_debug_objfile_link;
  if (res)
    return res;

  for (res = objfile->separate_debug_objfile_backlink;
       res != parent;
       res = res->separate_debug_objfile_backlink)
    {
      gdb_assert (res != NULL);
      if (res->separate_debug_objfile_link)
        return res->separate_debug_objfile_link;
    }
  return NULL;
}

/* Find an instance of objfile in the current program space.  */
struct objfile_instance *
find_objfile_instance (struct objfile *objfile)
{
  struct objfile_instance *objfile_instance;
  ALL_OBJFILES (objfile_instance)
  {
    if (objfile_instance->objfile == objfile)
      return objfile_instance;
  }
  return NULL;
}

/* Put one object file before a specified on in the global list.
   This can be used to make sure an object file is destroyed before
   another when using ALL_OBJFILES_SAFE to free all objfiles. */
void
put_objfile_before (struct objfile_instance *objfile, struct objfile_instance *before_this)
{
  struct objfile_instance **objp;

  unlink_objfile (objfile);
  
  for (objp = &object_files; *objp != NULL; objp = &((*objp)->next))
    {
      if (*objp == before_this)
	{
	  objfile->next = *objp;
	  *objp = objfile;
	  return;
	}
    }
  
  internal_error (__FILE__, __LINE__,
		  _("put_objfile_before: before objfile not in list"));
}

/* Put OBJFILE at the front of the list.  */

void
objfile_to_front (struct objfile_instance *objfile)
{
  struct objfile_instance **objp;
  for (objp = &object_files; *objp != NULL; objp = &((*objp)->next))
    {
      if (*objp == objfile)
	{
	  /* Unhook it from where it is.  */
	  *objp = objfile->next;
	  /* Put it in the front.  */
	  objfile->next = object_files;
	  object_files = objfile;
	  break;
	}
    }
}

/* Put OBJFILE at the back of the list.  */

void
objfile_to_back (struct objfile_instance *objfile)
{
  struct objfile_instance **objp;
  for (objp = &object_files; *objp != NULL; objp = &((*objp)->next))
    {
      if (*objp == objfile)
	{
	  /* Unhook it from where it is.  */
	  *objp = objfile->next;
	  /* Put it in the back.  */
	  objfile->next = NULL;
	  for (; *objp != NULL; objp = &((*objp)->next)) ;
	  *objp = objfile;
	  break;
	}
    }
}

/* Unlink OBJFILE from the list of known objfiles, if it is found in the
   list.

   It is not a bug, or error, to call this function if OBJFILE is not known
   to be in the current list.  This is done in the case of mapped objfiles,
   for example, just to ensure that the mapped objfile doesn't appear twice
   in the list.  Since the list is threaded, linking in a mapped objfile
   twice would create a circular list.

   If OBJFILE turns out to be in the list, we zap it's NEXT pointer after
   unlinking it, just to ensure that we have completely severed any linkages
   between the OBJFILE and the list. */

void
unlink_objfile (struct objfile_instance *objfile)
{
  struct objfile_instance **objpp;

  for (objpp = &object_files; *objpp != NULL; objpp = &((*objpp)->next))
    {
      if (*objpp == objfile)
	{
	  *objpp = (*objpp)->next;
	  objfile->next = NULL;
	  return;
	}
    }

  internal_error (__FILE__, __LINE__,
		  _("unlink_objfile: objfile already unlinked"));
}

/* Add OBJFILE as a separate debug objfile of PARENT.  */

void
add_separate_debug_objfile (struct objfile *objfile, struct objfile *parent)
{
  gdb_assert (objfile && parent);

  /* Must not be already in a list.  */
  gdb_assert (objfile->separate_debug_objfile_backlink == NULL);
  gdb_assert (objfile->separate_debug_objfile_link == NULL);

  objfile->separate_debug_objfile_backlink = parent;
  objfile->separate_debug_objfile_link = parent->separate_debug_objfile;
  parent->separate_debug_objfile = objfile;
}

/* Free all separate debug objfile of OBJFILE, but don't free OBJFILE
   itself.  */

void
free_objfile_separate_debug (struct objfile *objfile)
{
  struct objfile *child;

  for (child = objfile->separate_debug_objfile; child;)
    {
      struct objfile *next_child = child->separate_debug_objfile_link;
      free_objfile (child);
      child = next_child;
    }
}

static void
free_objfile_instance1 (struct objfile_instance *objfile_instance)
{
  struct objfile *debug_objfile;
  struct objfile_instance **objpp;

  for (debug_objfile = objfile_instance->objfile->separate_debug_objfile; debug_objfile; debug_objfile = debug_objfile->separate_debug_objfile_link)
    {
      struct objfile_instance *objfile_instance = find_objfile_instance (debug_objfile);
      if (objfile_instance)
	free_objfile_instance1 (objfile_instance);
    }
  
  /* Remove it from the chain of all objfiles. */

  unlink_objfile (objfile_instance);

  if (objfile_instance == symfile_objfile)
    symfile_objfile = NULL;

  /*
  if (objfile_instance->objfile == rt_common_objfile)
    rt_common_objfile = NULL;
  */

  /* Before the symbol table code was redone to make it easier to
     selectively load and remove information particular to a specific
     linkage unit, gdb used to do these things whenever the monolithic
     symbol table was blown away.  How much still needs to be done
     is unknown, but we play it safe for now and keep each action until
     it is shown to be no longer needed. */

  /* Not all our callers call clear_symtab_users (objfile_purge_solibs,
     for example), so we need to call this here.  */
  clear_pc_function_cache ();

  /* Clear globals which might have pointed into a removed objfile.
     FIXME: It's not clear which of these are supposed to persist
     between expressions and which ought to be reset each time.  */
  expression_context_block = NULL;
  innermost_block = NULL;

  /* Rebuild section map next time we need it.  */
  get_objfile_pspace_data (objfile_instance->pspace)->objfiles_changed_p = 1;

  for (objpp = &objfile_instance->objfile->instances; *objpp != NULL; objpp = &((*objpp)->instance_link))
    {
      if (*objpp == objfile_instance)
	{
	  *objpp = (*objpp)->next;
	  break;
	}
    }

  xfree (objfile_instance);
}

void
free_objfile_instance (struct objfile_instance *objfile_instance)
{
  int last_one = objfile_instance->objfile->instances == objfile_instance
		 && !objfile_instance->instance_link;
  struct objfile *objfile = objfile_instance->objfile;
  free_objfile_instance1 (objfile_instance);
  if (last_one)
    {
      /* If this is the last instance free the OBJFILE.  */
      free_objfile (objfile);
    }
}

/* Destroy an objfile and all the symtabs and psymtabs under it.  Note
   that as much as possible is allocated on the objfile_obstack 
   so that the memory can be efficiently freed.

   Things which we do NOT free because they are not in malloc'd memory
   or not in memory specific to the objfile include:

   objfile -> sf

   FIXME:  If the objfile is using reusable symbol information (via mmalloc),
   then we need to take into account the fact that more than one process
   may be using the symbol information at the same time (when mmalloc is
   extended to support cooperative locking).  When more than one process
   is using the mapped symbol info, we need to be more careful about when
   we free objects in the reusable area. */

void
free_objfile (struct objfile *objfile)
{
  struct objfile_instance *objfile_instance;

#if defined(USE_MMALLOC) && defined(HAVE_MMAP)     
  if (objfile->flags & OBJF_MAPPED)
    {
      /* We don't want to actually free this data in the file, so make a private
	 copy. */
      if (!mmalloc_makeprivate (objfile->md))
	  internal_error (__FILE__, __LINE__,
			  _("mmalloc_makeprivate failed"));
    }
#endif

  /* Free all instances.  */
  for (objfile_instance = objfile->instances; objfile_instance; )
    {
      struct objfile_instance *next = objfile_instance->next;
      free_objfile_instance1 (objfile_instance);
      objfile_instance = next;
    }

  /* Free all separate debug objfiles.  */
  free_objfile_separate_debug (objfile);  
  objfile->separate_debug_objfile = NULL;

  if (objfile->separate_debug_objfile_backlink)
    {
      /* We freed the separate debug file, make sure the base objfile
	 doesn't reference it.  */
      struct objfile *child;

      child = objfile->separate_debug_objfile_backlink->separate_debug_objfile;

      if (child == objfile)
        {
          /* OBJFILE is the first child.  */
          objfile->separate_debug_objfile_backlink->separate_debug_objfile =
            objfile->separate_debug_objfile_link;
        }
      else
        {
          /* Find OBJFILE in the list.  */
          while (1)
            {
              if (child->separate_debug_objfile_link == objfile)
                {
                  child->separate_debug_objfile_link =
                    objfile->separate_debug_objfile_link;
                  break;
                }
              child = child->separate_debug_objfile_link;
              gdb_assert (child);
            }
        }
    }

  /* Remove any references to this objfile in the global value
     lists.  */
  preserve_values (objfile);

  /* Check to see if the current_source_symtab belongs to this objfile,
     and if so, call clear_current_source_symtab_and_line. */

  {
    struct symtab_and_line cursal = get_current_source_symtab_and_line ();
    struct symtab *s;

    ALL_OBJFILE_SYMTABS (objfile, s)
      {
	if (s == cursal.symtab)
	  clear_current_source_symtab_and_line ();
      }
  }
  
  /* First do any symbol file specific actions required when we are
     finished with a particular symbol file.  Note that if the objfile
     is using reusable symbol information (via mmalloc) then each of
     these routines is responsible for doing the correct thing, either
     freeing things which are valid only during this particular gdb
     execution, or leaving them to be reused during the next one. */

  if (objfile->sf != NULL)
    {
      (*objfile->sf->sym_finish) (objfile);
    }

  gdb_bfd_unref (objfile->obfd);

  /* Discard any data modules have associated with the objfile.  */
  objfile_free_data (objfile);

#if defined(USE_MMALLOC) && defined(HAVE_MMAP)    
  if (objfile->flags & OBJF_MAPPED)
    {
      /* Remember the fd so we can close it.  We can't close it before
         doing the detach, and after the detach the objfile is gone. */
      int mmfd;

      mmfd = objfile->mmfd;
      mmalloc_detach (objfile->md);
      objfile = NULL;
      close (mmfd);
    }

#endif /* defined(USE_MMALLOC) && defined(HAVE_MMAP) */

  /* If we still have an objfile, then either we don't support reusable
     objfiles or this one was not reusable.  So free it normally. */

  if (objfile != NULL)
    {
      if (objfile->name != NULL)
	{
	  xmfree (objfile->md, objfile->name);
	}
      if (objfile->global_psymbols.list)
	xmfree (objfile->md, objfile->global_psymbols.list);
      if (objfile->static_psymbols.list)
	xmfree (objfile->md, objfile->static_psymbols.list);
      /* Free the obstacks for non-reusable objfiles */
      bcache_xmfree (objfile->psymbol_cache);
      bcache_xmfree (objfile->macro_cache);
      bcache_xmfree (objfile->filename_cache);
      if (objfile->demangled_names_hash)
	htab_delete (objfile->demangled_names_hash);
      obstack_free (&objfile->objfile_obstack, 0);
      xmfree (objfile->md, objfile);
    }
}

static void
do_free_objfile_cleanup (void *obj)
{
  free_objfile (obj);
}

struct cleanup *
make_cleanup_free_objfile (struct objfile *obj)
{
  return make_cleanup (do_free_objfile_cleanup, obj);
}

/* Free all the object files at once and clean up their users.  */

void
free_all_objfiles (void)
{
  struct objfile_instance *objfile_instance, *temp;
  struct so_list *so;

  /* Any objfile referencewould become stale.  */
  for (so = master_so_list (); so; so = so->next)
    gdb_assert (so->objfile == NULL);

  ALL_OBJFILES_SAFE (objfile_instance, temp)
  {
    free_objfile_instance (objfile_instance);
  }
  clear_symtab_users ();
}

/* Relocate OBJFILE to NEW_OFFSETS.  There should be OBJFILE->NUM_SECTIONS
   entries in new_offsets.  SEPARATE_DEBUG_OBJFILE is not touched here.
   Return non-zero iff any change happened.  */

static int
objfile_relocate1 (struct objfile *objfile, 
		   struct section_offsets *new_offsets)
{
  struct obj_section *s;
  struct section_offsets *delta =
    ((struct section_offsets *) 
     alloca (SIZEOF_N_SECTION_OFFSETS (objfile->num_sections)));
  struct objfile_instance *objfile_instance;

  int i;
  int something_changed = 0;

  /* The OBJFILE must not be shared.. */
  gdb_assert(objfile->instances->instance_link == NULL);
  
  for (i = 0; i < objfile->num_sections; ++i)
    {
      delta->offsets[i] =
	ANOFFSET (new_offsets, i) - ANOFFSET (objfile->section_offsets, i);
      if (ANOFFSET (delta, i) != 0)
	something_changed = 1;
    }
  if (!something_changed)
    return 0;

  /* OK, get all the symtabs.  */
  {
    struct symtab *s;

    ALL_OBJFILE_SYMTABS (objfile, s)
    {
      struct linetable *l;
      struct blockvector *bv;
      int i;

      /* First the line table.  */
      l = LINETABLE (s);
      if (l)
	{
	  for (i = 0; i < l->nitems; ++i)
	    l->item[i].pc += ANOFFSET (delta, s->block_line_section);
	}

      /* Don't relocate a shared blockvector more than once.  */
      if (!s->primary)
	continue;

      bv = BLOCKVECTOR (s);
      if (BLOCKVECTOR_MAP (bv))
	addrmap_relocate (BLOCKVECTOR_MAP (bv),
			  ANOFFSET (delta, s->block_line_section));

      for (i = 0; i < BLOCKVECTOR_NBLOCKS (bv); ++i)
	{
	  struct block *b;
	  struct symbol *sym;
	  struct dict_iterator iter;

	  b = BLOCKVECTOR_BLOCK (bv, i);
	  BLOCK_START (b) += ANOFFSET (delta, s->block_line_section);
	  BLOCK_END (b) += ANOFFSET (delta, s->block_line_section);

	  ALL_BLOCK_SYMBOLS (b, iter, sym)
	    {
	      fixup_symbol_section (sym, objfile);

	      /* The RS6000 code from which this was taken skipped
	         any symbols in STRUCT_DOMAIN or UNDEF_DOMAIN.
	         But I'm leaving out that test, on the theory that
	         they can't possibly pass the tests below.  */
	      if ((SYMBOL_CLASS (sym) == LOC_LABEL
		   || SYMBOL_CLASS (sym) == LOC_STATIC)
		  && SYMBOL_SECTION (sym) >= 0)
		{
		  SYMBOL_VALUE_ADDRESS (sym) +=
		    ANOFFSET (delta, SYMBOL_SECTION (sym));
		}
	    }
	}
    }
  }

  if (objfile->psymtabs_addrmap)
    addrmap_relocate (objfile->psymtabs_addrmap,
		      ANOFFSET (delta, SECT_OFF_TEXT (objfile)));

  if (objfile->sf)
    objfile->sf->qf->relocate (objfile, new_offsets, delta);

  {
    struct minimal_symbol *msym;

    ALL_OBJFILE_MSYMBOLS (objfile, msym)
      if (SYMBOL_SECTION (msym) >= 0)
      SYMBOL_VALUE_ADDRESS (msym) += ANOFFSET (delta, SYMBOL_SECTION (msym));
  }
  /* Relocating different sections by different amounts may cause the symbols
     to be out of order.  */
  msymbols_sort (objfile);

  if (objfile->ei.entry_point_p)
    {
      /* Relocate ei.entry_point with its section offset, use SECT_OFF_TEXT
	 only as a fallback.  */
      struct obj_section *s;
      s = find_pc_section (objfile->ei.entry_point);
      if (s)
        objfile->ei.entry_point += ANOFFSET (delta, s->the_bfd_section->index);
      else
        objfile->ei.entry_point += ANOFFSET (delta, SECT_OFF_TEXT (objfile));
    }

  {
    int i;

    for (i = 0; i < objfile->num_sections; ++i)
      (objfile->section_offsets)->offsets[i] = ANOFFSET (new_offsets, i);
  }

  /* Rebuild section map next time we need it.  */
  for (objfile_instance = objfile->instances; objfile_instance; objfile_instance = objfile_instance->instance_link)
    get_objfile_pspace_data (objfile_instance->pspace)->objfiles_changed_p = 1;

  /* Update the table in exec_ops, used to read memory.  */
  ALL_OBJFILE_OSECTIONS (objfile, s)
    {
      int idx = s->the_bfd_section->index;

      exec_set_section_address (bfd_get_filename (objfile->obfd), idx,
				obj_section_addr (s));
    }

  /* Data changed.  */
  return 1;
}

#if defined(USE_MMALLOC) && defined(HAVE_MMAP)

struct size_and_time
{
    off_t		size;
    time_t 		mtime;
};

struct mapped_file_header
{
    struct size_and_time 	gdb;
    struct size_and_time	obj;
};

static struct stat*
cached_gdbstat ()
{
   static struct stat gdbsbuf;  
   static int gdbsbuf_initialized, gdbsbuf_valid;
   if (!gdbsbuf_initialized)
     {
       gdbsbuf_initialized = 1;
       if (stat (gdb_program_name, &gdbsbuf) == 0)
        gdbsbuf_valid = 1;
     }
   if (!gdbsbuf_valid)
     return NULL;
   return &gdbsbuf;
};

static void
init_size_and_time_from_stat (struct size_and_time *sat, struct stat *s)
{
  sat->size = s->st_size;
  sat->mtime = s->st_mtime;
}

static int
size_and_time_with_stat_matches (struct size_and_time *sat, struct stat *s)
{
  if (sat->size != s->st_size)
    return 0;
  if (sat->mtime != s->st_mtime)
    return 0;
  return 1;
}

static void
init_mapped_file_header (void *md, struct stat *exesbuf)
{
  struct mapped_file_header *hdr = (struct mapped_file_header *)
    xmmalloc (md, sizeof (struct mapped_file_header));
  init_size_and_time_from_stat (&hdr->gdb, cached_gdbstat ());
  init_size_and_time_from_stat (&hdr->obj, exesbuf);
  mmalloc_setkey (md, 1, hdr);  
}

static int
is_mapped_file_current (const char *symsfilename, int fd, struct stat *exesbuf)
{
  void *md;
  struct mapped_file_header *hdr;  
  int ret = 0;
  struct stat *gdbsbuf;
  md = mmalloc_reuse (fd, 1);
  if (md == NULL)
    return 0;
  hdr = (struct mapped_file_header *) mmalloc_getkey (md, 1);
  if (hdr == NULL)
    {
      // file is corrupted or in an old format
    }
  else if ((gdbsbuf = cached_gdbstat())
	   && size_and_time_with_stat_matches (&hdr->gdb, gdbsbuf) == 0)
    {
      warning ("mapped symbol file `%s' is older than GDB, ignored it",
	       symsfilename);
    }
  else if (size_and_time_with_stat_matches (&hdr->obj, exesbuf) == 0)
    {
      warning ("mapped symbol file `%s' is out of date, ignored it",
	       symsfilename);
    }
  else
    {
      ret = 1;
    }
  mmalloc_detach (md);
  return ret;
}

/* Given the name of a mapped symbol file in SYMSFILENAME, and the timestamp
   of the corresponding symbol file in MTIME, try to open an existing file
   with the name SYMSFILENAME and verify it is more recent than the base
   file by checking it's timestamp against MTIME.

   If SYMSFILENAME does not exist (or can't be stat'd), simply returns -1.

   If SYMSFILENAME does exist, but is out of date, we check to see if the
   user has specified creation of a mapped file.  If so, we don't issue
   any warning message because we will be creating a new mapped file anyway,
   overwriting the old one.  If not, then we issue a warning message so that
   the user will know why we aren't using this existing mapped symbol file.
   In either case, we return -1.

   If SYMSFILENAME does exist and is not out of date, but can't be opened for
   some reason, then prints an appropriate system error message and returns -1.

   Otherwise, returns the open file descriptor.  */

static int
open_existing_mapped_file (char *symsfilename, struct stat *exesbuf, int flags)
{
  int fd = -1;
  if ((fd = open (symsfilename, O_RDONLY)) < 0)
    {
      if (errno != ENOENT
	  && errno != EISDIR
	  && errno != ENOTDIR)
	{
	  if (error_pre_print)
	    {
	      printf_unfiltered ("%s", error_pre_print);
	    }
	  print_sys_errmsg (symsfilename, errno);    
	}
    }
  else if (!is_mapped_file_current (symsfilename, fd, exesbuf))
    {
      close (fd);
      fd = -1;
    }
  return (fd);
}

static int
mkparents(const char *path, mode_t mode)
{
  char *copy = xstrdup (path);
  const char *sep = path;
  int ret = 0;
  while ((sep = strchr (sep, '/')) != NULL)
    {
      if (sep == path)
	{
	  sep++;
	  continue;
	}
      copy[sep - path] = '\0';
      ret = mkdir (copy, mode);
      if (ret != 0 && errno != EEXIST)
	goto out;
      copy[sep - path] = path[sep - path];
      sep++;
    }
out:
  free(copy);
  return ret;
}

/* Look for a mapped symbol file that corresponds to FILENAME and is more
   recent than MTIME.  If MAPPED is nonzero, the user has asked that gdb
   use a mapped symbol file for this file, so create a new one if one does
   not currently exist.

   If found, then return an open file descriptor for the file, otherwise
   return -1.

   This routine is responsible for implementing the policy that generates
   the name of the mapped symbol file from the name of a file containing
   symbols that gdb would like to read.  Currently this policy is to append
   ".syms" to the name of the file.

   This routine is also responsible for implementing the policy that
   determines where the mapped symbol file is found (the search path).
   This policy is that when reading an existing mapped file, a file of
   the correct name in the current directory takes precedence over a
   file of the correct name in the same directory as the symbol file.
   When creating a new mapped file, it is always created in the current
   directory.  This helps to minimize the chances of a user unknowingly
   creating big mapped files in places like /bin and /usr/local/bin, and
   allows a local copy to override a manually installed global copy (in
   /bin for example).  */

static int
open_mapped_file (char *filename, struct stat *exesbuf, int flags)
{
  int fd;
  char *symsfilename;
  
  /* First try to open an existing file in the current directory, and
     then try the directory where the symbol file is located. */

  symsfilename = concat (auxillary_symbol_file_directory, filename, ".syms", (char *) NULL);
  fd = open_existing_mapped_file (symsfilename, exesbuf, flags);

  /* If we don't have an open file by now, then either the file does not
     already exist, or the base file has changed since it was created.  In
     either case, if the user has specified use of a mapped file, then
     create a new mapped file, truncating any existing one.  If we can't
     create one, print a system error message saying why we can't.

     By default the file is rw for everyone, with the user's umask taking
     care of turning off the permissions the user wants off. */

  if ((fd < 0) && (flags & OBJF_MAPPED))
    {
      mkparents (symsfilename, 0777);
      if ((fd = open (symsfilename, O_RDWR | O_CREAT | O_TRUNC, 0666)) < 0)
	{
	  if (error_pre_print)
	    {
	      printf_unfiltered ("%s", error_pre_print);
	    }
	  print_sys_errmsg (symsfilename, errno);
	}
    }

  xfree (symsfilename);
  return (fd);
}
static void *
map_to_file (int fd, int flags)
{
  void *md;
  void *mapto;
  struct stat sbuf;
  size_t size;  

  if (fstat (fd, &sbuf) < 0)
    {
      return (NULL);
    }
  else if (sbuf.st_size > 0)
    {
      md = mmalloc_reuse (fd, 1);
      if (md)
	{
	  size_t size = mmalloc_mapsize (md);
	  size_t reserve = size / 10;
	  if (size < (1 << 20))
	    size = 1 << 20; /* 1 MiB */
	  else if (size > (16 >> 20))
	    size = 16 << 20; /* 16 MiB */
	  mmalloc_reserve (md, size + reserve);
	}
      else
	warning (_("mmalloc_reuse failed"));
      return md;
    }

  /* This is a freshly created mapping file. */
  /* Find a large hole in the address space. */
  size = (sizeof(size_t) > 4) ? (64LL << 30 /* 64 GiB */) : (1 << 31 /* 1.5GiB */);
  for (mapto = NULL; mapto == NULL && size >= (1 << 20 /* 20Mb */); size -= (size / 4))
    mapto = mmalloc_findbase (size);
  if (mapto == NULL)
    error (_("mmalloc_findbase failed"));
  md = mmalloc_attach (fd, mapto);
  if (md == NULL)
    error (_("mmalloc_attach failed"));
  /* Reserve the hole for mmalloc so libc's malloc doesn't use it. */
  if (!mmalloc_reserve (md, size))
    error (_("mmalloc_reserve failed"));
  return (md);
}

#endif /* defined(USE_MMALLOC) && defined(HAVE_MMAP) */

/* Relocate OBJFILE to NEW_OFFSETS.  There should be OBJFILE->NUM_SECTIONS
   entries in new_offsets.  Process also OBJFILE's SEPARATE_DEBUG_OBJFILEs.

   The number and ordering of sections does differ between the two objfiles.
   Only their names match.  Also the file offsets will differ (objfile being
   possibly prelinked but separate_debug_objfile is probably not prelinked) but
   the in-memory absolute address as specified by NEW_OFFSETS must match both
   files.  */

void
objfile_relocate (struct objfile *objfile, struct section_offsets *new_offsets)
{
  struct objfile *debug_objfile;
  int changed = 0;

  changed |= objfile_relocate1 (objfile, new_offsets);

  for (debug_objfile = objfile->separate_debug_objfile;
       debug_objfile;
       debug_objfile = objfile_separate_debug_iterate (objfile, debug_objfile))
    {
      struct section_addr_info *objfile_addrs;
      struct section_offsets *new_debug_offsets;
      struct cleanup *my_cleanups;

      objfile_addrs = build_section_addr_info_from_objfile (objfile);
      my_cleanups = make_cleanup (xfree, objfile_addrs);

      /* Here OBJFILE_ADDRS contain the correct absolute addresses, the
	 relative ones must be already created according to debug_objfile.  */
      addr_info_make_relative (objfile_addrs, debug_objfile->obfd);

      gdb_assert (debug_objfile->num_sections
		  == bfd_count_sections (debug_objfile->obfd));
      new_debug_offsets = 
	xmalloc (SIZEOF_N_SECTION_OFFSETS (debug_objfile->num_sections));
      make_cleanup (xfree, new_debug_offsets);
      relative_addr_info_to_section_offsets (new_debug_offsets,
					     debug_objfile->num_sections,
					     objfile_addrs);

      changed |= objfile_relocate1 (debug_objfile, new_debug_offsets);

      do_cleanups (my_cleanups);
    }

  /* Relocate breakpoints as necessary, after things are relocated. */
  if (changed)
    breakpoint_re_set ();
}

/* Return non-zero if OBJFILE has partial symbols.  */

int
objfile_has_partial_symbols (struct objfile *objfile)
{
  return objfile->sf ? objfile->sf->qf->has_symbols (objfile) : 0;
}

/* Return non-zero if OBJFILE has full symbols.  */

int
objfile_has_full_symbols (struct objfile *objfile)
{
  return objfile->symtabs != NULL;
}

/* Return non-zero if OBJFILE has full or partial symbols, either directly
   or through a separate debug file.  */

int
objfile_has_symbols (struct objfile *objfile)
{
  struct objfile *o;

  for (o = objfile; o; o = objfile_separate_debug_iterate (objfile, o))
    if (objfile_has_partial_symbols (o) || objfile_has_full_symbols (o))
      return 1;
  return 0;
}


/* Many places in gdb want to test just to see if we have any partial
   symbols available.  This function returns zero if none are currently
   available, nonzero otherwise. */

int
have_partial_symbols (void)
{
  struct objfile_instance *ofp;

  ALL_OBJFILES (ofp)
  {
    if (objfile_has_partial_symbols (ofp->objfile))
      return 1;
  }
  return 0;
}

/* Many places in gdb want to test just to see if we have any full
   symbols available.  This function returns zero if none are currently
   available, nonzero otherwise. */

int
have_full_symbols (void)
{
  struct objfile_instance *ofp;

  ALL_OBJFILES (ofp)
  {
    if (objfile_has_full_symbols (ofp->objfile))
      return 1;
  }
  return 0;
}


/* This operations deletes all objfile entries that represent solibs that
   weren't explicitly loaded by the user, via e.g., the add-symbol-file
   command.
 */
void
objfile_purge_solibs (void)
{
  struct objfile_instance *objf;
  struct objfile_instance *temp;

  ALL_OBJFILES_SAFE (objf, temp)
  {
    /* We assume that the solib package has been purged already, or will
       be soon.
     */
    if (!(objf->instance_flags & OBJF_USERLOADED) && (objf->instance_flags & OBJF_SHARED))
      free_objfile_instance (objf);
  }
}


/* Many places in gdb want to test just to see if we have any minimal
   symbols available.  This function returns zero if none are currently
   available, nonzero otherwise. */

int
have_minimal_symbols (void)
{
  struct objfile_instance *ofp;

  ALL_OBJFILES (ofp)
  {
    if (ofp->objfile->minimal_symbol_count > 0)
      {
	return 1;
      }
  }
  return 0;
}

/* Qsort comparison function.  */

static int
qsort_cmp (const void *a, const void *b)
{
  const struct obj_section *sect1 = *(const struct obj_section **) a;
  const struct obj_section *sect2 = *(const struct obj_section **) b;
  const CORE_ADDR sect1_addr = obj_section_addr (sect1);
  const CORE_ADDR sect2_addr = obj_section_addr (sect2);

  if (sect1_addr < sect2_addr)
    return -1;
  else if (sect1_addr > sect2_addr)
    return 1;
  else
    {
      /* Sections are at the same address.  This could happen if
	 A) we have an objfile and a separate debuginfo.
	 B) we are confused, and have added sections without proper relocation,
	 or something like that. */

      const struct objfile *const objfile1 = sect1->objfile;
      const struct objfile *const objfile2 = sect2->objfile;

      if (objfile1->separate_debug_objfile == objfile2
	  || objfile2->separate_debug_objfile == objfile1)
	{
	  /* Case A.  The ordering doesn't matter: separate debuginfo files
	     will be filtered out later.  */

	  return 0;
	}

      /* Case B.  Maintain stable sort order, so bugs in GDB are easier to
	 triage.  This section could be slow (since we iterate over all
	 objfiles in each call to qsort_cmp), but this shouldn't happen
	 very often (GDB is already in a confused state; one hopes this
	 doesn't happen at all).  If you discover that significant time is
	 spent in the loops below, do 'set complaints 100' and examine the
	 resulting complaints.  */

      if (objfile1 == objfile2)
	{
	  /* Both sections came from the same objfile.  We are really confused.
	     Sort on sequence order of sections within the objfile.  */

	  const struct obj_section *osect;

	  ALL_OBJFILE_OSECTIONS (objfile1, osect)
	    if (osect == sect1)
	      return -1;
	    else if (osect == sect2)
	      return 1;

	  /* We should have found one of the sections before getting here.  */
	  gdb_assert (0);
	}
      else
	{
	  /* Sort on sequence number of the objfile in the chain.  */

	  const struct objfile_instance *objfile_instance;

	  ALL_OBJFILES (objfile_instance)
	    if (objfile_instance->objfile == objfile1)
	      return -1;
	    else if (objfile_instance->objfile == objfile2)
	      return 1;

	  /* We should have found one of the objfiles before getting here.  */
	  gdb_assert (0);
	}
    }

  /* Unreachable.  */
  gdb_assert (0);
  return 0;
}

/* Select "better" obj_section to keep.  We prefer the one that came from
   the real object, rather than the one from separate debuginfo.
   Most of the time the two sections are exactly identical, but with
   prelinking the .rel.dyn section in the real object may have different
   size.  */

static struct obj_section *
preferred_obj_section (struct obj_section *a, struct obj_section *b)
{
  gdb_assert (obj_section_addr (a) == obj_section_addr (b));
  gdb_assert ((a->objfile->separate_debug_objfile == b->objfile)
	      || (b->objfile->separate_debug_objfile == a->objfile));
  gdb_assert ((a->objfile->separate_debug_objfile_backlink == b->objfile)
	      || (b->objfile->separate_debug_objfile_backlink == a->objfile));

  if (a->objfile->separate_debug_objfile != NULL)
    return a;
  return b;
}

/* Return 1 if SECTION should be inserted into the section map.
   We want to insert only non-overlay and non-TLS section.  */

static int
insert_section_p (const struct bfd *abfd,
		  const struct bfd_section *section)
{
  const bfd_vma lma = bfd_section_lma (abfd, section);

  if (lma != 0 && lma != bfd_section_vma (abfd, section)
      && (bfd_get_file_flags (abfd) & BFD_IN_MEMORY) == 0)
    /* This is an overlay section.  IN_MEMORY check is needed to avoid
       discarding sections from the "system supplied DSO" (aka vdso)
       on some Linux systems (e.g. Fedora 11).  */
    return 0;
  if ((bfd_get_section_flags (abfd, section) & SEC_THREAD_LOCAL) != 0)
    /* This is a TLS section.  */
    return 0;

  return 1;
}

/* Filter out overlapping sections where one section came from the real
   objfile, and the other from a separate debuginfo file.
   Return the size of table after redundant sections have been eliminated.  */

static int
filter_debuginfo_sections (struct obj_section **map, int map_size)
{
  int i, j;

  for (i = 0, j = 0; i < map_size - 1; i++)
    {
      struct obj_section *const sect1 = map[i];
      struct obj_section *const sect2 = map[i + 1];
      const struct objfile *const objfile1 = sect1->objfile;
      const struct objfile *const objfile2 = sect2->objfile;
      const CORE_ADDR sect1_addr = obj_section_addr (sect1);
      const CORE_ADDR sect2_addr = obj_section_addr (sect2);

      if (sect1_addr == sect2_addr
	  && (objfile1->separate_debug_objfile == objfile2
	      || objfile2->separate_debug_objfile == objfile1))
	{
	  map[j++] = preferred_obj_section (sect1, sect2);
	  ++i;
	}
      else
	map[j++] = sect1;
    }

  if (i < map_size)
    {
      gdb_assert (i == map_size - 1);
      map[j++] = map[i];
    }

  /* The map should not have shrunk to less than half the original size.  */
  gdb_assert (map_size / 2 <= j);

  return j;
}

/* Filter out overlapping sections, issuing a warning if any are found.
   Overlapping sections could really be overlay sections which we didn't
   classify as such in insert_section_p, or we could be dealing with a
   corrupt binary.  */

static int
filter_overlapping_sections (struct obj_section **map, int map_size)
{
  int i, j;

  for (i = 0, j = 0; i < map_size - 1; )
    {
      int k;

      map[j++] = map[i];
      for (k = i + 1; k < map_size; k++)
	{
	  struct obj_section *const sect1 = map[i];
	  struct obj_section *const sect2 = map[k];
	  const CORE_ADDR sect1_addr = obj_section_addr (sect1);
	  const CORE_ADDR sect2_addr = obj_section_addr (sect2);
	  const CORE_ADDR sect1_endaddr = obj_section_endaddr (sect1);

	  gdb_assert (sect1_addr <= sect2_addr);

	  if (sect1_endaddr <= sect2_addr)
	    break;
	  else
	    {
	      /* We have an overlap.  Report it.  */

	      struct objfile *const objf1 = sect1->objfile;
	      struct objfile *const objf2 = sect2->objfile;

	      const struct bfd *const abfd1 = objf1->obfd;
	      const struct bfd *const abfd2 = objf2->obfd;

	      const struct bfd_section *const bfds1 = sect1->the_bfd_section;
	      const struct bfd_section *const bfds2 = sect2->the_bfd_section;

	      const CORE_ADDR sect2_endaddr = obj_section_endaddr (sect2);

	      struct gdbarch *const gdbarch = get_objfile_arch (objf1);

	      complaint (&symfile_complaints,
			 _("unexpected overlap between:\n"
			   " (A) section `%s' from `%s' [%s, %s)\n"
			   " (B) section `%s' from `%s' [%s, %s).\n"
			   "Will ignore section B"),
			 bfd_section_name (abfd1, bfds1), objf1->name,
			 paddress (gdbarch, sect1_addr),
			 paddress (gdbarch, sect1_endaddr),
			 bfd_section_name (abfd2, bfds2), objf2->name,
			 paddress (gdbarch, sect2_addr),
			 paddress (gdbarch, sect2_endaddr));
	    }
	}
      i = k;
    }

  if (i < map_size)
    {
      gdb_assert (i == map_size - 1);
      map[j++] = map[i];
    }

  return j;
}


/* Update PMAP, PMAP_SIZE with sections from all objfiles, excluding any
   TLS, overlay and overlapping sections.  */

static void
update_section_map (struct program_space *pspace,
		    struct obj_section ***pmap, int *pmap_size)
{
  int alloc_size, map_size, i;
  struct obj_section *s, **map;
  struct objfile_instance *objfile_instance;

  gdb_assert (get_objfile_pspace_data (pspace)->objfiles_changed_p != 0);

  map = *pmap;
  xfree (map);

  alloc_size = 0;
  ALL_PSPACE_OBJFILES (pspace, objfile_instance)
  {
    struct objfile *objfile = objfile_instance->objfile;
    ALL_OBJFILE_OSECTIONS (objfile, s)
      if (insert_section_p (objfile->obfd, s->the_bfd_section))
	alloc_size += 1;
  }

  /* This happens on detach/attach (e.g. in gdb.base/attach.exp).  */
  if (alloc_size == 0)
    {
      *pmap = NULL;
      *pmap_size = 0;
      return;
    }

  map = xmalloc (alloc_size * sizeof (*map));

  i = 0;
  ALL_PSPACE_OBJFILES (pspace, objfile_instance)
  {
    struct objfile *objfile = objfile_instance->objfile;
    ALL_OBJFILE_OSECTIONS (objfile, s)
      if (insert_section_p (objfile->obfd, s->the_bfd_section))
	map[i++] = s;
  }

  qsort (map, alloc_size, sizeof (*map), qsort_cmp);
  map_size = filter_debuginfo_sections(map, alloc_size);
  map_size = filter_overlapping_sections(map, map_size);

  if (map_size < alloc_size)
    /* Some sections were eliminated.  Trim excess space.  */
    map = xrealloc (map, map_size * sizeof (*map));
  else
    gdb_assert (alloc_size == map_size);

  *pmap = map;
  *pmap_size = map_size;
}

/* Bsearch comparison function. */

static int
bsearch_cmp (const void *key, const void *elt)
{
  const CORE_ADDR pc = *(CORE_ADDR *) key;
  const struct obj_section *section = *(const struct obj_section **) elt;

  if (pc < obj_section_addr (section))
    return -1;
  if (pc < obj_section_endaddr (section))
    return 0;
  return 1;
}

/* Returns a section whose range includes PC or NULL if none found.   */

struct obj_section *
find_pc_section (CORE_ADDR pc)
{
  struct objfile_pspace_info *pspace_info;
  struct obj_section *s, **sp;

  /* Check for mapped overlay section first.  */
  s = find_pc_mapped_section (pc);
  if (s)
    return s;

  pspace_info = get_objfile_pspace_data (current_program_space);
  if (pspace_info->objfiles_changed_p != 0)
    {
      update_section_map (current_program_space,
			  &pspace_info->sections,
			  &pspace_info->num_sections);

      /* Don't need updates to section map until objfiles are added,
         removed or relocated.  */
      pspace_info->objfiles_changed_p = 0;
    }

  /* The C standard (ISO/IEC 9899:TC2) requires the BASE argument to
     bsearch be non-NULL.  */
  if (pspace_info->sections == NULL)
    {
      gdb_assert (pspace_info->num_sections == 0);
      return NULL;
    }

  sp = (struct obj_section **) bsearch (&pc,
					pspace_info->sections,
					pspace_info->num_sections,
					sizeof (*pspace_info->sections),
					bsearch_cmp);
  if (sp != NULL)
    return *sp;
  return NULL;
}


/* In SVR4, we recognize a trampoline by it's section name. 
   That is, if the pc is in a section named ".plt" then we are in
   a trampoline.  */

int
in_plt_section (CORE_ADDR pc, char *name)
{
  struct obj_section *s;
  int retval = 0;

  s = find_pc_section (pc);

  retval = (s != NULL
	    && s->the_bfd_section->name != NULL
	    && strcmp (s->the_bfd_section->name, ".plt") == 0);
  return (retval);
}


/* Keep a registry of per-objfile data-pointers required by other GDB
   modules.  */

struct objfile_data
{
  unsigned index;
  void (*save) (struct objfile *, void *);
  void (*free) (struct objfile *, void *);
  void (*restore) (struct objfile *, void *);
};

struct objfile_data_registration
{
  struct objfile_data *data;
  struct objfile_data_registration *next;
};
  
struct objfile_data_registry
{
  struct objfile_data_registration *registrations;
  unsigned num_registrations;
};

static struct objfile_data_registry objfile_data_registry = { NULL, 0 };

const struct objfile_data *
register_objfile_data_with_cleanup_and_restore (void (*save) (struct objfile *, void *),
				                void (*free) (struct objfile *, void *),
						void (*restore) (struct objfile *, void *))
{
  struct objfile_data_registration **curr;

  /* Append new registration.  */
  for (curr = &objfile_data_registry.registrations;
       *curr != NULL; curr = &(*curr)->next);

  *curr = XMALLOC (struct objfile_data_registration);
  (*curr)->next = NULL;
  (*curr)->data = XMALLOC (struct objfile_data);
  (*curr)->data->index = objfile_data_registry.num_registrations++;
  (*curr)->data->save = save;
  (*curr)->data->free = free;
  (*curr)->data->restore = restore;

  return (*curr)->data;
}

const struct objfile_data *
register_objfile_data_with_cleanup (void (*save) (struct objfile *, void *),
				    void (*free) (struct objfile *, void *))
{
  return register_objfile_data_with_cleanup_and_restore (save, free, NULL);
}

const struct objfile_data *
register_objfile_data (void)
{
  return register_objfile_data_with_cleanup (NULL, NULL);
}

static void
objfile_alloc_data (struct objfile *objfile)
{
  gdb_assert (objfile->data == NULL);
  objfile->num_data = objfile_data_registry.num_registrations;
  objfile->data = XMCALLOC (objfile->md, objfile->num_data, void *);
}

static void
objfile_free_data (struct objfile *objfile)
{
  gdb_assert (objfile->data != NULL);
  clear_objfile_data (objfile);
  xmfree (objfile->md, objfile->data);
  objfile->data = NULL;
}

void
clear_objfile_data (struct objfile *objfile)
{
  struct objfile_data_registration *registration;
  int i;

  gdb_assert (objfile->data != NULL);

  /* Process all the save handlers.  */

  for (registration = objfile_data_registry.registrations, i = 0;
       i < objfile->num_data;
       registration = registration->next, i++)
    if (objfile->data[i] != NULL && registration->data->save != NULL)
      registration->data->save (objfile, objfile->data[i]);

  /* Now process all the free handlers.  */

  for (registration = objfile_data_registry.registrations, i = 0;
       i < objfile->num_data;
       registration = registration->next, i++)
    if (objfile->data[i] != NULL && registration->data->free != NULL)
      registration->data->free (objfile, objfile->data[i]);

  memset (objfile->data, 0, objfile->num_data * sizeof (void *));
}

void
restore_objfile_data (struct objfile *objfile)
{
  struct objfile_data_registration *registration;
  int i;

  gdb_assert (objfile->data != NULL);

  /* Process all the restore handlers.  */

  for (registration = objfile_data_registry.registrations, i = 0;
       i < objfile->num_data;
       registration = registration->next, i++)
    {
      if (registration->data->restore == reset_objfile_data)
	objfile->data[i] = NULL;
      else if (registration->data->restore != NULL && objfile->data[i] != NULL)
	registration->data->restore (objfile, objfile->data[i]);
    }
}

void
set_objfile_data (struct objfile *objfile, const struct objfile_data *data,
		  void *value)
{
  gdb_assert (data->index < objfile->num_data);
  objfile->data[data->index] = value;
}

void *
objfile_data (struct objfile *objfile, const struct objfile_data *data)
{
  gdb_assert (data->index < objfile->num_data);
  return objfile->data[data->index];
}

void
reset_objfile_data (struct objfile *objfile, void *value)
{
}

/* Set objfiles_changed_p so section map will be rebuilt next time it
   is used.  Called by reread_symbols.  */

void
objfiles_changed (void)
{
  /* Rebuild section map next time we need it.  */
  get_objfile_pspace_data (current_program_space)->objfiles_changed_p = 1;
}

/* Close ABFD, and warn if that fails.  */

int
gdb_bfd_close_or_warn (struct bfd *abfd)
{
  int ret;
  char *name = bfd_get_filename (abfd);

  ret = bfd_close (abfd);

  if (!ret)
    warning (_("cannot close \"%s\": %s"),
	     name, bfd_errmsg (bfd_get_error ()));

  return ret;
}

/* Add reference to ABFD.  Returns ABFD.  */
struct bfd *
gdb_bfd_ref (struct bfd *abfd)
{
  int *p_refcount;

  if (abfd == NULL)
    return NULL;

  p_refcount = bfd_usrdata (abfd);

  if (p_refcount != NULL)
    {
      *p_refcount += 1;
      return abfd;
    }

  p_refcount = xmalloc (sizeof (*p_refcount));
  *p_refcount = 1;
  bfd_usrdata (abfd) = p_refcount;

  return abfd;
}

/* Unreference and possibly close ABFD.  */
void
gdb_bfd_unref (struct bfd *abfd)
{
  int *p_refcount;
  char *name;

  if (abfd == NULL)
    return;

  p_refcount = bfd_usrdata (abfd);

  /* Valid range for p_refcount: a pointer to int counter, which has a
     value of 1 (single owner) or 2 (shared).  */
  gdb_assert (*p_refcount == 1 || *p_refcount == 2);

  *p_refcount -= 1;
  if (*p_refcount > 0)
    return;

  xfree (p_refcount);
  bfd_usrdata (abfd) = NULL;  /* Paranoia.  */

  name = bfd_get_filename (abfd);
  gdb_bfd_close_or_warn (abfd);
  xfree (name);
}

#if defined(USE_MMALLOC) && defined(HAVE_MMAP)

static void
show_mapped_symbol_files (struct ui_file *file, int from_tty,
			  struct cmd_list_element *c, const char *value)
{
  fprintf_filtered (file, _("\
Creating auxillary symbol files is %s.\n"), value);
}

#endif

/* Provide a prototype to silence -Wmissing-prototypes.  */
extern initialize_file_ftype _initialize_objfiles;

void
_initialize_objfiles (void)
{
  objfiles_pspace_data
    = register_program_space_data_with_cleanup (objfiles_pspace_data_cleanup);

#if defined(USE_MMALLOC) && defined(HAVE_MMAP)

  add_setshow_filename_cmd ("mapped-directory", class_files,
			    &auxillary_symbol_file_directory, _("\
Set the auxillary symbol file directory."), _("\
Show the current auxillary symbol file directory."), _("\
GDB looks for auxillary symbol files in this directory.\n\
You can set the directory using\n\
`set auxillary-symbol-file-directory'."),
			    NULL,
			    NULL,
			    &setlist, &showlist);

  add_setshow_boolean_cmd ("mapped", class_files,
			   &mapped_symbol_files, _("\
Set whether auxillary symbol files will be created."), _("\
Show whether auxillary symbol files will be created."), _("\
When non-zero, GDB writes the symbols for your program into a reusable file.\n\
Future GDB debugging sessions map in symbol information from this auxiliary\n\
symbol file (if the program has not changed), rather than spending time\n\
reading the symbol table from the executable program. "),
			    NULL,
			    show_mapped_symbol_files,
			    &setlist, &showlist);

  auxillary_symbol_file_directory = xstrdup(".");
#endif
}
