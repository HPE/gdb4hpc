/* My Your Ours (MYO) code for GDB.

   Modified by Allinea Software.
   
   Copyright (C) Allinea Software Ltd. 2014
   Copyright (C) 2012 Free Software Foundation, Inc.

   Contributed by Intel Corporation.

   This file is part of GDB.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include "myo.h"
#include "server.h"
#include "linux-low.h"

#include <dlfcn.h>

#if   defined(__KNC__)
const static char myo_dbl_soname[] = "libmyodbl-service.so";
#elif defined(__x86_64___)
const static char myo_dbl_soname[] = "libmyodbl-client.so";
#else /* We assume client myodbl support.  */
const static char myo_dbl_soname[] = "libmyodbl-client.so";
#endif

/* Myo-Dbl memory read callback implementation.  */

static _MYOD_error_code
mem_read(_MYOD_VA dbgCtx, _MYOD_node_handle owner,
                _MYOD_VA address, _MYOD_size_t bytes,
                _MYOD_byte* buffer)
{
  int ret = linux_read_memory ((CORE_ADDR)address,
				  (unsigned char *)buffer, (int)bytes);

  return ret == 0 ? MYOD_SUCCESS : MYOD_ERROR;
}

/* Myo-Dbl memory write callback implementation.  */

static _MYOD_error_code
mem_write(_MYOD_VA dbgCtx, _MYOD_node_handle owner,
                 _MYOD_VA address, _MYOD_size_t bytes,
                 _MYOD_byte* buffer)
{
  int ret = linux_write_memory ((CORE_ADDR)address,
				  (unsigned char *)buffer, (int)bytes);

  return ret == 0 ? MYOD_SUCCESS : MYOD_ERROR;
}

/* Myo-Dbl symbol lookup callback implementation.  */

static _MYOD_error_code
lookup_symbol(_MYOD_VA dbgCtx,
                       _MYOD_node_handle owner,
                       const _MYOD_string name,
                       _MYOD_VA *address)
{
  int may_ask_gdb = 1;
  int ret = look_up_one_symbol (name, (CORE_ADDR *)address, may_ask_gdb);

  return ret == 1 ? MYOD_SUCCESS : MYOD_ERROR;
}

/* Resolve list of symbols. Returns 1 if all symbols could be resolved.  */

static int
lookup_myo_dbl_symbols (const char **sym_list)
{
  CORE_ADDR unused;

  for (; *sym_list; sym_list++)
    {
      int found = look_up_one_symbol (*sym_list, &unused, 1) == 1;

      if (debug_myo_dbl)
	fprintf (stderr, "myo: Symbol \"%s\"%s found.\n",
		 *sym_list, found ? "": " not" );

      if (!found)
	return 0;
    }

  return 1;
}

/* See myo.h.  */

void
myo_dbl_enable (struct process_info *proc)
{
  void *api;
  const char **sym_list;

  if (proc->private->myo_dbl != NULL)
    {
      if (debug_myo_dbl)
	fprintf (stderr, _("myo: Dbl aready initialized."));
      return;
    }

  api = myo_dbl_initialize (myo_dbl_soname, mem_read, mem_write,
			    lookup_symbol);

  if (!api)
    return;

  sym_list = get_symbol_list (api);

  if (!lookup_myo_dbl_symbols (sym_list))
    {
      myo_dbl_shutdown (api);
      proc->private->myo_dbl = NULL;

      return;
    }

  proc->private->myo_dbl = api;

  return;
}

/* See myo.h.  */

void
myo_dbl_disable (struct process_info *proc)
{
  void *api = proc->private->myo_dbl;

  myo_dbl_shutdown (api);
  proc->private->myo_dbl = NULL;
}
