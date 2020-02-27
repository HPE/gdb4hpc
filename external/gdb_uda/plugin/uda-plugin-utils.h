/* UDA plugin utiliies interface definition.

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

#ifndef _UDA_PLUGIN_UTILS_H_
#define _UDA_PLUGIN_UTILS_H_ 1

/* uda-plugin-image.c */
extern int uda_plugin_init_image (uda_job_t *, uda_image_t *);

/* uda-plugin-thread.c */
extern uda_thread_info_t *uda_plugin_thread_get_info (uda_thread_t *thread);

/* uda-plugin-shmem.c */
extern int uda_plugin_shmem_get (uda_thread_t *thread,
                                 void *to,
		                 const uda_taddr_t from, 
                                 const uda_tword_t thread_num,
		                 const uda_tword_t length);
extern int uda_plugin_shmem_put (uda_thread_t *thread,
                                 uda_taddr_t to,
		                 const void *from, 
                                 const uda_tword_t thread_num,
		                 const uda_tword_t length);
extern int uda_plugin_shared_to_local_addr (uda_job_t *job,
                                            const uda_taddr_t addrfield,
					    const uda_tword_t thread_num,
					    uda_taddr_t *local_addr);

/* uda-plugin-utils.c */
extern void uda_plugin_dprintf (const char *fmt, ...)
  __attribute__ ((__format__ (__printf__, 1, 2)));

extern void uda_plugin_dprintf (const char *fmt, ...);

extern uda_taddr_t uda_plugin_global_variable_lookup (uda_job_t *job,
                                                      const char *var_name);
extern uda_taddr_t uda_plugin_thread_variable_lookup (uda_thread_t *thread,
					              const char *var_name);
extern uda_taddr_t uda_plugin_get_scalar_value (uda_thread_t *thread,
					        const size_t len,
                                                const void *targ_bytes);
extern void uda_plugin_put_scalar_value (uda_thread_t *thread,
                                         const uda_taddr_t value,
					 const size_t len,
                                         void *targ_bytes);
extern int uda_plugin_read_scalar (uda_thread_t *thread,
				   uda_taddr_t addr, size_t len,
				   uda_taddr_t *result);
extern int uda_plugin_extract_bit_field (uda_thread_t *thread,
			                 const size_t length,
			                 const void *targ_bytes,
                                         const size_t bit_offset,
                                         const size_t bit_length,
			                 uda_taddr_t *result);
extern void uda_plugin_insert_bit_field (uda_thread_t *thread,
					 const uda_taddr_t value,
                                         const size_t bit_offset,
                                         const size_t bit_length,
			                 const size_t length,
			                 void *targ_bytes);
extern int uda_plugin_read_struct_member (uda_thread_t *thread,
			                  const uda_taddr_t struct_addr,
			                  const char *struct_name,
			                  const char *member_name,
			                  uda_tword_t *result);
extern int uda_plugin_lookup_and_read_scalar (uda_thread_t *thread,
					      const char *varname,
					      size_t len,
					      uda_taddr_t *result);

#endif  /* !_UDA_PLUGIN_UTILS_H_ */
