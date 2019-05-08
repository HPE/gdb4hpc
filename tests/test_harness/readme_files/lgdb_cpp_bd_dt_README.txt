LGDB Basic Debugging & Datatype Sanity Test: C++

1. ssh onto mainframe
> ssh mork

2. Load lgdb module
> module load cray-lgdb 

3. (If you have not already) - run the makefiles tool to compile all Sanity Tests. Ensure the files are in the current directory.
> ./make_all or ./make_lgdb

	or

4. compile test file
> CC -g cpp_bd_test.c -o cpp_test1     (These are C++ files)
> CC -g cpp_bd_test.c -o cpp_test2
> CC -g cpp_cmp_diff.c -o cpp_test3

resulting file is named a.out if not specified

5. start lgdb
> lgdb

dbg all>



-------------------------------------------------------------------------------------------
Test 1
-------------------------------------------------------------------------------------------



6. Launch the test file with two ranks.

dbg all> launch $a{2} cpp_test1 --debug
dgb all> launch $b{2} cpp_test2 --debug
dbg all> launch $c{2} cpp_test3 --debug


The program has halted at the call to MPI_init
(at this time, you can debug the program)

a{0..1}: Initial breakpoint, main at /cray/css/u18/kdavis/dev_sanity_tests/DataTypes/C++/cpp_bd_test.c:116
<$a>:
<$a>: Datattype             Value                       Sizeof
<$a>:
<$a>: ___________________________________________________________________
<$a>:
<$a>: Datattype             Value                       Sizeof
<$a>: ___________________________________________________________________
<$a>:
<$a>: Char =                  t                    Size = 1 Bytes
<$a>: Char =                  t                    Size = 1 Bytes
<$a>: Unsigned char =   236                  Size = 1 Bytes
<$a>: Unsigned char =   236                  Size = 1 Bytes
<$a>: Signed char =           -20                  Size = 1 Bytes
<$a>: Signed char =           -20                  Size = 1 Bytes
<$a>: Boolean =               1                    Size = 1 Bytes
<$a>:
<$a>: Boolean =               1                    Size = 1 Bytes
<$a>:
<$a>: Unsigned short int =      32767                Size = 2 Bytes
<$a>: Unsigned short int =      32767                Size = 2 Bytes
<$a>: Signed short int =        -32767               Size = 2 Bytes
<$a>: Signed short int =        -32767               Size = 2 Bytes
<$a>:
<$a>:
<$a>: Integer =               1                    Size = 4 Bytes
<$a>: Integer =               1                    Size = 4 Bytes
<$a>: Signed int =            -44                  Size = 4 Bytes
<$a>: Signed int =            -44                  Size = 4 Bytes
<$a>: Unsigned int =          44                   Size = 4 Bytes
<$a>: Unsigned int =          44                   Size = 4 Bytes
<$a>: Int Multiplication =      2                    Size = 4 Bytes
<$a>: Int Multiplication =      2                    Size = 4 Bytes
<$a>: Float =                 2.3307               Size = 4 Bytes
<$a>: Float =                 2.3307               Size = 4 Bytes
<$a>: Struct member =   100                  Size = 4 Bytes
<$a>: Struct member =   100                  Size = 4 Bytes
<$a>:
<$a>:
<$a>: Struct member =   Cray                 Size = 8 Bytes
<$a>: Struct member =   Cray                 Size = 8 Bytes
<$a>: String  =               Testing              Size = 8 Bytes
<$a>: String  =               Testing              Size = 8 Bytes
<$a>: Pointer to int =  1                    Size = 8 Bytes
<$a>: Pointer to int =  1                    Size = 8 Bytes
<$a>: Long double =           3.14159              Size = 8 Bytes
<$a>: Long double =           3.14159              Size = 8 Bytes
<$a>: Double =                3.14159              Size = 8 Bytes
<$a>: Double =                3.14159              Size = 8 Bytes
<$a>: unsigned long int =       4294967295           Size = 8 Bytes
<$a>: unsigned long int =       4294967295           Size = 8 Bytes
<$a>: signed long int =         -2147483648          Size = 8 Bytes
<$a>: signed long int =         -2147483648          Size = 8 Bytes
<$a>:
<$a>:
<$a>:
<$a>:
<$a>: Call to function pointer received: 2 It should receive: 2
<$a>: Call to function pointer received: 2 It should receive: 2
<$a>: Function pointer returned: 4 It should return: 4
<$a>: Function pointer returned: 4 It should return: 4
<$a>:
<$a>:

NOTE: There will be one set of output data for each procset.


