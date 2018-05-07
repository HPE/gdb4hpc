/*
* Copyright 2012 Free Software Foundation, Inc.
*
* Contributed by Intel Corp. <christian.himpel@intel.com>,
*                            <walfred.tedeschi@intel.com>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
* This test was converted from idb/test/td/runreg/Biendian/bi.c_bitfield
*
*/
#include <stdio.h>
#include "x86-cpuid.h"

#define MYTYPE   int
#define OUR_SIZE    5

MYTYPE gx[OUR_SIZE];
MYTYPE ga[OUR_SIZE];
MYTYPE gb[OUR_SIZE];
MYTYPE gc[OUR_SIZE];
MYTYPE gd[OUR_SIZE];

unsigned int
have_mpx (void)
{
  unsigned int eax, ebx, ecx, edx;

  if (!__get_cpuid (1, &eax, &ebx, &ecx, &edx))
    return 0;

  if ((ecx & bit_OSXSAVE) == bit_OSXSAVE)
    {
      if (__get_cpuid_max (0, NULL) < 7)
	return 0;

      __cpuid_count (7, 0, eax, ebx, ecx, edx);

      if ((ebx & bit_MPX) == bit_MPX)
	return 1;
      else
	return 0;
    }
  return 0;
}

int
bp1 (MYTYPE value)
{
  return 1;
}

int
bp2 (MYTYPE value)
{
  return 1;
}

void
upper (MYTYPE * p, MYTYPE * a, MYTYPE * b, MYTYPE * c, MYTYPE * d, int len)
{
  MYTYPE value;
  value = *(p + len);
  value = *(a + len);
  value = *(b + len);
  value = *(c + len);
  value = *(d + len);
}

MYTYPE *
upper_ptr (MYTYPE * p, MYTYPE * a, MYTYPE * b, MYTYPE * c, MYTYPE * d, int len)
{
  MYTYPE value;
  value = *(p + len);
  value = *(a + len);
  value = *(b + len);
  value = *(c + len);
  value = *(d + len);  /* bkpt 2.  */
  free (p);
  p = calloc (OUR_SIZE * 2, sizeof (MYTYPE));
  return ++p;
}


int
main (void)
{
  if (have_mpx ())
    {
      MYTYPE sx[OUR_SIZE];
      MYTYPE sa[OUR_SIZE];
      MYTYPE sb[OUR_SIZE];
      MYTYPE sc[OUR_SIZE];
      MYTYPE sd[OUR_SIZE];
      MYTYPE *x, *a, *b, *c, *d;

      x = calloc (OUR_SIZE, sizeof (MYTYPE));
      a = calloc (OUR_SIZE, sizeof (MYTYPE));
      b = calloc (OUR_SIZE, sizeof (MYTYPE));
      c = calloc (OUR_SIZE, sizeof (MYTYPE));
      d = calloc (OUR_SIZE, sizeof (MYTYPE));

      upper (sx, sa, sb, sc, sd, 0);  /* bkpt 1.  */
      x = upper_ptr (sx, sa, sb, sc, sd, 0);

      free (x); /* bkpt 3.  */
      free (a);
      free (b);
      free (c);
      free (d);
    }
  return 0;
}
