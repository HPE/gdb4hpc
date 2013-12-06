/* UDA plugin utilities.

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

void
uda_plugin_dprintf (const char *fmt, ...)
{
  char buffer[1024];
  va_list args;
  va_start (args, fmt);
  vsnprintf (&buffer[0], sizeof (buffer), fmt, args);
  buffer[sizeof (buffer) - 1] = 0;
  db_prints (buffer);
}

/* Return the address of the global symbol name indicated by 'var_name'.
   We assume that the address associated with 'var_name' has the
   same address in all threads, therefore it is safe to relocate
   it relative to thread 0, though no relocation should be necessary.  */

uda_taddr_t
uda_plugin_global_variable_lookup (uda_job_t *job, const char *var_name)
{
  int status;
  uda_taddr_t global_addr;
  uda_job_info_t *job_info;
  uda_image_t *image;
  uda_image_info_t *im_info;
  uda_relocatable_addr_t reloc_addr;
  uda_thread_t *thread0;

  // RWM
  UDA_PLUGIN_TRACE (("enter"));
  db_job_get_info (job, &job_info);
  db_job_get_image (job, &image);
  db_image_get_info (image, &im_info);
  status = db_variable_lookup (image, var_name, &reloc_addr);
  if (status != uda_ok)
    return 0;
  db_job_get_thread (job, 0, &thread0);
  db_relocate_address (thread0, &reloc_addr, &global_addr);
  return global_addr;
}

/* Lookup a variable in the UPC thread.  */

uda_taddr_t
uda_plugin_thread_variable_lookup (uda_thread_t *thread, const char *var_name)
{
  uda_taddr_t res;

  // RWM
  UDA_PLUGIN_TRACE (("enter"));
  int status = db_thread_variable_lookup (thread, var_name, &res);
  if (status == uda_ok)
    return res;
  else
    return 0;
}

/* Convert a target value, which be 1..sizoef(uda_taddr_t)
   bytes in length, into uda_taddr_t value.  */

uda_taddr_t
uda_plugin_get_scalar_value (uda_thread_t *thread,
			     const size_t len,
                             const void *targ_bytes)
{
  size_t i;
  uda_taddr_t result;
  unsigned char be_bytes[sizeof (uda_taddr_t)];

  // RWM
  UDA_PLUGIN_TRACE (("enter"));
  db_target_to_big_end (thread, len, targ_bytes, be_bytes);
  for (i = 0, result = 0; i < len; ++i)
    result = (result << 8) | be_bytes[i];
  return result;
}

void
uda_plugin_put_scalar_value (uda_thread_t *thread,
                             const uda_taddr_t value,
			     const size_t len,
                             void *targ_bytes)
{
  int i;
  uda_taddr_t v;
  char be_bytes[sizeof (uda_taddr_t)];

  // RWM
  UDA_PLUGIN_TRACE (("enter"));
  for (i = len - 1, v = value; i >= 0; --i, v >>= 8)
    be_bytes[i] = v & 0xff;
  db_big_end_to_target (thread, len, be_bytes, targ_bytes);
}

/* Extract the bit field in the target structure that
   has been read into memory at the address given by
   'targ_bytes'.  A check is made to insure that the
   field falls within 'length' bytes.  */

