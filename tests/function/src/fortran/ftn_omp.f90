program main
  use mpi
  integer :: aaa(1000), t, ierror, rank, num_ranks

  call MPI_Init(ierror)

  call MPI_Comm_Rank(MPI_COMM_WORLD, rank, ierror)
  call MPI_Comm_Size(MPI_COMM_WORLD, num_ranks, ierror);

  aaa = 0
  !$omp target teams distribute map(aaa) private(t)
  do i=1,1000
     t = aaa(i)
     t = t + i
     aaa(i) = t
  end do
  !$omp end target teams distribute
  if ( sum(abs(aaa)) .ne. 500500 ) then
     print *, "FAIL"
     call exit(-1)
  end if
  print *, "PASS"

  call MPI_Finalize(ierror)

end program main
