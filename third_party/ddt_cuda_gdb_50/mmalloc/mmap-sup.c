/* Support for an sbrk-like function that uses mmap.
   Copyright 1992, 2000 Free Software Foundation, Inc.

   Contributed by Fred Fish at Cygnus Support.   fnf@cygnus.com

This file is part of the GNU C Library.

The GNU C Library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public License as
published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

The GNU C Library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with the GNU C Library; see the file COPYING.LIB.  If
not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#if defined(HAVE_MMAP)

#ifdef HAVE_UNISTD_H
#include <unistd.h>	/* Prototypes for lseek */
#endif
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>

#ifndef SEEK_SET
#define SEEK_SET 0
#endif
#ifndef MAP_FILE
#define MAP_FILE 0
#endif

#include "mmprivate.h"

/* Cache the pagesize for the current host machine.  Note that if the host
   does not readily provide a getpagesize() function, we need to emulate it
   elsewhere, not clutter up this file with lots of kluges to try to figure
   it out. */

static size_t pagesize;
#if NEED_DECLARATION_GETPAGESIZE
extern int getpagesize PARAMS ((void));
#endif

#define PAGE_ALIGN(addr) (caddr_t) (((long)(addr) + pagesize - 1) & \
				    ~(pagesize - 1))

/* Return MAP_PRIVATE if MDP represents /dev/zero.  Otherwise, return
   MAP_SHARED.  */

#define MAP_PRIVATE_OR_SHARED(MDP) ((MDP -> flags & MMALLOC_PRIVATE) \
                                    ? MAP_PRIVATE \
                                    : MAP_SHARED)

#ifdef MAP_ANONYMOUS
#define MAP_FILE_OR_ANONYMOUS(MDP) ((MDP->flags & MMALLOC_PRIVATE) \
				    ? MAP_ANONYMOUS \
				    : MAP_FILE)
#else
#define MAP_FILE_OR_ANONYMOUS(MDP) (MAP_FILE)
#endif


/*  Get core for the memory region specified by MDP, using SIZE as the
    amount to either add to or subtract from the existing region.  Works
    like sbrk(), but using mmap(). */

PTR
__mmalloc_mmap_morecore (mdp, size)
  struct mdesc *mdp;
  ptrdiff_t size;
{
  PTR result = NULL;
  off_t foffset;	/* File offset at which new mapping will start */
  size_t mapbytes;	/* Number of bytes to map */
  caddr_t moveto;	/* Address where we wish to move "break value" to */
  caddr_t mapto;	/* Address we actually mapped to */
  char buf = 0;		/* Single byte to write to extend mapped file */

  if (pagesize == 0)
    {
      pagesize = getpagesize ();
    }
  if (size == 0)
    {
      /* Just return the current "break" value. */
      result = mdp -> breakval;
    }
  else if (size < 0)
    {
      /* We are deallocating memory.  If the amount requested would cause
	 us to try to deallocate back past the base of the mmap'd region
	 then do nothing, and return NULL.  Otherwise, deallocate the
	 memory and return the old break value. */
      if (mdp -> breakval + size >= mdp -> base)
	{
	  result = (PTR) mdp -> breakval;
	  mdp -> breakval += size;
	  moveto = PAGE_ALIGN (mdp -> breakval);
	  munmap (moveto, (size_t) (mdp -> top - moveto));
	  if (mdp->reserve_top > moveto)
	    __mmalloc_reserve (moveto, mdp->top - moveto);
	  mdp -> top = moveto;
	}
    }
  else
    {
      /* We are allocating memory.  Make sure we have an open file
	 descriptor and then go on to get the memory. */
      if (mdp -> breakval + size > mdp -> top)
	{
	  /* The request would move us past the end of the currently
	     mapped memory, so map in enough more memory to satisfy
	     the request.  This means we also have to grow the mapped-to
	     file by an appropriate amount, since mmap cannot be used
	     to extend a file. */
	  moveto = PAGE_ALIGN (mdp -> breakval + size);
	  mapbytes = moveto - mdp -> top;
	  foffset = mdp -> top - mdp -> base;
	  if (!(mdp->flags & MMALLOC_PRIVATE))
	    {
	      /* FIXME:  Test results of lseek() and write() */
	      lseek (mdp -> fd, foffset + mapbytes - 1, SEEK_SET);
	      write (mdp -> fd, &buf, 1);
	    }
	  if (mdp -> base == 0)
	    {
	      /* Let mmap pick the map start address */
	      mapto = mmap (0, mapbytes, PROT_READ | PROT_WRITE,
			    MAP_PRIVATE_OR_SHARED (mdp)
			      | MAP_FILE_OR_ANONYMOUS (mdp),
			    mdp -> fd, foffset);
	      if (mapto != (caddr_t) -1)
		{
		  mdp -> base = mdp -> breakval = mapto;
		  mdp -> top = mdp -> base + mapbytes;
		  result = (PTR) mdp -> breakval;
		  mdp -> breakval += size;
		}
	    }
	  else
	    {
	      mapto = mmap (mdp -> top, mapbytes, PROT_READ | PROT_WRITE,
			    MAP_PRIVATE_OR_SHARED (mdp)
			      | MAP_FILE_OR_ANONYMOUS(mdp)
			      | MAP_FIXED,
			    mdp -> fd, foffset);
	      if (mapto == mdp -> top)
		{
		  mdp -> top = moveto;
		  result = (PTR) mdp -> breakval;
		  mdp -> breakval += size;
		}
	    }
	}
      else
	{
	  result = (PTR) mdp -> breakval;
	  mdp -> breakval += size;
	}
    }
  return (result);
}

