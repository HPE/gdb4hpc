#ifndef __FAKE_AARCH64_ANDROIDEABI_ASM_H__
#define __FAKE_AARCH64_ANDROIDEABI_ASM_H__

#include <asm/ptrace.h>

/* Type for a general-purpose register.  */
typedef unsigned long elf_greg_t;

/* And the whole bunch of them.  We could have used `struct
   pt_regs' directly in the typedef, but tradition says that
   the register set is an array, which does have some peculiar
   semantics, so leave it that way.  */
#define ELF_NGREG (sizeof (struct user_pt_regs) / sizeof(elf_greg_t))
typedef elf_greg_t elf_gregset_t[ELF_NGREG];

/* Register set for the floating-point registers.  */
typedef struct user_fpsimd_state elf_fpregset_t;

#define NT_FPREGSET	2		/* Contains copy of fpregset struct */


#endif /* __FAKE_AARCH64_ANDROIDEABI_ASM_H__ */
