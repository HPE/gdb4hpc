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

#include "common-myo.h"
#include "myo-dbl.h"
#ifdef GDBSERVER
#include "server.h"
#else
#include "defs.h"
#endif

#include <dlfcn.h>

/* See common-myo.h.  */
int debug_myo_dbl;

/* Myo-Dbl api.  */
struct myo_dbl_api
{
  /* Handle to the libmyodbl-*.so.  */
  void *so_handle;

  /* See myo-dbl.h.  */
  _MYOD_initRuntime init_runtime;
  _MYOD_checkAddress check_address;
  _MYOD_bindOwner bind_owner;
  _MYOD_getSymbolList get_symbol_list;
};

/* Exported Myo-Dbl symbols.  */
static const char init_runtime_fn_sym[] = "initRuntime";
static const char check_address_fn_sym[] = "checkAddress";
static const char bind_owner_fn_sym[] = "bindOwner";
static const char get_symbol_list_fn_sym[] = "getSymbolList";

/* Printf routine.  */

static void
myo_printf (const char *format, ...)
{
  va_list args;

  va_start (args, format);
#ifdef GDBSERVER
  vfprintf (stderr, format, args);
#else
  vprintf_unfiltered (format, args);
#endif
  va_end (args);
}

/* Wrapper around dlopen to catch and report errors.  */

static void *
verbose_dlsym (void *handle, const char *name)
{
  void *sym;
  const char *err;

  dlerror ();
  sym = dlsym (handle, name);
  err = dlerror ();

  if (err != NULL)
    warning (_("myo-dbl: Symbol \"%s\" not found: %s.\n"),
	     name, err);

  return err ? NULL : sym;
}

/* Binds Myo-Dbl api, returns 1 on success.  */

static int
bind_myo_dbl_api (void *so_handle, struct myo_dbl_api *api)
{
  if (so_handle == NULL || api == NULL)
    return 0;

  api->init_runtime = verbose_dlsym (so_handle, init_runtime_fn_sym);
  if (!api->init_runtime)
    return 0;

  api->check_address = verbose_dlsym (so_handle, check_address_fn_sym);
  if (!api->check_address)
    return 0;

  api->bind_owner = verbose_dlsym (so_handle, bind_owner_fn_sym);
  if (!api->bind_owner)
    return 0;

  api->get_symbol_list = verbose_dlsym (so_handle, get_symbol_list_fn_sym);
  if (!api->get_symbol_list)
    return 0;

  return 1;
}

static int
myo_dbl_memory_accessible (struct myo_dbl_api *api, CORE_ADDR memaddr,
			   int len, DbgAccessType access)
{
  int res = EINVAL;

  if (api && api->check_address)
    {
      _MYOD_size_t owned_bytes;
      _MYOD_node_handle handle;
      _MYOD_error_code ec;

      /* Value of `owned_bytes` is bogus, do not use it.  */
      ec = api->check_address (NULL, (_MYOD_VA)memaddr,
			       len, &owned_bytes, &handle,
			       access);

      if (debug_myo_dbl > 1)
	myo_printf (_("myo-dbl: check_address() for address 0x%0lx with "
		      "length %ld returned %d.\n"),
		    memaddr, len, (int)ec);

      res = (ec == MYOD_SUCCESS            /* Shared & our memory, ok.  */
	     || ec == MYOD_ERR_NONSHARED); /* Non Shared, ok.  */
    }

  return res;
}

/* See common-myo.h.  */

int
myo_dbl_mem_readable (void *dbl_api, long long memaddr, int len)
{
  int res = myo_dbl_memory_accessible (dbl_api, (CORE_ADDR)memaddr,
				       len, MYO_READONLY);

  return res;
}

/* See common-myo.h.  */

int
myo_dbl_mem_writeable (void *dbl_api, long long memaddr, int len)
{
  int res = myo_dbl_memory_accessible (dbl_api, (CORE_ADDR)memaddr,
				       len, MYO_READWRITE);

  return res;
}

/* See common-myo.h.  */

void*
myo_dbl_initialize (const char *dbl_so_name,
		    _MYOD_MemReadCB read, _MYOD_MemWriteCB write,
		    _MYOD_GetSymbolAddressCB lookup)
{
  struct myo_dbl_api api;
  void *handle = dlopen (dbl_so_name, RTLD_NOW);

  if (handle == NULL)
    {
      if (debug_myo_dbl)
	{
	  const char *err = dlerror ();

	  myo_printf (_("myo-dbl: dlopen failed (%s).\n"),
		      err ? err : "unknown");
	}
      return NULL;
    }

  if (bind_myo_dbl_api (handle, &api))
    {
      _MYOD_TargetAddress begin_addr;
      _MYOD_TargetAddress end_addr;
      _MYOD_Version version = 0;
      char tls_name[255] = {0};
      int res;

      res = api.init_runtime (NULL, &version, read, write,
			      lookup, tls_name,
			      &begin_addr, &end_addr);

      if (res == MYOD_SUCCESS)
	{
         struct myo_dbl_api *alloc_api;

	 if (debug_myo_dbl)
	   myo_printf (_("myo-dbl: Dbl Version: %d,\n"
			 "\tTlsName: %s\n"), version, tls_name);

         api.so_handle = handle;

         alloc_api = xcalloc (1, sizeof (api));
         *alloc_api = api;

         return alloc_api;
	}

      if (debug_myo_dbl)
	myo_printf (_("myo-dbl: Failed to initialize Myo debug library.\n"));
    }

  dlclose (handle);

  return NULL;
}

/* See common-myo.h.  */

const char **
get_symbol_list (void *api)
{
  const struct myo_dbl_api *dbl_api = api;

  if (!dbl_api)
    return NULL;

  return dbl_api->get_symbol_list ();
}

/* See common-myo.h.  */

void
myo_dbl_shutdown (void *api)
{
  struct myo_dbl_api *dbl_api = api;

  if (dbl_api && dbl_api->so_handle)
    dlclose (dbl_api->so_handle);
  xfree (dbl_api);

  if (debug_myo_dbl)
    myo_printf (_("myo-dbl: Shutdown.\n"));
}
