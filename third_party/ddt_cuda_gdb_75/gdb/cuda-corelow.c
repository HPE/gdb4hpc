/*
 * NVIDIA CUDA Debugger CUDA-GDB Copyright (C) 2014 NVIDIA Corporation
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

#include "cuda-corelow.h"

#include "target.h"
#include "gdbthread.h"
#include "regcache.h"
#include "completer.h"
#include "readline/readline.h"
#include "gdb_assert.h"

#include "cuda-api.h"
#include "cuda-tdep.h"
#include "cuda-events.h"
#include "cuda-state.h"
#include "cuda-exceptions.h"
#include "cuda-context.h"
#include "cuda-iterator.h"
#include "cuda-linux-nat.h"

#include "../libcudacore/libcudacore.h"

void _initialize_cuda_corelow (void);

struct target_ops cuda_core_ops;
static CudaCore *cuda_core = NULL;

static void cuda_core_close (int quitting);

static void cuda_core_close_cleanup (void *ignore);

static int
cuda_core_has_memory (struct target_ops *ops)
{
  return true;
}

static int
cuda_core_has_stack (struct target_ops *ops)
{
  return true;
}

static int
cuda_core_has_registers (struct target_ops *ops)
{
  return true;
}

static int
cuda_core_thread_alive (struct target_ops *ops, ptid_t ptid)
{
  return 1;
}

static char *
cuda_core_pid_to_str (struct target_ops *ops, ptid_t ptid)
{
  static char buf[64];

  xsnprintf (buf, sizeof buf, "Thread %ld", ptid.tid);
  return buf;
}

void
cuda_core_fetch_registers (struct target_ops *ops,
                           struct regcache *regcache, int regno)
{
  cuda_coords_t c;
  unsigned reg_no, reg_value, num_regs;
  uint64_t pc;
  struct gdbarch *gdbarch = cuda_get_gdbarch();
  uint32_t pc_regnum = gdbarch ? gdbarch_pc_regnum (gdbarch): 256;


  if (cuda_coords_get_current (&c))
    return;

  num_regs = device_get_num_registers (c.dev);
  for (reg_no = 0; reg_no < num_regs; ++reg_no)
    {
      reg_value = lane_get_register (c.dev, c.sm, c.wp, c.ln, reg_no);
      regcache_raw_supply (regcache, reg_no, &reg_value);
    }

  /* Save PC as well */
  pc = lane_get_virtual_pc (c.dev, c.sm, c.wp, c.ln);
  regcache_raw_supply (regcache, pc_regnum, &pc);
}

#define CUDA_CORE_PID 966617

void
cuda_core_register_tid (uint32_t tid)
{
  ptid_t ptid;

  if (!ptid_equal (inferior_ptid, null_ptid))
    return;

  ptid.pid = CUDA_CORE_PID;
  ptid.lwp = tid;
  ptid.tid = tid;
  add_thread (ptid);
  inferior_ptid = ptid;
}

void
cuda_core_load_api (char *filename)
{
  CUDBGAPI api;

  printf_unfiltered (_("Opening GPU coredump: %s\n"), filename);

  cuda_core = cuCoreOpenByName (filename);
  if (cuda_core == NULL)
    error ("Failed to read core file: %s", cuCoreErrorMsg());
  api = cuCoreGetApi (cuda_core);
  if (api == NULL)
    error ("Failed to get debugger APIs: %s", cuCoreErrorMsg());

  cuda_api_set_api (api);

  /* Initialize the APIs */
  cuda_initialize ();
  if (!cuda_initialized)
    error ("Failed to initialize CUDA Core debugger API!");

  /* Set debuggers architecture to CUDA */
  set_target_gdbarch (cuda_get_gdbarch ());
}

void
cuda_core_free (void)
{
  if (cuda_core == NULL)
    return;

  cuda_cleanup ();
  cuda_gdb_session_destroy ();
  cuCoreFree(cuda_core);
  cuda_core = NULL;
}

void
cuda_core_initialize_events_exceptions (void)
{
  CUDBGEvent event;

  /* Flush registers cache */
  registers_changed ();

  /* Create session directory */
  if (cuda_gdb_session_create ())
    error ("Failed to create session directory");

  /* Drain the event queue */
  while (true) {
    cuda_api_get_next_sync_event (&event);

    if (event.kind == CUDBG_EVENT_INVALID)
      break;

    if (event.kind == CUDBG_EVENT_CTX_CREATE)
      cuda_core_register_tid (event.cases.contextCreate.tid);

    cuda_process_event (&event);
  }

  /* Figure out, where exception happened */
  if (cuda_exception_hit_p (cuda_exception))
    {
      uint64_t kernelId;
      cuda_coords_t c = cuda_exception_get_coords (cuda_exception);

      cuda_coords_set_current (&c);

      /* Set the current coordinates context to current */
      if (!cuda_coords_get_current_logical (&kernelId, NULL, NULL, NULL))
        {
          kernel_t kernel = kernels_find_kernel_by_kernel_id (kernelId);
          context_t ctx = kernel ? kernel_get_context (kernel) : get_current_context ();
          if (ctx != NULL)
             set_current_context (ctx);
        }

      cuda_exception_print_message (cuda_exception);
    }

  /* Fetch latest information about coredump grids */
  kernels_update_args ();
}

