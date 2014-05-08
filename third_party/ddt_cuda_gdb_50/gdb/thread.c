/* Multi-process/thread control for GDB, the GNU debugger.

   Copyright (C) 1986, 1987, 1988, 1993, 1994, 1995, 1996, 1997, 1998, 1999,
   2000, 2001, 2002, 2003, 2004, 2007, 2008, 2009, 2010
   Free Software Foundation, Inc.

   Contributed by Lynx Real-Time Systems, Inc.  Los Gatos, CA.

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

/*
 * NVIDIA CUDA Debugger CUDA-GDB Copyright (C) 2007-2012 NVIDIA Corporation
 * Modified from the original GDB file referenced above by the CUDA-GDB 
 * team at NVIDIA <cudatools@nvidia.com>.
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

#include "defs.h"
#include "symtab.h"
#include "frame.h"
#include "inferior.h"
#include "environ.h"
#include "value.h"
#include "target.h"
#include "gdbthread.h"
#include "exceptions.h"
#include "command.h"
#include "gdbcmd.h"
#include "regcache.h"
#include "gdb.h"
#include "gdb_string.h"

#include <ctype.h>
#include <sys/types.h>
#include <signal.h>
#include "ui-out.h"
#include "observer.h"
#include "annotate.h"
#include "cli/cli-decode.h"
#include "upc-thread.h"
#include "block.h"
#include "dictionary.h"
#include "gdbtypes.h"
#include "language.h"
#include "valprint.h"

/* Definition of struct thread_info exported to gdbthread.h */

/* Prototypes for exported functions. */

void _initialize_thread (void);

/* Prototypes for local functions. */

struct thread_info *thread_list = NULL;
int highest_thread_num;

static void thread_command (char *tidstr, int from_tty);
static void thread_apply_all_command (char *, int);
static int thread_alive (struct thread_info *);
static void info_threads_command (char *, int);
static void thread_apply_command (char *, int);
void restore_current_thread (ptid_t);
void prune_threads (void);
static void restore_selected_frame (struct frame_id a_frame_id, int frame_level);

/* Frontend view of the thread state.  Possible extensions: stepping,
   finishing, until(ling),...  */
enum thread_state
{
  THREAD_STOPPED,
  THREAD_RUNNING,
  THREAD_EXITED,
};

struct thread_info*
inferior_thread (void)
{
  struct thread_info *tp = find_thread_ptid (inferior_ptid);
  gdb_assert (tp);
  return tp;
}

void
delete_step_resume_breakpoint (struct thread_info *tp)
{
  if (tp && tp->step_resume_breakpoint)
    {
      delete_breakpoint (tp->step_resume_breakpoint);
      tp->step_resume_breakpoint = NULL;
    }
}

static void
clear_thread_inferior_resources (struct thread_info *tp)
{
  /* NOTE: this will take care of any left-over step_resume breakpoints,
     but not any user-specified thread-specific breakpoints.  We can not
     delete the breakpoint straight-off, because the inferior might not
     be stopped at the moment.  */
  if (tp->step_resume_breakpoint)
    {
      tp->step_resume_breakpoint->disposition = disp_del_at_next_stop;
      tp->step_resume_breakpoint = NULL;
    }

  bpstat_clear (&tp->stop_bpstat);

  discard_all_intermediate_continuations_thread (tp);
  discard_all_continuations_thread (tp);
}

static void
free_thread (struct thread_info *tp)
{
  clear_thread_inferior_resources (tp);

  if (tp->private)
    {
      if (tp->private_dtor)
	tp->private_dtor (tp->private);
      else
	xfree (tp->private);
    }

  xfree (tp);
}

void
init_thread_list (void)
{
  struct thread_info *tp, *tpnext;

  highest_thread_num = 0;

  if (!thread_list)
    return;

  for (tp = thread_list; tp; tp = tpnext)
    {
      tpnext = tp->next;
      free_thread (tp);
    }

  thread_list = NULL;
}

/* Allocate a new thread with target id PTID and add it to the thread
   list.  */

static struct thread_info *
new_thread (ptid_t ptid)
{
  struct thread_info *tp;

  tp = xcalloc (1, sizeof (*tp));

  tp->ptid = ptid;
  tp->num = ++highest_thread_num;
  tp->next = thread_list;
  thread_list = tp;

  /* Nothing to follow yet.  */
  tp->pending_follow.kind = TARGET_WAITKIND_SPURIOUS;
  tp->state_ = THREAD_STOPPED;

  /* No collective breakpoint and step */
  tp->collective_bp_num = 0;
  tp->collective_step = 0;

  tp->selected_frame_level = -1;

  return tp;
}

struct thread_info *
add_thread_silent (ptid_t ptid)
{
  struct thread_info *tp;

  tp = find_thread_ptid (ptid);
  if (tp)
    /* Found an old thread with the same id.  It has to be dead,
       otherwise we wouldn't be adding a new thread with the same id.
       The OS is reusing this id --- delete it, and recreate a new
       one.  */
    {
      /* In addition to deleting the thread, if this is the current
	 thread, then we need to take care that delete_thread doesn't
	 really delete the thread if it is inferior_ptid.  Create a
	 new template thread in the list with an invalid ptid, switch
	 to it, delete the original thread, reset the new thread's
	 ptid, and switch to it.  */

      if (ptid_equal (inferior_ptid, ptid))
	{
	  tp = new_thread (null_ptid);

	  /* Make switch_to_thread not read from the thread.  */
	  tp->state_ = THREAD_EXITED;
	  switch_to_thread (null_ptid);

	  /* Now we can delete it.  */
	  delete_thread (ptid);

	  /* Now reset its ptid, and reswitch inferior_ptid to it.  */
	  tp->ptid = ptid;
	  tp->state_ = THREAD_STOPPED;
	  switch_to_thread (ptid);

	  observer_notify_new_thread (tp);

	  /* All done.  */
	  return tp;
	}
      else
	/* Just go ahead and delete it.  */
	delete_thread (ptid);
    }

  tp = new_thread (ptid);
  observer_notify_new_thread (tp);

  return tp;
}

struct thread_info *
add_thread_with_info (ptid_t ptid, struct private_thread_info *private)
{
  struct thread_info *result = add_thread_silent (ptid);

  result->private = private;

  if (print_thread_events)
    printf_unfiltered (_("[New %s]\n"), target_pid_to_str (ptid));

  annotate_new_thread ();
  return result;
}

