/* UDA server implementation.

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

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include "uda-defs.h"
#include "uda-types.h"
#include "uda-rmt-utils.h"
#include "uda-server-plugin.h"
#include "uda-plugin-cb.h"
#include "uda-server.h"

/* UDA job information.  */
uda_job_t *uda_job;

/* Local callback implementation routines.  */
static void uda_prints (const char *);
static int uda_job_thread_count (uda_job_t *, size_t *);
static int uda_job_get_thread (uda_job_t *, uda_tword_t, uda_thread_t **);
static int uda_job_get_image (uda_job_t *, uda_image_t **);
static int uda_thread_get_job (uda_thread_t *, uda_job_t **);
static int uda_job_set_info (uda_job_t *, uda_job_info_t *);
static int uda_job_get_info (uda_job_t *, uda_job_info_t **);
static int uda_thread_set_info (uda_thread_t *, uda_thread_info_t *);
static int uda_thread_get_info (uda_thread_t *, uda_thread_info_t **);
static int uda_image_set_info (uda_image_t *, uda_image_info_t *);
static int uda_image_get_info (uda_image_t *, uda_image_info_t **);
static int uda_get_type_sizes (uda_image_t *, uda_target_type_sizes_t *);
static int uda_variable_lookup (uda_image_t *, const char *,
				uda_relocatable_addr_t *);
static int uda_type_lookup (uda_image_t *, const char *, uda_type_t **);
static int uda_type_length (uda_type_t *, uda_tword_t *);
static int uda_type_get_member_info (uda_type_t *, const char *,
				     uda_tword_t * bit_offset,
				     uda_tword_t * bit_length, uda_type_t **);
static int uda_relocate_address (uda_thread_t *,
				 const uda_relocatable_addr_t *,
				 uda_taddr_t *);
static int uda_thread_variable_lookup (uda_thread_t *, const char *,
				       uda_taddr_t *);
static int uda_thread_type_lookup (uda_thread_t *, const char *,
				   uda_type_t **);
static int uda_read_store (uda_thread_t *, uda_taddr_t, uda_tword_t, void *);
static int uda_write_store (uda_thread_t *, uda_taddr_t, uda_tword_t,
			    uda_tword_t *, void *);
static int uda_target_to_big_end (uda_thread_t *, uda_tword_t, const void *,
				  void *);
static int uda_big_end_to_target (uda_thread_t *, uda_tword_t, const void *,
				  void *);

/* Define the call back vector that is passed to the
   UDA plugin.  */
static const uda_basic_callbacks_t uda_callbacks = {
  uda_malloc,
  uda_free,
  uda_prints,
  uda_db_error_string,
  uda_get_type_sizes,
  uda_variable_lookup,
  uda_type_lookup,
  uda_relocate_address,
  uda_job_thread_count,
  uda_job_get_thread,
  uda_job_get_image,
  uda_thread_get_job,
  uda_job_set_info,
  uda_job_get_info,
  uda_thread_set_info,
  uda_thread_get_info,
  uda_image_set_info,
  uda_image_get_info,
  uda_type_length,
  uda_type_get_member_info,
  uda_read_store,
  uda_write_store,
  uda_target_to_big_end,
  uda_big_end_to_target,
  uda_thread_type_lookup,
  uda_thread_variable_lookup
};

static uda_type_t *uda_type_id_to_type (uda_tword_t);

int
uda_server_set_num_threads_cmd (const uda_tword_t num_threads)
{
  size_t n;
  if (num_threads < 1 || num_threads > 65535)
    return uda_bad_num_threads;
  if (uda_job->num_threads != 0)
    return uda_num_threads_already_set;
  uda_job->num_threads = num_threads;
  uda_job->threads = (uda_thread_t *) calloc (num_threads,
					      sizeof (uda_thread_t));
  if (!uda_job->threads)
    {
      perror ("UDA threads table allocation failed");
      abort ();
    }
  for (n = 0; n < num_threads; ++n)
    {
      uda_thread_t *thread = &uda_job->threads[n];
      thread->mark = UDA_THREAD_MARK;
      thread->id = n;
      thread->job = uda_job;
    }
  uda_job->current_thread = &uda_job->threads[0];
  uda_initialize_job (uda_job);
  return uda_ok;
}

