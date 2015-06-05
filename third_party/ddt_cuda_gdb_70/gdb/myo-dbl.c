/* My Your Ours (MYO) support for GDB.

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
#include "common-myo.h"
#include "defs.h"
#include "value.h"
#include "exceptions.h"
#include "inferior.h"
#include "objfiles.h"
#include "solist.h"
#include "solib.h"
#include "target.h"
#include "breakpoint.h"
#include "gdbcmd.h"
#include "observer.h"
#include "gdbthread.h"
#include "gdbcore.h"


/* Myo-Dbl target vector.  */
static struct target_ops myo_ops;

/* Lookup key to access the inferior payload.  */
static const struct inferior_data *myo_dbl_inferior_data;

/* Myo runtime library prefix.  */
static const char myo_library_prefix[] = "libmyo-";

/* Per inferior data for Myo shared memory handling via debug library.  */
struct myo_dbl_inferior_data
{
  /* Debugees Myo runtime.  */
  struct objfile *objfile;

  /* Myo-Dbl data .  */
  void *dbl;
};

/* Show Myo-Dbl debug flag.  */

static void
show_debug_myo_dbl (struct ui_file *file, int from_tty,
		    struct cmd_list_element *c, const char *value)
{
  fprintf_filtered (file, _("Myo-Dbl debugging is %s.\n"), value);
}

/* Return dbl_inferior_data for current inferior.  Allocate if not already
   present.  */

static struct myo_dbl_inferior_data *
get_dbl_inferior_data (struct inferior *inf)
{
  struct myo_dbl_inferior_data *inf_data;

  inf_data = inferior_data (inf, myo_dbl_inferior_data);
  if (inf_data == NULL)
    {
      inf_data = xzalloc (sizeof(*inf_data));
      set_inferior_data (inf, myo_dbl_inferior_data, inf_data);
    }

  return inf_data;
}

/* Myo-Dbl memory read callback implementation.  */

static _MYOD_error_code
myo_mem_read(_MYOD_VA dbgCtx, _MYOD_node_handle owner,
	     _MYOD_VA address, _MYOD_size_t bytes,
	     _MYOD_byte* buffer)
{
  struct target_ops *beneath = find_target_beneath (&myo_ops);
  int ret = target_read (beneath, TARGET_OBJECT_RAW_MEMORY, NULL,
			 (gdb_byte *)buffer, (ULONGEST)address, bytes);

  return ret != -1 ? MYOD_SUCCESS : MYOD_ERROR;
}

/* Myo-Dbl memory write callback implementation.  */

static _MYOD_error_code
myo_mem_write(_MYOD_VA dbgCtx, _MYOD_node_handle owner,
	      _MYOD_VA address, _MYOD_size_t bytes,
	      _MYOD_byte* buffer)
{
  struct target_ops *beneath = find_target_beneath (&myo_ops);
  int ret = target_read (beneath, TARGET_OBJECT_RAW_MEMORY, NULL,
			 (gdb_byte *)buffer, (ULONGEST)address, bytes);

  return ret != -1 ? MYOD_SUCCESS : MYOD_ERROR;
}

/* Myo-Dbl symbol lookup callback implementation.  */

static _MYOD_error_code
myo_lookup_symbol(_MYOD_VA dbgCtx,
		  _MYOD_node_handle owner,
		  const _MYOD_string name,
		  _MYOD_VA *address)
{
  struct minimal_symbol *sym = lookup_minimal_symbol (name, NULL, NULL);

  if (!sym)
    return MYOD_ERROR;

  if (address)
    *address = (_MYOD_VA)SYMBOL_VALUE_ADDRESS (sym);

  return MYOD_SUCCESS;
}

/* Override the to_xfer_partial target ops routine to filter memory access
   through Myo-Dbl. Throws EIO if memory could not be accessed. */

static LONGEST
myo_xfer_partial (struct target_ops *ops, enum target_object object,
		  const char *annex, gdb_byte *readbuf,
		  const gdb_byte *writebuf,
		  ULONGEST offset, LONGEST len)
{
  struct target_ops *beneath = find_target_beneath (ops);

  if (object == TARGET_OBJECT_MEMORY)
    {
      struct myo_dbl_inferior_data *inf_data;

      inf_data = get_dbl_inferior_data (current_inferior ());
      if (inf_data->dbl)
	{
	  int ok;

	  if (writebuf)
	    ok = myo_dbl_mem_writeable (inf_data->dbl, offset, len);
	  else
	    ok = myo_dbl_mem_readable (inf_data->dbl, offset, len);

	  if (!ok)
	    memory_error (EIO, (CORE_ADDR)offset);
	}
    }

  return beneath->to_xfer_partial (ops, object, annex,
				   readbuf, writebuf, offset, len);
}

