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
! This file is the Fortran source file for pointers-to-vla.exp

program main
    ! Test types of at least two different sizes to ensure the strides
    ! are handled correctly.
    ! 1D
    character, target, allocatable :: characterArrayTarget(:)
    integer, target, allocatable :: integerArrayTarget(:)

    character, pointer :: characterArrayPointer(:)
    integer, pointer :: integerArrayPointer(:)

    character, pointer :: characterSubArrayPointer(:)
    integer, pointer :: integerSubArrayPointer(:)

    ! 2D
    character, target, allocatable :: character2DArrayTarget(:,:)
    integer, target, allocatable :: integer2DArrayTarget(:,:)

    character, pointer :: character2DArrayPointer(:,:)
    integer, pointer :: integer2DArrayPointer(:,:)

    character, pointer :: character2DSubArrayPointer(:,:)
    integer, pointer :: integer2DSubArrayPointer(:,:)

    allocate(characterArrayTarget(1:10))
    ! Test printing of character array target
    characterArrayTarget(:) = 'a'

    allocate(integerArrayTarget(1:10))
    ! Test printing of integer array target
    integerArrayTarget(:) = 2

    characterArrayPointer => characterArrayTarget(1:10)
    ! Test printing of a character array pointer
    characterArrayPointer(:) = 'b'

    integerArrayPointer => integerArrayTarget(1:10)
    ! Test printing of an integer array pointer
    integerArrayPointer(:) = 3

    characterSubArrayPointer => characterArrayTarget(2:9)
    ! Test printing a character array through the pointer
    characterSubArrayPointer(1) = 's'
    characterSubArrayPointer(8) = 'e'

    integerSubArrayPointer => integerArrayTarget(2:9)
    ! Test printing an integer array through the pointer
    integerSubArrayPointer(1) = 0
    integerSubArrayPointer(8) = 9

    deallocate(characterArrayTarget)
    deallocate(integerArrayTarget)

    ! Negative array indices
    allocate(characterArrayTarget(-3:10))
    ! Test printing of character array target
    characterArrayTarget(:) = 'a'

    allocate(integerArrayTarget(-3:10))
    ! Test printing of integer array target
    integerArrayTarget(:) = 2

    characterArrayPointer => characterArrayTarget(-3:10)
    ! Test printing of a character array pointer
    characterArrayPointer(:) = 'b'

    integerArrayPointer => integerArrayTarget(-3:10)
    ! Test printing of an integer array pointer
    integerArrayPointer(:) = 3

    characterSubArrayPointer => characterArrayTarget(-1:9)
    ! Test printing a character array through the pointer
    characterSubArrayPointer(1) = 's'
    characterSubArrayPointer(11) = 'e'

    integerSubArrayPointer => integerArrayTarget(-1:9)
    ! Test printing an integer array through the pointer
    integerSubArrayPointer(-1) = 0
    integerSubArrayPointer(8) = 9
    integerSubArrayPointer(2) = 1

    deallocate(characterArrayTarget)
    deallocate(integerArrayTarget)

    allocate(character2DArrayTarget(1:3,1:4))
    ! Test printing of character array target
    character2DArrayTarget(:,:) = 'a'

    allocate(integer2DArrayTarget(1:3,1:4))
    ! Test printing of integer array target
    integer2DArrayTarget(:,:) = 2

    character2DArrayPointer => character2DArrayTarget(1:3,1:4)
    ! Test printing of a character array pointer
    character2DArrayPointer(:,:) = 'b'

    integer2DArrayPointer => integer2DArrayTarget(1:3,1:4)
    ! Test printing of an integer array pointer
    integer2DArrayPointer(:,:) = 3

    character2DSubArrayPointer => character2DArrayTarget(2:3,2:3)
    ! Test printing a character array through the pointer
    character2DSubArrayPointer(1,1) = 's'
    character2DSubArrayPointer(2,2) = 'e'

    integer2DSubArrayPointer => integer2DArrayTarget(2:3,2:3)
    ! Test printing an integer array through the pointer
    integer2DSubArrayPointer(1,1) = 0
    integer2DSubArrayPointer(2,2) = 9

    deallocate(character2DArrayTarget)
    deallocate(integer2DArrayTarget)

    allocate(character2DArrayTarget(-3:1,-4:2))
    ! Test printing of character array target
    character2DArrayTarget(:,:) = 'a'

    allocate(integer2DArrayTarget(-3:1,-4:2))
    ! Test printing of integer array target
    integer2DArrayTarget(:,:) = 2

    character2DArrayPointer => character2DArrayTarget(-3:1,-4:2)
    ! Test printing of a character array pointer
    character2DArrayPointer(:,:) = 'b'

    integer2DArrayPointer => integer2DArrayTarget(-3:1,-4:2)
    ! Test printing of an integer array pointer
    integer2DArrayPointer(:,:) = 3

    character2DSubArrayPointer => character2DArrayTarget(-2:-1,-3:1)
    ! Test printing a character array through the pointer
    character2DSubArrayPointer(1,1) = 's'
    character2DSubArrayPointer(2,2) = 'e'

    integer2DSubArrayPointer => integer2DArrayTarget(-2:-1,-3:1)
    ! Test printing an integer array through the pointer
    integer2DSubArrayPointer(1,1) = 0
    integer2DSubArrayPointer(2,2) = 9
    integer2DSubArrayPointer(2,2) = 10

    deallocate(character2DArrayTarget)
    deallocate(integer2DArrayTarget)

  stop
end program main
