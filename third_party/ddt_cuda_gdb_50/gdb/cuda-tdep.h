/*
 * NVIDIA CUDA Debugger CUDA-GDB Copyright (C) 2007-2012 NVIDIA Corporation
 * Written by CUDA-GDB team at NVIDIA <cudatools@nvidia.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _CUDA_TDEP_H
#define _CUDA_TDEP_H 1

#include "bfd.h"
#include "elf-bfd.h"
#include "defs.h"
#include "cudadebugger.h"
#include "gdbarch.h"
#include "dis-asm.h"
#include "environ.h"
#include "cuda-api.h"
#include "cuda-coords.h"
#include "cuda-defs.h"
#include "cuda-kernel.h"
#include "cuda-modules.h"
#include "progspace.h"

bool is_caps_hmpp_debugging(); /* Allinea */

extern bool cuda_elf_path; /* REMOVE THIS ONCE CUDA ELF PATH IS COMPLETE! */

/* CUDA - skip prologue
   REMOVE ONCE TRANSITION TESLA KERNELS HAVE PROLOGUES ALL THE TIME */
extern bool cuda_producer_is_open64;

#define DEFAULT_PROMPT   "(cuda-gdb) "
#define GDBINIT_FILENAME ".cuda-gdbinit"

/*---------------------------- CUDA ELF Specification --------------------------*/

#define EV_CURRENT                   1
#define ELFOSABI_CUDA             0x33
#define CUDA_ELFOSABIV_16BIT         0  /* 16-bit ctaid.x size */
#define CUDA_ELFOSABIV_32BIT         1  /* 32-bit ctaid.x size */
#define CUDA_ELFOSABIV_RELOC         2  /* ELFOSABIV_32BIT + All relocators in DWARF */
#define CUDA_ELFOSABIV_ABI           3  /* ELFOSABIV_RELOC + Calling Convention */
#define CUDA_ELFOSABIV_SYSCALL       4  /* ELFOSABIV_ABI + Improved syscall relocation */
#define CUDA_ELFOSABIV_SEPCOMP       5  /* ELFOSABIV_SYSCALL + new caller-callee save conventions */
#define CUDA_ELFOSABIV_ABI3          6  /* ELFOSABIV_SEPCOMP + fixes */
#define CUDA_ELFOSABIV_LATEST        CUDA_ELFOSABIV_ABI3
#define CUDA_ELF_TEXT_PREFIX  ".text."  /* CUDA ELF text section format: ".text.KERNEL" */

/*Return values that exceed 384-bits in size are returned in memory.
   (R4-R15 = 12 4-byte registers = 48-bytes = 384-bits that can be
   used to return values in registers). */
#define CUDA_ABI_MAX_REG_RV_SIZE  48 /* Size in bytes */

/*------------------------------ Type Declarations -----------------------------*/

typedef struct {
  bool valid;
  uint32_t value;
  bool recoverable;
} cuda_exception_t;

extern cuda_exception_t cuda_exception;

typedef enum {
  cuda_bp_none = 0,
  cuda_bp_runtime_api, /* Transition from host stub code to device code */
  cuda_bp_driver_api,  /* Always dynamically resolved (initially pending) */
} cuda_bptype_t;

#define CUDA_MAX_NUM_RESIDENT_BLOCKS_PER_GRID 256
#define CUDA_MAX_NUM_RESIDENT_THREADS_PER_BLOCK 1024
#define CUDA_MAX_NUM_RESIDENT_THREADS (CUDA_MAX_NUM_RESIDENT_BLOCKS_PER_GRID * CUDA_MAX_NUM_RESIDENT_THREADS_PER_BLOCK)

typedef enum return_value_convention rvc_t;

extern CuDim3 gridDim;
extern CuDim3 blockDim;

typedef bool (*cuda_thread_func)(cuda_coords_t *, void *);

/* Linked list of warps for a specific sm, used when doing a multiple-warp sstep */
struct cuda_sstep_multiple_warp_data
{
  struct cuda_sstep_multiple_warp_data* next; /* Linked list of warps */

  uint32_t wp_id;       /* id of the warp this record represents */
  CORE_ADDR start;      /* start pc for line of source to step over*/
  CORE_ADDR end;        /*   end pc for line of source to step over */
  bool should_step;     /* does this warp need to be stepped? */
};

/* Element in linked list holding data on which warps to step when doing a mulitple-warp sstep */
struct cuda_sstep_multiple_warp_element
{
  struct cuda_sstep_multiple_warp_element* next; /* Linked list of blocks */
  struct cuda_sstep_multiple_warp_data* warp;    /* Linked list of warps for this block */

  uint32_t grid_id;
  uint32_t dev_id;
  uint32_t sm_id;
  uint64_t warp_mask;
};

/* The list of warps to step in the current multiple-warp sstep */
extern struct cuda_sstep_multiple_warp_element* cuda_sstep_multiple_warp_stepping_list;

/*------------------------------ Global Variables ------------------------------*/

extern bool cuda_debugging_enabled;
extern bool cuda_is_target_mourn_pending;
struct gdbarch * cuda_get_gdbarch (void);
bool cuda_is_cuda_gdbarch (struct gdbarch *);

cuda_coords_t cuda_coords_current;

/* Offsets of the CUDA built-in variables */
#define CUDBG_BUILTINS_BASE                        ((CORE_ADDR) 0)
#define CUDBG_THREADIDX_OFFSET           (CUDBG_BUILTINS_BASE - 6)
#define CUDBG_BLOCKIDX_OFFSET         (CUDBG_THREADIDX_OFFSET - 6)
#define CUDBG_BLOCKDIM_OFFSET          (CUDBG_BLOCKIDX_OFFSET - 6)
#define CUDBG_GRIDDIM_OFFSET           (CUDBG_BLOCKDIM_OFFSET - 6)
#define CUDBG_WARPSIZE_OFFSET          (CUDBG_GRIDDIM_OFFSET - 32)
#define CUDBG_BUILTINS_MAX                 (CUDBG_WARPSIZE_OFFSET)

