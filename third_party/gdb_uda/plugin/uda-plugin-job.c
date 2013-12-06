/* UDA plugin job support.

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

/* Uda_initialize_job() is called after the program being debugged
   hits the breakpoint indicating that the runtime has initialized
   and is about to execute the user program.  Prior to this,
   the debugger client communicated the number of threads
   in the job, and the target-specific basic type sizes.  Thus,
   the UDA plugin has the info. it needs to initialize both the
   job-related data, and image-related data.  */

int
uda_initialize_job (uda_job_t *job)
{
  uda_job_info_t *job_info;
  uda_image_t *image;
  UDA_PLUGIN_TRACE (("enter"));
  job_info = db_malloc (sizeof (*job_info));
  if (!job_info)
    {
      db_prints ("can't allocate job info.\n");
      return uda_bad_assistant;
    }
  db_job_set_info (job, job_info);
  db_job_thread_count (job, &job_info->n_threads);
  db_job_get_image (job, &image);
  uda_plugin_init_image (job, image);
  UDA_PLUGIN_TRACE (("exit"));
  return uda_ok;
}

void
uda_destroy_job_info (uda_job_t *job)
{
}
