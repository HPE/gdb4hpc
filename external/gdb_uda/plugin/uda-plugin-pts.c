/* UDA plugin point-to-shared operations.

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


/* Unpacks the target representation into its component parts.  */

static
int
uda_plugin_extract_shared_ptr (uda_thread_t *thread,
		               const uda_target_pts_t *packed_pts,
                               uda_debugger_pts_t *pts)
{
  int status;
  uda_job_t *job;
  uda_image_t *image;
  uda_image_info_t *im_info;
  uda_taddr_t addrfield_value, phase_value, thread_value;

  // RWM
  UDA_PLUGIN_TRACE (("begin"));
  memset (pts, '\0', sizeof (*pts));
  db_thread_get_job (thread, &job);
  db_job_get_image (job, &image);
  db_image_get_info (image, &im_info);
  status = uda_plugin_extract_bit_field (thread,
                                         sizeof (uda_target_pts_t), packed_pts,
                                         im_info->addrfield_bit_offset,
                                         im_info->addrfield_bit_length,
					 &addrfield_value);
  if (status != uda_ok)
    return status;
  status = uda_plugin_extract_bit_field (thread,
                                         sizeof (uda_target_pts_t), packed_pts,
                                         im_info->thread_bit_offset,
                                         im_info->thread_bit_length,
					 &thread_value);
  if (status != uda_ok)
    return status;
  status = uda_plugin_extract_bit_field (thread,
                                         sizeof (uda_target_pts_t), packed_pts,
                                         im_info->phase_bit_offset,
                                         im_info->phase_bit_length,
					 &phase_value);
  if (status != uda_ok)
    return status;
  pts->addrfield = addrfield_value;
#ifndef STRUCT_SPTR_32BIT
  /* for packed shared pointer add shared start */
  if (im_info->pts_len == 8)
    pts->addrfield += im_info->shared_start;
#endif
  pts->thread = thread_value;
  pts->phase = phase_value;
  return uda_ok;
}

/* Packs the debugger's representation of a shared
   pointer into its target representation.  */

static
void
uda_plugin_make_shared_ptr (uda_thread_t *thread,
                            const uda_debugger_pts_t *pts,
			    size_t *packed_pts_len,
		            uda_target_pts_t *packed_pts)
{
  uda_job_t *job;
  uda_image_t *image;
  uda_image_info_t *im_info;

  // RWM
  UDA_PLUGIN_TRACE (("begin"));
  memset (packed_pts, '\0', sizeof (uda_target_pts_t));
  db_thread_get_job (thread, &job);
  db_job_get_image (job, &image);
  db_image_get_info (image, &im_info);
  *packed_pts_len = im_info->pts_len;
  uda_plugin_insert_bit_field (thread,
#ifdef STRUCT_SPTR_32BIT
                               pts->addrfield,
#else
                               (im_info->pts_len == 8) ? 
			          (pts->addrfield-im_info->shared_start) : pts->addrfield,
#endif
                               im_info->addrfield_bit_offset,
                               im_info->addrfield_bit_length,
                               sizeof (uda_target_pts_t),
			       packed_pts);
  uda_plugin_insert_bit_field (thread,
                               pts->thread,
                               im_info->thread_bit_offset,
                               im_info->thread_bit_length,
                               sizeof (uda_target_pts_t),
			       packed_pts);
  uda_plugin_insert_bit_field (thread,
                               pts->phase,
                               im_info->phase_bit_offset,
                               im_info->phase_bit_length,
                               sizeof (uda_target_pts_t),
			       packed_pts);
}

/*  Return the size of a shared pointer for this target.  */
int
uda_length_of_pts (uda_image_t *image, uda_tword_t block_size,
		   uda_tword_t *res)
{
  uda_image_info_t *im_info;
  UDA_PLUGIN_TRACE (("block_size=%ld", (long) block_size));
  db_image_get_info (image, &im_info);
  *res = im_info->pts_len;
  UDA_PLUGIN_TRACE (("sizeof sptr=%ld", (long) *res));
  return uda_ok;
}