int
uda_server_set_target_info_cmd (const uda_target_type_sizes_t sizes,
				const uda_tword_t byte_order)
{
  uda_image_t *image = uda_job->image;
  if (image->target_sizes.int_size != 0)
    return uda_target_sizes_already_set;
  image->target_sizes = sizes;
  image->target_is_big_end = byte_order;
  image->target_pts_has_opaque = 0;
  return uda_ok;
}

int
uda_server_symbol_to_pts_cmd (const char *symbol,
			      const uda_taddr_t sym_addr,
			      const uda_tword_t block_size,
			      const uda_tword_t elem_size,
			      uda_debugger_pts_t * pts)
{
  uda_thread_t *thread = uda_job->current_thread;
  int status;
  status = uda_symbol_to_pts (thread,
			      symbol, sym_addr, block_size, elem_size, pts);
  return status;
}

int
uda_server_unpack_pts_cmd (const uda_target_pts_t * packed_pts,
			   const uda_tword_t block_size,
			   uda_debugger_pts_t * pts)
{
  uda_thread_t *thread = uda_job->current_thread;
  int status;
  status = uda_unpack_pts (thread, packed_pts, block_size, pts);
  return status;
}

int
uda_server_pack_pts_cmd (const uda_debugger_pts_t * pts,
			 const uda_tword_t block_size,
			 size_t * packed_pts_len,
			 uda_target_pts_t * packed_pts)
{
  uda_thread_t *thread0;
  int status;
  uda_job_get_thread (uda_job, 0, &thread0);
  memset (packed_pts, '\0', sizeof (uda_target_pts_t));
  *packed_pts_len = 0;
  status = uda_pack_pts (thread0, pts, block_size,
			 packed_pts_len, packed_pts);
  return status;
}

int
uda_server_length_of_pts_cmd (const uda_tword_t block_size,
			      uda_tword_t * pts_len)
{
  uda_image_t *image = uda_job->image;
  int status;
  *pts_len = 0;
  status = uda_length_of_pts (image, block_size, pts_len);
  return status;
}

int
uda_server_pts_index_add_cmd (const uda_debugger_pts_t * pts_operand,
			      const uda_tint_t index,
			      const uda_tword_t elem_size,
			      const uda_tword_t block_size,
			      uda_debugger_pts_t * pts)
{
  uda_thread_t *thread = uda_job->current_thread;
  uda_tword_t thread_cnt = uda_job->num_threads;
  int status;
  status = uda_index_pts (thread, pts_operand, index,
			  elem_size, block_size, thread_cnt, pts);
  return status;
}

int
uda_server_pts_diff_cmd (const uda_debugger_pts_t * pts_oprnd_1,
			 const uda_debugger_pts_t * pts_oprnd_2,
			 const uda_tword_t elem_size,
			 const uda_tword_t block_size, uda_tint_t * diff)
{
  uda_thread_t *thread = uda_job->current_thread;
  const uda_tword_t thread_count = uda_job->num_threads;
  int status;
  status = uda_pts_difference (thread, pts_oprnd_1, pts_oprnd_2,
			       elem_size, block_size, thread_count, diff);
  return status;
}

int
uda_server_read_upc_shared_mem_cmd (const uda_taddr_t addrfield,
				    const uda_tword_t thread_num,
				    const uda_tword_t length,
				    uda_binary_data_t * data)
{
  uda_thread_t *thread;
  int status;
  uda_tword_t actual_length;
  if (thread_num >= uda_job->num_threads)
    return uda_bad_assistant;
  thread = &uda_job->threads[thread_num];
  status = uda_read_upc_shared_mem (thread, addrfield, &actual_length,
				    length, data->bytes);
  data->len = actual_length;
  return status;
}

int
uda_server_pts_to_addr_cmd (const uda_debugger_pts_t * pts,
			    const uda_tword_t block_size,
			    const uda_tword_t elem_size, uda_taddr_t * addr)
{
  int status;
  const uda_tword_t thread_num = pts->thread;
  uda_thread_t *thread;
  if (thread_num >= uda_job->num_threads)
    return uda_bad_assistant;
  thread = &uda_job->threads[thread_num];
  status = uda_pts_to_addr (thread, pts, block_size, elem_size, addr);
  return status;
}

int
uda_server_write_upc_shared_mem_cmd (const uda_taddr_t addrfield,
				     const uda_tword_t thread_num,
				     uda_tword_t * bytes_written,
				     const uda_binary_data_t * data)
{
  int status;
  uda_thread_t *thread;
  status = uda_job_get_thread (uda_job, thread_num, &thread);
  if (status != uda_ok)
    return status;
  status = uda_write_upc_shared_mem (thread, addrfield, data->len,
				     bytes_written, data->bytes);
  return status;
}

