/* UDA plugin thread support.

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

#define CRAY_CCE_UPC 1

/* Extract and cache the thread info we need.  */

uda_thread_info_t *
uda_plugin_thread_get_info (uda_thread_t *thread)
{
  uda_thread_info_t thr_info;
  uda_thread_info_t *thr_info_ref;
  uda_image_t *image;
  uda_image_info_t *im_info;
  uda_job_t *job;
  uda_taddr_t mythread;

  // RWM
  UDA_PLUGIN_TRACE (("enter"));
  db_thread_get_info (thread, &thr_info_ref);
  if (thr_info_ref)
    return thr_info_ref;
  db_thread_get_job (thread, &job);
  db_job_get_image (job, &image);
  db_image_get_info (image, &im_info);
  /* Don't have it yet so we must get the needed info. and allocate it.  */
  memset (&thr_info, '\0', sizeof (thr_info));
#ifndef CRAY_CCE_UPC // RWM Get CCE version of MYTHREAD
  if (uda_plugin_lookup_and_read_scalar (thread, "MYTHREAD",
				         im_info->type_sizes.int_size,
					 &mythread) != uda_ok)
#else
  if (uda_plugin_lookup_and_read_scalar (thread, "_dm_my_pe",
				         im_info->type_sizes.int_size,
					 &mythread) != uda_ok)
#endif
    {
      db_prints ("can't read MYTHREAD value\n");
      return NULL;
    }
  thr_info.mythread = mythread;
#ifndef CRAY_CCE_UPC // RWM Default it to zero for CCE
  if (uda_plugin_lookup_and_read_scalar (thread, "__upc_lpt",
					 im_info->type_sizes.pointer_size,
					 &thr_info.lpt_base))
    {
      db_prints ("can't read __upc_lpt value\n");
      return NULL;
    }
#else
  thr_info.lpt_base = 0;
#endif
  thr_info.cur_page_alloc = 0;
  thr_info.lpt = NULL;
  thr_info_ref = (uda_thread_info_t *) db_malloc (sizeof (thr_info));
  if (!thr_info_ref)
    return NULL;
  memcpy (thr_info_ref, &thr_info, sizeof (thr_info));
  /* Associate thread info. with this thread */
  db_thread_set_info (thread, thr_info_ref);
  return thr_info_ref;
}

void
uda_destroy_thread_info (uda_thread_t * thread)
{
  uda_thread_info_t *thread_info = 0;

  // RWM
  UDA_PLUGIN_TRACE (("enter"));
  db_thread_get_info (thread, &thread_info);
  if (thread_info)
    db_free (thread_info);
}

/* Return the value of MYTHREAD for this thread. */

int
uda_get_threadno (uda_thread_t *thread, int *mythread)
{
  uda_thread_info_t *info;

  // RWM
  UDA_PLUGIN_TRACE (("enter"));
  *mythread = 0;
  info = uda_plugin_thread_get_info (thread);
  if (!info)
    return uda_no_information;
  *mythread = info->mythread;
  return uda_ok;
}
