/* UDA plugin shared memory operations.

Copyright 2007 by Intrepid Technology, Inc.
Copyright 2007 by Hewlett-Packard.

Written by; Gary Funck <gary@intrepid.com>
Reviewed by: Brian Wibecan and Tanya Klinchina, HP

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

(1) Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
(2) Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.
(3) Neither the name of Intrepid Technology, or Hewlett-Packard
nor the names of its contributors may be used to endorse or promote
products derived from this software without specific prior written
permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  */

/*
 * Copyright 2013 Cray Inc. All Rights Reserved.
 */

#include <assert.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "uda-types.h"
#include "uda-defs.h"
#include "uda-plugin-config.h"
#include "uda-plugin-types.h"
#include "uda-plugin-defs.h"
#include "uda-plugin.h"
#include "uda-plugin-cb.h"
#include "uda-plugin-utils.h"

#define CRAY_CCE_UPC // RWM

/* Translate a shared address in (addrfield, thread)
   form into an address that is present in the thread.
   In GCC/UPC, this is useful only if the named thread
   has mapped the shared address that it has affinity to
   into its local address space.  In a more general implemenation,
   we should map the shared address into an offset into the
   global shared memory region, and access the memory there.
   Note that vaddr_offset  must be a zero-based offset into the
   thread's contribution to the shared address space.  */

int
uda_plugin_shared_to_local_addr (uda_job_t *job,
                                 const uda_taddr_t vaddr_offset,
				 const uda_tword_t thread_num,
			         uda_taddr_t *local_addr)
{
  int status;
  uda_job_info_t *job_info;
  uda_image_t *image;
  uda_image_info_t *im_info;
  uda_thread_t *thread;
  uda_thread_info_t *thr_info;
  uda_tword_t page_num, page_shift;
  uda_tword_t page_base, page_offset, offset_mask;
  uda_tword_t ptr_size;
  uda_taddr_t cur_page_alloc;

  db_job_get_thread (job, thread_num, &thread);
  db_job_get_info (job, &job_info);
  db_job_get_image (job, &image);
  db_image_get_info (image, &im_info);
  thr_info = uda_plugin_thread_get_info (thread);
  cur_page_alloc = thr_info->cur_page_alloc;
  ptr_size = im_info->type_sizes.pointer_size;
  page_shift = im_info->page_shift;
  page_num = vaddr_offset >> page_shift;

#ifdef CRAY_CCE_UPC // RWM Can print "shared int" etc
  *local_addr = vaddr_offset;
  UDA_PLUGIN_TRACE (("shmem: %lx", (long) *local_addr));
  return uda_ok;
#endif

  if (page_num >= cur_page_alloc)
  //if (0)
    {
      uda_taddr_t new_page_alloc;
      /* Get the latest number of pages allocated.  */
      if (uda_plugin_lookup_and_read_scalar (thread, "__upc_cur_page_alloc",
					     im_info->type_sizes.int_size,
					     &new_page_alloc))
	{
	  db_prints ("can't read __upc_cur_page_alloc value\n");
	  return uda_read_failed;
	}
      /* Check again.  */
      if (page_num >= new_page_alloc)
        {
	  uda_plugin_dprintf ("page number: %lu exceeds allocation: %lu\n",
	                      (long unsigned) page_num,
			      (long unsigned) new_page_alloc);
	  return uda_read_failed;
        }
      /* If current page alloc exceeds previous, read in additional
         LPT entries.  */
      if (new_page_alloc > cur_page_alloc)
        {
          uda_tword_t *old_lpt = thr_info->lpt;
          uda_tword_t p;
	  uda_taddr_t lpt_entry_addr;
          thr_info->lpt = db_malloc ((size_t) new_page_alloc
                                      * sizeof (uda_taddr_t));
          if (old_lpt)
	    {
              memcpy (thr_info->lpt, old_lpt,
	              cur_page_alloc * sizeof (uda_taddr_t));
	      db_free (old_lpt);
	    }
          lpt_entry_addr = thr_info->lpt_base
	                   + (cur_page_alloc * ptr_size);
          for (p = cur_page_alloc; p < new_page_alloc; ++p)
            {
              status = uda_plugin_read_scalar (thread, lpt_entry_addr,
				               ptr_size, &thr_info->lpt[p]);
	      if (status != uda_ok)
		{
		  db_prints ("can't read lpt entry\n");
		  return uda_read_failed;
		}
              lpt_entry_addr += ptr_size;
            }
          thr_info->cur_page_alloc = new_page_alloc;
        }
    }
  page_base = thr_info->lpt[page_num];
  offset_mask = (((uda_taddr_t) 1) << page_shift) - 1;
  page_offset = (vaddr_offset & offset_mask);
  *local_addr = page_base + page_offset;
  UDA_PLUGIN_TRACE (("shmem: %lx", (long) *local_addr));
  return uda_ok;
}