int
uda_plugin_extract_bit_field (uda_thread_t *thread,
			      const size_t length,
			      const void *targ_bytes,
                              const size_t bit_offset,
                              const size_t bit_length,
			      uda_tword_t *result)
{
  size_t byte_offset, byte_length, last_byte;
  uda_tword_t field_value;
  uda_job_t *job;
  uda_image_t *image;
  uda_image_info_t *im_info;
  int sptr_struct;

  // RWM
  UDA_PLUGIN_TRACE (("enter"));
  db_thread_get_job (thread, &job);
  db_job_get_image (job, &image);
  db_image_get_info (image, &im_info);
#ifdef STRUCT_SPTR_32BIT
  sptr_struct = 1;
#else
  sptr_struct = im_info->pts_len == 8 ? 0 : 1;
#endif
  *result = 0;
  byte_offset = bit_offset / 8;
  byte_length = (bit_length + 7) / 8;
  last_byte = byte_offset + byte_length - 1;
  if (byte_length > sizeof (uda_taddr_t))
    {
      uda_plugin_dprintf ("uda_plugin_extract_bit_field: bit field size (%ld)"
                           " exceeds %ld bytes", (long) byte_length,
			   (long) sizeof (uda_taddr_t));
      abort ();
    }
  if (last_byte >= length)
    {
      uda_plugin_dprintf ("uda_plugin_extract_bit_field: last byte"
                           " at %ld falls outsie of %ld bytes in structure",
			   (long) last_byte, (long) byte_length);
      abort ();
    }

  if (sptr_struct)
    {
      field_value = uda_plugin_get_scalar_value (thread,
			    byte_length, (char *)targ_bytes + byte_offset);
      if (byte_length < sizeof (uda_taddr_t))
        {
          const size_t n_left_bits   = bit_offset % 8;
          const size_t left_span     = bit_length + n_left_bits;
          const size_t bit_shift     = (byte_length * 8) - left_span;
          const uda_tword_t bit_mask = (((uda_taddr_t) 1) << bit_length) - 1;
          field_value = (field_value >> bit_shift) & bit_mask;
        }
    }
  else
    {
      field_value = uda_plugin_get_scalar_value (thread,
			    im_info->pts_len, (char *)targ_bytes);
      const size_t bit_shift     = im_info->pts_len * 8 - bit_offset - bit_length;
      const uda_tword_t bit_mask = (((uda_taddr_t) 1) << bit_length) - 1;
      field_value = (field_value >> bit_shift) & bit_mask;
    }
  *result = field_value;
  return uda_ok;
}

void
uda_plugin_insert_bit_field (uda_thread_t *thread,
			     const uda_taddr_t value,
                             const size_t bit_offset,
                             const size_t bit_length,
			     const size_t length,
			     void *targ_bytes)
{
  size_t byte_offset, byte_length, last_byte;
  uda_tword_t field_value;
  uda_job_t *job;
  uda_image_t *image;
  uda_image_info_t *im_info;
  int sptr_struct;

  // RWM
  UDA_PLUGIN_TRACE (("enter"));
  db_thread_get_job (thread, &job);
  db_job_get_image (job, &image);
  db_image_get_info (image, &im_info);
#ifdef STRUCT_SPTR_32BIT
  sptr_struct = 1;
#else
  sptr_struct = im_info->pts_len == 8 ? 0 : 1;
#endif
  byte_offset = bit_offset / 8;
  byte_length = (bit_length + 7) / 8;
  last_byte = byte_offset + byte_length - 1;
  if (byte_length > sizeof (uda_taddr_t))
    {
      uda_plugin_dprintf ("uda_plugin_insert_bit_field: bit field size (%ld)"
                           " exceeds %ld bytes", (long) byte_length,
			   (long) sizeof (uda_taddr_t));
      abort ();
    }
  if (last_byte >= length)
    {
      uda_plugin_dprintf ("uda_plugin_insert_bit_field: last byte"
                           " at %ld falls outsie of %ld bytes in structure",
			   (long) last_byte, (long) byte_length);
      abort ();
    }

  if (sptr_struct)
    { 
      if (((bit_offset % 8) == 0) && ((bit_length % 8) == 0))
        field_value = value;
      else
        {
          const size_t n_left_bits  = bit_offset % 8;
          const size_t left_span = bit_length + n_left_bits;
          const size_t bit_shift = (byte_length * 8) - left_span;
          const uda_taddr_t bit_mask = ((uda_taddr_t) 1 << bit_length) - 1;
          const uda_taddr_t zap_mask = (~bit_mask) << bit_shift;
          const uda_taddr_t val_insert = (value & bit_mask) << bit_shift;
          field_value = uda_plugin_get_scalar_value (thread,
			   byte_length, (char *)targ_bytes + byte_offset);
          field_value = (field_value & zap_mask) | val_insert;
        }
      uda_plugin_put_scalar_value (thread, field_value, byte_length,
                           targ_bytes + byte_offset);
    } 
  else  
    {
      const size_t bit_shift = im_info->pts_len * 8 - bit_offset - bit_length;
      const uda_taddr_t bit_mask = ((uda_taddr_t) 1 << bit_length) - 1;
      const uda_taddr_t zap_mask = (~bit_mask) << bit_shift;
      const uda_taddr_t val_insert = (value & bit_mask) << bit_shift;
      field_value = uda_plugin_get_scalar_value (thread, im_info->pts_len, 
                                                 (char *)targ_bytes);
      field_value = (field_value & zap_mask) | val_insert;
      uda_plugin_put_scalar_value (thread, field_value, im_info->pts_len,
                                                  targ_bytes);
    }
}

