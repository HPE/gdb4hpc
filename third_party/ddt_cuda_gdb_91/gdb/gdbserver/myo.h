/* My Your Ours (MYO) code for GDB.

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

#ifndef MYO_H
#define MYO_H

#include "common-myo.h"

struct process_info;

/* Enable Myo-Dbl for process `proc`.  */

void myo_dbl_enable (struct process_info *proc);

/* Disable Myo-Dbl for process `proc`.  */

void myo_dbl_disable (struct process_info *proc);

#endif /* MYO_H */