struct thread_info *
add_thread (ptid_t ptid)
{
  return add_thread_with_info (ptid, NULL);
}

/* Delete thread PTID.  If SILENT, don't notify the observer of this
   exit.  */
static void
delete_thread_1 (ptid_t ptid, int silent)
{
  struct thread_info *tp, *tpprev;

  tpprev = NULL;

  for (tp = thread_list; tp; tpprev = tp, tp = tp->next)
    if (ptid_equal (tp->ptid, ptid))
      break;

  if (!tp)
    return;

  /* If this is the current thread, or there's code out there that
     relies on it existing (refcount > 0) we can't delete yet.  Mark
     it as exited, and notify it.  */
  if (tp->refcount > 0
      || ptid_equal (tp->ptid, inferior_ptid))
    {
      if (tp->state_ != THREAD_EXITED)
	{
	  observer_notify_thread_exit (tp, silent);

	  /* Tag it as exited.  */
	  tp->state_ = THREAD_EXITED;

	  /* Clear breakpoints, etc. associated with this thread.  */
	  clear_thread_inferior_resources (tp);
	}

       /* Will be really deleted some other time.  */
       return;
     }

  if (tpprev)
    tpprev->next = tp->next;
  else
    thread_list = tp->next;

  /* Notify thread exit, but only if we haven't already.  */
  if (tp->state_ != THREAD_EXITED)
    observer_notify_thread_exit (tp, silent);

  free_thread (tp);
}

/* Delete thread PTID and notify of thread exit.  If this is
   inferior_ptid, don't actually delete it, but tag it as exited and
   do the notification.  If PTID is the user selected thread, clear
   it.  */
void
delete_thread (ptid_t ptid)
{
  delete_thread_1 (ptid, 0 /* not silent */);
}

void
delete_thread_silent (ptid_t ptid)
{
  delete_thread_1 (ptid, 1 /* silent */);
}

struct thread_info *
find_thread_id (int num)
{
  struct thread_info *tp;

  for (tp = thread_list; tp; tp = tp->next)
    if (tp->num == num)
      return tp;

  return NULL;
}

/* Find a thread_info by matching PTID.  */
struct thread_info *
find_thread_ptid (ptid_t ptid)
{
  struct thread_info *tp;

  for (tp = thread_list; tp; tp = tp->next)
    if (ptid_equal (tp->ptid, ptid))
      return tp;

  return NULL;
}

/*
 * Thread iterator function.
 *
 * Calls a callback function once for each thread, so long as
 * the callback function returns false.  If the callback function
 * returns true, the iteration will end and the current thread
 * will be returned.  This can be useful for implementing a 
 * search for a thread with arbitrary attributes, or for applying
 * some operation to every thread.
 *
 * FIXME: some of the existing functionality, such as 
 * "Thread apply all", might be rewritten using this functionality.
 */

struct thread_info *
iterate_over_threads (int (*callback) (struct thread_info *, void *),
		      void *data)
{
  struct thread_info *tp, *next;

  for (tp = thread_list; tp; tp = next)
    {
      next = tp->next;
      if (!upcmode || is_upc_thread(tp))
        if ((*callback) (tp, data))
 	  return tp;
    }

  return NULL;
}

struct thread_info *
iterate_over_inferior_threads (int pid,
			       int (*callback) (struct thread_info *, void *),
			       void *data)
{
  struct thread_info *tp, *next;

  for (tp = thread_list; tp; tp = next)
    {
      next = tp->next;
      if (ptid_get_pid (tp->ptid) != pid)
        continue;
      if (!upcmode || is_upc_thread(tp))
        if ((*callback) (tp, data))
 	  return tp;
    }

  return NULL;
}

int
thread_count (void)
{
  int result = 0;
  struct thread_info *tp;

  for (tp = thread_list; tp; tp = tp->next)
    ++result;

  return result;  
}

int
valid_thread_id (int num)
{
  struct thread_info *tp;

  for (tp = thread_list; tp; tp = tp->next)
    if (tp->num == num)
      return 1;

  return 0;
}

int
pid_to_thread_id (ptid_t ptid)
{
  struct thread_info *tp;

  for (tp = thread_list; tp; tp = tp->next)
    if (ptid_equal (tp->ptid, ptid))
      return tp->num;

  return 0;
}

ptid_t
thread_id_to_pid (int num)
{
  struct thread_info *thread = find_thread_id (num);

  if (thread)
    return thread->ptid;
  else
    return pid_to_ptid (-1);
}

int
in_thread_list (ptid_t ptid)
{
  struct thread_info *tp;

  for (tp = thread_list; tp; tp = tp->next)
    if (ptid_equal (tp->ptid, ptid))
      return 1;

  return 0;			/* Never heard of 'im */
}

/* Finds the first thread of the inferior given by PID.  If PID is -1,
   return the first thread in the list.  */

struct thread_info *
first_thread_of_process (int pid)
{
  struct thread_info *tp, *ret = NULL;

  for (tp = thread_list; tp; tp = tp->next)
    if (pid == -1 || ptid_get_pid (tp->ptid) == pid)
      if (ret == NULL || tp->num < ret->num)
	ret = tp;

  return ret;
}

struct thread_info *
any_thread_of_process (int pid)
{
  struct thread_info *tp;

  for (tp = thread_list; tp; tp = tp->next)
    if (ptid_get_pid (tp->ptid) == pid)
      return tp;

  return NULL;
}

struct thread_info *
any_live_thread_of_process (int pid)
{
  struct thread_info *tp;
  struct thread_info *tp_running = NULL;

  for (tp = thread_list; tp; tp = tp->next)
    if (ptid_get_pid (tp->ptid) == pid)
      {
	if (tp->state_ == THREAD_STOPPED)
	  return tp;
	else if (tp->state_ == THREAD_RUNNING)
	  tp_running = tp;
      }

  return tp_running;
}

/* Print a list of thread ids currently known, and the total number of
   threads. To be used from within catch_errors. */
static int
do_captured_list_thread_ids (struct ui_out *uiout, void *arg)
{
  struct thread_info *tp;
  int num = 0;
  struct cleanup *cleanup_chain;
  int current_thread = -1;

  update_thread_list ();

  cleanup_chain = make_cleanup_ui_out_tuple_begin_end (uiout, "thread-ids");

  for (tp = thread_list; tp; tp = tp->next)
    {
      if (tp->state_ == THREAD_EXITED)
	continue;

      if (ptid_equal (tp->ptid, inferior_ptid))
	current_thread = tp->num;

      num++;
      ui_out_field_int (uiout, "thread-id", tp->num);
    }

  do_cleanups (cleanup_chain);

  if (current_thread != -1)
    ui_out_field_int (uiout, "current-thread-id", current_thread);
  ui_out_field_int (uiout, "number-of-threads", num);
  return GDB_RC_OK;
}

