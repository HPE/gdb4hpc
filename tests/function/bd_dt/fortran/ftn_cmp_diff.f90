Program ftn_DT

!IMPLICIT NONE

  include 'mpif.h'
  
  INTEGER myid, numprocs
  INTEGER x,y,z
  CHARACTER l
  REAL e
  LOGICAL tf
  CHARACTER*5 word
  COMPLEX r
  INTEGER ierr
  INTEGER m(10)
  REAL*8 df


  type person
     CHARACTER*4 :: name
     INTEGER :: age
  end type

  type(person) :: myself
    myself%age=60
    myself%name='Test'

 call mpi_init(ierr)

  x = 5
  y = 8
  z = x*y
  l = 'k'
  e = 1501.205
  tf = .true.
  word = 'Error'
  df = 1.d+0 + 2.d-02

  if(x.gt.y) then
     tf=.true.
  else
     tf=.false.
  endif

 
  r = (5.229, -4.78)

  print*,'string= ',word                   !should print Error
  print*,'Integer= ',x                     !should print 5
  print*,'char= ',l                        !should print k
  print*,'real= ',e                        !should print 1501.205
  print*,'real8= ', df			
  print*,'complex= ',r                     !should print (327.670013,-6.500000134E-3)
  print*,'Integer multiplication= ',z      !should print 40
  print*,'Boolean= ',tf                    !should print False
  print*,'Struct member= ',myself%name     !should print Test
  print*,'Struct member= ',myself%age      !should print 60


  !Call a function to allow for testing of lgdb step command (which steps into a function)
  CALL getState(m)
  print*,'Contents of array from subroutine= ', m

  call mpi_finalize(ierr)
  end program !ftn_DTend

  SUBROUTINE getState(m)
    INTEGER m(10)
    do i=1,10
       m(i)= i+1;
    end do
    RETURN
  END SUBROUTINE
  


  