Display current location - should be on line 116
dgb all> l
a{0..1}: 116           ierr = MPI_Init(&argc, &argv);
a{0..1}: 117           if (ierr != MPI_SUCCESS) {
a{0..1}: 118                               cout << stderr << "Error: MPI_init() failed: " << ierr;
a{0..1}: 119                   return(2);
a{0..1}: 120           }
a{0..1}: 121
a{0..1}: 122           ierr = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
a{0..1}: 123           if (ierr != MPI_SUCCESS) {
a{0..1}: 124                               cout << stderr << "Error: MPI_init() failed: " << ierr;
a{0..1}: 125                   return(2);



7. Set breakpoints
dbg all> b 134
dbg all> b 136




8. Run to first breakpoint:
dbg all> continue
a{0..1}: Breakpoint 1, main(int, signed char**) at /cray/css/u18/kdavis/dev_sanity_tests/DataTypes/C++/cpp_bd_test.c:134
b{0..1}: Breakpoint 1, main(int, signed char**) at /cray/css/u18/kdavis/dev_sanity_tests/DataTypes/C++/cpp_cmp_same.c:134
c{0..1}: Breakpoint 1, main(int, signed char**) at /cray/css/u18/kdavis/dev_sanity_tests/DataTypes/C++/cpp_cmp_diff.c:134



9. Confirm program is at specified line:
dbg all> l
a{0..1}: 134           fprintf(stderr, "Hello world!  I am rank %d of %d", rank, size);
a{0..1}: 135
a{0..1}: 136           ierr = MPI_Finalize();
a{0..1}: 137           if (ierr != MPI_SUCCESS) {
a{0..1}: 138                               cout << stderr << "Error: MPI_init() failed: " << ierr;
a{0..1}: 139                   return(2);
a{0..1}: 140           }
a{0..1}: 141    }
a{0..1}: 142
a{0..1}: 143    int my_Function(int x){



10. print a few variables x = 1, $a::ret = 4, $b::ret = 6
dbg all> p x
a{0..1}: 1
b{0..1}: 1
c{0..1}: 5


dbg all> p ret
a{0..1}: 4
b{0..1}: 4
b{0..1}: 6

11. Test the compare command
dbg all> compare $c{0}::ret=$a{0}::ret
Warning: Range use detected in procset for command. Only operating on a single rank!
Warning: Range use detected in procset for command. Only operating on a single rank!
a{0}: Comparison is false.
The difference delta is:
a{0}: 2



dbg all> compare $a{0}::"sizeof(ret)" = $b{0}::"sizeof(ret)"
Warning: Range use detected in procset for command. Only operating on a single rank!
Warning: Range use detected in procset for command. Only operating on a single rank!
a{0}: Comparison is true.


12. Continue to second break point
a{0..1}: Breakpoint 2, main(int, signed char**) at /cray/css/u18/kdavis/dev_sanity_tests/DataTypes/C++/cpp_bd_test.c:136
<$a>:
<$a>: Hello world!  I am rank 0 of 2
<$a>:
<$a>: Hello world!  I am rank 1 of 2

NOTE: There will be one set of output data for each procset.



13. #print a variable - should be 2
dbg all> p y
a{0..1}: 2
b{0..1}: 2
c{0..1}: 8



14. Ensure program is at second breakpoint - line # 136
dbg all> l
a{0..1}: 136           ierr = MPI_Finalize();
a{0..1}: 137           if (ierr != MPI_SUCCESS) {
a{0..1}: 138                               cout << stderr << "Error: MPI_init() failed: " << ierr;
a{0..1}: 139                   return(2);
a{0..1}: 140           }
a{0..1}: 141    }
a{0..1}: 142
a{0..1}: 143    int my_Function(int x){
a{0..1}: 144
a{0..1}: 145      int y = 2;



15. Run to end of program:
dbg all> continue
a{0..1}: The application has reached an exit breakpoint. Please quit CCDB!
b{0..1}: The application has reached an exit breakpoint. Please quit CCDB!
c{0..1}: The application has reached an exit breakpoint. Please quit CCDB!


16. Exit debugger
dbg all> q
Shutting down debugger and killing application for 'a'.
Shutting down debugger and killing application for 'b'.
Shutting down debugger and killing application for 'c'.



---------------------------------------------------------------------------------------------------------
TEST 2
---------------------------------------------------------------------------------------------------------

You'll be running a compare source script in test 2.  It tests the compare function where
variables from two files should be the same.  Output from ALL of these commands should be true.

Run lgdb:
>lgdb

dbg all> source lgdb_cpp_t2.rc

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

Run lgdb:
>lgdb

dbg all> source lgdb_cpp_t3.rc

The script will run, verify that the outputs from each comparision is false.

The script will automatically exit lgdb when done.


DONE