/* Official gdblib interface function to get a list of thread ids and
   the total number. */
enum gdb_rc
gdb_list_thread_ids (struct ui_out *uiout, char **error_message)
{
  if (catch_exceptions_with_msg (uiout, do_captured_list_thread_ids, NULL,
				 error_message, RETURN_MASK_ALL) < 0)
    return GDB_RC_FAIL;
  return GDB_RC_OK;
}

/* Return true if TP is an active thread. */
static int
thread_alive (struct thread_info *tp)
{
  if (tp->state_ == THREAD_EXITED)
    return 0;
  if (!target_thread_alive (tp->ptid))
    return 0;
  return 1;
}

void
prune_threads (void)
{
  struct thread_info *tp, *next;

  for (tp = thread_list; tp; tp = next)
    {
      next = tp->next;
      if (!thread_alive (tp))
	delete_thread (tp->ptid);
    }
}

void
thread_change_ptid (ptid_t old_ptid, ptid_t new_ptid)
{
  struct inferior *inf;
  struct thread_info *tp;

  /* It can happen that what we knew as the target inferior id
     changes.  E.g, target remote may only discover the remote process
     pid after adding the inferior to GDB's list.  */
  inf = find_inferior_pid (ptid_get_pid (old_ptid));
  inf->pid = ptid_get_pid (new_ptid);

  tp = find_thread_ptid (old_ptid);
  tp->ptid = new_ptid;

  observer_notify_thread_ptid_changed (old_ptid, new_ptid);
}

void
set_running (ptid_t ptid, int running)
{
  struct thread_info *tp;
  int all = ptid_equal (ptid, minus_one_ptid);

  /* We try not to notify the observer if no thread has actually changed 
     the running state -- merely to reduce the number of messages to 
     frontend.  Frontend is supposed to handle multiple *running just fine.  */
  if (all || ptid_is_pid (ptid))
    {
      int any_started = 0;

      for (tp = thread_list; tp; tp = tp->next)
	if (all || ptid_get_pid (tp->ptid) == ptid_get_pid (ptid))
	  {
	    if (tp->state_ == THREAD_EXITED)
	      continue;
	    if (running && tp->state_ == THREAD_STOPPED)
	      any_started = 1;
	    tp->state_ = running ? THREAD_RUNNING : THREAD_STOPPED;
	  }
      if (any_started)
	observer_notify_target_resumed (ptid);
    }
  else
    {
      int started = 0;

      tp = find_thread_ptid (ptid);
      gdb_assert (tp);
      gdb_assert (tp->state_ != THREAD_EXITED);
      if (running && tp->state_ == THREAD_STOPPED)
 	started = 1;
      tp->state_ = running ? THREAD_RUNNING : THREAD_STOPPED;
      if (started)
  	observer_notify_target_resumed (ptid);
    }
}

static int
is_thread_state (ptid_t ptid, enum thread_state state)
{
  struct thread_info *tp;

  tp = find_thread_ptid (ptid);
  /* There is sometimes a window between inferior_ptid being set and the thread
     actually being added to the list.  */
  if (!tp)
    return state == THREAD_RUNNING;
  gdb_assert (tp);
  return tp->state_ == state;
}

int
is_stopped (ptid_t ptid)
{
  return is_thread_state (ptid, THREAD_STOPPED);
}

int
is_exited (ptid_t ptid)
{
  return is_thread_state (ptid, THREAD_EXITED);
}

int
is_running (ptid_t ptid)
{
  return is_thread_state (ptid, THREAD_RUNNING);
}

int
any_running (void)
{
  struct thread_info *tp;

  for (tp = thread_list; tp; tp = tp->next)
    if (tp->state_ == THREAD_RUNNING)
      return 1;

  return 0;
}

int
is_executing (ptid_t ptid)
{
  struct thread_info *tp;

  tp = find_thread_ptid (ptid);
  gdb_assert (tp);
  return tp->executing_;
}

void
set_executing (ptid_t ptid, int executing)
{
  struct thread_info *tp;
  int all = ptid_equal (ptid, minus_one_ptid);

  if (all || ptid_is_pid (ptid))
    {
      for (tp = thread_list; tp; tp = tp->next)
	if (all || ptid_get_pid (tp->ptid) == ptid_get_pid (ptid))
	  {
	    tp->executing_ = executing;
	    tp->selected_frame_level = -1;
	  }
    }
  else
    {
      tp = find_thread_ptid (ptid);
      gdb_assert (tp);
      tp->executing_ = executing;
      tp->selected_frame_level = -1;
    }
}

void
set_stop_requested (ptid_t ptid, int stop)
{
  struct thread_info *tp;
  int all = ptid_equal (ptid, minus_one_ptid);

  if (all || ptid_is_pid (ptid))
    {
      for (tp = thread_list; tp; tp = tp->next)
	if (all || ptid_get_pid (tp->ptid) == ptid_get_pid (ptid))
	  tp->stop_requested = stop;
    }
  else
    {
      tp = find_thread_ptid (ptid);
      if (!tp)
        warning(_("set_stop_requested: failed to find thread `%s'."), target_pid_to_str (ptid));
      else
        tp->stop_requested = stop;
    }

  /* Call the stop requested observer so other components of GDB can
     react to this request.  */
  if (stop)
    observer_notify_thread_stop_requested (ptid);
}

void
finish_thread_state (ptid_t ptid)
{
  struct thread_info *tp;
  int all;
  int any_started = 0;

  all = ptid_equal (ptid, minus_one_ptid);

  if (all || ptid_is_pid (ptid))
    {
      for (tp = thread_list; tp; tp = tp->next)
	{
 	  if (tp->state_ == THREAD_EXITED)
  	    continue;
	  if (all || ptid_get_pid (ptid) == ptid_get_pid (tp->ptid))
	    {
	      if (tp->executing_ && tp->state_ == THREAD_STOPPED)
		any_started = 1;
	      tp->state_ = tp->executing_ ? THREAD_RUNNING : THREAD_STOPPED;
	    }
	}
    }
  else
    {
      tp = find_thread_ptid (ptid);
      gdb_assert (tp);
      if (tp->state_ != THREAD_EXITED)
	{
	  if (tp->executing_ && tp->state_ == THREAD_STOPPED)
	    any_started = 1;
	  tp->state_ = tp->executing_ ? THREAD_RUNNING : THREAD_STOPPED;
	}
    }

  if (any_started)
    observer_notify_target_resumed (ptid);
}