/* Print a message (intended for debugging use *ONLY*).  */

static void
uda_prints (const char *str)
{
  fputs (str, stderr);
}

/* Given a job return the number of UPC threads in it. */
static int
uda_job_thread_count (uda_job_t * job, size_t * n_threads)
{
  if (!(job && job->mark == UDA_JOB_MARK))
    return uda_bad_job;
  *n_threads = job->num_threads;
  return uda_ok;
}

/* Given a job return the requested UPC thread within it. */
static int
uda_job_get_thread (uda_job_t * job, uda_tword_t thread_id,
		    uda_thread_t ** thread)
{
  if (!(job && job->mark == UDA_JOB_MARK))
    return uda_bad_job;
  if (thread_id >= job->num_threads)
    return uda_bad_thread_index;
  *thread = &job->threads[thread_id];
  if (!*thread)
    return uda_no_information;
  return uda_ok;
}

/* Given a job return the image associated with it. */
static int
uda_job_get_image (uda_job_t * job, uda_image_t ** image)
{
  if (!(job && job->mark == UDA_JOB_MARK))
    return uda_bad_job;
  *image = job->image;
  if (!*image)
    return uda_no_information;
  return uda_ok;
}

/* Given a thread return the job it belongs to. */
static int
uda_thread_get_job (uda_thread_t * thread, uda_job_t ** job)
{
  if (!(thread && thread->mark == UDA_THREAD_MARK))
    return uda_bad_assistant;
  *job = thread->job;
  return uda_ok;
}

/* These functions let the assistant library associate information
 *with specific jobs and threads and images. The definition of
 *uda_*_info is up to the assistant; the debugger only ever has a
 *pointer to it and doesn't look inside, so the assistant can store
 *whatever it wants.
 *
 *If a uda_get_*_info function is called before the corresponding
 *uda_set_*_info function has been called (or after the info has been
 *reset to (void *)0), then the get function will return
 *uda_no_information and nullify the result pointer.
 */
/* Associate information with a UPC job object */
static int
uda_job_set_info (uda_job_t * job, uda_job_info_t * info)
{
  if (!(job && job->mark == UDA_JOB_MARK))
    return uda_bad_job;
  job->info = info;
  return uda_ok;
}

static int
uda_job_get_info (uda_job_t * job, uda_job_info_t ** info)
{
  if (!(job && job->mark == UDA_JOB_MARK))
    return uda_bad_job;
  *info = job->info;
  if (!*info)
    return uda_no_information;
  return uda_ok;
}

/* Associate information with a UPC thread object */
static int
uda_thread_set_info (uda_thread_t * thread, uda_thread_info_t * info)
{
  if (!thread)
    return uda_bad_assistant;
  if (thread->mark != UDA_THREAD_MARK)
    return uda_bad_assistant;
  thread->info = info;
  return uda_ok;
}

static int
uda_thread_get_info (uda_thread_t * thread, uda_thread_info_t ** info)
{
  if (!(thread && thread->mark == UDA_THREAD_MARK))
    return uda_bad_assistant;
  *info = thread->info;
  if (!*info)
    return uda_no_information;
  return uda_ok;
}

/* Associate information with an image object */
static int
uda_image_set_info (uda_image_t * image, uda_image_info_t * info)
{
  if (!(image && image->mark == UDA_IMAGE_MARK))
    return uda_bad_assistant;
  image->info = info;
  return uda_ok;
}

static int
uda_image_get_info (uda_image_t * image, uda_image_info_t ** info)
{
  if (!(image && image->mark == UDA_IMAGE_MARK))
    return uda_bad_assistant;
  *info = image->info;
  if (!*info)
    return uda_no_information;
  return uda_ok;
}

/* Calls on an image */
/* Fill in the sizes of target types for this image */
static int
uda_get_type_sizes (uda_image_t * image, uda_target_type_sizes_t * sizes)
{
  if (!(image && image->mark == UDA_IMAGE_MARK))
    return uda_bad_assistant;
  *sizes = image->target_sizes;
  return uda_ok;
}

