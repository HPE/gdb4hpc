/* UDA plugin definitions.

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

#ifndef _UDA_PLUGIN_DEFS_H_ 
#define _UDA_PLUGIN_DEFS_H_  1

#include "uda-plugin-cb.h"

/* helper functions */
#define min(x,y) (((x) < (y)) ? (x): (y))
#define max(x,y) (((x) > (y)) ? (x): (y))
#define abs(x) (((x) > 0) ? (x): -(x))
#define round_up(x, r) (((x) + (r) - 1)/(r)*(r))

#if ! defined (__GNUC__) && ! defined (__attribute__)
#define __attribute__(flags)
#endif

#ifdef DEBUG_UDA_PLUGIN 
  #if __GNUC__
    #define UDA_PLUGIN_TRACE(call) UDA_PLUGIN_DO_TRACE call ;
    #define UDA_PLUGIN_DO_TRACE(format, args...) \
       uda_plugin_dprintf("%s: " format "\n", __FUNCTION__, ## args)
  #elif HAVE_FUNC
    #define UDA_PLUGIN_TRACE(call) uda_plugin_dprintf("%s: ", __func__)
  #else
    #define UDA_PLUGIN_TRACE(call) 
  #endif
#else
  #define UDA_PLUGIN_TRACE(call) 
#endif


/* Defined in uda-plugin.c */
extern const uda_basic_callbacks_t *callbacks;

/* Macros so that we don't need to worry about the fact that the callback
 * functions are called via function pointers.
 */
#define db_malloc(sz)             ((*(callbacks->malloc_cb))((sz)))
#define db_free(a)                ((*(callbacks->free_cb))((a)))
#define db_prints(a)              ((*(callbacks->prints_cb))((a)))
#define db_error_string(a)        ((*(callbacks->error_string_cb))((a)))

#define db_relocate_address(a,b,c)((*(callbacks->relocate_address_cb))((a),(b),(c)))

#define db_job_thread_count(a,b)  ((*(callbacks->job_thread_count_cb))((a),(b)))
#define db_job_get_thread(a,b,c)((*(callbacks->job_get_thread_cb))((a),(b),(c)))
#define db_job_get_image(a,b)     ((*(callbacks->job_get_image_cb))((a),(b)))
#define db_thread_get_job(a,b)    ((*(callbacks->thread_get_job_cb))((a),(b)))

#define db_image_set_info(a,b)    ((*(callbacks->image_set_info_cb))((a),(b)))
#define db_image_get_info(a,b)    ((*(callbacks->image_get_info_cb))((a),(b)))

#define db_job_set_info(a,b)      ((*(callbacks->job_set_info_cb))((a),(b)))
#define db_job_get_info(a,b)      ((*(callbacks->job_get_info_cb))((a),(b)))

#define db_thread_set_info(a,b)   ((*(callbacks->thread_set_info_cb))((a),(b)))
#define db_thread_get_info(a,b)   ((*(callbacks->thread_get_info_cb))((a),(b)))

#define db_get_type_sizes(a,b)    ((*(callbacks->get_type_sizes_cb))((a),(b)))
#define db_variable_lookup(a,b,c) ((*(callbacks->variable_lookup_cb))((a),(b),(c)))
#define db_type_lookup(a,b,c)     ((*(callbacks->type_lookup_cb))((a),(b),(c)))
#define db_type_length(a,b)      ((*(callbacks->type_length_cb))((a),(b)))
#define db_type_get_member_info(a,b,c,d,e)((*(callbacks->type_get_member_info_cb))((a),(b),(c),(d),(e)))

#define db_read_store(a,b,c,d)    ((*(callbacks->read_store_cb))((a),(b),(c),(d)))
#define db_write_store(a,b,c,d,e) ((*(callbacks->write_store_cb))((a),(b),(c),(d),(e)))
#define db_target_to_big_end(a,b,c,d)((*(callbacks->target_to_big_end_cb))((a),(b),(c),(d)))
#define db_big_end_to_target(a,b,c,d)((*(callbacks->big_end_to_target_cb))((a),(b),(c),(d)))
#define db_thread_variable_lookup(a,b,c) ((*(callbacks->thread_variable_lookup_cb))((a),(b),(c)))
#define db_thread_type_lookup(a,b,c) ((*(callbacks->thread_type_lookup_cb))((a),(b),(c)))

#endif /* !_UDA_PLUGIN_DEFS_H_  */