void
finish_thread_state_cleanup (void *arg)
{
  ptid_t *ptid_p = arg;

  gdb_assert (arg);

  finish_thread_state (*ptid_p);
}

/* Prints the list of threads and their details on UIOUT.
   This is a version of 'info_thread_command' suitable for
   use from MI.  
   If REQUESTED_THREAD is not -1, it's the GDB id of the thread
   that should be printed.  Otherwise, all threads are
   printed.  
   If PID is not -1, only print threads from the process PID.
   Otherwise, threads from all attached PIDs are printed.   
   If both REQUESTED_THREAD and PID are not -1, then the thread
   is printed if it belongs to the specified process.  Otherwise,
   an error is raised.  */
void
print_thread_info (struct ui_out *uiout, int requested_thread, int pid)
{
  struct thread_info *tp;
  ptid_t current_ptid;
  struct cleanup *old_chain;
  char *extra_info;
  int current_thread = -1;

  update_thread_list ();

  if (upcmode && (thread_count() != 1))
    { /* make sure UPC thread is selected if in UPC mode */
      tp = inferior_thread ();
      if (!is_upc_thread(tp))
        {
          if (upc_thread0)
            switch_to_thread (upc_thread0->ptid);
        }
    }
  current_ptid = inferior_ptid;

  /* We'll be switching threads temporarily.  */
  old_chain = make_cleanup_restore_current_thread ();

  make_cleanup_ui_out_list_begin_end (uiout, "threads");
  for (tp = thread_list; tp; tp = tp->next)
    {
      struct cleanup *chain2;
      int core;

      if (requested_thread != -1 && tp->num != requested_thread)
	continue;

      if (upcmode)
        { /* Do not show non UPC threads */
          if (!is_upc_thread(tp))
            continue;
        }

      if (pid != -1 && PIDGET (tp->ptid) != pid)
	{
	  if (requested_thread != -1)
	    error (_("Requested thread not found in requested process"));
	  continue;
	}

      if (ptid_equal (tp->ptid, current_ptid))
	current_thread = tp->num;

      if (tp->state_ == THREAD_EXITED)
	continue;

      chain2 = make_cleanup_ui_out_tuple_begin_end (uiout, NULL);

      if (ptid_equal (tp->ptid, current_ptid) &&
          !cuda_focus_is_device () /* CUDA - focus */)
	ui_out_text (uiout, "* ");
      else
	ui_out_text (uiout, "  ");

      if (upcmode)
        ui_out_field_int (uiout, "id", tp->unum);
      else
        ui_out_field_int (uiout, "id", tp->num);
      ui_out_text (uiout, " ");
      ui_out_field_string (uiout, "target-id", target_pid_to_str (tp->ptid));

      extra_info = target_extra_thread_info (tp);
      if (extra_info)
	{
	  ui_out_text (uiout, " (");
	  ui_out_field_string (uiout, "details", extra_info);
	  ui_out_text (uiout, ")");
	}
      ui_out_text (uiout, "  ");

      if (tp->state_ == THREAD_RUNNING)
	ui_out_text (uiout, "(running)\n");
      else
	{
	  /* The switch below puts us at the top of the stack (leaf
	     frame).  */

          switch_to_thread (tp->ptid);
	  print_stack_frame (get_selected_frame (NULL),
			     /* For MI output, print frame level.  */
			     ui_out_is_mi_like_p (uiout),
			     LOCATION);
	}

      if (ui_out_is_mi_like_p (uiout))
	{
	  char *state = "stopped";

	  if (tp->state_ == THREAD_RUNNING)
	    state = "running";
	  ui_out_field_string (uiout, "state", state);
	}

      core = target_core_of_thread (tp->ptid);
      if (ui_out_is_mi_like_p (uiout) && core != -1)
	ui_out_field_int (uiout, "core", core);

      do_cleanups (chain2);
    }

  /* Restores the current thread and the frame selected before
     the "info threads" command.  */
  do_cleanups (old_chain);

  if (pid == -1 && requested_thread == -1)
    {
      gdb_assert (current_thread != -1
		  || !thread_list
		  || ptid_equal (inferior_ptid, null_ptid));
      if (current_thread != -1 && ui_out_is_mi_like_p (uiout))
	ui_out_field_int (uiout, "current-thread-id", current_thread);

      if (current_thread != -1 && is_exited (current_ptid))
	ui_out_message (uiout, 0, "\n\
The current thread <Thread ID %d> has terminated.  See `help thread'.\n",
			current_thread);
      else if (thread_list
	       && current_thread == -1
	       && ptid_equal (current_ptid, null_ptid))
	ui_out_message (uiout, 0, "\n\
No selected thread.  See `help thread'.\n");
    }
}


/* Print information about currently known threads 

 * Note: this has the drawback that it _really_ switches
 *       threads, which frees the frame cache.  A no-side
 *       effects info-threads command would be nicer.
 */

static void
info_threads_command (char *arg, int from_tty)
{
  print_thread_info (uiout, -1, -1);
}

/* Switch from one thread to another. */

