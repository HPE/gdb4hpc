/* My Your Ours (MYO) support for GDB.

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

#ifndef MYO_H
#define MYO_H

/* Deal with SEGV caused by the myo runtime.  In case the SEGV was consumed the
   function returns 1 and the signal should not be forwarded to the user.  */

extern int myo_handle_segv (void);

#endif /* MYO_H  */
