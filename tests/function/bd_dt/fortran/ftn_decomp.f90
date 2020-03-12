
program ftn_decomp

   use iso_fortran_env, only: int8, int64
   use mpi
   implicit none
   
   integer :: ierror, application, exponent
   integer array_size, sizeI, sizeJ, sliceN, sliceJ, decomp_cycle
   integer bcSizeI, bcSizeJ, bcSliceJ, cycle2, sliceIT, sizeIT, sizeJT
   integer modIdx, modValue, idx, i, j
   integer(kind=4), allocatable :: block1(:)
   integer(kind=4), allocatable :: cyclic1(:)
   integer(kind=4), allocatable :: blockCyclic1(:)
   integer(kind=8), allocatable :: block2(:,:)
   integer(kind=4), allocatable :: cyclic2(:,:)
   integer(kind=4), allocatable :: blockCyclic2(:,:)
   integer(kind=4), allocatable :: block2T(:,:)
   integer(kind=4), allocatable :: offsetBlock1(:)
   integer(kind=4), allocatable :: offsetCyclic1(:)
   character(len=10) :: arg_application, arg_exponent

   integer :: rank, num_ranks;

   call get_command_argument(1, arg_application)
   call get_command_argument(2, arg_exponent)

   read (arg_application, '(I10)') application

   ! exponent is not currently used, but I may want to modify this test to work on larger data
   ! in the future
   if (len_trim(arg_exponent) .NE. 0) then
      read (arg_exponent, '(I10)') exponent
   else
      exponent = 1
   end if

    call MPI_Init(ierror)

    call MPI_Comm_Rank(MPI_COMM_WORLD, rank, ierror)
    call MPI_Comm_Size(MPI_COMM_WORLD, num_ranks, ierror);

    array_size = 120
    sizeI = 10
    sizeJ = 12
    decomp_cycle = 5
    bcSizeI = 5
    bcSizeJ = 24
    bcSliceJ = bcSizeJ / num_ranks;
    cycle2 = 2

    sizeJT = sizeI
    sizeIT = sizeJ
    sliceIT = sizeIT / num_ranks

    sliceN = array_size / num_ranks
    sliceJ = sizeJ / num_ranks

    allocate(block1(sliceN))
    allocate(cyclic1(sliceN))
    allocate(blockCyclic1(sliceN))
    allocate(block2(sizeI, sliceJ))
    allocate(cyclic2(sizeI, sliceJ))
    allocate(blockCyclic2(bcSizeI, bcSliceJ))
    allocate(block2T(sliceIT, sizeJT))
    allocate(offsetBlock1(5:sliceN+4))
    allocate(offsetCyclic1(5:sliceN+4))
    

    do idx=1,sliceN
        i = idx-1
        block1(idx) = getValue1(sliceN * rank + i)
        cyclic1(idx) = getValue1(rank + i * num_ranks)
        blockCyclic1(idx) = getValue1(rank*decomp_cycle + (i/decomp_cycle)*num_ranks*decomp_cycle + mod(i,decomp_cycle))
        offsetBlock1(idx+4) = getValue1(sliceN * rank + i)
        offsetCyclic1(idx+4) = getValue1(rank + i * num_ranks)
    end do

    do j=0, sliceJ-1
        do i=0, sizeI-1
            block2(i+1,j+1) = getValue2(i, rank * (sliceN/sizeI) + j)
            cyclic2(i+1,j+1) = getValue2(i, rank + num_ranks * j)
        end do
    end do

    do j=0,bcSliceJ-1
        do i=0,bcSizeI-1
            blockCyclic2(i+1,j+1) = getValue2(i, rank*cycle2 + (j/cycle2) * num_ranks * cycle2 + mod(j,cycle2))
        end do
    end do

    do j=0,sizeJT-1
        do i=0,sliceIT-1
            block2T(i+1,j+1) = getValue2(rank * sliceIT + i, j)
        end do
    end do


    modIdx = sliceN/2
    modValue = -100
    if (application .NE. 0) then
        block1(modIdx) = modValue
        cyclic1(modIdx) = modValue
        blockCyclic1(modIdx) = modValue
        offsetBlock1(modIdx+4) = modValue
        offsetCyclic1(modIdx+4) = modValue
        block2(1,1) = modValue
        cyclic2(1,1) = modValue
        blockCyclic2(1,1) = modValue
        block2T(1,1) = modValue
    end if

    deallocate(block1)
    deallocate(cyclic1)
    deallocate(blockCyclic1)
    deallocate(offsetBlock1)
    deallocate(offsetCyclic1)
    deallocate(block2)
    deallocate(cyclic2)
    deallocate(blockCyclic2)
    deallocate(block2T)

    call MPI_Finalize(ierror)
    

 contains
    function getValue1(i) result(val)
       integer, intent(in) :: i
       integer             :: val
       val = i
    end function getValue1

    function getValue2(i,j) result(val)
        integer, intent(in) :: i
        integer, intent(in) :: j
        integer             :: val
        val = getValue1(i + 100*j)
    end function getValue2

end program ftn_decomp
