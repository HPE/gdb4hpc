! Copyright 2012 Free Software Foundation, Inc.

! Contributed by Intel Corp. <markus.t.metzger@intel.com>

! This program is free software; you can redistribute it and/or modify
! it under the terms of the GNU General Public License as published by
! the Free Software Foundation; either version 3 of the License, or
! (at your option) any later version.
!
! This program is distributed in the hope that it will be useful,
! but WITHOUT ANY WARRANTY; without even the implied warranty of
! MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
! GNU General Public License for more details.
!
! You should have received a copy of the GNU General Public License
! along with this program.  If not, see <http://www.gnu.org/licenses/>.

  subroutine test(n)
    integer :: i, n, t
    integer :: loc
    integer, dimension(3) :: shared

    do i=1,n
!$omp parallel num_threads(2) ! bp.thread
!$omp sections
!$omp section
       shared(i) = 42 ! bp.write
!$omp section
       loc = shared(i) ! bp.read
!$omp end sections
!$omp barrier
!$omp end parallel
    enddo

  end subroutine test

  program main
    integer :: iterations = 3

    call test(iterations)
  end program main