int
uda_plugin_shmem_get (uda_thread_t *thread,
                      void *to,
		      const uda_taddr_t from, 
                      const uda_tword_t thread_num,
		      const uda_tword_t length)
{
  int status;
  char *dest = (char *)to;
  uda_taddr_t src_addr =  from;
  uda_tword_t n = length;
  uda_job_t *job;
  uda_job_info_t *job_info;
  uda_thread_info_t *thr_info;
  uda_image_t *image;
  uda_image_info_t *im_info;
  uda_taddr_t shared_start;
  uda_tword_t page_size, offset_mask;

  // RWM
  UDA_PLUGIN_TRACE (("enter"));

  db_thread_get_job (thread, &job);
  thr_info = uda_plugin_thread_get_info (thread);
  db_job_get_info (job, &job_info);
  db_job_get_image (job, &image);
  db_image_get_info (image, &im_info);
  shared_start = im_info->shared_start;
  page_size = ((uda_tword_t)1 << im_info->page_shift);
  offset_mask = (page_size - 1);
  for (;;)
    {
      uda_tword_t offset = (uda_tword_t) (src_addr - shared_start);
      uda_tword_t p_offset = (offset & offset_mask);
      uda_tword_t n_copy = min (page_size - p_offset, n);
      uda_taddr_t local_addr;
      status = uda_plugin_shared_to_local_addr (job, offset,
                                                thread_num, &local_addr);
      if (status != uda_ok)
        return uda_read_failed;
      status = db_read_store (thread, local_addr, n_copy, dest);
      if (status != uda_ok)
        return uda_read_failed;
      n -= n_copy;
      if (!n)
        break;
      src_addr += n_copy;
      dest += n_copy;
    }
  return uda_ok;
}

int
uda_plugin_shmem_put (uda_thread_t *thread,
                      uda_taddr_t to,
		      const void *from, 
                      const uda_tword_t thread_num,
		      const uda_tword_t length)
{
  int status;
  char *src = (char *)from;
  uda_taddr_t dest_addr = to;
  uda_tword_t n = length;
  uda_tword_t n_written;
  uda_job_t *job;
  uda_job_info_t *job_info;
  uda_thread_info_t *thr_info;
  uda_image_t *image;
  uda_image_info_t *im_info;
  uda_taddr_t shared_start;
  uda_tword_t page_size, offset_mask;

  // RWM
  UDA_PLUGIN_TRACE (("enter"));
  db_thread_get_job (thread, &job);
  thr_info = uda_plugin_thread_get_info (thread);
  db_job_get_info (job, &job_info);
  db_job_get_image (job, &image);
  db_image_get_info (image, &im_info);
  shared_start = im_info->shared_start;
  page_size = ((uda_tword_t)1 << im_info->page_shift);
  offset_mask = (page_size - 1);
  for (;;)
    {
      uda_tword_t offset = (uda_tword_t) (dest_addr - shared_start);
      uda_tword_t p_offset = (offset & offset_mask);
      uda_tword_t n_copy = min (page_size - p_offset, n);
      uda_taddr_t local_addr;
      status = uda_plugin_shared_to_local_addr (job, offset,
                                                thread_num, &local_addr);
      if (status != uda_ok)
        return uda_write_failed;
      status = db_write_store (thread, local_addr, n_copy, &n_written, src);
      if (status != uda_ok)
        return uda_write_failed;
      n -= n_copy;
      if (!n)
        break;
      dest_addr += n_copy;
      src += n_copy;
    }
  return uda_ok;
}

int
uda_read_upc_shared_mem (uda_thread_t *thread,
                         const uda_taddr_t addrfield,
			 uda_tword_t *actual_length,
		         const uda_tword_t length,
		         void *bytes)
{
  int status;
  int thread_num;

  // RWM
  UDA_PLUGIN_TRACE (("enter"));
  status = uda_get_threadno (thread, &thread_num);
  if (status != uda_ok)
    return status;
  UDA_PLUGIN_TRACE (("addrfield=%lx, thread=%ld length=%ld",
		  (unsigned long) addrfield, (long) thread_num,
		  (long) length));
  *actual_length = 0;
  status = uda_plugin_shmem_get (thread, bytes, addrfield, thread_num, length);
  if (status == uda_ok)
    *actual_length = length;
  UDA_PLUGIN_TRACE (("return actual_length=%ld", (long) *actual_length));
  return status;
}

int
uda_write_upc_shared_mem (uda_thread_t *thread,
			  const uda_taddr_t addrfield,
			  const uda_tword_t length,
			  uda_tword_t *bytes_written,
			  void *bytes)
{
  int status;
  int thread_num;

  // RWM
  UDA_PLUGIN_TRACE (("enter"));
  status = uda_get_threadno (thread, &thread_num);
  if (status != uda_ok)
    return status;
  UDA_PLUGIN_TRACE (("addrfield=%lx, thread=%ld length=%ld",
		  (unsigned long) addrfield, (long) thread_num,
		  (long) length));
  status = uda_plugin_shmem_put (thread, addrfield, bytes, thread_num, length);
  *bytes_written = length;
  UDA_PLUGIN_TRACE (("exit"));
  return status;
}
