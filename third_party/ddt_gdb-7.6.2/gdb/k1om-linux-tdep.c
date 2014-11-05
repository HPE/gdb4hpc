/* Target-dependent code for GNU/Linux K1OM.

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

#include "defs.h"
#include "gdb_assert.h"
#include "linux-tdep.h"
#include "i386-tdep.h"
#include "amd64-tdep.h"
#include "k1om-tdep.h"
#include "reggroups.h"
#include "regcache.h"
#include "regset.h"
#include "solib-svr4.h"
#include "symtab.h"
#include "arch-utils.h"
#include "i386-xstate.h"

#include "features/i386/k1om-linux.c"

#define K1OM_LINUX_ORIG_RAX_REGNUM (K1OM_MXCSR_REGNUM + 1)

/* From <sys/reg.h>.  */
int k1om_linux_gregset_reg_offset[] =
{
  10 * 8,			/* %rax */
  5 * 8,			/* %rbx */
  11 * 8,			/* %rcx */
  12 * 8,			/* %rdx */
  13 * 8,			/* %rsi */
  14 * 8,			/* %rdi */
  4 * 8,			/* %rbp */
  19 * 8,			/* %rsp */
  9 * 8,			/* %r8 ...  */
  8 * 8,
  7 * 8,
  6 * 8,
  3 * 8,
  2 * 8,
  1 * 8,
  0 * 8,			/* ... %r15 */
  16 * 8,			/* %rip */
  18 * 8,			/* %eflags */
  17 * 8,			/* %cs */
  20 * 8,			/* %ss */
  23 * 8,			/* %ds */
  24 * 8,			/* %es */
  25 * 8,			/* %fs */
  26 * 8,			/* %gs */
  /* st0 - st7 */
  -1, -1, -1, -1, -1, -1, -1, -1,
  /* fctrl - ftop */
  -1, -1, -1, -1, -1, -1, -1, -1,
  /* k0 - k7 */
  -1, -1, -1, -1, -1, -1, -1, -1,
  /* zmm0 - zmm31 */
  -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1,
  -1,                           /* mxcsr */
  15 * 8			/* "orig_rax" */

};

/* Supported register note sections.  */
static struct core_regset_section k1om_linux_regset_sections[] =
{
  { ".reg", 27 * 8, "general-purpose" },
  { ".reg-xstate", I386_XSTATE_MAX_SIZE, "XSAVE extended state" },
  { NULL, 0 }
};

static int
k1om_dwarf_reg_to_regnum (struct gdbarch *gdbarch, int reg)
{
  enum dwarf_reg
  {
    DWARF_ST0 = 33, DWARF_ST7 = 40,
    DWARF_SEG0 = 50, DWARF_SEG5 = 55,
    DWARF_ZMM0 = 86, DWARF_ZMM31 = 117,
    DWARF_K0 = 118, DWARF_K7 = 125,

    DWARF_RIP = 16,
    DWARF_EFLAGS = 49,
    DWARF_FCTRL = 65,
    DWARF_FSTAT = 66,
    DWARF_MXCSR = 125
  };

  switch (reg)
    {
    case 0:
      return AMD64_RAX_REGNUM;
    case 1:
      return AMD64_RDX_REGNUM;
    case 2:
      return AMD64_RCX_REGNUM;
    case 3:
      return AMD64_RBX_REGNUM;
    case 4:
      return AMD64_RSI_REGNUM;
    case 5:
      return AMD64_RDI_REGNUM;
    case 6:
      return AMD64_RBP_REGNUM;
    case 7:
      return AMD64_RSP_REGNUM;
    case 8: case 9:
    case 10: case 11:
    case 12: case 13:
    case 14: case 15:
      return AMD64_R8_REGNUM + reg - 8;

    case DWARF_RIP:
      return AMD64_RIP_REGNUM;
    case DWARF_EFLAGS:
      return AMD64_EFLAGS_REGNUM;
    case DWARF_FCTRL:
      return AMD64_FCTRL_REGNUM;
    case DWARF_FSTAT:
      return AMD64_FSTAT_REGNUM;
    case DWARF_MXCSR:
      return K1OM_MXCSR_REGNUM;
    }

  if (reg >= DWARF_ST0 && reg <= DWARF_ST7)
      return AMD64_ST0_REGNUM + reg - DWARF_ST0;

  if (reg >= DWARF_SEG0 && reg <= DWARF_SEG5)
      return AMD64_CS_REGNUM + reg - DWARF_SEG0;

  if (reg >= DWARF_ZMM0 && reg <= DWARF_ZMM31)
      return K1OM_ZMM0_REGNUM + reg - DWARF_ZMM0;

  if (reg >= DWARF_K0 && reg <= DWARF_K7)
      return K1OM_K0_REGNUM + reg - DWARF_K0;

  return -1;
}

