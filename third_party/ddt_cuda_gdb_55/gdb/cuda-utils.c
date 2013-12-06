/*
 * NVIDIA CUDA Debugger CUDA-GDB Copyright (C) 2007-2013 NVIDIA Corporation
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

/* Utility functions for cuda-gdb */
#ifdef GDBSERVER
#include "gdb_locale.h"
#include "server.h"
#else
#include "gdb_assert.h"
#include "defs.h"
#include "inferior.h"
#include "gdb/signals.h"
#endif

#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>

#include <fcntl.h>
#include "cuda-utils.h"
#include "cuda-defs.h"

#define RECORD_FORMAT_MASTER   "LOCK:%10d\n"
#define RECORD_FORMAT_DEVICE    "%4d:%10d\n"
#define RECORD_SIZE                       16
#define RECORD_MASTER                      0
#define RECORD_DEVICE(i)           ((i) + 1)
#define DEVICE_RECORD(i)           ((i) - 1)

int cuda_use_lockfile = 1;

static const char cuda_gdb_lock_file[] = "cuda-gdb.lock";
static char cuda_gdb_tmp_basedir[CUDA_GDB_TMP_BUF_SIZE];
static int cuda_gdb_lock_fd;
static char* cuda_gdb_tmp_dir;
static uint64_t dev_mask;

int
cuda_gdb_dir_create (const char *dir_name, uint32_t permissions,
                     bool override_umask, bool *dir_exists)
{
  struct stat st;
  mode_t old_umask = 0;
  int ret = 0;

  if (stat (dir_name, &st) || !(S_ISDIR(st.st_mode)))
    {
      /* Save the old umask and reset it */
      if (override_umask)
        old_umask = umask (0);

      ret = mkdir (dir_name, permissions);

      /* Restore the old umask */
      if (override_umask)
        umask (old_umask);
    }
  else
    *dir_exists = true;

  return ret;
}

static void
cuda_gdb_tmpdir_create_basedir ()
{
  int ret = 0;
  bool dir_exists = false;
  bool override_umask = true;

  if (getenv ("TMPDIR"))
    snprintf (cuda_gdb_tmp_basedir, sizeof (cuda_gdb_tmp_basedir), 
              "%s/cuda-dbg", getenv ("TMPDIR"));
  else
    snprintf (cuda_gdb_tmp_basedir, sizeof (cuda_gdb_tmp_basedir),
              "/tmp/cuda-dbg");

  ret = cuda_gdb_dir_create (cuda_gdb_tmp_basedir,
                             S_IRWXU | S_IRWXG | S_IRWXO,
                             override_umask, &dir_exists);
  if (ret)
    error (_("Error creating temporary directory %s\n"),
           cuda_gdb_tmp_basedir);
}

void
cuda_gdb_dir_cleanup_files (char* dirpath)
{
  char path[CUDA_GDB_TMP_BUF_SIZE];
  DIR* dir = opendir (dirpath);
  struct dirent* dir_ent = NULL;

  if (!dir)
    return;

  while ((dir_ent = readdir (dir)))
    {
      if (!strcmp(dir_ent->d_name,".") ||
          !strcmp(dir_ent->d_name, ".."))
        continue;
      snprintf (path, sizeof (path), "%s/%s", dirpath, dir_ent->d_name);
      if (dir_ent->d_type == DT_DIR) {
        cuda_gdb_dir_cleanup_files (path);
        rmdir (path);
      }
      else
        unlink (path);
    };

  closedir (dir);
}

static void
cuda_gdb_tmpdir_cleanup_dir (char* dirpath)
{
  cuda_gdb_dir_cleanup_files (dirpath);
  rmdir (dirpath);
}

void
cuda_gdb_tmpdir_cleanup_self (void *unused)
{
  cuda_gdb_tmpdir_cleanup_dir (cuda_gdb_tmp_dir);
  xfree (cuda_gdb_tmp_dir);
}

static void
cuda_gdb_record_write (int record_idx, int pid)
{
  char record[CUDA_GDB_TMP_BUF_SIZE];
  int res;

  if (record_idx == 0)
    snprintf (record, CUDA_GDB_TMP_BUF_SIZE, RECORD_FORMAT_MASTER,
              pid);
  else
    snprintf (record, CUDA_GDB_TMP_BUF_SIZE, RECORD_FORMAT_DEVICE,
              DEVICE_RECORD (record_idx), pid);

  res = lseek (cuda_gdb_lock_fd, record_idx * RECORD_SIZE, SEEK_SET);
  if (res == -1)
    return;

  res = write (cuda_gdb_lock_fd, record, strlen (record));
  if (res == -1)
    return;
}

