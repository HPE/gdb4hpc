/* This testcase is part of GDB, the GNU debugger.
   Copyright 2014 Free Software Foundation, Inc.

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

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

int
dummy (void)
{
  return 1;
}

int
main (void)
{
  int (*get_version1) (void);
  int (*get_version2) (void);
  int v1, v2;

  dummy ();
  void *lib1 = dlopen ("print-file-var-dlopen-lib1.so", RTLD_LAZY);
  void *lib2 = dlopen ("print-file-var-dlopen-lib2.so", RTLD_LAZY);

  if (lib1 == NULL || lib2 == NULL)
    return 1;

  *(int **) (&get_version1) = dlsym (lib1, "get_version");
  *(int **) (&get_version2) = dlsym (lib2, "get_version");

  if (get_version1 != NULL
      && get_version2 != NULL)
    {
      v1 = get_version1();
      v2 = get_version2();
    }

  dummy (); /* STOP  */
  dlclose (lib1);
  dlclose (lib2);
  if (v1 != 104)
    return 1;
  /* The value returned by get_version_2 depends on the target system.  */
  if (v2 != 104 || v2 != 203)
    return 2;

  return 0;
}