/* Determine if the target PTS representation has
   any opaque data.  */
int
uda_show_opaque (uda_image_t * thread, uda_tword_t block_size,
		 uda_tword_t target_length)
{
  return uda_no_information;
}

/* Unpack the target representation of a PTS into the representation
   used by the debugger. */

int
uda_unpack_pts (uda_thread_t *thread,
		const uda_target_pts_t *packed_pts,
		const uda_tword_t block_size,
		uda_debugger_pts_t *pts)
{
  int status;
  UDA_PLUGIN_TRACE (("begin"));
  status = uda_plugin_extract_shared_ptr (thread, packed_pts, pts);
  if (status != uda_ok)
    {
      db_prints ("Unpack PTS failed.\n");
      return status;
    }
  UDA_PLUGIN_TRACE (("addrfield=%lx,thread=%lx,phase=%lx,opaque=%lx",
                     (long) pts->addrfield, (long) pts->thread,
		     (long) pts->phase, (long)pts->opaque));
  return uda_ok;
}

/* Pack the debugger's representation of a PTS into
   the target's representation.  */
int
uda_pack_pts (uda_thread_t *thread,
	      const uda_debugger_pts_t *pts,
	      const uda_tword_t block_size,
	      size_t *packed_pts_len,
	      uda_target_pts_t *packed_pts)
{
  UDA_PLUGIN_TRACE (("begin"));
  uda_plugin_make_shared_ptr (thread, pts, packed_pts_len,
		              packed_pts);
  UDA_PLUGIN_TRACE (("end"));
  return uda_ok;
}

/* Convert a PTS into an absolute address.  */
int
uda_pts_to_addr (uda_thread_t *thread,
		 const uda_debugger_pts_t *pts,
		 uda_tword_t block_size,
		 uda_tword_t element_size,
		 uda_taddr_t *local_addr)
{
  int status;
  const uda_tword_t thread_num = pts->thread;
  uda_taddr_t shared_start, offset;
  uda_job_t *job;
  uda_image_t *image;
  uda_image_info_t *im_info;
  db_thread_get_job (thread, &job);
  db_job_get_image (job, &image);
  db_image_get_info (image, &im_info);
  UDA_PLUGIN_TRACE (("addr=0x%lx,thread=%lu,block_size=%lu,element_size=%lu",
		  (unsigned long) pts->addrfield, (long) thread_num,
		  (unsigned long) block_size, (unsigned long) element_size));
  shared_start = im_info->shared_start;
  offset = pts->addrfield - shared_start;
  *local_addr = 0;
  status = uda_plugin_shared_to_local_addr (job, offset,
                                            thread_num, local_addr);
  if (status != uda_ok)
    {
      db_prints ("PTS to local address failed.\n");
      return status;
    }
  UDA_PLUGIN_TRACE (("addr=0x%lx", (long) *local_addr));
  return uda_ok;
}