void
switch_to_thread (ptid_t ptid)
{
  struct thread_info *tp;
  struct frame_info *frame;
  
  if (!ptid_equal (inferior_ptid, null_ptid) && !ptid_equal (inferior_ptid, ptid))
    {
      tp = find_thread_ptid (inferior_ptid);
      if (tp)
        {
	  if (is_stopped (inferior_ptid)
	      && target_has_registers
	      && target_has_stack
	      && target_has_memory)
	    {
	      volatile struct gdb_exception ex;

	      TRY_CATCH (ex, RETURN_MASK_ALL)
		{
		  struct frame_info *frame;
		  frame = get_selected_frame (NULL);
		  tp->selected_frame_id = get_frame_id (frame);
		  tp->selected_frame_level = frame_relative_level (frame);
		}
	      if (ex.reason < 0)
		{
		  tp->selected_frame_level = -1;
		}
	    }
	  else
	    tp->selected_frame_level = -1;
	}
    }

  /* Switch the program space as well, if we can infer it from the now
     current thread.  Otherwise, it's up to the caller to select the
     space it wants.  */
  if (!ptid_equal (ptid, null_ptid))
    {
      struct inferior *inf;

      inf = find_inferior_pid (ptid_get_pid (ptid));
      gdb_assert (inf != NULL);
      set_current_program_space (inf->pspace);
      set_current_inferior (inf);
    }

  /* CUDA - focus */
  /* We can only compare ptid equivalence (for early exit) if
     we're not focused on the device, as otherwise it can prevent
     the full switch from happening when changing focus from the
     device to a host thread (as the host thread ID may not have
     changed based on the user's switch). */
  if (!cuda_focus_is_device ())
    if (ptid_equal (ptid, inferior_ptid))
      return;

  /* CUDA - focus */
  cuda_coords_invalidate_current ();
  
  inferior_ptid = ptid;
  reinit_frame_cache ();
  registers_changed ();

  /* We don't check for is_stopped, because we're called at times
     while in the TARGET_RUNNING state, e.g., while handling an
     internal event.  */
  if (!ptid_equal (inferior_ptid, null_ptid)
      && !is_exited (ptid)
      && !is_executing (ptid))
    stop_pc = regcache_read_pc (get_thread_regcache (ptid));
  else
    stop_pc = ~(CORE_ADDR) 0;

  if (!ptid_equal (inferior_ptid, null_ptid))
    {
      tp = find_thread_ptid (inferior_ptid);
      if (tp)
        {
	  if (is_stopped (inferior_ptid)
	      && target_has_registers
	      && target_has_stack
	      && target_has_memory
	      && tp->selected_frame_level != -1)
	    restore_selected_frame (tp->selected_frame_id, tp->selected_frame_level);
	}
    }
}

void
restore_current_thread (ptid_t ptid)
{
  switch_to_thread (ptid);
}

static void
restore_selected_frame (struct frame_id a_frame_id, int frame_level)
{
  struct frame_info *frame = NULL;
  int count;

  gdb_assert (frame_level >= 0);

  /* Restore by level first, check if the frame id is the same as
     expected.  If that fails, try restoring by frame id.  If that
     fails, nothing to do, just warn the user.  */

  count = frame_level;
  frame = find_relative_frame (get_current_frame (), &count);
  if (count == 0
      && frame != NULL
      /* The frame ids must match - either both valid or both outer_frame_id.
	 The latter case is not failsafe, but since it's highly unlikely
	 the search by level finds the wrong frame, it's 99.9(9)% of
	 the time (for all practical purposes) safe.  */
      && frame_id_eq (get_frame_id (frame), a_frame_id))
    {
      /* Cool, all is fine.  */
      select_frame (frame);
      return;
    }

  frame = frame_find_by_id (a_frame_id);
  if (frame != NULL)
    {
      /* Cool, refound it.  */
      select_frame (frame);
      return;
    }

  /* Nothing else to do, the frame layout really changed.  Select the
     innermost stack frame.  */
  select_frame (get_current_frame ());

  /* Warn the user.  */
  if (frame_level > 0 && !ui_out_is_mi_like_p (uiout))
    {
      warning (_("\
Couldn't restore frame #%d in current thread, at reparsed frame #0\n"),
	       frame_level);
      /* For MI, we should probably have a notification about
	 current frame change.  But this error is not very
	 likely, so don't bother for now.  */
      print_stack_frame (get_selected_frame (NULL), 1, SRC_LINE);
    }
}

struct current_thread_cleanup
{
  ptid_t inferior_ptid;
  struct frame_id selected_frame_id;
  int selected_frame_level;
  int was_stopped;
  int inf_id;
  /* CUDA - focus */
  cuda_coords_t cuda_coords;
};

static void
do_restore_current_thread_cleanup (void *arg)
{
  struct thread_info *tp;
  struct current_thread_cleanup *old = arg;

  tp = find_thread_ptid (old->inferior_ptid);

  /* If the previously selected thread belonged to a process that has
     in the mean time been deleted (due to normal exit, detach, etc.),
     then don't revert back to it, but instead simply drop back to no
     thread selected.  */
  if (tp
           && find_inferior_pid (ptid_get_pid (tp->ptid)) != NULL)
    restore_current_thread (old->inferior_ptid);
  else
    {
      restore_current_thread (null_ptid);
      /* all our inferiors might be gone */
      if (find_inferior_id (old->inf_id))
        set_current_inferior (find_inferior_id (old->inf_id));
    }

  /* CUDA - focus */
  if (old->cuda_coords.valid)
    {
      cuda_coords_set_current (&old->cuda_coords);
      switch_to_cuda_thread (NULL);
    }

  /* The running state of the originally selected thread may have
     changed, so we have to recheck it here.  */
  if (!ptid_equal (inferior_ptid, null_ptid)
      && old->was_stopped
      && is_stopped (inferior_ptid)
      && target_has_registers
      && target_has_stack
      && target_has_memory
      && old->selected_frame_level != -1)
    restore_selected_frame (old->selected_frame_id,
			    old->selected_frame_level);
}

static void
restore_current_thread_cleanup_dtor (void *arg)
{
  struct current_thread_cleanup *old = arg;
  struct thread_info *tp;

  tp = find_thread_ptid (old->inferior_ptid);
  if (tp)
    tp->refcount--;
  xfree (old);
}

struct cleanup *
make_cleanup_restore_current_thread (void)
{
  struct thread_info *tp;
  struct frame_info *frame;
  struct current_thread_cleanup *old;

  old = xmalloc (sizeof (struct current_thread_cleanup));
  old->inferior_ptid = inferior_ptid;
  old->inf_id = current_inferior ()->num;
  /* CUDA - focus */
  old->cuda_coords = CUDA_INVALID_COORDS;
  cuda_coords_get_current (&old->cuda_coords);

  if (!ptid_equal (inferior_ptid, null_ptid))
    {
      old->was_stopped = is_stopped (inferior_ptid);
      if (old->was_stopped
	  && target_has_registers
	  && target_has_stack
	  && target_has_memory)
	frame = get_selected_frame (NULL);
      else
	frame = NULL;

      old->selected_frame_id = get_frame_id (frame);
      old->selected_frame_level = frame_relative_level (frame);

      tp = find_thread_ptid (inferior_ptid);
      if (tp)
	tp->refcount++;
    }

  return make_cleanup_dtor (do_restore_current_thread_cleanup, old,
			    restore_current_thread_cleanup_dtor);
}