/* Lookup a global variable and return its relocatable address */
static int
uda_variable_lookup (uda_image_t * image,
		     const char *symbol, uda_relocatable_addr_t * addr)
{
  int status;
  if (!(image && image->mark == UDA_IMAGE_MARK))
    return uda_bad_assistant;
  uda_rmt_send_cmd ("qupc.var:%s", symbol);
  status = uda_rmt_recv_reply ("%lux", addr);
  if (status != uda_ok)
    return uda_no_information;
  return uda_ok;
}

/* Lookup a type and return it */
static int
uda_type_lookup (uda_image_t * image,
		 const char *type_name, uda_type_t ** type)
{
  uda_tword_t type_id;
  int status;
  if (!(image && image->mark == UDA_IMAGE_MARK))
    return uda_bad_assistant;
  uda_rmt_send_cmd ("qupc.type:%s", type_name);
  status = uda_rmt_recv_reply ("%lux", &type_id);
  if (status != uda_ok)
    return uda_no_information;
  *type = uda_type_id_to_type (type_id);
  return uda_ok;
}

/* Calls on a type */
/* Get the length of the type in bytes */
static int
uda_type_length (uda_type_t * type, uda_tword_t * length)
{
  int status;
  if (!(type && type->mark == UDA_TYPE_MARK))
    return uda_bad_assistant;
  uda_rmt_send_cmd ("qupc.type.len:%lux", type->type_id);
  status = uda_rmt_recv_reply ("%lux", length);
  if (status != uda_ok)
    return uda_no_information;
  return uda_ok;
}

/* Lookup a field within an aggregate type by name, and return
 *its _bit_ offset within the aggregate, its bit length and its type.
 */
static int
uda_type_get_member_info (uda_type_t * type,
			  const char *member_id,
			  uda_tword_t * bit_offset,
			  uda_tword_t * bit_length, uda_type_t ** member_type)
{
  uda_tword_t member_type_id;
  int status;
  if (!(type && type->mark == UDA_TYPE_MARK))
    return uda_bad_assistant;
  uda_rmt_send_cmd ("qupc.type.member:%lux,%s",
		    (uda_tword_t) type->type_id, member_id);
  status = uda_rmt_recv_reply ("%lux,%lux,%lux",
			       bit_offset, bit_length, &member_type_id);
  if (status != uda_ok)
    return uda_no_information;
  *member_type = uda_type_id_to_type (member_type_id);
  return uda_ok;
}

/* Calls on a uda_thread_t */
/* Relocate a relocatable address for use in a specific UPC thread */
static int
uda_relocate_address (uda_thread_t * thread,
		      const uda_relocatable_addr_t * reloc_addr,
		      uda_taddr_t * addr)
{
  int status;
  if (!(thread && thread->mark == UDA_THREAD_MARK))
    return uda_bad_assistant;
  uda_rmt_send_cmd ("qupc.thread.address:%lux,%lux",
		    reloc_addr->value, thread->id);
  status = uda_rmt_recv_reply ("%lux", addr);
  if (status != uda_ok)
    return uda_no_information;
  return uda_ok;
}

/* Look up a variable in a thread.
 *This is more general than looking it up in an image, since it also
 *looks in any shared libraries which are linked into the thread.
 *Since this is thread specific the result is an absolute (rather than
 *relocatable) address. The address is therefore _only_ valid within
 *the specific thread.
 */
static int
uda_thread_variable_lookup (uda_thread_t * thread,
			    const char *symbol, uda_taddr_t * addr)
{
  int status;
  uda_rmt_send_cmd ("qupc.var:%s", symbol);
  status = uda_rmt_recv_reply ("%lux", addr);
  if (status != uda_ok)
    return uda_no_information;
  return uda_ok;
}

/* Look up a type in a thread.
 *This is more general than looking it up in an image, since it also
 *looks in any shared libraries which are linked into the thread.
 */
static int
uda_thread_type_lookup (uda_thread_t * thread,
			const char *type_id, uda_type_t ** type)
{
  if (!(thread && thread->mark == UDA_THREAD_MARK))
    return uda_bad_assistant;
  /* not yet implemented */
  return uda_no_information;
}

/* Read store from a specific UPC thread */
static int
uda_read_store (uda_thread_t * thread,
		uda_taddr_t addr, uda_tword_t length, void *bytes)
{
  int status;
  size_t n_bytes;
  char *buf;
  if (!(thread && thread->mark == UDA_THREAD_MARK))
    return uda_bad_assistant;
  uda_rmt_send_cmd ("qupc.read.local:%lux,%lux,%lux",
		    addr, thread->id, length);
  status = uda_rmt_recv_reply ("%*b", &n_bytes, &buf);
  /* a pointer to an allocated temporary buffer is returned
     in 'buf'.  Copy the data to the caller's buffer and
     free the temporary buffer.  */
  if (n_bytes)
    {
      memcpy (bytes, buf, n_bytes);
      uda_free (buf);
    }
  if (status != uda_ok)
    return status;
  if (n_bytes != length)
    return uda_read_failed;
  return uda_ok;
}

