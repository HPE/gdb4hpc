/* UDA plug type definitions.

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

#ifndef _UDA_PLUGIN_TYPES_H_
#define _UDA_PLUGIN_TYPES_H_ 1

/* Information we cache on the UDA job.  */
struct uda_job_info_
{
  /* number of UPC threads. */
  size_t n_threads;
};

/* Information we cache on the UDA image.  */
struct uda_image_info_
{
  /* Sizes of the basic types */
  uda_target_type_sizes_t type_sizes;
  /* beginning of linked UPC shared section */
  uda_taddr_t shared_start;
  /* page size of shared memory store, in bits  */
  size_t page_shift;
  /* size of pointer-to-shared, in bytes  */
  size_t pts_len;
  /* Offsets and lengths of components of a shared pointer. */
  uda_tword_t phase_bit_offset;
  uda_tword_t phase_bit_length;
  uda_tword_t thread_bit_offset;
  uda_tword_t thread_bit_length;
  uda_tword_t addrfield_bit_offset;
  uda_tword_t addrfield_bit_length;
};

/* Information we cache on the UDA thread.  */
struct uda_thread_info_
{
  uda_tword_t mythread;
  uda_taddr_t lpt_base;
  uda_tword_t cur_page_alloc;
  uda_tword_t *lpt;
};

#endif /* !_UDA_PLUGIN_TYPES_H_ */
