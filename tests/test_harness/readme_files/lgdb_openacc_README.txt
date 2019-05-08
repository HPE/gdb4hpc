LGDB Open Acc Sanity Test - Instructions:

The following test must be run on a mainframe having GPU's.

1. ssh onto mainframe
> ssh kaibab 

2. load lgdb module
> module load cray-lgdb 

3.Load the following module to ensure open acc is enabled 
-> module load craype-accel-nvidia35

4.Set Debug env variable
-> export CRAY_DBG_LOG_DIR=/lus/scratch/username

4.Confirm openacc runs on GPU, you must specify the nodes to run on
Message of the Day
->cat /etc/motd.wlm
	will show command which will indicate the GPU nodes available
 	example: cnselect numcores.ne.24

->cnselect numcores.ne.24
  result: 60-61,34-35,32-33,62-65,94-95,92-93,66-69,58-59,36-37,90-91,88-89,70-71,56-57. . .
Take note of availabe GPU's

5. (If you have not already) - run the makefiles tool to compile all Sanity Tests. Ensure the files are in the current directory.
-> ./make_lgdb or ./make_all

	or
compile test file
-> ftn -g open_acc_test.f90 -o open_acc

6. Run lgdb
-> lgdb


7. Run open_acc program in lgdb
dbg all> launch $a{2} --aprun-args="-N1" --qsub=batch.pbs open_acc --gpu --debug

Program will hit initial break at mpi_init()


8. get into gdbmode
dbg all> gdbmode
Entering gdb pass-thru mode. Type "end" to exit mode...

Notice if the response contains the following:

NVIDIA (R) CUDA Debugger
If so, then the program is running on GPU nodes. Continue to step 9.  If not, test fails. Execute step 9, then goto step 13. 

9. Exit gdbmode
> end


10. set breakpoints:

dbg all> break 24
a{0..1}: Breakpoint 1: file , line 0.


dbg all> break 28
a{0..1}: Breakpoint 2: file /cray/css/u18/kdavis/dev_sanity_tests/open_acc/ftn_test1/open_acc_test.f90, line 28.


9. Continue to first breakpoint
dbg all> continue
a{0..1}: Breakpoint 1, in

10. Ensure the program is at lin 26
dbg all> l
a{0..1}: 24                     end do
a{0..1}: 25             !$acc end parallel loop
a{0..1}: 26                     call foo(a,b,c)
a{0..1}: 27             !$acc end data
a{0..1}: 28             print *, a
a{0..1}: 29
a{0..1}: 30             tag= 1
a{0..1}: 31             dest= 0
a{0..1}: 32
a{0..1}: 33             100 format('Hello World! From process ', I6)




11. Print contents of 3 arrays:
dbg all> print a
a{0..1}: (0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)

dbg all> print b
a{0..1}: (1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1)

dbg all> print c
a{0..1}: (2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2)


12. Continue to next breakpoint:
dbg all> continue
a{0..1}: Breakpoint 2, test at /cray/css/u18/kdavis/dev_sanity_tests/open_acc/ftn_test1/open_acc_test.f90:28


13. print contents of a
dbg all> print a
a{0..1}: (3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3)

The elements of array should be the sum of the contents of b and c.  (3).



14.  Continue to end:
dbg all> continue
<$¦¦l>:  100*3.
<$¦¦l>:  Hello World! From process      0
<$¦¦l>:  Hello World! From process      1
<$¦¦l>:  100*3.
<$¦¦l>:  STOP
a{0..1}: The application has reached an exit breakpoint. Please quit CCDB!
<$¦¦l>:  STOP

	NOTE:  This inidicates that there are 100 elements in the array a, and each are set to 3.


15.End program & Exit debugger
lgdb all> q

END