/* Initialize Myo-Dbl target ops.  */

static void
myo_init_ops (void)
{
  myo_ops.to_shortname = "myo";
  myo_ops.to_longname = "mine yours ours";
  myo_ops.to_doc = "MYO support.";
  myo_ops.to_xfer_partial = myo_xfer_partial;
  myo_ops.to_stratum = myo_stratum;
  myo_ops.to_magic = OPS_MAGIC;
}

/* Enable Myo-Dbl support. Throws if already enabled.  */

static void
enable_myo_dbl (const char *dbl_so_name, struct objfile *myo_so)
{
  struct myo_dbl_inferior_data *inf_data;
  struct inferior *inf = current_inferior ();
  void *dbl;

  inf_data = get_dbl_inferior_data (inf);

  if (inf_data->dbl != NULL)
    {
      warning (_("Myo-Dbl already enabled for %s."),
	       target_pid_to_str (pid_to_ptid (inf->pid)));
      return;
    }


  /* Allow call backs to happen during initialization and thus
     push target upfront.  */
  push_target (&myo_ops);

  dbl = myo_dbl_initialize (dbl_so_name, myo_mem_read, myo_mem_write,
			    myo_lookup_symbol);
  if (dbl != NULL)
    {
      inf_data->dbl = dbl;
      inf_data->objfile = myo_so;

      return;
    }

  /* Failed to initialize Myo-Dbl. Drop top of stack target.  */
  unpush_target (&myo_ops);
}

/* Disable Myo-Dbl support.  */

static void
disable_myo_dbl ()
{
  struct myo_dbl_inferior_data *inf_data;

  inf_data = get_dbl_inferior_data (current_inferior ());
  if (inf_data->dbl)
    myo_dbl_shutdown (inf_data->dbl);
  memset (inf_data, 0, sizeof (*inf_data));

  unpush_target (&myo_ops);
}

/* solib_loaded observer callback.  */

static void
myo_solib_loaded (struct so_list *so)
{
  const char *match;

  match = strstr (so->so_original_name, myo_library_prefix);

  if (match != NULL)
    {
      void *so_handle;
      const char *suffix = match + strlen (myo_library_prefix);
      char dbl_so_name[64] = "libmyodbl-";
      size_t size = sizeof (dbl_so_name) - strlen (dbl_so_name);

      strncat(dbl_so_name, suffix, size - 1);
      dbl_so_name[sizeof (dbl_so_name) - 1] = 0;

      solib_read_symbols (so, 0);

      if (debug_myo_dbl)
	printf_unfiltered (_("Myo runtime detected in library %s.\n"),
			   so->so_name);

      enable_myo_dbl (dbl_so_name, so->objfile);
    }
}

/* solib_unloaded observer callback.  */

static void
myo_solib_unloaded (struct so_list *so)
{
  struct myo_dbl_inferior_data *inf_data;

  inf_data = get_dbl_inferior_data (current_inferior ());

  if (inf_data->objfile == so->objfile)
    disable_myo_dbl ();
}

/* inferior_exit observer callback.  */

static void
myo_inferior_exit (struct inferior *inf)
{
  struct myo_dbl_inferior_data *inf_data;

  inf_data = get_dbl_inferior_data (inf);

  if (inf_data->objfile != NULL)
    disable_myo_dbl ();
}

void _initialize_myo_dbl (void);

/* Initialize MYO support for GDB.  */

void
_initialize_myo_dbl (void)
{
  myo_init_ops ();
  add_target (&myo_ops);

  /* Install observer to enable/disable Myo-Dbl support.  */
  observer_attach_solib_unloaded (myo_solib_unloaded);
  observer_attach_solib_loaded (myo_solib_loaded);
  observer_attach_inferior_exit (myo_inferior_exit);

  /* Register inferior local data.  */
  myo_dbl_inferior_data = register_inferior_data ();

  add_setshow_zinteger_cmd ("myo-dbl", class_maintenance, &debug_myo_dbl, _("\
Set Myo debug library debugging."), _("\
Show Myo debug library debugging."), _("\
When non-zero, MYO debug library specific debugging is enabled."),
                            NULL,
                            show_debug_myo_dbl,
                            &setdebuglist, &showdebuglist);
}