/* Extract a scalar value from target store and return it into a 
   uda_tword_t. Need to be careful if we're extracting a small item to
   get it into the right part of the result.  The target address, ADDR,
   need not be aligned.  */

int
uda_plugin_read_scalar (uda_thread_t *thread,
                        uda_taddr_t addr,
			size_t len,
			uda_taddr_t *result)
{
  char buffer[sizeof (uda_taddr_t)];
  int status;

  // RWM
  UDA_PLUGIN_TRACE (("enter"));
  *result = 0;
  if (len > sizeof (uda_taddr_t))
    {
      uda_plugin_dprintf
	("Bad length for uda_plugin_read_scalar %lu, max %lu\n",
	  (unsigned long) len, (unsigned long) sizeof (uda_taddr_t));
      abort ();
    }
  status = db_read_store (thread, addr, len, buffer);
  if (status != uda_ok)
    return status;
  *result = uda_plugin_get_scalar_value (thread, len, buffer);
  return uda_ok;
}

int
uda_plugin_read_struct_member (uda_thread_t *thread,
			       const uda_taddr_t struct_addr,
			       const char *struct_name,
			       const char *member_name,
			       uda_tword_t *result)
{
  int status;
  uda_type_t *struct_type;
  uda_type_t *field_type;
  uda_tword_t bit_offset, bit_length;
  uda_tword_t struct_length;
  uda_tword_t field_value;
  void *struct_bytes;

  // RWM
  UDA_PLUGIN_TRACE (("enter"));
  *result = 0;
  status = db_thread_type_lookup (thread, struct_name, &struct_type);
  if (status != uda_ok)
    {
      uda_plugin_dprintf ("Failed to get type info for '%s'\n", struct_name);
      return status;
    }
  status = db_type_length (struct_type, &struct_length);
  if (status != uda_ok)
    {
      uda_plugin_dprintf ("Failed to get type length for '%s'\n", struct_name);
      return status;
    }
  status = db_type_get_member_info (struct_type, member_name,
                                    &bit_offset, &bit_length, &field_type);
  if (status != uda_ok)
    {
      uda_plugin_dprintf ("Failed to find member '%s' in type '%s'\n",
			  member_name, struct_name);
      return status;
    }
  struct_bytes = db_malloc (struct_length);
  if (!struct_bytes)
    {
      uda_plugin_dprintf ("buffer allocation failed while"
                          " reading structure '%s'\n", struct_name);
      return status;
    }
  status = db_read_store (thread, struct_addr, struct_length, struct_bytes);
  if (status != uda_ok)
    {
      uda_plugin_dprintf ("Failed to read structure '%s'\n", struct_name);
      goto  read_struct_return;
    }
  status = uda_plugin_extract_bit_field (thread, struct_length, struct_bytes,
                                         bit_offset, bit_length, &field_value);
  if (status != uda_ok)
    {
      uda_plugin_dprintf
	("Failed to extract member '%s' of struct type '%s' at 'x%lx'\n",
	 member_name, struct_name, (long) struct_addr);
      goto  read_struct_return;
    }
  *result = field_value;
read_struct_return:
  db_free (struct_bytes);
  return status;
}

/* Combines uda_plugin_lookup_variable with uda_plugin_read_scalar */
int
uda_plugin_lookup_and_read_scalar (uda_thread_t *thread, const char *varname,
				   size_t len, uda_taddr_t *result)
{
  int status;
  uda_taddr_t var_addr;

  // RWM
  UDA_PLUGIN_TRACE (("enter"));
  status = db_thread_variable_lookup (thread, varname, &var_addr);
  if (status != uda_ok)
    {
      uda_plugin_dprintf ("Failed to find symbol %s\n", varname);
      return status;
    }
  status = uda_plugin_read_scalar (thread, var_addr, len, result);
  if (status != uda_ok)
    {
      uda_plugin_dprintf ("Failed to read %s at 0x%08lx\n", varname,
			  (long) var_addr);
      return status;
    }
  return uda_ok;
}
