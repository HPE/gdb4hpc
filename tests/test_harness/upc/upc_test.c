/*
UPC tests

Copyright (C) 2001 
Written by Gary Funck <gary@intrepid.com>
and Nenad Vukicevic <nenad@intrepid.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
#include <upc_strict.h>
#include <stdio.h>

struct data_struct
  {
    char x1;
    short x2;
    int x3;
    long long x4;
  };

#define FACTOR 10
shared struct data_struct array[FACTOR*THREADS];

void
test05()
{
  int i;
  for (i = MYTHREAD; i < FACTOR*THREADS; i += THREADS)
    {
      struct data_struct * const s = (struct data_struct *)&array[i];
      s->x1 = i*4 + 1;
      s->x2 = i*4 + 2;
      s->x3 = i*4 + 3;
      s->x4 = i*4 + 4;
    }
  upc_barrier;
  if (MYTHREAD == 0)
    {
      for (i = 0; i < FACTOR*THREADS; ++i)
	{
	  struct data_struct got = array[i];
	  struct data_struct expected;
	  int correct;
	  expected.x1 = i*4 + 1;
	  expected.x2 = i*4 + 2;
	  expected.x3 = i*4 + 3;
	  expected.x4 = i*4 + 4;
	  correct = got.x1 == expected.x1 && got.x2 == expected.x2 && got.x3 == expected.x3 && got.x4 == expected.x4;
	  if (!correct)
	    {
	      fprintf(stderr,
		"test05: Error at element %d."
		" Expected (%d,%d,%d,%lld),"
		" got (%d,%d,%d,%lld)\n",
		i, expected.x1, expected.x2, expected.x3, expected.x4,
		got.x1, got.x2, got.x3, got.x4);
	      upc_global_exit (1);
	    }
	}
      printf ("test05: access structured shared array element\n"
	      "		using a local pointer - passed.\n");
    }
}

int
main()
{
  test05 ();
  return 0;
}
