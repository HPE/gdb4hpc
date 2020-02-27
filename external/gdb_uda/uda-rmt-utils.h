/* UDA remote protocol utilities interface definition.

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

#ifndef _UDA_RMT_UTILS_H_
#define _UDA_RMT_UTILS_H_ 1

extern void *uda_malloc(size_t size);
extern void uda_free(void *ptr);
typedef int (*uda_rmt_cmd_fp_t) (const char *cmd);
extern void uda_rmt_init (FILE *rmt_in, FILE *rmt_out,
                          const char *debug_tag,
			  const uda_rmt_cmd_fp_t);
extern void uda_rmt_set_target_info (const uda_target_type_sizes_t *,
                                     const int target_big_endian,
                                     const int target_pts_has_opaque);
extern void uda_rmt_swap_bytes (uint8_t *, const uint8_t *, const size_t);
extern size_t uda_decode_binary_data (char *, const char *, const size_t);
extern size_t uda_decode_hex_bytes (char *, const char *, const size_t);
extern uint64_t uda_decode_hex_word (const char *, const size_t n_chars);
extern void uda_encode_binary_data (char *, const uint8_t *, const size_t);
extern void uda_encode_hex_bytes (char *, const uint8_t *,
                                  const size_t n_bytes,
                                  const int skip_leading_zeros);
extern int uda_errno;
extern void uda_error (const char *);
extern const char *uda_db_error_string (int);
extern void uda_rmt_format_msg (char *msg, const char *fmt, ...);
extern int uda_rmt_recv_reply (const char *, ...);
#define uda_rmt_recv_status() (uda_rmt_recv_reply (""))
extern int uda_rmt_scan_msg (const char *msg, const char *fmt, ...);
extern void uda_rmt_send_cmd (const char *, ...);
extern void uda_rmt_send_reply (const char *, ...);
extern void uda_rmt_send_status (const int);
extern size_t uda_scan_binary_data (const char **);
extern size_t uda_scan_hex_bytes (const char **);
#include <stdarg.h>
extern void uda_rmt_vformat_msg (char *msg, const char *fmt, va_list);
extern int uda_rmt_vscan_msg (const char *msg, const char *fmt, va_list);

#endif /* !_UDA_RMT_UTILS_H_ */