/* Apply a GDB command to a list of threads.  List syntax is a whitespace
   seperated list of numbers, or ranges, or the keyword `all'.  Ranges consist
   of two numbers seperated by a hyphen.  Examples:

   thread apply 1 2 7 4 backtrace       Apply backtrace cmd to threads 1,2,7,4
   thread apply 2-7 9 p foo(1)  Apply p foo(1) cmd to threads 2->7 & 9
   thread apply all p x/i $pc   Apply x/i $pc cmd to all threads
 */

static void
thread_apply_all_command (char *cmd, int from_tty)
{
  struct thread_info *tp;
  struct cleanup *old_chain;
  char *saved_cmd;

  if (cmd == NULL || *cmd == '\000')
    error (_("Please specify a command following the thread ID list"));

  update_thread_list ();

  old_chain = make_cleanup_restore_current_thread ();

  /* Save a copy of the command in case it is clobbered by
     execute_command */
  saved_cmd = xstrdup (cmd);
  make_cleanup (xfree, saved_cmd);
  for (tp = thread_list; tp; tp = tp->next)
    {
      if (upcmode && !is_upc_thread(tp)) continue;
      if (thread_alive (tp))
        {
	  switch_to_thread (tp->ptid);

          if (upcmode)
	    printf_filtered (_("\nUPC thread %d:\n"), tp->unum);
          else
	    printf_filtered (_("\nThread %d (%s):\n"),
			 tp->num, target_pid_to_str (inferior_ptid));
	  execute_command (cmd, from_tty);
	  strcpy (cmd, saved_cmd);	/* Restore exact command used previously */
        }
    }

  do_cleanups (old_chain);
}

static void
thread_apply_command (char *tidlist, int from_tty)
{
  char *cmd;
  char *p;
  struct cleanup *old_chain;
  char *saved_cmd;

  if (tidlist == NULL || *tidlist == '\000')
    error (_("Please specify a thread ID list"));

  for (cmd = tidlist; *cmd != '\000' && !isalpha (*cmd); cmd++);

  if (*cmd == '\000')
    error (_("Please specify a command following the thread ID list"));

  /* Save a copy of the command in case it is clobbered by
     execute_command */
  saved_cmd = xstrdup (cmd);
  old_chain = make_cleanup (xfree, saved_cmd);
  while (tidlist < cmd)
    {
      struct thread_info *tp;
      int start, end;

      start = strtol (tidlist, &p, 10);
      if (p == tidlist)
	error (_("Error parsing %s"), tidlist);
      tidlist = p;

      while (*tidlist == ' ' || *tidlist == '\t')
	tidlist++;

      if (*tidlist == '-')	/* Got a range of IDs? */
	{
	  tidlist++;		/* Skip the - */
	  end = strtol (tidlist, &p, 10);
	  if (p == tidlist)
	    error (_("Error parsing %s"), tidlist);
	  tidlist = p;

	  while (*tidlist == ' ' || *tidlist == '\t')
	    tidlist++;
	}
      else
	end = start;

      make_cleanup_restore_current_thread ();

      for (; start <= end; start++)
	{
          int vtid = valid_input_thread_id (start);
	  tp = find_thread_id (vtid);
          if (upcmode && !is_upc_thread(tp)) continue;

	  if (!tp)
	    warning (_("Unknown thread %d."), start);
	  else if (!thread_alive (tp))
	    warning (_("Thread %d has terminated."), start);
	  else
	    {
	      switch_to_thread (tp->ptid);

              if (upcmode)
	        printf_filtered (_("\nUPC thread %d:\n"), tp->unum);
              else
	        printf_filtered (_("\nThread %d (%s):\n"), tp->num,
			       target_pid_to_str (inferior_ptid));
	      execute_command (cmd, from_tty);

	      /* Restore exact command used previously.  */
	      strcpy (cmd, saved_cmd);
	    }
	}
    }

  do_cleanups (old_chain);
}

/* Switch to the specified thread.  Will dispatch off to thread_apply_command
   if prefix of arg is `apply'.  */

static void
thread_command (char *tidstr, int from_tty)
{
  if (!tidstr)
    {
      if (ptid_equal (inferior_ptid, null_ptid))
	error (_("No thread selected"));
      
      /* CUDA - focus */
      if (cuda_focus_is_device ())
        {
          printf_unfiltered (_("Focus not set on any host thread.\n"));
          return;
        }

      if (target_has_stack)
	{
	  if (is_exited (inferior_ptid))
            if (upcmode)
	      printf_filtered (_("[Current UPC thread is %d (exited)]\n"),
			     show_thread_id (pid_to_thread_id (inferior_ptid)));
            else
	      printf_filtered (_("[Current thread is %d (%s) (exited)]\n"),
			     show_thread_id (pid_to_thread_id (inferior_ptid)),
			     target_pid_to_str (inferior_ptid));
	  else
            if (upcmode)
	      printf_filtered (_("[Current UPC thread is %d]\n"),
			     show_thread_id (pid_to_thread_id (inferior_ptid)));
            else
	      printf_filtered (_("[Current thread is %d (%s)]\n"),
			     show_thread_id (pid_to_thread_id (inferior_ptid)),
			     target_pid_to_str (inferior_ptid));
	}
      else
        error (_("No stack."));
      return;
    }

  gdb_thread_select (uiout, tidstr, NULL);
}

/* Print notices when new threads are attached and detached.  */
int print_thread_events = 1;
static void
show_print_thread_events (struct ui_file *file, int from_tty,
                          struct cmd_list_element *c, const char *value)
{
  fprintf_filtered (file, _("\
Printing of thread events is %s.\n"),
                    value);
}

static int
do_captured_thread_select (struct ui_out *uiout, void *tidstr)
{
  int num, input_num;
  struct thread_info *tp;

  input_num = value_as_long (parse_and_eval (tidstr));
  num = valid_input_thread_id (input_num);

  tp = find_thread_id (num);

  if (!tp)
    error (_("Thread ID %d not known."), input_num);

  if (!thread_alive (tp))
    error (_("Thread ID %d has terminated."), input_num);

  if (upcmode && !is_upc_thread(tp))
    error (_("Thread ID %d is not UPC thread."), input_num);

  if (current_inferior())
    current_inferior()->last_thread = inferior_ptid;
  
  switch_to_thread (tp->ptid);

  annotate_thread_changed ();

  ui_out_text (uiout, "[Switching to thread ");
  ui_out_field_int (uiout, "new-thread-id", show_thread_id (pid_to_thread_id (inferior_ptid)));
  ui_out_text (uiout, " (");
  ui_out_text (uiout, target_pid_to_str (inferior_ptid));
  ui_out_text (uiout, ")]");

  /* Note that we can't reach this with an exited thread, due to the
     thread_alive check above.  */
  if (tp->state_ == THREAD_RUNNING)
    ui_out_text (uiout, "(running)\n");
  else
    print_stack_frame (get_selected_frame (NULL), 1, SRC_AND_LOC);

  /* Since the current thread may have changed, see if there is any
     exited thread we can now delete.  */
  prune_threads ();

  return GDB_RC_OK;
}

