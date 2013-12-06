/*
 * NVIDIA CUDA Debugger CUDA-GDB Copyright (C) 2013 NVIDIA Corporation
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

#include <string.h>

#include "defs.h"
#include "gdb_assert.h"
#include "ui-out.h"

#include "cuda-coords.h"
#include "cuda-exceptions.h"
#include "cuda-iterator.h"
#include "cuda-state.h"

struct cuda_exception_st {
  bool valid;
  bool recoverable;
  uint32_t value;
  cuda_coords_t coords;
};

// XXX temporary, until we get list of exceptions instead
struct cuda_exception_st cuda_exception_object;
cuda_exception_t cuda_exception = &cuda_exception_object;

bool
cuda_exception_is_valid (cuda_exception_t exception)
{
  gdb_assert (exception);
  return exception->valid;
}

bool
cuda_exception_is_recoverable (cuda_exception_t exception)
{
  gdb_assert (exception);
  gdb_assert (exception->valid);
  return exception->recoverable;
}

uint32_t
cuda_exception_get_value (cuda_exception_t exception)
{
  gdb_assert (exception);
  gdb_assert (exception->valid);
  return exception->value;
}

cuda_coords_t
cuda_exception_get_coords (cuda_exception_t exception)
{
  gdb_assert (exception);
  gdb_assert (exception->valid);
  gdb_assert (exception->coords.valid);
  return exception->coords;
}

void
cuda_exception_reset (cuda_exception_t exception)
{
  gdb_assert (exception);
  exception->valid = false;
  exception->recoverable = false;
  exception->value = 0U;
  exception->coords.valid = false;
}

static void
cuda_print_memcheck_message (cuda_exception_t exception)
{
  cuda_coords_t c;
  uint64_t address = 0;
  ptxStorageKind storage;
  const char *addr_space = NULL;

  c = cuda_exception_get_coords (exception);
  address = lane_get_memcheck_error_address (c.dev, c.sm, c.wp, c.ln);
  storage = lane_get_memcheck_error_address_segment (c.dev, c.sm, c.wp, c.ln);

  if (!address)
    return;

  ui_out_text (uiout, "Memcheck detected an illegal access to address ");
  switch (storage)
    {
      case ptxGlobalStorage:
        addr_space = "@global";
        break;
      case ptxSharedStorage:
        addr_space = "@shared";
        break;
      case ptxLocalStorage:
        addr_space = "@local";
        break;
      default:
        addr_space = NULL;
        break;
    };

  if (addr_space)
    {
      ui_out_text (uiout, "(");
      ui_out_field_string (uiout, "error-address-space", addr_space);
      ui_out_text (uiout, ")");
    }
  ui_out_field_fmt (uiout, "address", "0x%"PRIx64,  address);
  ui_out_text (uiout, "\n");
}

static void
cuda_print_assert_message (cuda_exception_t exception)
{
  cuda_coords_t c;
  struct symtab_and_line sa;
  uint64_t phys_addr, virt_addr;
  kernel_t kernel;
  uint64_t start_pc;
  struct symbol *symbol;
  const char *func_name;
  struct symtab_and_line sal;

  c = cuda_exception_get_coords (exception);
  virt_addr = lane_get_virtual_pc (c.dev, c.sm, c.wp, c.ln);
  kernel = warp_get_kernel (c.dev, c.sm, c.wp);
  start_pc = kernel_get_virt_code_base (kernel);
  symbol = find_pc_function ((CORE_ADDR) virt_addr);
  func_name = cuda_find_function_name_from_pc (start_pc, false);
  sal = find_pc_line ((CORE_ADDR)virt_addr, 0);

  if (sal.symtab && sal.line)
    {
      char *file = strrchr (sal.symtab->filename, '/');

      if (file)
        ++file;
      else
        file = sal.symtab->filename;
      ui_out_text         (uiout, "\n");
      ui_out_field_string (uiout, "filename"    , file);
      ui_out_text         (uiout, ":");
      ui_out_field_int    (uiout, "line"        , sal.line);
      ui_out_text         (uiout, ": ");
      ui_out_field_string (uiout, "kernel"      , func_name);
      ui_out_text         (uiout, ": ");
      ui_out_text         (uiout, "Assertion failed at block (");
      ui_out_field_int    (uiout, "blockidx.x"  , c.blockIdx.x);
      ui_out_text         (uiout, ",");
      ui_out_field_int    (uiout, "blockidx.y"  , c.blockIdx.y);
      ui_out_text         (uiout, ",");
      ui_out_field_int    (uiout, "blockidx.z"  , c.blockIdx.z);
      ui_out_text         (uiout, "), thread (");
      ui_out_field_int    (uiout, "threadidx.x"  , c.threadIdx.x);
      ui_out_text         (uiout, ",");
      ui_out_field_int    (uiout, "threadidx.y"  , c.threadIdx.y);
      ui_out_text         (uiout, ",");
      ui_out_field_int    (uiout, "threadidx.z"  , c.threadIdx.z);
      ui_out_text         (uiout, ")\n");
    }
  else
    {
      ui_out_text         (uiout, "\n");
      ui_out_field_string (uiout, "kernel"      , func_name);
      ui_out_text         (uiout, ": ");
      ui_out_text         (uiout, "Assertion failed at block (");
      ui_out_field_int    (uiout, "blockidx.x"  , c.blockIdx.x);
      ui_out_text         (uiout, ",");
      ui_out_field_int    (uiout, "blockidx.y"  , c.blockIdx.y);
      ui_out_text         (uiout, ",");
      ui_out_field_int    (uiout, "blockidx.z"  , c.blockIdx.z);
      ui_out_text         (uiout, "), thread (");
      ui_out_field_int    (uiout, "threadidx.x"  , c.threadIdx.x);
      ui_out_text         (uiout, ",");
      ui_out_field_int    (uiout, "threadidx.y"  , c.threadIdx.y);
      ui_out_text         (uiout, ",");
      ui_out_field_int    (uiout, "threadidx.z"  , c.threadIdx.z);
      ui_out_text         (uiout, ")\n");
    }
}

static void
cuda_exception_print_generic_message (cuda_exception_t exception)
{
  //XXX
}

void
cuda_exception_print_message (cuda_exception_t exception)
{
  switch (cuda_exception_get_value (exception))
  {
    case TARGET_SIGNAL_CUDA_WARP_ASSERT:
        cuda_print_assert_message (exception);
        break;
    case TARGET_SIGNAL_CUDA_LANE_ILLEGAL_ADDRESS:
        cuda_print_memcheck_message (exception);
        break;
    default:
        cuda_exception_print_generic_message (exception);
        break;
  }
}

bool
cuda_exception_hit_p (cuda_exception_t exception)
{
  CUDBGException_t exception_type = CUDBG_EXCEPTION_NONE;
  cuda_coords_t c = CUDA_INVALID_COORDS, filter = CUDA_WILDCARD_COORDS;
  cuda_iterator itr;

  /* Iteration should be limited to single sm if sstep is active */
  if (cuda_sstep_is_active())
    {
      filter.dev = cuda_sstep_dev_id();
      filter.sm = cuda_sstep_sm_id();
      /* If only one bit is set in warp mask limit iteration to it */
      if (((1LL)<<cuda_sstep_wp_id()) == cuda_sstep_wp_mask())
        filter.wp = cuda_sstep_wp_id();
    }

  itr = cuda_iterator_create (CUDA_ITERATOR_TYPE_THREADS, &filter,
                               CUDA_SELECT_VALID | CUDA_SELECT_EXCPT | CUDA_SELECT_SNGL);
  cuda_iterator_start (itr);
  if (!cuda_iterator_end (itr))
    {
      c = cuda_iterator_get_current (itr);
      exception_type = lane_get_exception (c.dev, c.sm, c.wp, c.ln);
    }
  cuda_iterator_destroy (itr);

  exception->coords = c;

  switch (exception_type)
    {
    case CUDBG_EXCEPTION_LANE_ILLEGAL_ADDRESS:
      exception->value = TARGET_SIGNAL_CUDA_LANE_ILLEGAL_ADDRESS;
      exception->valid = true;
      exception->recoverable = false;
      break;
    case CUDBG_EXCEPTION_LANE_USER_STACK_OVERFLOW:
      exception->value = TARGET_SIGNAL_CUDA_LANE_USER_STACK_OVERFLOW;
      exception->valid = true;
      exception->recoverable = false;
      break;
    case CUDBG_EXCEPTION_DEVICE_HARDWARE_STACK_OVERFLOW:
      exception->value = TARGET_SIGNAL_CUDA_DEVICE_HARDWARE_STACK_OVERFLOW;
      exception->valid = true;
      exception->recoverable = false;
      break;
    case CUDBG_EXCEPTION_WARP_ILLEGAL_INSTRUCTION:
      exception->value = TARGET_SIGNAL_CUDA_WARP_ILLEGAL_INSTRUCTION;
      exception->valid = true;
      exception->recoverable = false;
      break;
    case CUDBG_EXCEPTION_WARP_OUT_OF_RANGE_ADDRESS:
      exception->value = TARGET_SIGNAL_CUDA_WARP_OUT_OF_RANGE_ADDRESS;
      exception->valid = true;
      exception->recoverable = false;
      break;
    case CUDBG_EXCEPTION_WARP_MISALIGNED_ADDRESS:
      exception->value = TARGET_SIGNAL_CUDA_WARP_MISALIGNED_ADDRESS;
      exception->valid = true;
      exception->recoverable = false;
      break;
    case CUDBG_EXCEPTION_WARP_INVALID_ADDRESS_SPACE:
      exception->value = TARGET_SIGNAL_CUDA_WARP_INVALID_ADDRESS_SPACE;
      exception->valid = true;
      exception->recoverable = false;
      break;
    case CUDBG_EXCEPTION_WARP_INVALID_PC:
      exception->value = TARGET_SIGNAL_CUDA_WARP_INVALID_PC;
      exception->valid = true;
      exception->recoverable = false;
      break;
    case CUDBG_EXCEPTION_WARP_HARDWARE_STACK_OVERFLOW:
      exception->value = TARGET_SIGNAL_CUDA_WARP_HARDWARE_STACK_OVERFLOW;
      exception->valid = true;
      exception->recoverable = false;
      break;
    case CUDBG_EXCEPTION_DEVICE_ILLEGAL_ADDRESS:
      exception->value = TARGET_SIGNAL_CUDA_DEVICE_ILLEGAL_ADDRESS;
      exception->valid = true;
      exception->recoverable = false;
      break;
    case CUDBG_EXCEPTION_WARP_ASSERT:
      exception->value = TARGET_SIGNAL_CUDA_WARP_ASSERT;
      exception->valid = true;
      exception->recoverable = true;
      break;
    case CUDBG_EXCEPTION_LANE_SYSCALL_ERROR:
      exception->value = TARGET_SIGNAL_CUDA_LANE_SYSCALL_ERROR;
      exception->valid = true;
      exception->recoverable = false;
      break;
    case CUDBG_EXCEPTION_WARP_ILLEGAL_ADDRESS:
      exception->value = TARGET_SIGNAL_CUDA_WARP_ILLEGAL_ADDRESS;
      exception->valid = true;
      exception->recoverable = false;
      break;
    case CUDBG_EXCEPTION_NONE:
      break;
    case CUDBG_EXCEPTION_UNKNOWN:
    default:
      /* If for some reason the device encounters an unknown exception, we
         still need to halt the chip and allow state inspection.  Just emit
         a warning indicating this is something that was unexpected, but
         handle it as a normal device exception. */
      warning ("Encountered unhandled device exception (%d)\n", exception_type);
      exception->value = TARGET_SIGNAL_CUDA_UNKNOWN_EXCEPTION;
      exception->valid = true;
      exception->recoverable = false;
      break;
    }

  return exception_type != CUDBG_EXCEPTION_NONE;
}

