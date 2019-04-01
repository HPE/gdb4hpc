LGDB Basic Debugging & Datatype Sanity Test: FORTRAN  


1. ssh onto mainframe
> ssh mork

2. Load lgdb module
> module load cray-lgdb 

3. (If you have not already) - run the makefiles tool to compile all Sanity Tests. Ensure the files are in the current directory.
> ./make_all or ./make_lgdb

	or

3. compile test file
> ftn -g ftn_bd_dt.f90 -o fnt_test1
> ftn -g ftn_cmp_same.f90 -o ftn_test2
> ftn -g ftn_cmp_diff.f90 -o ftn_test3
   
resulting file is named a.out if not specified

4. start lgdb
> lgdb

dbg all>

---------------------------------------------------------------------------------------------------------
TEST 1
---------------------------------------------------------------------------------------------------------

5. Launch the test file with two ranks on proc sets a and b.  lauch test file with 50 ranks on proc set c.

dbg all> launch $a{2} --aprun-args="-N1" ftn_test1 --debug
dbg all> launch $b{2} --aprun-args="-N1" ftn_test2 --debug
dbg all> launch $c{2} --aprun-args="-N1" ftn_test3 --debug


The program has halted at the call to MPI_init
(at this time, you can debug the program)

a{0..1}: Initial breakpoint, ftn_dt at /cray/css/u18/kdavis/dev_sanity_tests/DataTypes/fortran/ftn_bd_dt.f90:30

6. Confirm program is at line 30.
dbg all> l
a{0..1}: 30       x = 1
a{0..1}: 31       y = 2
a{0..1}: 32       z = x*y
a{0..1}: 33       l = 't'
a{0..1}: 34       e = 30.32
a{0..1}: 35       tf = .false.
a{0..1}: 36       word = 'Testing'
a{0..1}: 37       df = 3.1415926535d+0
a{0..1}: 38
a{0..1}: 39       if(x.lt.y) then



7. Set a breakpoints at specified line numbers:
dbg all> b 48
a{0..1}: Breakpoint 1: file /cray/css/u18/kdavis/dev_sanity_tests/DataTypes/fortran/ftn_bd_dt.f90, line 48.
b{0..1}: Breakpoint 1: file /cray/css/u18/kdavis/dev_sanity_tests/DataTypes/fortran/ftn_cmp.f90, line 48.
dbg all> b 62
a{0..1}: Breakpoint 2: file /cray/css/u18/kdavis/dev_sanity_tests/DataTypes/fortran/ftn_bd_dt.f90, line 62.
b{0..1}: Breakpoint 2: file /cray/css/u18/kdavis/dev_sanity_tests/DataTypes/fortran/ftn_cmp.f90, line 62.



8. Run to breakpoint 1:
dbg all> c
a{0..1}: Breakpoint 1, ftn_dt at /cray/css/u18/kdavis/dev_sanity_tests/DataTypes/fortran/ftn_bd_dt.f90:48
b{0..1}: Breakpoint 1, ftn_dt at /cray/css/u18/kdavis/dev_sanity_tests/DataTypes/fortran/ftn_cmp.f90:48

9. Confirm program is at first breakpoint - line 46
dbg all> l
a{0..1}: 48       print*,'string= ',word                   !should print Testing
a{0..1}: 49       print*,'Integer= ',x                     !should print 1
a{0..1}: 50       print*,'char= ',l                        !should print t
a{0..1}: 51       print*,'real= ',e                        !should print 30.3199997
a{0..1}: 52       print*,'real8= ', df
a{0..1}: 53       print*,'complex= ',r                     !should print (327.670013,-6.500000134E-3)
a{0..1}: 54       print*,'Integer multiplication= ',z      !should print 2
a{0..1}: 55       print*,'Boolean= ',tf                    !should print T
a{0..1}: 56       print*,'Struct member= ',myself%name     !should print Cray
a{0..1}: 57       print*,'Struct member= ',myself%age      !should print 100



//the std out will be duplicated, one set of data for each process set.
//Ensure rank range is correct for each process set.

10. print some local variables
dbg all> p x
a{0..1}: 1
b{0..1}: 1
c{0..1}: 5


dbg all> p tf
a{0..1}: .TRUE.
b{0..1}: .TRUE.
c{0..1}: .FALSE.



11. run compare command
dbg all> compare $a{0}::tf=$b{0}::tf
Warning: Range use detected in procset for command. Only operating on a single rank!
Warning: Range use detected in procset for command. Only operating on a single rank!
a{0}: Comparison is true.

NOTE: The first compare should be true

dbg all> compare $a{0}::tf=$c{0}::tf
Warning: Range use detected in procset for command. Only operating on a single rank!
Warning: Range use detected in procset for command. Only operating on a single rank!
a{0}: Comparison is false.
The difference delta is:
a{0}: 1


NOTE: The second compare should be false


lgdgb all> compare $a{0}::"sizeof(tf)" = $b{0}::"sizeof(tf)"
Warning: Range use detected in procset for command. Only operating on a single rank!
Warning: Range use detected in procset for command. Only operating on a single rank!
a{0}: Comparison is true.