static void
cuda_gdb_record_set_lock (int record_idx, bool enable_lock)
{
  struct flock lock = {0};
  int e = 0;

  lock.l_type = enable_lock ? F_WRLCK: F_UNLCK;
  lock.l_whence = SEEK_SET;
  lock.l_start = record_idx * RECORD_SIZE;
  lock.l_len = RECORD_SIZE;

  e = fcntl (cuda_gdb_lock_fd, F_SETLK, &lock);

  if (e && (errno == EACCES || errno == EAGAIN))
    {
      if (record_idx == RECORD_MASTER)
        error (_("An instance of cuda-gdb is already using device %d.\n"
                 "If you believe you are seeing this message in error, try "
                 "deleting %s/%s.\n"), DEVICE_RECORD (record_idx),
               cuda_gdb_tmp_basedir, cuda_gdb_lock_file);
      else
        error (_("Another cuda-gdb instance is working with the lock file. Try again.\n"
                 "If you believe you are seeing this message in error, try deleting %s/%s.\n"),
               cuda_gdb_tmp_basedir, cuda_gdb_lock_file);
    }
  else if (e)
    error (_("Internal error with the cuda-gdb lock file (errno=%d).\n"), errno);
}

static void
cuda_gdb_lock_file_initialize ()
{
  uint32_t i;

  for (i = 0; i < CUDBG_MAX_DEVICES; i++)
    {
      cuda_gdb_record_write (RECORD_DEVICE(i), 0);
    }
}

void
cuda_gdb_record_remove_all (void *unused)
{
  char buf[CUDA_GDB_TMP_BUF_SIZE];
  int i;

  snprintf (buf, sizeof (buf), "%s/%s",
            cuda_gdb_tmp_basedir, cuda_gdb_lock_file);

  for (i = 0; i < CUDBG_MAX_DEVICES; i++)
    {
      if (dev_mask & (1 << i))
        {
          cuda_gdb_record_write (RECORD_DEVICE(i), 0);
          cuda_gdb_record_set_lock (RECORD_DEVICE(i), false);
          dev_mask &= ~(1 << i);
        }
    }

  if (cuda_gdb_lock_fd != -1)
    close (cuda_gdb_lock_fd);
}

/* Check for the presence of the CUDA_VISIBLE_DEVICES variable. If it is
 * present, lock records */
static void
cuda_gdb_lock_file_create ()
{
  struct stat st;
  char buf[CUDA_GDB_TMP_BUF_SIZE];
  char *visible_devices;
  uint32_t dev_id, num_devices = 0;
  int i;
  bool initialize_lock_file = false;
  mode_t old_umask;
  int my_pid = (int) getpid();

  /* Default == 1, can be overriden via a command-line option */
  if (cuda_use_lockfile == 0)
    return;

  snprintf (buf, sizeof (buf), "%s/%s",
              cuda_gdb_tmp_basedir, cuda_gdb_lock_file);

  visible_devices = getenv ("CUDA_VISIBLE_DEVICES");

  if (stat (buf, &st) || !(S_ISREG(st.st_mode)))
    initialize_lock_file = true;

  /* Save the old umask and reset it */
  old_umask = umask (0);
  cuda_gdb_lock_fd = open (buf, O_CREAT | O_RDWR,
                           S_IRWXU | S_IRWXG | S_IRWXO);
  /* Restore the old umask */
  umask (old_umask);

  if (cuda_gdb_lock_fd == -1)
    error (_("Cannot open %s. \n"), buf);

  /* Register cleanup routine */
  /* No final cleanup chain at server side,
     cleanup function is called explicitly when server quits */
#ifndef GDBSERVER
  make_final_cleanup (cuda_gdb_record_remove_all, NULL);
#endif

  /* Get the mutex ("work") lock before doing anything */
  cuda_gdb_record_set_lock (RECORD_MASTER, true);
  cuda_gdb_record_write (RECORD_MASTER, my_pid);

  if (initialize_lock_file)
    cuda_gdb_lock_file_initialize ();

  if (NULL == visible_devices)
    {
      /* Lock all devices */
      for (i = 0; i < CUDBG_MAX_DEVICES; i++)
        {
          cuda_gdb_record_set_lock (RECORD_DEVICE(i), true);
          cuda_gdb_record_write (RECORD_DEVICE(i), my_pid);
          dev_mask |= 1 << i;
        }
    }
  else
    {
      /* Copy to local storage to prevent buffer overflows */
      strncpy (buf, visible_devices, CUDA_GDB_TMP_BUF_SIZE);

      visible_devices = buf;

      do
        {
          if (*visible_devices == ',')
            visible_devices++;

          if ((sscanf (visible_devices, "%u,", &dev_id) > 0) &&
              (++num_devices < CUDBG_MAX_DEVICES) &&
              (dev_id < CUDBG_MAX_DEVICES))
            {
              cuda_gdb_record_set_lock (RECORD_DEVICE(dev_id), true);
              cuda_gdb_record_write (RECORD_DEVICE(dev_id), my_pid);
              dev_mask |= 1 << dev_id;
            }
          else
            break;
        } while ((visible_devices = strstr (visible_devices, ",")));
    }

    cuda_gdb_record_write (RECORD_MASTER, 0);
    cuda_gdb_record_set_lock (RECORD_MASTER, false);
}

