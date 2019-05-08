program test

	integer ierr, myid, numprocs
	integer source, dest, tag
	real a(100),b(100),c(100)
	integer n
	CHARACTER*100 message

	include 'mpif.h'
	integer status(MPI_STATUS_SIZE)

	call mpi_init(ierr)
	call mpi_comm_rank(MPI_COMM_WORLD,myid,ierr)
	call mpi_comm_size(MPI_COMM_WORLD,numprocs,ierr)

	n = 100

	!$acc data create(b,c) copyout( a )
	!$acc parallel loop
		do i=1,n
			a(i) = 0.0
			b(i) = 1.0
			c(i) = 2.0
		end do
	!$acc end parallel loop
		call foo(a,b,c)
	!$acc end data
	print *, a
	
	tag= 1
	dest= 0

	100 format('Hello World! From process ', I6)
	write(message,100) myid

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

contains

	subroutine foo( a, b, c )
		real a(100), b(100), c(100)
		integer n
		n = 100
		!$acc data present_or_copyout( a ) present_or_create(b,c)
		!$acc parallel loop
			do i=1,n
				a(i) = b(i) + c(i)
			end do
		!$acc end parallel loop
		!$acc end data
	end subroutine foo

end program test 
