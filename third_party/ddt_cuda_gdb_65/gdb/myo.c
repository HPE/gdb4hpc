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


/* Myo debug flag.  */
static int debug_myo;

/* Lookup key to access the inferior payload.  */
static const struct inferior_data *myo_segv_inferior_data;

/* Per inferior data for MYO segv handling.  */
struct myo_segv_inferior_data
{
  /* Signal catchpoint.  */
  struct breakpoint *bp;
};

/* Symbol defined in the myo library. A predicate to distinguish user
   from Myo SIGSEGV.  */
static char is_myo_segv_sym[] = "isMYO_SEGV";

/* See myo.h.  */

int
myo_handle_segv (void)
{
  int is_myo_segv = 1;

  volatile struct gdb_exception ex;

  TRY_CATCH (ex, RETURN_MASK_ALL)
    is_myo_segv = parse_and_eval_long (is_myo_segv_sym) == 1;

  if (ex.reason < 0)
    /* End up here if either tls is not yet allocated or the symbol lookup
    failed.  Unfortunately the exception does not preserve this
    information anymore.  Explicitly check if the symbol is not present,
    which indicates the absence of a myo runtume and thus deal with a
    real SIGSEGV */
    is_myo_segv =
      lookup_minimal_symbol (is_myo_segv_sym, NULL, NULL) != NULL;

  if (debug_myo)
    printf_unfiltered (_("myo: Received SIGSEGV, is_myo_segv = %d.\n"),
                       is_myo_segv);

  return is_myo_segv;
}

void _initialize_myo (void);

/* Initialize MYO support for GDB.  */

void
_initialize_myo (void)
{
  add_setshow_boolean_cmd ("myo", class_maintenance, &debug_myo, _("\
Set Myo debugging."), _("\
Show Myo debugging."), _("\
When non-zero, MYO specific debugging is enabled."),
                            NULL,
                            NULL,
                            &setdebuglist, &showdebuglist);
}
