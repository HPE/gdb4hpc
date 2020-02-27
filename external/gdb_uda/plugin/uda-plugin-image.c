/* UDA plugin image (oject file) support.

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

int
uda_plugin_init_image (uda_job_t *job, uda_image_t *image)
{
  uda_image_info_t *im_info;
#ifndef CRAY_CCE_UPC // RWM Allows set up UPC for CCE
  int status;
  uda_thread_t *thread0;
  uda_taddr_t page_shift_addr;
  uda_type_t *pts_type, *pts_phase_type, *pts_thread_type, *pts_addrfield_type;
  uda_taddr_t value;
#endif

  // RWM
  UDA_PLUGIN_TRACE (("begin"));

  im_info = db_malloc (sizeof (*im_info));
  if (!im_info)
    {
      db_prints ("can't allocate image info.\n");
      return uda_bad_assistant;
    }
  db_image_set_info (image, im_info);
  db_get_type_sizes (image, &im_info->type_sizes);
#ifndef CRAY_CCE_UPC // RWM
  im_info->shared_start = uda_plugin_global_variable_lookup (
                                     job, "__upc_shared_start");
  if (!im_info->shared_start)
    {
      db_prints ("can't find __upc_shared_start symbol\n");
      return uda_bad_assistant;
    }
#else
  // beginning of linked UPC shared section: Bogus for CCE???
  im_info->shared_start = 0;
#endif
#ifndef CRAY_CCE_UPC // RWM
  page_shift_addr = uda_plugin_global_variable_lookup (job, "__upc_page_shift");
  if (!page_shift_addr)
    {
      db_prints ("can't find __upc_page_shift symbol\n");
      return uda_bad_assistant;
    }
  db_job_get_thread (job, 0, &thread0);
  status = uda_plugin_read_scalar (thread0, page_shift_addr,
				   im_info->type_sizes.int_size,
				   &value);
  if (status != uda_ok)
    {
      db_prints ("can't read __upc_page_size value\n");
      return uda_bad_assistant;
    }
  im_info->page_shift = value;
#else
  im_info->page_shift = 0;
#endif

/* Look up the type of the dbg shared pointer and obtain info that 
   describes each field. Cache the info in the job's image info. */

#ifndef CRAY_CCE_UPC // RWM
  status = db_type_lookup (image, "upc_dbg_shared_ptr_t", &pts_type);
  if (status != uda_ok)
    {
      db_prints ("can't find upc_dbg_shared_ptr_t type info.\n");
      return uda_bad_assistant;
    }
  status = db_type_length (pts_type, &value);
  if (status != uda_ok)
    {
      db_prints ("can't find upc_shared_ptr_t type size.\n");
      return uda_bad_assistant;
    }
  im_info->pts_len = value;
#else
  im_info->pts_len = 16; // bytes
#endif
#ifndef CRAY_CCE_UPC // RWM
  status = db_type_get_member_info (pts_type, "phase",
                                    &im_info->phase_bit_offset,
                                    &im_info->phase_bit_length,
				    &pts_phase_type);
  if (status != uda_ok)
    {
      db_prints ("can't find phase info.\n");
      return uda_bad_assistant;
    }
#else
  im_info->phase_bit_offset = 64;
  im_info->phase_bit_length = 32;
#endif
#ifndef CRAY_CCE_UPC // RWM
  status = db_type_get_member_info (pts_type, "thread",
                                    &im_info->thread_bit_offset,
                                    &im_info->thread_bit_length,
				    &pts_thread_type);
  if (status != uda_ok)
    {
      db_prints ("can't find thread field info.\n");
      return uda_bad_assistant;
    }
#else
  im_info->thread_bit_offset = 96;
  im_info->thread_bit_length = 32;
#endif
#ifndef CRAY_CCE_UPC // RWM
  status = db_type_get_member_info (pts_type, "vaddr",
                                    &im_info->addrfield_bit_offset,
                                    &im_info->addrfield_bit_length,
				    &pts_addrfield_type);
  if (status != uda_ok)
    {
      db_prints ("can't find vaddr field info.\n");
      return uda_bad_assistant;
    }
#else
  im_info->addrfield_bit_offset = 0;
  im_info->addrfield_bit_length = 64;
#endif
  return uda_ok;
}

void
uda_destroy_image_info (uda_image_t *image)
{
  uda_image_info_t *image_info = 0;

  db_image_get_info (image, &image_info);

  if (image_info)
    db_free (image_info);
}