/*----------- Prototypes to avoid implicit declarations (hack-hack) ------------*/

extern bool cuda_initialized;

struct partial_symtab;
void switch_to_cuda_thread (cuda_coords_t *coords);
int  cuda_thread_select (char *, int);
void cuda_update_cudart_symbols (void);
void cuda_cleanup_cudart_symbols (void);
void cuda_update_convenience_variables (void);
void cuda_set_environment (struct gdb_environ *);

/*-------------------------------- Prototypes ----------------------------------*/

int  cuda_startup (void);
void cuda_kill (void);
void cuda_cleanup (void);
void cuda_final_cleanup (void *unused);
void cuda_initialize_target (void);
bool cuda_inferior_in_debug_mode (void);
void cuda_inferior_update_suspended_devices_mask (void);
void cuda_load_device_info (char *, struct partial_symtab *);

char *   cuda_find_kernel_name_from_pc (CORE_ADDR pc, bool demangle);
bool     cuda_breakpoint_hit_p (cuda_clock_t clock);
bool     cuda_exception_hit_p (cuda_exception_t *exception);
const char * cuda_exception_type_to_name (CUDBGException_t exception_type);

uint64_t cuda_get_last_driver_api_error_code (void);
void     cuda_get_last_driver_api_error_func_name (char **name);
uint64_t cuda_get_last_driver_internal_error_code (void);

/*Debugging */
void cuda_trace (char *fmt, ...);

/*----------------------------------------------------------------------------*/

/*Single-Stepping */
bool     cuda_sstep_is_active (void);
uint32_t cuda_sstep_dev_id (void);
uint32_t cuda_sstep_grid_id (void);
ptid_t   cuda_sstep_ptid (void);
void     cuda_sstep_set_ptid (ptid_t ptid);
void     cuda_sstep_initialize (bool stepping);
void     cuda_sstep_execute (ptid_t ptid);
void     cuda_sstep_reset (bool sstep);
bool     cuda_sstep_kernel_has_terminated (void);

struct cuda_sstep_multiple_warp_element* cuda_sstep_multiple_warp_find_element(uint32_t dev, uint32_t sm, uint32_t grid);
uint64_t cuda_sstep_multiple_warp_create_warp_mask(struct cuda_sstep_multiple_warp_element* elem);
void cuda_sstep_multiple_warp_reset_warp_masks();
void cuda_sstep_multiple_warp_initialize();
void cuda_sstep_multiple_warp_clear();

/*Registers */
bool          cuda_get_dwarf_register_string (reg_t reg, char *deviceReg, size_t sz);

/*Storage addresses and names */
void        cuda_print_lmem_address_type (void);
int         cuda_address_class_type_flags (int byte_size, int addr_class);

/*ABI/BFD/ELF/DWARF/objfile calls */
int             cuda_inferior_word_size ();
bool            cuda_is_bfd_cuda (bfd *obfd);
bool            cuda_is_bfd_version_call_abi (bfd *obfd);
bool            cuda_get_bfd_abi_version (bfd *obfd, unsigned int *abi_version);
bool            cuda_current_active_elf_image_uses_abi (void);
void            cuda_update_elf_images (void);
CORE_ADDR       cuda_dwarf2_func_baseaddr (struct objfile *objfile, char *func_name);
bool            cuda_find_function_pc_from_objfile (struct objfile *objfile, char *func_name, CORE_ADDR *func_addr);
bool            cuda_find_func_text_vma_from_objfile (struct objfile *objfile, char *func_name, CORE_ADDR *vma);
uint64_t        cuda_find_context_id (CORE_ADDR addr);
context_t       cuda_find_context_by_addr (CORE_ADDR addr);
bool            cuda_is_device_code_address (CORE_ADDR addr);
int             cuda_abi_sp_regnum (struct gdbarch *);
int             cuda_special_regnum (struct gdbarch *);
int             cuda_pc_regnum (struct gdbarch *);
CORE_ADDR       cuda_get_symbol_address (char *name);

/*Segmented memory reads/writes */
int cuda_read_memory_partial (CORE_ADDR address, gdb_byte *buf, int len, struct type *type);
void cuda_read_memory  (CORE_ADDR address, struct value *val, struct type *type, int len);
int cuda_write_memory_partial (CORE_ADDR address, const gdb_byte *buf, struct type *type);
void cuda_write_memory (CORE_ADDR address, const gdb_byte *buf, struct type *type);

/*Breakpoints */
void cuda_resolve_breakpoints (elf_image_t elf_image);
void cuda_unresolve_breakpoints (uint64_t context);
void cuda_reset_invalid_breakpoint_location_section (struct objfile *objfile);
int cuda_breakpoint_address_match (struct gdbarch *gdbarch,
                                   struct address_space *aspace1, CORE_ADDR addr1,
                                   struct address_space *aspace2, CORE_ADDR addr2);

/* Linux vs. Mac OS X */
bool cuda_platform_supports_tid (void);
int  cuda_gdb_get_tid (ptid_t ptid);
int  cuda_get_signo (void);
void cuda_set_signo (int signo);

/* Session Management */
int         cuda_gdb_session_create (void);
void        cuda_gdb_session_destroy (void);
const char *cuda_gdb_session_get_dir (void);
uint32_t    cuda_gdb_session_get_id (void);

/* Attach support */
void cuda_nat_attach (void);
 
#endif

