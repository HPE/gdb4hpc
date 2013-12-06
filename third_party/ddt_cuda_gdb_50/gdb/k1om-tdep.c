/* Target-dependent code for K1OM.

   Modified by Allinea Software.
   
   Copyright (C) Allinea Software Ltd. 2013
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

#include "defs.h"
#include "amd64-tdep.h"
#include "i387-tdep.h"
#include "k1om-tdep.h"
#include "reggroups.h"

#include "features/i386/k1om.c"

/* The registers used to pass float arguments during a function call.  */

struct reggroup *k1om_zmm_reggroup;

extern void
i386_init_reggroups (void);

static void
k1om_init_reggroups (void)
{
  k1om_zmm_reggroup = reggroup_new ("zmm", USER_REGGROUP);
}

static void
k1om_add_reggroups (struct gdbarch *gdbarch)
{
  reggroup_add (gdbarch, k1om_zmm_reggroup);
}

/* k1om register (zmm and opmask included)?  */
int
k1om_zmm_regnum_p (struct gdbarch *gdbarch, int regnum)
{
  struct gdbarch_tdep *tdep = gdbarch_tdep (gdbarch);
  int kreg, zreg;

  if (I387_ZMM0_REGNUM (tdep) < 0)
    return 0;

  kreg = regnum >= tdep->k0_regnum &&
         regnum < (tdep->k0_regnum + tdep->num_k_regs);

  zreg = regnum >= tdep->zmm0_regnum &&
         regnum < (tdep->zmm0_regnum + tdep->num_zmm_regs);

  return kreg || zreg;
}

static int k1om_dummy_call_float_regs[] =
{
  /* zmm0 - zmm7 */
  K1OM_ZMM0_REGNUM + 0, K1OM_ZMM0_REGNUM + 1,
  K1OM_ZMM0_REGNUM + 2, K1OM_ZMM0_REGNUM + 3,
  K1OM_ZMM0_REGNUM + 4, K1OM_ZMM0_REGNUM + 5,
  K1OM_ZMM0_REGNUM + 6, K1OM_ZMM0_REGNUM + 7
};

void
k1om_init_abi (struct gdbarch_info info, struct gdbarch *gdbarch)
{
  struct gdbarch_tdep *tdep = gdbarch_tdep (gdbarch);
  const struct target_desc *tdesc = info.target_desc;

  if (! tdesc_has_registers (tdesc))
    tdesc = tdesc_k1om;
  tdep->tdesc = tdesc;

  amd64_init_abi (info, gdbarch);

  tdep->call_dummy_num_float_regs =
    ARRAY_SIZE (k1om_dummy_call_float_regs);
  tdep->call_dummy_float_regs = k1om_dummy_call_float_regs;

  set_gdbarch_num_regs (gdbarch, K1OM_NUM_REGS);

  tdep->zmm0_regnum = K1OM_ZMM0_REGNUM;
  tdep->k0_regnum = K1OM_K0_REGNUM;

  tdep->num_zmm_regs = K1OM_MXCSR_REGNUM - K1OM_ZMM0_REGNUM;
  tdep->num_k_regs = K1OM_ZMM0_REGNUM - K1OM_K0_REGNUM;

  /* no MMX, SSE or AVX on the K1OM */
  tdep->num_mmx_regs = 0;
  tdep->num_xmm_regs = 0;
  tdep->num_ymm_regs = 0;

  tdep->mm0_regnum = -1;
  tdep->ymm0_regnum = -1;
  tdep->ymm0h_regnum = -1;

  k1om_add_reggroups (gdbarch);
}

/* Provide a prototype to silence -Wmissing-prototypes.  */
extern void _initialize_k1om_tdep (void);

void
_initialize_k1om_tdep (void)
{
  register_gdbarch_init (bfd_arch_k1om, i386_gdbarch_init);

  initialize_tdesc_k1om ();

  k1om_init_reggroups ();
}
