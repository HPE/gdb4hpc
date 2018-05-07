/* This testcase is part of GDB, the GNU debugger.

   Copyright 2013-2017 Free Software Foundation, Inc.

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

#include <pthread.h>
#include <unistd.h>

static void *
start (void *arg)
{
  return NULL;
}

static void
end (void)
{
}

int
main (void)
{
  pthread_t thread;

  pthread_create (&thread, NULL, start, NULL);
  pthread_join (thread, NULL);

  // There is a race condition between when gdb detects that
  // the thread has joined and when the parent hits the break
  // point on end(). The gdb unit test expects that the thread
  // is reported as joined before the break point is hit. This
  // sleep is a nasty solution, but it works.
  sleep (1);

  end ();
  return 0;
}
