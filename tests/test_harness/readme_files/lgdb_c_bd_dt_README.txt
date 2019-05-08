LGDB Basic Debugging & Datatype Sanity Test: c   NOTE: This test also includes the test for most LGDB commands


1. ssh onto mainframe
> ssh mork

2. Load lgdb module
> module load cray-lgdb 

3. (If you have not already) - run the makefiles tool to compile all Sanity Tests. Ensure the files are in the current directory.
> ./make

	or

4. compile test files
> cc -g c_type_test.c -o c_test1   
> cc -g c_type_cmp.c -o c_test2
> cc -g c_type_pause.c -o c_test3  


--------------------------------------------------------------------------------------------------------------
TEST 1:
--------------------------------------------------------------------------------------------------------------


1. start lgdb
> lgdb

dbg all>


2. Launch the test file with two ranks on procsets a and b.  lauch test file with 15 ranks on procset c.

dbg all> launch $a{2} --aprun-args="-N1" c_test1 --debug
dbg all> launch $b{2} --aprun-args="-N1" c_test2 --debug
dbg all> launch $c{15} --aprun-args="-N1" c_test3 --debug

The program has halted at the call to MPI_init  :on line 138
(at this time, you can debug the program)


3. Test set and stop commands - the following command ensures that assertion script will halt if assertion failure is encountered.
dbg all> set stop true

4. Test show command - the following command shows of stopping assertion failure.
dbg all> show stop
Assertion script stopping mode: true

5. Test usage command
dbg all> usage assert
Will display help information on how to structure assertion command


6. Test basic assertion script
dbg all> build $mine
> assert $a{0}::assertTest@"c_type_test.c":160=$b{0}::assertTest@"c_type_cmp.c":160
> assert $a{1}::assertTest@"c_type_test.c":160=$b{1}::assertTest@"c_type_cmp.c":160
>end
Assertion script $mine compiled.

dbg all> start $mine
*** Starting execution of application ***
<$a>: Hello World! from process 0
<$b>: Hello World! from process 0
<$a>: Hello World! from process 1
<$b>: Hello World! from process 1
*** Difference found in AssertID 1 ***
*** Difference found in AssertID 2 ***

*** The interpreter has halted. ***
Assertion script $mine complete.
Successful Assertion Set Iterations: 0
Total Passed Assertions: 0
Total Warned Assertions: 0
Total Failed Assertions: 2

Assertion summary:
AssertID 1:  Pass: 0 Warn: 0 Fail: 1
AssertID 2:  Pass: 0 Warn: 0 Fail: 1
***********************************

Current location:
b{0..1}: Application halted in main at /cray/css/u18/kdavis/dev_sanity_tests/DataTypes/c/c_type_cmp.c:160
a{0..1}: Application halted in main at /cray/css/u18/kdavis/dev_sanity_tests/DataTypes/c/c_type_test.c:160



Ensure that the current location is on line 160 for both proc sets and all ranks.

Total Failed Assertions: 2.

7. Test watch command ------------------------------------------------------------ERROR - watch command not currently working
dbg all> watch -w si
a{0..1}: Access Watchpoint 1: Set on variable si
b{0..1}: Access Watchpoint 1: Set on variable si                                        SKIP UNTIL FIXED
c{0..14}: Access Watchpoint 1: Set on variable si
------------------------------------------------------------------------------------------------------------------------------

8. Set a breakpoint at specified line numbers:
dbg all> b 193

dbg all> b 223

dbg all> b 242



9. Run to breakpoint 1:
dbg all> continue



-------------------------------------------------------------------------------------------------------------------------------
a{0..1}: Watchpoint 1, variable expression: si
a{0..1}: In main at /cray/css/u18/kdavis/dev_sanity_tests/DataTypes/c/c_type_test.c:183               SKIPPED UNTIL WATCHPOINT IS FIXED

The above response indicates watchpiont 1 has been found and written to on line just before 183.
------------------------------------------------------------------------------------------------------------------------------




