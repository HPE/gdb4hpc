LGDB UPC Sanity Test - Instructions:

1. ssh onto mainframe.
-> ssh mork

2. Load lgdb module
-> module load cray-lgdb 

3. Set LGDB environment variable
-> CRAY_DBG_LOG_DIR=/lus/scratch/username


4. (If you have not already) - run the makefiles tool to compile all Sanity Tests. Ensure the files are in the current directory.
-> ./make_lgdb or ./make_all

	or

5. compile test file
-> cc -g -X5 -hupc -o upc upc_test.c


6. start lgdb
> lgdb

dbg all>

7. Launch the test file with 5 ranks.

dbg all> launch $a{5} --aprun-args="-N1" upc --debug

Program will automatically break at main function(): line 78

8. Ensure program is at line 78
dbg all> l
a{0..4}: 78     main()
a{0..4}: 79     {
a{0..4}: 80       test05 ();
a{0..4}: 81       return 0;
a{0..4}: 82     }


8. Create breakpoints
dbg all> break 81
a{0..4}: Breakpoint 1: file /cray/css/u18/kdavis/dev_sanity_tests/upc/intrepid_test05.c, line 81.


9. Continue to breakpoint:
dbg all> continue
a{0..4}: Breakpoint 1, test05 at /cray/css/u18/kdavis/dev_sanity_tests/upc/intrepid_test05.c:81

10. Ensure program is at line 81
dbg all> l
a{0..4}: 81       return 0;
a{0..4}: 82     }




11. Enter gdbmode
dbg all> gdbmode
Entering gdb pass-thru mode. Type "end" to exit mode...

12. While still in gdbmode, display the language used
> show lang
The current source language is "auto; currently upc".



13. While still in gdbmode, display address of shared array
> p & array[0]

	PASS STATE:
	If upc is turned on and functioning you should get the following (address will vary)
	$1 = (shared struct data_struct *) (0x934fc0,0)

	If you get an address, continue to step 13.


	FAIL STATE:
	If upc is turned off and/or not functioning you should get the following
	a{0..4}: Debugger error: UPC language support is not initialised

	A.  Attempt to initialize upc
		> upc-init
		a{0..4}: Debugger error: connect: No such file or directory.

	b.  Attempt to print array contents
		> p array
		a{0..4}: Debugger error: UPC language support is not initialised



	If you have reached step b, and you receive the error.  Test has FAILED. 
	>end
	dgb all> continue
	dbg all> ctl+c
	dbg all> q




14.  Print contents at address of array (use address displayed in last command)
> p *(shared struct data_struct*) 0x934fc0


a{0}:
$3 = {x1 = 1 '001', x2 = 2, x3 = 3, x4 = 4}


a{1}:
$3 = {x1 = 5 '005', x2 = 6, x3 = 7, x4 = 8}


a{2}:
$3 = {x1 = 9 '  ', x2 = 10, x3 = 11, x4 = 12}


a{3}:
$3 = {x1 = 13 'r', x2 = 14, x3 = 15, x4 = 16}


a{4}:
$3 = {x1 = 17 '021', x2 = 18, x3 = 19, x4 = 20}

Ensure the output is same as above.


15. Exit gdbmode
> end
Ending gdb pass-thru mode. If program location has changed (i.e. continue) debugger is in an unknown state.


16. continue to end

dbg all> continue
a{1..4}: *** The application is at an exit breakpoint.
<$a>: test05: access structured shared array element
<$a>:           using a local pointer - passed.
a{0}: The application has reached an exit breakpoint. Please quit CCDB!


17. Exit debugger
dbg all> q
Shutting down debugger and killing application for 'a'.



END



