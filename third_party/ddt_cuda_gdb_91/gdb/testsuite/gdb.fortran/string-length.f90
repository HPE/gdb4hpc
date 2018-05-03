program main

implicit none

character(*), parameter :: demo_string = "Hello, world!"
call my_print_string(demo_string)

contains

subroutine my_print_string(my_string)
   use, intrinsic :: iso_c_binding, only: C_CHAR
   implicit none
   character(kind=C_CHAR, len=*), intent(in) :: my_string
   ! stop-here
   print *, my_string
end subroutine my_print_string

end program main
