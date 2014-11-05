/* Copyright 2012 Free Software Foundation, Inc.
 *
 * Contributed by Intel Corp. <markus.t.metzger@intel.com>
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
 * along with this program.  If not, see <http:*www.gnu.org/licenses/>.
 *
 *
 * A simple race on a global variable by two threads: one read and one write.
 */

#include <stdint.h>

union shared_u
{
  uint32_t arr4[2];
  uint8_t arr1[8];
} static volatile shared;

static void
test (volatile union shared_u *var, int n)
{
  int i;

  for (i = 0; i < n; ++i)
    {
#pragma omp parallel num_threads(2) /* bp.thread */
      {
#pragma omp sections
		{
#pragma omp section
		  {
			var->arr4[i] = 42; /* bp.write */
		  }
#pragma omp section
		  {
			uint32_t loc = var->arr4[i]; /* bp.read */
		  }
		}

		// Keep threads alive to guarantee that we see the data race.
		//
#pragma omp barrier
	  }
	}
}

int main (void)
{
  test (&shared, 2);

  return 0;
}
