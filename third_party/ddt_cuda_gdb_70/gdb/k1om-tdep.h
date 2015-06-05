/* Target-dependent definitions for K1OM.

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

#ifndef K1OM_TDEP_H
#define K1OM_TDEP_H

struct gdbarch;

enum k1om_regnum
{
  K1OM_K0_REGNUM = 40,
  K1OM_ZMM0_REGNUM = K1OM_K0_REGNUM + 8,
  K1OM_MXCSR_REGNUM = K1OM_ZMM0_REGNUM + 32
};

#define K1OM_NUM_REGS (K1OM_MXCSR_REGNUM+1)

extern void k1om_init_abi (struct gdbarch_info info, struct gdbarch *gdbarch);

int k1om_zmm_regnum_p (struct gdbarch *gdbarch, int regnum);

#endif /* k1om-tdep.h */
