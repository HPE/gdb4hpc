Program ftn_DT

!IMPLICIT NONE

  include 'mpif.h'
  
  INTEGER myid, numprocs
  INTEGER x,y,z
  CHARACTER l
  REAL e
  LOGICAL tf
  CHARACTER*7 word
  COMPLEX r
  INTEGER ierr
  INTEGER m(10)
  REAL*8 df


  type person
     CHARACTER*4 :: name
     INTEGER :: age
  end type

  type(person) :: myself
    myself%age=100
    myself%name='Cray'

 call mpi_init(ierr)

  x = 1
  y = 2
  z = x*y
  l = 't'
  e = 30.32
  tf = .false.
  word = 'Testing'
  df = 1.d+0

  if(x.lt.y) then
     tf=.true.
  else
     tf=.false.
  endif

 
  r = (3.2767e+2, -0.65e-2)

  print*,'string= ',word                   !should print Testing
  print*,'Integer= ',x                     !should print 1
  print*,'char= ',l                        !should print t
  print*,'real= ',e                        !should print 30.3199997
  print*,'real8= ', df					   
  print*,'complex= ',r                     !should print (327.670013,-6.500000134E-3)
  print*,'Integer multiplication= ',z      !should print 2
  print*,'Boolean= ',tf                    !should print T
  print*,'Struct member= ',myself%name     !should print Cray
  print*,'Struct member= ',myself%age      !should print 100


  !Call a function to allow for testing of lgdb step command (which steps into a function)
  CALL getState(m)
  print*,'Contents of array from subroutine= ', m

  call mpi_finalize(ierr)
  end program !ftn_DTend

  SUBROUTINE getState(m)
    INTEGER m(10)
    do i=1,10
       m(i)= i;
    end do
    RETURN
  END SUBROUTINE
  


  