enum gdb_rc
gdb_thread_select (struct ui_out *uiout, char *tidstr, char **error_message)
{
  if (catch_exceptions_with_msg (uiout, do_captured_thread_select, tidstr,
				 error_message, RETURN_MASK_ALL) < 0)
    return GDB_RC_FAIL;
  return GDB_RC_OK;
}

void
update_thread_list (void)
{
  prune_threads ();
  target_find_new_threads ();
}

/* Return a new value for the selected thread's id.  Return a value of 0 if
   no thread is selected, or no threads exist.  */

static struct value *
thread_id_make_value (struct gdbarch *gdbarch, struct internalvar *var)
{
  struct thread_info *tp = find_thread_ptid (inferior_ptid);

  return value_from_longest (builtin_type (gdbarch)->builtin_int,
			     (tp ? tp->num : 0));
}

#define COLLECTIVE_BP_NOWAIT 0

/* Collective breakpoints - clear threads from collective wait.
   If bptno is 0 then clear any thread that is waiting, otherwise
   clear only threads waiting on bptno breakpoint. */
int
thread_clear_collective_bp(int bptno)
{
  struct thread_info *tp;
  int cleared_bpts = 0;

  for (tp = thread_list; tp; tp = tp->next)
    {
      if (bptno != CLEAR_ALL_COLLECTIVE_BPS)
        {
          if (tp->collective_bp_num == bptno)
            {
              tp->collective_bp_num = COLLECTIVE_BP_NOWAIT;
              cleared_bpts++;
            }
        }
      else
        {
          tp->collective_bp_num = COLLECTIVE_BP_NOWAIT;
          cleared_bpts = 1;
        }
    }   
  return cleared_bpts;
}

/* Collective breakpoints - check for breakpoint missmatch */
int
thread_check_collective_bp (int bpnum)
{
  struct thread_info *tp;
  for (tp = thread_list; tp; tp = tp->next)
    {
      if (tp->collective_bp_num &&
          (tp->collective_bp_num != bpnum))
        return 1;
    }   
  return 0;
}

/* Collective step - check if threads completed a step */
int
thread_check_collective_step ()
{
  struct thread_info *tp;
  for (tp = thread_list; tp; tp = tp->next)
    {
      if (tp->collective_step) return 0;
    }   
  return 1;
}

/* Commands with a prefix of `thread'.  */
struct cmd_list_element *thread_cmd_list = NULL;

void
_initialize_thread (void)
{
  static struct cmd_list_element *thread_apply_list = NULL;

  add_info ("threads", info_threads_command,
	    _("IDs of currently known threads."));

  add_prefix_cmd ("thread", class_run, thread_command, _("\
Use this command to switch between threads.\n\
The new thread ID must be currently known."),
		  &thread_cmd_list, "thread ", 1, &cmdlist);

  add_prefix_cmd ("apply", class_run, thread_apply_command,
		  _("Apply a command to a list of threads."),
		  &thread_apply_list, "thread apply ", 1, &thread_cmd_list);

  add_cmd ("all", class_run, thread_apply_all_command,
	   _("Apply a command to all threads."), &thread_apply_list);

  if (!xdb_commands)
    add_com_alias ("t", "thread", class_run, 1);

  add_setshow_boolean_cmd ("thread-events", no_class,
         &print_thread_events, _("\
Set printing of thread events (such as thread start and exit)."), _("\
Show printing of thread events (such as thread start and exit)."), NULL,
         NULL,
         show_print_thread_events,
         &setprintlist, &showprintlist);

  create_internalvar_type_lazy ("_thread", thread_id_make_value);
}


/* Allinea. Search through the frame stack, look for frame with
 * hmpp_codelet_. Start from the currently selected frame (if we
 * have just switch to this thread, current frame will be at the
 * top of the frame stack) */
static struct frame_info*
caps_hmpp_find_codelet_frame()
{
  struct symtab_and_line sal;
  struct frame_info *frame = get_selected_frame (NULL);

  while(frame)
    {
      /* Find name of the function we are in */
      struct symbol *func;
      func = find_pc_function (get_frame_address_in_block (frame));

      if (func) /* If we can find the function */
        {
          struct minimal_symbol *msymbol =
              lookup_minimal_symbol_by_pc (get_frame_address_in_block (frame));

          if (msymbol != NULL) /* If we can get a symbol for the function */
            {
              char* funname = SYMBOL_LINKAGE_NAME (msymbol);
              if (funname!=NULL && strncmp("hmpp_codelet_",funname,13)==0)
                {
                  /* We have found that 'frame' is in function starting with 'hmpp_codelet_' */
                  if (strncmp("hmpp_codelet_",funname,13)==0)
                    {
                      return frame;
                    }
                }
            }
        }
      frame = get_prev_frame(frame);
    }

  /* Failed to find codelet frame */
  return NULL;
}


/* Allinea. Cache the thread identifier that contains caps hmpp metadata */
static ptid_t caps_hmpp_metadata_thread;
/* Allinea. Flag used to prevent caps_hmpp_metadata_thread being used before
 * it is set a real value. False if caps_hmpp_metadata_thread is unset, true
 * once it has been set. */
static bool caps_hmpp_metadata_thread_set = false;


/* Allinea. Search through the non-cuda threads for a frame
 * beginning with hmpp_codelet. If found select this frame
 * and return true, otherwise return false.
 *
 * Note: Whatever the return value, this function will change
 * the currently selected thread and frame. */
