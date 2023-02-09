MODULE mpi
  implicit none

  integer :: MPI_COMM_WORLD

  contains

  subroutine MPI_Init(ierr)
    integer :: ierr
    ierr = 0
  END subroutine MPI_Init

  subroutine MPI_Finalize(ierr)
     integer :: ierr
    ierr = 0
  end subroutine MPI_Finalize

  subroutine MPI_Comm_Rank(comm, rank, ierr)
     integer :: comm, rank, ierr
     rank = 0
     ierr = 0
   end subroutine MPI_Comm_Rank
   
   subroutine MPI_Comm_Size(comm, size, ierr)
     integer :: comm, size, ierr
     size = 1
     ierr = 0
   end subroutine MPI_Comm_Size

END MODULE mpi

  
  
