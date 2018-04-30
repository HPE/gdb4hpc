! Copyright 2016 Free Software Foundation, Inc.
!
! This program is free software; you can redistribute it and/or modify
! it under the terms of the GNU General Public License as published by
! the Free Software Foundation; either version 2 of the License, or
! (at your option) any later version.
!
! This program is distributed in the hope that it will be useful,
! but WITHOUT ANY WARRANTY; without even the implied warranty of
! MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
! GNU General Public License for more details.
!
! You should have received a copy of the GNU General Public License
! along with this program; if not, write to the Free Software
! Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
!

module md
  type iv
    integer :: ident
    integer,allocatable :: inner(:)
  end type iv

  type nv
    integer :: ident
	TYPE(iv),ALLOCATABLE :: nested(:)
  end type nv

  type (iv),allocatable :: outer(:)
  type (nv),allocatable :: outer_nested(:)

end module md

program vla_test
  use md
  implicit none

  !-----------------
  ! VLA of structures with VLA of basic type
  allocate(outer(3))

  allocate(outer(1)%inner(2))
  allocate(outer(2)%inner(3))
  allocate(outer(3)%inner(2))

  outer(1)%ident    = 1
  outer(1)%inner(1) = 101
  outer(1)%inner(2) = 102

  outer(2)%ident    = 2
  outer(2)%inner(1) = 201
  outer(2)%inner(2) = 202
  outer(2)%inner(3) = 201 + 2

  outer(3)%ident    = 3
  outer(3)%inner(1) = 301
  outer(3)%inner(2) = 302

  outer(3)%inner(2) = 302  + 1 ! bp 1.

  !-----------------
  ! VLA of structures with VLA of structures
  allocate(outer_nested(2))

  allocate(outer_nested(1)%nested(2))
  allocate(outer_nested(2)%nested(3))

  allocate(outer_nested(1)%nested(1)%inner(2))
  allocate(outer_nested(1)%nested(2)%inner(3))
  allocate(outer_nested(2)%nested(1)%inner(1))
  allocate(outer_nested(2)%nested(2)%inner(4))
  allocate(outer_nested(2)%nested(3)%inner(2))

  outer_nested(1)%ident = 1
  outer_nested(1)%nested(1)%ident = 11
  outer_nested(1)%nested(1)%inner(1) = 111
  outer_nested(1)%nested(1)%inner(2) = 112
  outer_nested(1)%nested(2)%ident = 12
  outer_nested(1)%nested(2)%inner(1) = 121
  outer_nested(1)%nested(2)%inner(2) = 122
  outer_nested(1)%nested(2)%inner(3) = 123

  outer_nested(2)%ident = 2
  outer_nested(2)%nested(1)%ident = 21
  outer_nested(2)%nested(1)%inner(1) = 211
  outer_nested(2)%nested(2)%ident = 22
  outer_nested(2)%nested(2)%inner(1) = 221
  outer_nested(2)%nested(2)%inner(2) = 222
  outer_nested(2)%nested(2)%inner(3) = 223
  outer_nested(2)%nested(2)%inner(4) = 224
  outer_nested(2)%nested(3)%ident = 23
  outer_nested(2)%nested(3)%inner(1) = 231
  outer_nested(2)%nested(3)%inner(2) = 232

  outer_nested(2)%nested(3)%inner(2) = 232 + 1 ! bp 2.

continue
end program vla_test