/* The heart of UPC, index a shared pointer.  */
int
uda_index_pts (uda_thread_t *thread,
	       const uda_debugger_pts_t *p,
	       const uda_tint_t delta,
	       const uda_tword_t element_size,
	       const uda_tword_t block_size,
	       const uda_tword_t upc_threads,
	       uda_debugger_pts_t *result)
{

  // RWM
  UDA_PLUGIN_TRACE (("begin"));
  /* UPC Language specification para 6.3.4.2,
   * which omits to mention that the base pointer value also moves !
   */
  uda_tint_t n_threads = (uda_tint_t)upc_threads;
  uda_tint_t index = (uda_tint_t)delta;
  UDA_PLUGIN_TRACE (("operand: addrfield=%lx, thread=%d, phase=%d",
		  (unsigned long) p->addrfield, (int) p->thread,
		  (int) p->phase));
  UDA_PLUGIN_TRACE ((
    "block_size=%lu, element_size=%lu, upc_threads=%lu, delta=%ld",
    (unsigned long) block_size, (unsigned long) element_size,
    (unsigned long) upc_threads, (long) delta));
  if (block_size == 0)
    {
      result->phase = 0;
      result->thread = p->thread;
      result->addrfield = p->addrfield + (index * element_size);
    }
  else
    {
      uda_tint_t elem_per_block;
      uda_tint_t t1, t2, tincr, phase_diff;
      /* block_size is passed as elements per block.  */
      elem_per_block = block_size;
      t1 = p->phase + index;
      tincr = t1 / elem_per_block;
      result->phase = t1 % elem_per_block;
      phase_diff = result->phase - p->phase;
      t2 = p->thread + tincr;
      if (t2 >= 0)
        result->thread = t2 % n_threads;
      else
        result->thread = n_threads + t2 % n_threads;
      result->addrfield =
         p->addrfield + ((phase_diff + ((t2 / n_threads) * elem_per_block))
	                  * element_size);
    }

  UDA_PLUGIN_TRACE (("result: addrfield=%lx, thread=%d, phase=%d",
		  (unsigned long) result->addrfield, (int) result->thread,
		  (int) result->phase));
  return uda_ok;
}

/* Work out the difference between two pointers to shared.
 * 
 * What we're after is the same result as one would get by doing a pointer
 * subtraction in UPC.
 *
 */
int
uda_pts_difference (uda_thread_t *thread,
		    const uda_debugger_pts_t *p1,
		    const uda_debugger_pts_t *p2,
		    const uda_tword_t element_size,
		    const uda_tword_t block_size,
		    const uda_tword_t thread_count,
		    uda_tint_t *delta)
{
  const uda_tint_t pdiff = p1->phase - p2->phase;
  const uda_tint_t adiff = (uda_tint_t) (p1->addrfield - p2->addrfield)
                                          / (uda_tint_t) element_size;
  const uda_tint_t tdiff = p1->thread - p2->thread;
  UDA_PLUGIN_TRACE (("operand1: addrfield=%lx, thread=%d, phase=%d",
		  (unsigned long) p1->addrfield, (int) p1->thread,
		  (int) p1->phase));
  UDA_PLUGIN_TRACE (("operand2: addrfield=%lx, thread=%d, phase=%d",
		  (unsigned long) p2->addrfield, (int) p2->thread,
		  (int) p2->phase));
  UDA_PLUGIN_TRACE (("block_size=%ld, element_size=%ld,"
                     "thread_count=%ld",
                     (long) block_size, (long) element_size,
		     (long) thread_count));
  if (block_size == 0)
    *delta = adiff;
  else
    *delta = thread_count * (adiff - pdiff) + block_size * tdiff + pdiff;
  UDA_PLUGIN_TRACE (("result=%ld", (long) *delta));
  return uda_ok;
}


/* Convert a symbol into a pointer to shared.  For GCC/UPC,
   the address handle, 'sym_addr', is already in the correct
   form, to be incorporated into a PTS value.  */

int
uda_symbol_to_pts (uda_thread_t *thread,
		   const char *symbol,
		   const uda_taddr_t sym_addr,
		   const uda_tword_t block_size,
		   const uda_tword_t element_size,
		   uda_debugger_pts_t *pts)
{
  UDA_PLUGIN_TRACE (("symbol=%s,sym_addr=%lx, block_size=%ld,"
                     " element_size=%ld", symbol,
		     (unsigned long) sym_addr, (long) block_size,
		     (long) element_size));
  memset (pts, '\0', sizeof (*pts));
  /* Variables always start on thread 0, whether they are scalars
     or otherwise.  */
  pts->thread = 0;
  pts->addrfield = sym_addr;
  UDA_PLUGIN_TRACE (("returning: addrfield=%lx, thread=%ld, phase=%ld",
		  (unsigned long) pts->addrfield, (long) pts->thread,
		  (long) pts->phase));
  return uda_ok;
}
