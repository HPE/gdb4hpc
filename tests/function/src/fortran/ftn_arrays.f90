program ftn_arrays

   use iso_fortran_env, only: int8, int64
   use mpi
   implicit none
   integer :: ierror, application, exponent
   character(len=10) :: arg_application, arg_exponent
   integer(kind=4), allocatable :: int_1(:)
   integer(kind=4), allocatable :: int_2(:,:)
   integer(kind=4), allocatable :: int_3(:,:,:)

   integer(kind=8), allocatable :: long_1(:)
   integer(kind=8), allocatable :: long_2(:,:)
   integer(kind=8), allocatable :: long_3(:,:,:)

   integer(kind=4) :: int_0(0:9)
   integer(kind=4) :: int_0_2( -10:10, -5:13)

   real*4, allocatable :: float_1(:)
   real*4, allocatable :: float_2(:,:)
   real*4, allocatable :: float_3(:,:,:)

   real*8, allocatable :: double_1(:)
   real*8, allocatable :: double_2(:,:)
   real*8, allocatable :: double_3(:,:,:)

   complex, allocatable :: complex_1(:)
   complex, allocatable :: complex_2(:,:)
   complex, allocatable :: complex_3(:,:,:)

   logical, allocatable :: bool_1(:)
   logical, allocatable :: bool_2(:,:)
   logical, allocatable :: bool_3(:,:,:)

   character(len=20) :: word

   integer(kind=8) :: size_1, size_2_1, size_2_2, size_3_1, size_3_2, size_3_3
   integer(kind=8) :: i,j,k

   call get_command_argument(1, arg_application)
   call get_command_argument(2, arg_exponent)

   read (arg_application, '(I10)') application
   read (arg_exponent, '(I10)') exponent

   call MPI_Init(ierror)

   size_1 = 2_int64 ** exponent

   size_2_1 = 2_int64 ** (exponent/2)
   size_2_2 = size_2_1 * 2 + 1

   size_3_1 = 2_int64 ** (exponent/4)
   size_3_2 = 2 * size_3_1 + 1
   size_3_3 = 3 * size_3_1 + 1

   allocate(int_1(size_1))
   allocate(int_2(size_2_1,size_2_2))
   allocate(int_3(size_3_1,size_3_2,size_3_3))

   allocate(long_1(size_1))
   allocate(long_2(size_2_1, size_2_2))
   allocate(long_3(size_3_1, size_3_2, size_3_3))

   allocate(float_1(size_1))
   allocate(float_2(size_2_1,size_2_2))
   allocate(float_3(size_3_1,size_3_2,size_3_3))

   allocate(double_1(size_1))
   allocate(double_2(size_2_1, size_2_2))
   allocate(double_3(size_3_1, size_3_2, size_3_3))

   allocate(complex_1(size_1))
   allocate(complex_2(size_2_1, size_2_2))
   allocate(complex_3(size_3_1, size_3_2, size_3_3))

   allocate(bool_1(size_1))
   allocate(bool_2(size_2_1, size_2_2))
   allocate(bool_3(size_3_1, size_3_2, size_3_3))

   word = "boy howdy!"

   forall(i=1:size_1)
      int_1(i) = 2*i+1
      long_1(i) = 2*i+1
      float_1(i) = 2*i+1
      double_1(i) = 2*i+1
      complex_1(i) = CMPLX(1.1*i, 2.1*i+1)
   end forall

   bool_1 = .FALSE.  
   forall(i=1:size_1:2) bool_1(i) = .TRUE.

   forall(i=1:size_2_1, j=1:size_2_2)
      int_2(i,j) = i + 2*j
      long_2(i,j) = i + 2*j
      float_2(i,j) = i + 2.1 * j
      double_2(i,j) = i + 2.1 * j
      complex_2(i,j) = CMPLX(i + 2.1 *j, i-2.1*j)
   end forall

   bool_2 = .FALSE.
   forall(i=1:size_2_1, j=1:size_2_2, mod(i+j,2).eq.1) bool_2(i,j) = .TRUE.

   forall(i=1:size_3_1, j=1:size_3_2, k=1:size_3_3)
      int_3(i,j,k) = i + 2*j + 3*k
      long_3(i,j,k) = i + 2*j + 3*k
      float_3(i,j,k) = i + 2.1*j + 3.1*k
      double_3(i,j,k) = i + 2.1*j + 3.1*k
      complex_3(i,j,k) = CMPLX(i + 2.1*j + 3.1*k, i-2.1*j - 3.1*k)
   end forall
   
   bool_3 = .FALSE.
   forall(i=1:size_3_1, j=1:size_3_2, k=1:size_3_3, mod(i+j+k,2).eq.1) bool_3(i,j,k) = .TRUE.

   forall(i=0:9) int_0(i) = 2 * (i+5) - 3
   forall(i=-10:10, j=-5:13) int_0_2(i,j) = i + j

   if (application.ne.0) then
      int_1(1) = 100
      long_1(1) = 100
      float_1(1) = 100.0
      double_1(1) = 100.0

      int_2(1,2) = 100
      long_2(1,2) = 100
      float_2(1,2) = 100.0
      double_2(1,2) = 100.0

      int_3(1,2,3) = 100
      long_3(1,2,3) = 100
      float_3(1,2,3) = 100.0
      double_3(1,2,3) = 100.0

      complex_1(1) = (100.0,100.0)
      complex_2(1,2) = (100.0,100.0)
      complex_3(1,2,3) = (100.0, 100.0)

      bool_1(1) = .FALSE.
      bool_2(1,2) = .FALSE.
      bool_3(1,2,3) = .TRUE.

      int_0(0) = 99
      int_0(9) = -99
      int_0_2(3,-2) = 98
      
      word = "bad beef"
   endif

   deallocate(int_1)
   deallocate(int_2)
   deallocate(int_3)

   deallocate(long_1)
   deallocate(long_2)
   deallocate(long_3)

   deallocate(float_1)
   deallocate(float_2)
   deallocate(float_3)

   deallocate(double_1)
   deallocate(double_2)
   deallocate(double_3)
   
   call MPI_Finalize(ierror)

end program ftn_arrays