static bool
caps_hmpp_switch_to_metadata_thread_and_frame()
{
  struct thread_info *tp;
  prune_threads ();

  if (caps_hmpp_metadata_thread_set && in_thread_list(caps_hmpp_metadata_thread)) /* Check cached thread ptid is still valid */
    {
      struct frame_info* frame;
      switch_to_thread(caps_hmpp_metadata_thread);

      frame = caps_hmpp_find_codelet_frame();
      if (frame)
        {
          select_frame(frame);
          return true;
        }
    }

  /* caps_hmpp_metadata_thread is either NULL or does not contain
   * a codelet frame. Search through all threads for one containing
   * hmpp_codelet. */
  target_find_new_threads ();

  for (tp = thread_list; tp; tp = tp->next)
    {
      struct frame_info* frame;
      switch_to_thread (tp->ptid);
      frame = caps_hmpp_find_codelet_frame();
      if (frame)
        {
          caps_hmpp_metadata_thread = tp->ptid;
          caps_hmpp_metadata_thread_set = true;
          select_frame(frame);
          return true;
        }
    }

  /* Failed to find thread containing a codelet frame */
  return false;
}


/* Allinea. Takes as input a struct val pointer to an array
 * containing a hmpp array's shape metadata, and the type
 * of the array the metadata refers to. Returns the type
 * created from the metadata, or NULL if *val does not have
 * the expected contents. */
static struct type*
caps_hmpp_read_metadata(struct value *val,struct type *target_type)
{
  /* Check the type of this metadata variable */
  struct type *type = value_type(val);
  if (type!=NULL && TYPE_CODE(type)==TYPE_CODE_ARRAY)
    {
      struct type *elttype = check_typedef (TYPE_TARGET_TYPE (type));
      if (TYPE_LENGTH (type) > 0 && TYPE_LENGTH (TYPE_TARGET_TYPE (type)) > 0)
        {
          enum bfd_endian order;
          unsigned eltlen = TYPE_LENGTH (elttype);
          unsigned length = TYPE_LENGTH (type) / eltlen;
          int dim;
          const gdb_byte *valaddr = value_contents (val) + value_embedded_offset (val);

          /* First index of array is number of dimensions, remaining elements in pairs, giving
           * the lower and upper bounds */
          int number_dims;
          order = gdbarch_byte_order (get_current_arch());
          number_dims = (int) extract_signed_integer(valaddr,eltlen, order);
          if (length != number_dims*2 +1)
            {
              warning("caps_hmpp_read_metadata: unexpected length of metadata array. length=%i, dimensions (first element)=%i, expected length=%i\n",
                  length, number_dims, number_dims*2+1);
              return NULL;
            }

          struct type *result_type = check_typedef(target_type);

          /* Create type for multidimensional array */
          for (dim=0; dim<number_dims; ++dim)
            {
              int low_bound  = (int) extract_signed_integer(valaddr + (1 + dim*2) * eltlen,eltlen, order);
              int high_bound = (int) extract_signed_integer(valaddr + (2 + dim*2) * eltlen,eltlen, order);

              struct type *range_type = create_range_type(NULL,elttype,low_bound,high_bound);
              result_type = create_array_type(NULL,result_type,range_type);
            }

          return result_type;
        }
      else
        {
          /* Can't get the length of the array of metadata - give up */
          warning("caps_hmpp_read_metadata: cannot get length of metadata array");
          return NULL;
        }
    }
  else
    {
      /* *val does not refer to an array */
      warning("caps_hmpp_read_metadata: Not an array");
      return NULL;
    }
}


/* Allinea. Searches through the non-cuda threads for a frame
 * in a method beginning with hmpp_codelet, and extracts
 * the requested variable (metadata_name) from the locals.
 * This is then converted into a type that is returned.
 *
 * Based on info_threads_command. As with that command, this
 * has the drawback that it _really_ switches threads, which
 * frees the frame cache.  A no-side effects implementation
 * would be nicer.  */
struct type*
caps_hmpp_create_type_from_metadata(char* metadata_name, struct type *target_type)
{
  struct value_print_options user_options;
  unsigned int old_print_max;
  unsigned int old_print_smax;
  struct type *result_type;
  struct frame_id saved_frame_id;
  ptid_t current_ptid;
  cuda_coords_t current_cuda_coords;
  bool foundMetadata;
  get_user_print_options(&user_options);
  old_print_max = user_options.print_max;
  old_print_smax = user_options.print_smax;

  result_type = NULL;

  saved_frame_id = get_frame_id (get_selected_frame (NULL));
  current_ptid = inferior_ptid;

  current_cuda_coords = CUDA_INVALID_COORDS;
  cuda_coords_get_current (&current_cuda_coords);

  foundMetadata = caps_hmpp_switch_to_metadata_thread_and_frame();
  if (!foundMetadata)
    {
      warning (_("caps_hmpp_create_type_from_metadata: Unable to find hmpp_codelet_ thread/frame"));
    }
  else
    {
      struct frame_info *frame = get_selected_frame (NULL);
      struct block *block = get_frame_block (frame, 0);
      if (block != NULL) /* If NULL, then symbol table not available */
        {
          struct dict_iterator iter;
          struct symbol *sym;

          while (block)
            {
              ALL_BLOCK_SYMBOLS (block, iter, sym)
                {
                  if(strcmp(SYMBOL_PRINT_NAME (sym),metadata_name)==0)
                    {
                      /* Found metadata symbol. Extract data and break out of
                       * loop if successful. */
                      result_type = caps_hmpp_read_metadata(read_var_value (sym, frame),target_type);
                      if (result_type!=NULL)
                        break;
                    }
                }
              /* After handling the function's top-level block, stop.  Don't
               * continue to its superblock, the block of per-file symbols.  */
              if (BLOCK_FUNCTION (block))
                break;
              block = BLOCK_SUPERBLOCK (block);
            }
        }
    }

  /* If the current focus is on a CUDA kernel, make sure we save off
     the current device frame id so that it can be properly restored
     after the 'info cuda threads' command has executed. */
  if (current_cuda_coords.valid)
    {
      cuda_coords_set_current (&current_cuda_coords);
      switch_to_cuda_thread (NULL);
      saved_frame_id = get_frame_id (get_selected_frame (NULL));
    }
  else
    switch_to_thread (current_ptid);

  /* Restores the frame set by the user. Switching back to the current thread has
   * put us at the top of the stack (leaf frame).  */
  struct frame_info *cur_frame = frame_find_by_id (saved_frame_id);
  if (cur_frame == NULL)
    {
      /* Ooops, can't restore, tell user where we are.  */
      warning (_("caps_hmpp_create_type_from_metadata: Couldn't restore frame in current thread, at frame 0"));
      print_stack_frame (get_selected_frame (NULL), 0, LOCATION);
    }
  else
    {
      select_frame (cur_frame);
    }

  user_options.print_max = old_print_max;
  user_options.print_smax = old_print_smax;

  return result_type;
}
