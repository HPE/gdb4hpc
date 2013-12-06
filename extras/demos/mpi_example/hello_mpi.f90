program helloworldmpi
  implicit none
      
  integer ierr,myid,numprocs
  integer source, dest, tag
  CHARACTER*100 message

  include 'mpif.h'
  integer status(MPI_STATUS_SIZE)

  call mpi_init(ierr)
  call mpi_comm_rank(MPI_COMM_WORLD,myid,ierr)
  call mpi_comm_size(MPI_COMM_WORLD,numprocs,ierr)

  100  format('Hello World! From process ', I6)
  write(message,100) myid

  tag= 1
  dest= 0

  if(myid.eq.0) then
    write(6,*) message
    do source=1,numprocs-1
      call mpi_recv(message, 100, MPI_CHARACTER, source, tag, MPI_COMM_WORLD, status, ierr)
      write(6,*) message
    end do
  else
    call mpi_send(message, 100, MPI_CHARACTER, dest, tag, MPI_COMM_WORLD, ierr)
  end if

  call mpi_finalize(ierr)

  stop
end program