static int
k1om_linux_register_reggroup_p (struct gdbarch *gdbarch, int regnum,
                                struct reggroup *group)
{
  if (regnum == K1OM_LINUX_ORIG_RAX_REGNUM)
    return (group == system_reggroup
            || group == save_reggroup
            || group == restore_reggroup);
  return i386_register_reggroup_p (gdbarch, regnum, group);
}

/* Set the program counter for process PTID to PC.  */

static void
k1om_linux_write_pc (struct regcache *regcache, CORE_ADDR pc)
{
  regcache_cooked_write_unsigned (regcache, AMD64_RIP_REGNUM, pc);

  /* We must be careful with modifying the program counter.  If we
     just interrupted a system call, the kernel might try to restart
     it when we resume the inferior.  On restarting the system call,
     the kernel will try backing up the program counter even though it
     no longer points at the system call.  This typically results in a
     SIGSEGV or SIGILL.  We can prevent this by writing `-1' in the
     "orig_rax" pseudo-register.

     Note that "orig_rax" is saved when setting up a dummy call frame.
     This means that it is properly restored when that frame is
     popped, and that the interrupted system call will be restarted
     when we resume the inferior on return from a function call from
     within GDB.  In all other cases the system call will not be
     restarted.  */
  regcache_cooked_write_unsigned (regcache, K1OM_LINUX_ORIG_RAX_REGNUM, -1);
}

/* Get target description from core dump.  */

static const struct target_desc *
k1om_linux_core_read_description (struct gdbarch *gdbarch,
				  struct target_ops *target,
				  bfd *abfd)
{
    return tdesc_k1om_linux;
}

/* Provide a prototype to silence -Wmissing-prototypes.  */
extern void _initialize_k1om_linux_tdep (void);

static void
k1om_linux_init_abi (struct gdbarch_info info, struct gdbarch *gdbarch)
{
  struct gdbarch_tdep *tdep = gdbarch_tdep (gdbarch);
  const struct target_desc *tdesc = info.target_desc;
  struct tdesc_arch_data *tdesc_data = (void *)info.tdep_info;
  const struct tdesc_feature *feature;
  int num_core_regs = 0;

  gdb_assert (tdesc_data);

  linux_init_abi (info, gdbarch);

  tdep->gregset_reg_offset = k1om_linux_gregset_reg_offset;
  tdep->gregset_num_regs = ARRAY_SIZE (k1om_linux_gregset_reg_offset);
  tdep->sizeof_gregset = 27 * 8;

  k1om_init_abi (info, gdbarch);

  if (!tdesc_has_registers (tdesc))
    tdesc = tdesc_k1om_linux;
  tdep->tdesc = tdesc;

  feature = tdesc_find_feature (tdesc, "org.gnu.gdb.i386.core");
  if (feature == NULL)
    return;

  tdep->register_reggroup_p = k1om_linux_register_reggroup_p;

  num_core_regs = tdesc_feature_num_registers (feature);

  /* Remaining registers are populated in tdesc_use_registers and
     num_regs is updated accordingly. */
  set_gdbarch_num_regs (gdbarch, num_core_regs);

  set_gdbarch_dwarf2_reg_to_regnum (gdbarch, k1om_dwarf_reg_to_regnum);

  /* GNU/Linux uses SVR4-style shared libraries.  */
  set_solib_svr4_fetch_link_map_offsets
    (gdbarch, svr4_lp64_fetch_link_map_offsets);

  set_gdbarch_skip_trampoline_code (gdbarch, find_solib_trampoline_target);

  /* Add the %orig_rax register used for syscall restarting.  */
  set_gdbarch_write_pc (gdbarch, k1om_linux_write_pc);

  /* Enable TLS support.  */
  set_gdbarch_fetch_tls_load_module_address (gdbarch,
                                             svr4_fetch_objfile_link_map);

  /* Displaced stepping.  */
  set_gdbarch_displaced_step_copy_insn (gdbarch,
                                        amd64_displaced_step_copy_insn);
  set_gdbarch_displaced_step_fixup (gdbarch, amd64_displaced_step_fixup);
  set_gdbarch_displaced_step_free_closure (gdbarch,
                                           simple_displaced_step_free_closure);
  set_gdbarch_displaced_step_location (gdbarch,
                                       displaced_step_at_entry_point);

  set_gdbarch_get_siginfo_type (gdbarch, linux_get_siginfo_type);

  set_gdbarch_core_regset_sections (gdbarch, k1om_linux_regset_sections);

  set_gdbarch_core_read_description (gdbarch,
				     k1om_linux_core_read_description);
}

void
_initialize_k1om_linux_tdep (void)
{
  gdbarch_register_osabi (bfd_arch_k1om, bfd_mach_k1om,
                          GDB_OSABI_LINUX, k1om_linux_init_abi);


  /* Initialize the Linux target description.  */
  initialize_tdesc_k1om_linux ();
}