static void
cuda_find_first_valid_lane (void)
{
  cuda_iterator itr;
  cuda_coords_t c;
  itr = cuda_iterator_create (CUDA_ITERATOR_TYPE_THREADS, NULL,
                              CUDA_SELECT_VALID | CUDA_SELECT_SNGL);
  cuda_iterator_start (itr);
  c  = cuda_iterator_get_current (itr);
  cuda_iterator_destroy (itr);
  if (!c.valid)
    {
      cuda_coords_update_current (false, false);
      return;
    }
  cuda_coords_set_current (&c);
}

static void
cuda_core_open (char *filename, int from_tty)
{
  struct inferior *inf;
  struct cleanup *old_chain, *old_chain2;
  volatile struct gdb_exception e;

  target_preopen (from_tty);

  if (filename == NULL)
    error (_("No core file specified."));

  filename = tilde_expand (filename);
  old_chain = make_cleanup (xfree, filename);
  old_chain2 = make_cleanup (cuda_core_close_cleanup, 0 /*ignore*/);

  cuda_core_load_api (filename);

  TRY_CATCH (e, RETURN_MASK_ALL)
    {
      /* Push the target */
      push_target (&cuda_core_ops);

      /* Flush existing thread information */
      init_thread_list ();

      /* Switch focus to null ptid */
      inferior_ptid = null_ptid;

      /* Add fake PID*/
      inf = current_inferior();
      if (inf->pid == 0)
        {
          inferior_appeared (inf, CUDA_CORE_PID);
          inf->fake_pid_p = true;
        }

      post_create_inferior (&cuda_core_ops, from_tty);

      cuda_core_initialize_events_exceptions ();

      /* If no exception found try to set focus to first valid thread */
      if (!cuda_focus_is_device())
        {
          warning ("No exception was found on the device");
          cuda_find_first_valid_lane ();
        }

      if (!cuda_focus_is_device())
        throw_error (GENERIC_ERROR, "No focus could be set on device");

      cuda_print_message_focus (false);

      /* Fetch all registers from core file.  */
      target_fetch_registers (get_current_regcache (), -1);

      /* Now, set up the frame cache, and print the top of stack.  */
      reinit_frame_cache ();
      print_stack_frame (get_selected_frame (NULL), 1, SRC_AND_LOC);
    }
  if (e.reason < 0)
    {
      pop_all_targets_above (process_stratum - 1, 0);

      inferior_ptid = null_ptid;
      discard_all_inferiors ();

      registers_changed ();
      reinit_frame_cache ();
      cuda_cleanup ();

      error (_("Could not open CUDA core file: %s"), e.message);
    }

  discard_cleanups (old_chain2);
  do_cleanups (old_chain);
}

static void
cuda_core_close (int quitting)
{
  inferior_ptid = null_ptid;
  discard_all_inferiors ();

  cuda_core_free ();
}

static void
cuda_core_detach (struct target_ops *ops, char *args, int from_tty)
{
  if (args)
    error (_("Too many arguments"));
  unpush_target (ops);
  reinit_frame_cache ();
  if (from_tty)
    printf_filtered (_("No core file now.\n"));
}

static void
cuda_core_close_cleanup (void *ignore)
{
  cuda_core_close (0/*ignored*/);
}

/* Fill in cuda_core_ops */
static void
init_cuda_core_ops (void)
{
  cuda_core_ops.to_shortname = "cudacore";
  cuda_core_ops.to_longname = "CUDA core dump file";
  cuda_core_ops.to_doc =
    "Use CUDA core file as a target. Specify the filename to the core file.";
  cuda_core_ops.to_open = cuda_core_open;
  cuda_core_ops.to_detach = cuda_core_detach;
  cuda_core_ops.to_close = cuda_core_close;
  cuda_core_ops.to_has_memory = cuda_core_has_memory;
  cuda_core_ops.to_has_stack = cuda_core_has_stack;
  cuda_core_ops.to_has_registers = cuda_core_has_registers;
  cuda_core_ops.to_thread_alive = cuda_core_thread_alive;
  cuda_core_ops.to_fetch_registers = cuda_core_fetch_registers;
  cuda_core_ops.to_pid_to_str = cuda_core_pid_to_str;
  cuda_core_ops.to_stratum = process_stratum;
  cuda_core_ops.to_magic = OPS_MAGIC;
}

void
_initialize_cuda_corelow (void)
{
  struct cmd_list_element *c;

  init_cuda_core_ops ();

  c = add_target (&cuda_core_ops);
  set_cmd_completer (c, filename_completer);
}