PTR
__mmalloc_remap_core (mdp)
  struct mdesc *mdp;
{
  caddr_t base;

  /* FIXME:  Quick hack, needs error checking and other attention. */

  base = mmap (mdp -> base, mdp -> top - mdp -> base,
	       PROT_READ | PROT_WRITE,
	       MAP_PRIVATE_OR_SHARED (mdp) | MAP_FILE,
	       mdp -> fd, 0);
  return ((PTR) base);
}

PTR
mmalloc_findbase (size)
  size_t size;
{
  int fd;
  int flags;
  caddr_t base = NULL;

#ifdef MAP_ANONYMOUS
  flags = MAP_PRIVATE | MAP_ANONYMOUS;
  fd = -1;
#else
  flags = MAP_PRIVATE | MAP_FILE;
  fd = open ("/dev/zero", O_RDWR);
  if (fd == -1)
    {
      return ((PTR) NULL);
    }
#endif
  base = mmap (0, size, PROT_READ | PROT_WRITE, flags, fd, 0);
  if (base != (caddr_t) -1)
    {
      munmap (base, (size_t) size);
    }
  if (fd != -1)
    {
      close (fd);
    }
  if (base == 0)
    {
      /* Don't allow mapping at address zero.  We use that value
	 to signal an error return, and besides, it is useful to
	 catch NULL pointers if it is unmapped.  Instead start
	 at the next page boundary. */
      base = (caddr_t) getpagesize ();
    }
  else if (base == (caddr_t) -1)
    {
      base = NULL;
    }
  return ((PTR) base);
}

int
__mmalloc_reserve (base, size)
  PTR base;
  size_t size;
{
  int fd;
  int flags;
  caddr_t ret = NULL;
#ifdef MAP_ANONYMOUS
  flags = MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED;
  fd = -1;
#else
  flags = MAP_PRIVATE | MAP_FILE | MAP_FIXED;
  fd = open ("/dev/zero", O_RDWR);
  if (fd == -1)
    {
      return 0;
    }
#endif
#ifdef MAP_NORESERVE
  flags |= MAP_NORESERVE;
#endif

  ret = mmap (base, size, PROT_NONE, flags, fd, 0);
  if (fd != -1)
    {
      close (fd);
    }
  return (base == ret);
}

int
mmalloc_reserve (md, size)
  PTR md;
  size_t size;
{
  struct mdesc *mdp;
  caddr_t moveto;
  caddr_t mapto;
  int ret;

  mdp = MD_TO_MDP (md);

  moveto = mdp->base + size;
  mapto = MAX (moveto, mdp->top);

  if (mapto > mdp->top)
    {
      ret = __mmalloc_reserve (mdp->top, mapto - mdp->top);
    }
  else if (mapto < mdp->reserve_top)
    {
      ret = (munmap (mdp->top, mdp->reserve_top - mapto) == 0);
    }
  if (ret)
    mdp->reserve_top = moveto;
  return ret;
}

int
mmalloc_makeprivate (md)
  PTR md;
{
  struct mdesc *mdp;
  int fd;
  
  mdp = MD_TO_MDP (md);
  
  if (!(mdp->flags & MMALLOC_PRIVATE))
    {
      fd = mdp->fd;
      mmalloc_detach (md);
      return mmalloc_reuse (fd, 1) != NULL;
    }
  return 1;
}

size_t
mmalloc_mapsize (md)
  PTR md;
{
  struct mdesc *mdp;
  
  mdp = MD_TO_MDP (md);
  
  return mdp->top - mdp->base;
}

#else	/* defined(HAVE_MMAP) */

int
mmalloc_reserve (md, size)
  PTR md;
  size_t size;
{
  return 0;
}

int
mmalloc_makeprivate (md)
  PTR md;
{
  return 0;
}

size_t
mmalloc_mapsize (md)
  PTR md;
{
  return 0;
}

#endif	/* defined(HAVE_MMAP) */