/* Write store in a specific UPC thread */
static int
uda_write_store (uda_thread_t * thread,
		 uda_taddr_t addr,
		 uda_tword_t length,
		 uda_tword_t * length_written, void *bytes)
{
  int status;
  if (!(thread && thread->mark == UDA_THREAD_MARK))
    return uda_bad_assistant;
  uda_rmt_send_cmd ("Qupc.write.local:%lux,%lux,%*b",
		    addr, thread->id, (size_t) length, bytes);
  *length_written = 0;
  status = uda_rmt_recv_reply ("%lux", length_written);
  return status;
}

/* Convert data from target byte order to big endian byte order */
static int
uda_target_to_big_end (uda_thread_t * thread,
		       uda_tword_t length, const void *t_bytes, void *bytes)
{
  int targ_is_big_end = thread->job->image->target_is_big_end;
  if (targ_is_big_end)
    memcpy (bytes, t_bytes, (size_t) length);
  else
    uda_rmt_swap_bytes (bytes, t_bytes, (size_t) length);
  return uda_ok;
}

/* Convert data from big endian byte order to target byte order */
static int
uda_big_end_to_target (uda_thread_t * thread,
		       uda_tword_t length, const void *bytes, void *t_bytes)
{
  int targ_is_big_end = thread->job->image->target_is_big_end;
  if (targ_is_big_end)
    memcpy (t_bytes, bytes, (size_t) length);
  else
    uda_rmt_swap_bytes (t_bytes, bytes, (size_t) length);
  return uda_ok;
}

static uda_type_t *
uda_type_id_to_type (uda_tword_t type_id)
{
  uda_type_t *type;
  for (type = uda_job->types; type; type = type->next)
    {
      if (type->type_id == type_id)
	return type;
    }
  type = (uda_type_t *) calloc (1, sizeof (uda_type_t));
  if (!type)
    {
      perror ("UDA cannot allocate type node");
      abort ();
    }
  type->mark = UDA_TYPE_MARK;
  type->type_id = type_id;
  type->next = uda_job->types;
  uda_job->types = type;
  return type;
}

static void
uda_server_init (void)
{
  uda_job = (uda_job_t *) calloc (1, sizeof (uda_job_t));
  if (!uda_job)
    {
      perror ("UDA job allocation failed");
      abort ();
    }
  uda_job->mark = UDA_JOB_MARK;
  uda_job->image = (uda_image_t *) calloc (1, sizeof (uda_image_t));
  if (!uda_job->image)
    {
      perror ("UDA image allocation failed");
      abort ();
    }
  uda_job->image->mark = UDA_IMAGE_MARK;
  load_uda_plugin ();
  uda_setup_basic_callbacks (&uda_callbacks);
}

void
uda_server (int connect_fd)
{
  FILE *s_in, *s_out;
  int status;
  char cmd[4096];
  uda_server_init ();
  s_in = fdopen (connect_fd, "r");
  if (!s_in)
    {
      perror ("fdopen of s_in failed");
      abort ();
    }
  s_out = fdopen (connect_fd, "w");
  if (!s_out)
    {
      perror ("fdopen of s_out failed");
      abort ();
    }
  setlinebuf (s_out);
  uda_rmt_init (s_in, s_out, "<--", NULL);
  /* Parse and execute UDA server commands.  */
  while (fgets (cmd, sizeof (cmd), s_in))
    {
      size_t c_len = strlen (cmd);
      if (cmd[c_len - 1] == '\n')
	cmd[--c_len] = '\0';
#ifdef DEBUG
      printf ("server: command: %s\n", cmd);
#endif /* DEBUG */
      /* skip the leading '$' */
      assert (cmd[0] == '$');
      status = uda_server_cmd_exec (cmd + 1);
      if (status != 0)
	{
	  fprintf (stderr, "syntax error in remote command: %s\n", cmd);
	  abort ();
	}
    }
  fclose (s_in);
  fclose (s_out);
  close (connect_fd);
  exit (0);
}