Ensure both results in "Comparision is true."

 

12. Continue to the next breakpoint
dbg all> continue
b{0..1}: Breakpoint 2, getstate at /cray/css/u18/kdavis/dev_sanity_tests/DataTypes/fortran/ftn_cmp.f90:62
a{0..1}: Breakpoint 2, getstate at /cray/css/u18/kdavis/dev_sanity_tests/DataTypes/fortran/ftn_bd_dt.f90:62
<$a>:  string= Testing
<$a>:  Integer=  1
<$a>:  char= t
<$a>:  real=  30.3199997
<$a>:  real8= 3.1415926535000001
<$a>:  complex=  (327.670013,-6.500000134E-3)
<$a>:  Integer multiplication=  2
<$a>:  Boolean=  T
<$a>:  Struct member= Cray
<$a>:  Struct member=  100
<$a>:  string= Testing
<$a>:  Integer=  1
<$a>:  char= t
<$a>:  real=  30.3199997
<$a>:  real8= 3.1415926535000001
<$a>:  complex=  (327.670013,-6.500000134E-3)
<$a>:  Integer multiplication=  2
<$a>:  Boolean=  T
<$a>:  Struct member= Cray
<$a>:  Struct member=  100
<$b>:  string= Error
<$b>:  Integer=  1
<$b>:  char= t
<$b>:  real=  30.3199997
<$a>:  real8= 3.1415926535000001
<$b>:  complex=  (327.670013,-6.500000134E-3)
<$b>:  Integer multiplication=  4
<$b>:  Boolean=  F
<$b>:  Struct member= Test
<$b>:  Struct member=  100
<$b>:  string= Error
<$b>:  Integer=  1
<$b>:  char= t
<$b>:  real=  30.3199997
<$a>:  real8= 3.1415926535000001
<$b>:  complex=  (327.670013,-6.500000134E-3)
<$b>:  Integer multiplication=  4
<$b>:  Boolean=  F
<$b>:  Struct member= Test
<$b>:  Struct member=  100



13. Ensure proram is at 2nd breakpoint - line 62
dbg all> l
a{0..1}: 62       print*,'Contents of array from subroutine= ', m
a{0..1}: 63
a{0..1}: 64       call mpi_finalize(ierr)
a{0..1}: 65       end program !ftn_DTend
a{0..1}: 66
a{0..1}: 67       SUBROUTINE getState(m)
a{0..1}: 68         INTEGER m(10)
a{0..1}: 69         do i=1,10
a{0..1}: 70            m(i)= i;
a{0..1}: 71         end do



14. Print variable to stdout
dbg all> p m
a{0..1}: (1, 2, 3, 4, 5, 6, 7, 8, 9, 10)
b{0..1}: (1, 2, 3, 4, 5, 6, 7, 8, 9, 10)
c{0..1}: (2, 3, 4, 5, 6, 7, 8, 9, 10, 11)



15. Run program to its completion.
dbg all>continue 
<$b>:  Contents of array from subroutine=  1,  2,  3,  4,  5,  6,  7,  8,  9,  10
<$b>:  Contents of array from subroutine=  1,  2,  3,  4,  5,  6,  7,  8,  9,  10
<$c>:  Contents of array from subroutine=  2,  3,  4,  5,  6,  7,  8,  9,  10,  11
<$a>:  Contents of array from subroutine=  1,  2,  3,  4,  5,  6,  7,  8,  9,  10
<$a>:  Contents of array from subroutine=  1,  2,  3,  4,  5,  6,  7,  8,  9,  10
<$c>:  Contents of array from subroutine=  2,  3,  4,  5,  6,  7,  8,  9,  10,  11
b{0..1}: The application has reached an exit breakpoint. Please quit CCDB!
c{0..1}: The application has reached an exit breakpoint. Please quit CCDB!
a{0..1}: The application has reached an exit breakpoint. Please quit CCDB!



25. Kill the applications
dbg all> kill $a
Shutting down debugger and killing application for 'a'.

dbg all> kill $b
Shutting down debugger and killing application for 'b'.

dbg all> kill $c
Shutting down debugger and killing application for 'c'.



---------------------------------------------------------------------------------------------------------
TEST 2
---------------------------------------------------------------------------------------------------------

You'll be running a compare source script in test 2.  It tests the compare function where
variables from two files should be the same.  Output from ALL of these commands should be true.


dbg all> source lgdb_ftn_t2.rc

The script will run, verify that the outputs from each comparision is true.

The script will automatically exit lgdb when done.



---------------------------------------------------------------------------------------------------------
TEST 3
---------------------------------------------------------------------------------------------------------

You'll be running a compare source script in test 3.  It tests the compare function where
variables from two files should be different.  Output from ALL of these commands should be false.
Note that this test is a bit shorter than Test2, because the compare function doesn't currently capture 
inequalities between strings and chars.  When the bug is resolved these datatypes will be enabled in this
test.


dbg all> source lgdb_ftn_t3.rc

The script will run, verify that the outputs from each comparision is false.

The script will automatically exit lgdb when done.








































DONE