10. Confirm program is at specified line:
dbg all> l
a{0..1}: 193            for (ii=0; ii<10; ++ii)
a{0..1}: 194            {
a{0..1}: 195                    one_d_i[ii] = one_d_i_cnt++;
a{0..1}: 196                    one_d_l[ii] = one_d_l_cnt++;
a{0..1}: 197                    one_d_f[ii] = one_d_f_cnt;
a{0..1}: 198                    one_d_f_cnt += 1.0;
a{0..1}: 199                    one_d_d[ii] = one_d_d_cnt;
a{0..1}: 200                    one_d_d_cnt += 1.0;
a{0..1}: 201                    one_d_ld[ii] = one_d_ld_cnt;
a{0..1}: 202                    one_d_ld_cnt += 1.0;



//the std out will be duplicated, one for each procset.
//Ensure rank range is correct for each process set.


11. print some local variables
dgb all> p ld
a{0..1}: 28.2
b{0..1}: 28.2
c{0..14}: 28.2

dgb all> p si
a{0..1}: -11
b{0..1}: -11
c{0..14}: -11






12. Run next command
lgdb all> n   

dbg all> l   # should now be on line 190
a{0..1}: 195                    one_d_i[ii] = one_d_i_cnt++;
a{0..1}: 196                    one_d_l[ii] = one_d_l_cnt++;
a{0..1}: 197                    one_d_f[ii] = one_d_f_cnt;
a{0..1}: 198                    one_d_f_cnt += 1.0;
a{0..1}: 199                    one_d_d[ii] = one_d_d_cnt;
a{0..1}: 200                    one_d_d_cnt += 1.0;
a{0..1}: 201                    one_d_ld[ii] = one_d_ld_cnt;
a{0..1}: 202                    one_d_ld_cnt += 1.0;




13. Test defset command

dbg all> defset $new_set $c{0..4},$c{10..14}


dbg all> compare $new_set{1}::si=$c{1}::si
Warning: Range use detected in procset for command. Only operating on a single rank!
Warning: Range use detected in procset for command. Only operating on a single rank!
a{1}: Comparison is true.

This should be true.


14. Validate contents on $new_set and $c are equal (reference $c::si #8)
dbg all> p $new_set::si
new_set{0..4,10..14}: -11


Plus ensure that rank ranges reflect actual number of ranks requested in the defset command


15. Test focus command
dbg all> focus $new_set
dbg new_set> p d
new_set{0..4,10..14}: 27.1

16. Test assign command   ----- this command assign $b::si -12 currently doesnt work but should be supported.  Minor bug to raise.
dbg new_set> focus $b
dbg b> assign si -12
 

dbg b> focus $all
dbg all> p si
a{0..1}: -11
b{0..1}: -12
c{0..14}: -11

Verify that the value for variable si for the $b procset has changed to -12

17. run compare command

dbg all> compare $a{0}::si=$b{0}::si
Warning: Range use detected in procset for command. Only operating on a single rank!
Warning: Range use detected in procset for command. Only operating on a single rank!
a{0}: Comparison is false.
The difference delta is:
a{0}: 1

This should be false.


dbg all> compare $a{0}::"sizeof(si)" = $b{0}::"sizeof(si)"
Warning: Range use detected in procset for command. Only operating on a single rank!
Warning: Range use detected in procset for command. Only operating on a single rank!
a{0}: Comparison is true.

This should be true.



Ensure that after each focus command is sent that the standard output displays:
dbg setFocusedOn>

Ensure that after each print command standard output displays:
setFocusedOn{range}: value

Plus ensure that rank ranges reflect actual number of ranks requested in the launch command.


18. Test viewset command
dbg all> viewset $b
Name       Procs
b          b{0..1}

dbg all> viewset $new_set
Name       Procs
new_set    c{0..4,10..14}



19. Test whatis command  //this will show all invoked process sets only.
dbg all> whatis d
a{0..1}: double
b{0..1}: double
c{0..14}: double
 

dbg all> focus $new_set  //must set focus to the new set to access datatypes

dbg new_set> whatis d
new_set{0..4,10..14}: double


Reset focus:
dbg all> focus $all


20. contintue to next breakpoint
dbg all> continue
			

21. Ensure program is set at breakpoint line # 223
dbg all> l
a{0..1}: 223            stackTest = test_stack(stackTest);
a{0..1}: 224
a{0..1}: 225            /* initialize the structs */
a{0..1}: 226
a{0..1}: 227            struct_test_1.i = 1;
a{0..1}: 228            struct_test_1.d = 2.2;
a{0..1}: 229            struct_test_1.c_ptr = char_ptr_test_1;
a{0..1}: 230            struct_test_1.func_ptr = &test_func;
a{0..1}: 231
a{0..1}: 232            /* initialize the pointers */
b{0..1}: 223            stackTest = test_stack(stackTest);
b{0..1}: 224



22. Test Step command
dbg all> s

23. Ensure program is set at first line of subroutine Line #247
dbg all> l
a{0..1}: 247    int test_stack(int x){
a{0..1}: 248            return x * 5;
a{0..1}: 249    }
a{0..1}: 250
a{0..1}: 251
a{0..1}: 252




24.Test up command
lgdb all> up

25. Ensure program is back at the previous function call line #223
dbg all> l
a{0..1}: 223            stackTest = test_stack(stackTest);
a{0..1}: 224
a{0..1}: 225            /* initialize the structs */
a{0..1}: 226
a{0..1}: 227            struct_test_1.i = 1;
a{0..1}: 228            struct_test_1.d = 2.2;
a{0..1}: 229            struct_test_1.c_ptr = char_ptr_test_1;
a{0..1}: 230            struct_test_1.func_ptr = &test_func;
a{0..1}: 231
a{0..1}: 232            /* initialize the pointers */



26. Test disable command
dbg all> disable 3  //should be no response to std out.

27. Test enable command
dbg all> enable 3  //should be no response to std out.

28. Delete the 3rd breakpoint
delete 3

29. Test down command
dbg all> down

30.  Ensure program is back at the subroutine line #247
dbg all> l
a{0..1}: 247    int test_stack(int x){
a{0..1}: 248            return x * 5;
a{0..1}: 249    }
a{0..1}: 250
a{0..1}: 251
a{0..1}: 252





31. Test frame command
dbg all> frame
a{0..1}: #0  0x0000000000401960 in test_stack at /cray/css/u18/kdavis/dev_sanity_tests/DataTypes/c/c_type_test.c:247
b{0..1}: #0  0x0000000000401960 in test_stack at /cray/css/u18/kdavis/dev_sanity_tests/DataTypes/c/c_type_cmp.c:247
c{0..14}: #0  0x0000000000401960 in test_stack at /cray/css/u18/kdavis/dev_sanity_tests/DataTypes/c/c_type_test.c:247



32. Test backtrace command
dbg all> bt
a{0..1}: #0  0x0000000000401960 in test_stack at /cray/css/u18/kdavis/dev_sanity_tests/DataTypes/c/c_type_test.c:247
a{0..1}: #1  0x0000000000401850 in main at /cray/css/u18/kdavis/dev_sanity_tests/DataTypes/c/c_type_test.c:223

b{0..1}: #0  0x0000000000401960 in test_stack at /cray/css/u18/kdavis/dev_sanity_tests/DataTypes/c/c_type_cmp.c:247
b{0..1}: #1  0x0000000000401850 in main at /cray/css/u18/kdavis/dev_sanity_tests/DataTypes/c/c_type_cmp.c:223

c{0..14}: #0  0x0000000000401960 in test_stack at /cray/css/u18/kdavis/dev_sanity_tests/DataTypes/c/c_type_test.c:247
c{0..14}: #1  0x0000000000401850 in main at /cray/css/u18/kdavis/dev_sanity_tests/DataTypes/c/c_type_test.c:223



33. Test the finish command
dbg all> finish

34. Ensure pogram is at the line just after the function call. 
dbg all> l
a{0..1}: 223            stackTest = test_stack(stackTest);
a{0..1}: 224
a{0..1}: 225            /* initialize the structs */
a{0..1}: 226
a{0..1}: 227            struct_test_1.i = 1;
a{0..1}: 228            struct_test_1.d = 2.2;
a{0..1}: 229            struct_test_1.c_ptr = char_ptr_test_1;
a{0..1}: 230            struct_test_1.func_ptr = &test_func;
a{0..1}: 231
a{0..1}: 232            /* initialize the pointers */




35. Run to end of program:
dbg all> continue
a{0..1}: The application has reached an exit breakpoint. Please quit CCDB!
b{0..1}: The application has reached an exit breakpoint. Please quit CCDB!
c{0..14}: The application has reached an exit breakpoint. Please quit CCDB!


36. Test kill command
dbg all> kill $a
Shutting down debugger and killing application for 'a'.

37. Continue to end of program.
dbg all>continue

38. Test release command
dbg all> release $b
Shutting down debugger and resuming application for 'b'.


39. Test help command
dgb help> help  //Should print out list of lgdb commands along with brief description.

40. Test gdbmode command
dbg all> gdbmode
Entering gdb pass-thru mode. Type "end" to exit mode...

GNU gdb (GDB) 7.2-modified
Copyright (C) 2013 Cray Inc.
Modified by Allinea Software.
Copyright (C) Allinea Software Ltd. 2013
Copyright (C) 2010 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "x86_64-unknown-linux-gnu".
For bug reporting instructions, please see:
<http://www.gnu.org/software/gdb/bugs/>.


Warning: Multiple process sets in focus. Only operating on first process set.

//Ensure title and copyright info is reflected in standard out.

41. Test maint command
> maint system ls
Should display contents of current directory


42. Exit debugger
dbg all> q
Shutting down debugger and killing application for 'c'.




-----------------------------------------------------------------------------------------------------------
TEST 2:
-----------------------------------------------------------------------------------------------------------

Now you will be testing the assertion + decomposition commands

1. run lgdb

2. launch to progams
dbg all> launch $a{2} --aprun-args="-N1" c_test1 --debug
dbg all> launch $b{2} --aprun-args="-N1" c_test2 --debug


The program has halted at the call to MPI_init
(at this time, you can debug the program)

3. Create decomposition scheme
dbg all> decomposition $myComp
>dimension 2
>distribute block
>proc_grid 2
>dim_order 1
>end



4. Test basic assertion script
build $mine
	#use assertion with decomposition
	assert $myComp{$a::assertTest@"c_type_test.c":160}=$myComp{$b::assertTest@"c_type_cmp.c":160}
end

5. Start assertion script
start $mine

Assertion script $mine compiled.
dbg all> start $mine
*** Starting execution of application ***
<$b>: Hello World! from process 0
<$b>: Hello World! from process 1
<$a>: Hello World! from process 0
<$a>: Hello World! from process 1
*** Difference found in AssertID 1 ***

*** The interpreter has halted. ***
Assertion script $mine complete.
Successful Assertion Set Iterations: 0
Total Passed Assertions: 0
Total Warned Assertions: 0
Total Failed Assertions: 1

Assertion summary:
AssertID 1:  Pass: 0 Warn: 0 Fail: 1
***********************************

Current location:
b{0..1}: Application halted in main at /cray/css/u18/kdavis/dev_sanity_tests/DataTypes/c/c_type_cmp.c:160
a{0..1}: Application halted in main at /cray/css/u18/kdavis/dev_sanity_tests/DataTypes/c/c_type_test.c:160



Ensure that the current location is on line 160 for both proc sets and all ranks.

This test should show Total Failed Assertions: 1.

6.  Exit lgdb
dbg all> q
Shutting down debugger and killing application for 'a'.
Shutting down debugger and killing application for 'b'.



--------------------------------------------------------------------------------------------------------------------
TEST 3:
--------------------------------------------------------------------------------------------------------------------

Now you need to test the attach command

Ensure two Xterm windows are open.

1. create a qsub session as follows:
>qsub -I -Display -l mppwidth=2

2. Run the c_type_pause.c program in aprun:
>aprun -n2 ./c_test3

This program will run and then pause so you can attach.

3. In the second Xterm window, get the apid of the running program.
> apstat


4. load lgdb
>module load cray-lgdb

5. run lgdb
>lgdb
dbg all> attach $a apid --debug
Attaching to application, please wait...
Creating MRNet communication network...
Waiting for debug servers to attach to MRNet communications network...
Timeout in 60 seconds. Please wait for the attach to complete.
Number of dbgsrvs connected: [1];  Timeout Counter: [0]
Number of dbgsrvs connected: [1];  Timeout Counter: [1]
Number of dbgsrvs connected: [2];  Timeout Counter: [0]
Finalizing setup...
Attach complete.
Current rank location:
a{0}: #0  0x000000000055cf90 in __read_nocancel at /usr/src/packages/BUILD/glibc-2.11.3/nptl/../sysdeps/unix/syscall-template.S:82
a{0}: #1  0x000000000057a338 in _IO_new_file_underflow at /usr/src/packages/BUILD/glibc-2.11.3/libio/fileops.c:598
a{0}: #2  0x000000000057b60e in _IO_default_uflow at /usr/src/packages/BUILD/glibc-2.11.3/libio/genops.c:440
a{0}: #3  0x00000000005789a9 in getchar at /usr/src/packages/BUILD/glibc-2.11.3/libio/getchar.c:38
a{0}: #4  0x000000000040136b in main at /cray/css/u18/kdavis/dev_sanity_tests/DataTypes/c/c_type_pause.c:186
a{1}: #0  0x000000000055cf90 in __read_nocancel at /usr/src/packages/BUILD/glibc-2.11.3/nptl/../sysdeps/unix/syscall-template.S:82
a{1}: #1  0x000000000057a338 in _IO_new_file_underflow at /usr/src/packages/BUILD/glibc-2.11.3/libio/fileops.c:598
a{1}: #2  0x000000000057b60e in _IO_default_uflow at /usr/src/packages/BUILD/glibc-2.11.3/libio/genops.c:440
a{1}: #3  0x00000000005789a9 in getchar at /usr/src/packages/BUILD/glibc-2.11.3/libio/getchar.c:38
a{1}: #4  0x0000000000401380 in main at /cray/css/u18/kdavis/dev_sanity_tests/DataTypes/c/c_type_pause.c:186



6. Type the following command x4, to access global variables in main function
>up
>up
>up
>up
a{0}: #4  0x000000000040136b in main at /cray/css/u18/kdavis/dev_sanity_tests/DataTypes/c/c_type_pause.c:186
a{1}: #4  0x0000000000401380 in main at /cray/css/u18/kdavis/dev_sanity_tests/DataTypes/c/c_type_pause.c:186

7. Print contents of a variable to ensure you can debug.
dbg all> p si
a{0..1}: -11

dbg all> p d
a{0..1}: 27.
dbg all>


8. exit lgdb
>q
Shutting down debugger and resuming application for 'a'.


9. Go to Xterm which is running the program. 
>Ctrl+c exits the program



DONE