static void
cuda_gdb_tmpdir_setup (void)
{
  char dirpath [CUDA_GDB_TMP_BUF_SIZE];
  int ret;
  bool dir_exists = false;
  bool override_umask = false;

  snprintf (dirpath, sizeof (dirpath), "%s/%u", cuda_gdb_tmp_basedir,
            getpid ());

  ret = cuda_gdb_dir_create (dirpath, S_IRWXU | S_IRWXG, override_umask,
                             &dir_exists); 
  if (ret)
    error (_("Error creating temporary directory %s\n"), dirpath);

  if (dir_exists)
    cuda_gdb_dir_cleanup_files (dirpath);

  cuda_gdb_tmp_dir = xmalloc (strlen (dirpath) + 1);
  strncpy (cuda_gdb_tmp_dir, dirpath, strlen (dirpath) + 1);

  /* No final cleanup chain at server side,
   * cleanup function is called explicitly when server quits */
#ifndef GDBSERVER
  make_final_cleanup (cuda_gdb_tmpdir_cleanup_self, NULL);
#endif
}

const char*
cuda_gdb_tmpdir_getdir (void)
{
  return cuda_gdb_tmp_dir;
}

static cuda_clock_t cuda_clock_ = 0;

cuda_clock_t
cuda_clock (void)
{
  return cuda_clock_;
}

void
cuda_clock_increment (void)
{
  ++cuda_clock_;
  if (cuda_clock_ == 0)
    warning (_("The internal clock counter used for cuda debugging wrapped around.\n"));
}

#ifndef GDBSERVER
static unsigned char *
cuda_nat_save_gdb_signal_handlers (void)
{
  unsigned char *sigs;
  int i,j;
  static int (*sighand_savers[])(int) =
    {signal_stop_state, signal_print_state, signal_pass_state};

  sigs = xmalloc (TARGET_SIGNAL_LAST*ARRAY_SIZE(sighand_savers));

  for (i=0; i < ARRAY_SIZE(sighand_savers); i++)
    for (j=0; j < TARGET_SIGNAL_LAST; j++)
      sigs[i*TARGET_SIGNAL_LAST+j] = sighand_savers[i](j);

  return sigs;
}

static void
cuda_nat_restore_gdb_signal_handlers (unsigned char *sigs)
{
  int i,j;
  static int (*sighand_updaters[])(int,int) =
    {signal_stop_update, signal_print_update, signal_pass_update};

  for (i=0; i < ARRAY_SIZE(sighand_updaters); i++)
    for (j=0; j < TARGET_SIGNAL_LAST; j++)
      sighand_updaters[i] (j, sigs[i*TARGET_SIGNAL_LAST+j]);
}

static void cuda_nat_bypass_signals_cleanup (void *ptr)
{
  unsigned char *sigs = ptr;

  cuda_nat_restore_gdb_signal_handlers (sigs);
  xfree (ptr);
}

struct cleanup *
cuda_gdb_bypass_signals (void)
{
  unsigned char *sigs;
  int i;

  sigs = cuda_nat_save_gdb_signal_handlers ();
  for (i=0;i< TARGET_SIGNAL_LAST; i++)
    {
      if ( i == TARGET_SIGNAL_TRAP ||
           i == TARGET_SIGNAL_KILL ||
           i == TARGET_SIGNAL_STOP ||
           i >= TARGET_SIGNAL_CUDA_UNKNOWN_EXCEPTION ) continue;
      signal_stop_update (i, 0);
      signal_pass_update (i, 1);
      signal_print_update (i, 1);
    }

  return make_cleanup (cuda_nat_bypass_signals_cleanup, sigs);
}

#endif /* GDBSERVER */

void
cuda_utils_initialize (void)
{
  /* Create the base temporary directory */
  cuda_gdb_tmpdir_create_basedir ();

  /* Create a lockfile to prevent multiple instances of cuda-gdb from
   * interfering with each other */
  cuda_gdb_lock_file_create ();

  /* Populate the temporary directory with a unique subdirectory for this
   * instance. */
  cuda_gdb_tmpdir_setup ();
}
