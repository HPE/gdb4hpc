/* My Your Ours (MYO) definitions for GDB.

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

#ifndef COMMON_MYO_H
#define COMMON_MYO_H

#include "myo-dbl.h"

/* Myo-Dbl debug flag.  */

extern int debug_myo_dbl;

/* Initialize and install Myo debug library.  */

void* myo_dbl_initialize (const char *dbl_so_name, _MYOD_MemReadCB read,
			  _MYOD_MemWriteCB write,
			  _MYOD_GetSymbolAddressCB lookup);

/* Shutdown and remove Myo-Dbl.  */

void myo_dbl_shutdown (void *api);

/* Returns a list of required symbols needed by Myo-Dbl.  */

const char **get_symbol_list (void *api);

/* Returns 1 if memory range is read-/writeable, 0 otherwise. If the Myo-Dbl is
   not present returns EINVAL.  */

int myo_dbl_mem_readable (void *dbl_api, long long memaddr, int len);
int myo_dbl_mem_writeable (void *dbl_api, long long memaddr, int len);

#endif /* COMMON_MYO_H */
