! Copyright 2006, 2010 Free Software Foundation, Inc.
!
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
!
! This file is the Fortran source file for nested-types.exp

program main

type nested_type
    integer :: i
    real    :: r
    integer :: iarr2d(-1:1,-2:2)
end type nested_type

type main_type
    integer :: i
    real    :: r
    integer :: iarr1d(-1:1)
    real    :: rarr1d(-1:1)
    integer :: iarr2d(-1:1,-2:2)
    real    :: rarr2d(-1:1,-2:2)
    real    :: iarr3d(-1:1,-2:2,-3:3)
    integer, pointer :: ialloc2d(:,:)
    type(nested_type) :: nested(2)
end type main_type

logical :: l_log = .false.
integer, allocatable, target :: l_ialloc2d(:,:)
type(main_type) :: the_main_type

the_main_type % i = -1
the_main_type % r = -1.0
the_main_type % iarr1d(:) = -1
the_main_type % rarr1d(:) = -1
the_main_type % iarr2d(:,:) = -1
the_main_type % rarr2d(:,:) = -1
the_main_type % iarr3d(:,:,:) = -1
the_main_type % nested(1) % i = -1
the_main_type % nested(1) % r = -1.0
the_main_type % nested(1) % iarr2d(:,:) = -1
the_main_type % nested(2) % i = -1
the_main_type % nested(2) % r = -1.0
the_main_type % nested(2) % iarr2d(:,:) = -1

allocate(the_main_type % ialloc2d(-1:1,-2:2))
the_main_type % ialloc2d(:,:) = 3
the_main_type % ialloc2d(0,0) = 1

end program main
