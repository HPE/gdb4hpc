---
title: "Getting Started with Gdb4hpc"
---

# The help system
In classic unix tradition, starting up gdb4hpc presents a prompt expecting the user to know what to type.   In this case, `help` is a good choice, it will present you with a list of every available command, and `help <command>` gives you the full details.

That is a reference manual, this user guide is intended to present what you need to get up and debugging.    

# Building and launching your first application

This section will be based on `mpi_example` in the `demos` directory of the gdb4hpc package.  You should be able to copy that to a writable directory and build the C, C++ and fortran versions.

For gdb to be able to show you the most information you will want to build your *(MPI)* test application with the `-g -O0` flags.   `-g` tells the compiler to adds information to the executable to map the executable state back to the line numbers and variable names in your source files.

Depending on your level of experience with debugging, the hardest part of getting started it literally *getting started*.

So start up gdb4hpc with no arguments.   
```
jvogt@pegasus mpi_example]$ gdb4hpc
gdb4hpc 4.14.1.0 - Cray Line Mode Parallel Debugger
With Cray Comparative Debugging Technology.
Copyright 2007-2022 Hewlett Packard Enterprise Development LP.
Copyright 1996-2016 University of Queensland. All Rights Reserved.

Type "help" for a list of commands.
Type "help <cmd>" for detailed help about a command.
dbg all>
```

Then launch your application on 10 nodes with the `launch` command:
```
dbg all> launch $a{10} --launcher-args="--exclusive" ./hello_mpi_c
Starting application, please wait...
Creating MRNet communication network...
Waiting for debug servers to attach to MRNet communications network...
Timeout in 400 seconds. Please wait for the attach to complete.
Number of dbgsrvs connected: [1];  Timeout Counter: [0]
Number of dbgsrvs connected: [1];  Timeout Counter: [1]
Number of dbgsrvs connected: [10];  Timeout Counter: [0]
Finalizing setup...
Launch complete.
a{0..9}: Initial breakpoint, main at /home/users/jvogt/rpm_install/gdb4hpc/4.14.0.0/demos/mpi_example/hello_mpi.c:13
dbg all>
```

And gdb4hpc will start up 10 copies of `testApp 0` and stop them all at `main`.  From this point on, you will be able to control and observe the running of your application.

The `launcher-args` argument is the set of argument you would pass to srun, mpiexec, aprun, or qsub, though you don't need to include the `-n` or `-N` argument for the number of tasks.   

`-a` or `--program-args` are the arguments going to your application.

The `$a{10}` is a **process set**, sometimes called a **procset** in gdb4hpc.   Since you've done the launch, you can think of your application as an array of 10 processes.   In this instance, it's acting like an array declaration creating an array named `a` with 10 processes.

To distinguish gdb and gdb4hpc variables from program variables, they prepended with a dollar sign, and it used the curly braces for the indexing.

The choice `a` for the procset name is arbitrary.  The reason it has a name is the gdb4hpc can control more than one application run at a time.

# If you've never used a debugger

If you were following along, at this point you are debugging an HPC program.  There are 10 application processes being held at entry by ten instances of gdb which are communicating with one gdb4hpc.

Use the `list` command to show source around the current location:

```
dbg all> list
a{0..9}: 13   	  int source, dest=0, tag=0;
a{0..9}: 14   	  char message[100];
a{0..9}: 15   	  MPI_Status status;
a{0..9}: 16   	
a{0..9}: 17   	  MPI_Init(&argc,&argv);
a{0..9}: 18   	
a{0..9}: 19   	  MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
a{0..9}: 20   	  MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
a{0..9}: 21   	
a{0..9}: 22   	  if (myRank != 0) {
dbg all> list
a{0..9}: 23   	    sprintf(message,"Hello World! from process %d", myRank);
a{0..9}: 24   	    MPI_Send(message, strlen(message)+1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
a{0..9}: 25   	  } else {
a{0..9}: 26   	    printf("Hello World! from process %d\n", myRank);
a{0..9}: 27   	
a{0..9}: 28   	    for(source=1; source<numProcs; source++) {
a{0..9}: 29   	      MPI_Recv(message, 100, MPI_CHAR, source, tag, MPI_COMM_WORLD, &status);
a{0..9}: 30   	      printf("%s\n", message);
a{0..9}: 31   	    }
a{0..9}: 32   	  }
dbg all>
```

Note how using the `list` commands continues the listing.   You can also list specific lines or lines in a particular file by adding line number arguments.

Now we can move ahead a few lines until something interesting happens using the `next` command:
```
dbg all> n
a{0..9}: 19	  MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
dbg all> n
a{0..9}: 20	  MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
dbg all> n
a{0..9}: 22	  if (myRank != 0) {
dbg all> 
```

Now we can print the value of these variables.

```
dbg all> p numProcs
a{0..9}: 10
dbg all> p myRank
a{0}: 0
a{1}: 1
a{2}: 2
a{3}: 3
a{4}: 4
a{5}: 5
a{6}: 6
a{7}: 7
a{8}: 8
a{9}: 9
dbg all> 
```

So even in this simple program, there are variables with the same value in every rank, and with different values in every rank.   Note how gdb4hpc has *aggregated* the first result into a single line.

Now we'll set a breakpoint on line 24 and continue.

```
dbg all> b 24
a{0..9}: Breakpoint 1: file /home/users/jvogt/rpm_install/gdb4hpc/4.14.1.0/demos/mpi_example/hello_mpi.c, line 24.
dbg all> c
a{1..9}: Breakpoint 1, main at /home/users/jvogt/rpm_install/gdb4hpc/4.14.1.0/demos/mpi_example/hello_mpi.c:24
dbg all>
```

The `continue` command, aliased to `c` lets the program run normally until it finishes, gets a signal, or hits our breakpoint.

In this case, it's the breakpoint, which was hit by 9 of the ranks.  Rank 0 took the other branch, and is in fact, still running.   

```
dbg all> info status
a{0}: The application is running.
a{1..9}: The application is stopped
```

We now force it to stop, so we can see what it's doing.

```
dbg all> halt
a{0}: Application halted in MPIDI_CH3I_SMP_read_progress
a{1..9}: Application halted in main at /home/users/jvogt/rpm_install/gdb4hpc/4.14.1.0/demos/mpi_example/hello_mpi.c:24
dbg all> bt
a{0}: #3  main at /home/users/jvogt/rpm_install/gdb4hpc/4.14.1.0/demos/mpi_example/hello_mpi.c:29
a{0}: #2  PMPI_Recv
a{0}: #1  MPIDI_CH3I_Progress
a{0}: #0  MPIDI_CH3I_SMP_read_progress
   
a{1..9}: #0  main at /home/users/jvogt/rpm_install/gdb4hpc/4.14.1.0/demos/mpi_example/hello_mpi.c:24
   
dbg all>
```

`bt` is short for "backtrace" and shows the call stack for each process.   

Not too surprisingly, rank 0 is waiting for the messages from the other 9 ranks that won't happen until we continue.  

This was a very bare bones introduction, but it does show off the key features of interactive debugging, both controlling and examining the program state.

# If you've never used gdb before

If you're more used to a GUI based debugger, gdb and gdb4hpc provide the same functionality but in a shell-like environment where you type in commands and it prints results.   The most common commands have one or two character aliases.   Here are the most frequently used:

- print (p) print the value of a program variable
- next (n) continue to the next line stepping over function calls
- step (s) continue to line, stepping into the function if the current line is a function
- finish continue until the exit from the current function
- until continue to a specific line number
- backtrace (bt) list the stack frames of the current location
- up move the focus one frame up the stack (i.e. toward main). 
- down move the focus on frame down the stack
- break (b) set a breakpoint on a function or line number
- info break (i br) list the current breakpoints
- del delete a breakpoint
- continue ( c) let the program run freely until it reaches the exit, a signal or a breakpoint

For the full list of commands type `help` .   `help <function>` will give the full information on any command.

There are many gdb cheat-sheets and tutorials on-line and nearly everything there can be applied to gdb4hpc.

# If you are familiar with gdb

You should fell at home with all the familiar commands : n,s,c,b etc.  There are just a few things you'll need to know about the "*4hpc*" part.

## Procset output
In gdb, when you do `print i` there is exactly one value to print, but with parallel debugging there is potentially a different value in every rank.

When gdb4hpc prints the values, it aggregates all ranks with the same values and prints those on a single line with the processes that share that value.

```
dbg all> p numProcs
a{0..9}: 10
```

Note the use of `..` sequences to shorten the output.    Examples of non-sequential results are: `$a{0,3}`, or `$a{1,5..7,9}`.

## The focus

By default, every command you enter is sent to every rank, but you will often want to work with a single rank or possibly a subset of ranks.   The `focus` command takes the same process set notation.

```
dbg all> focus $a{1..9}
dbg a_temp> focus $a{1..2}
dbg a_temp> p myRank
a{1}: 1
a{2}: 2
dbg a_temp> 
```

Gdb4hpc provides the `$all` procset to return the focus to every process.  Note how the prompt changes when you change focus.

## The scope operator

The procset::expression notation lets you print values without need to change the focus.

```
dbg a_temp> focus $all
dbg all> p $a{5}::myRank
a{5}: 5
dbg all>
```

## Gdb mode
Gdb4hpc tries to cover all the most commonly used gdb commands, but there are hundreds of lesser used functions that gdb supports.

You can access the raw gdb via the gdbmode command.

In gdbmode you are interacting directly with gdb.   There are just some provisos:

- The breakpoint numbers will be different : gdb4hpc outside of gdbmode maintains the mapping from its breakpoint numbers to gdb's breakpoint numbers.
- Avoid adding or removing breakpoints, and realize that changing the program location via something like `continue` has some potential to confuse gdb4hpc.

The `end` command exits gdb mode.

## Pointers

You will notice that gdb4hpc seldom prints out the hex values of pointers.  In a multi-process application these aren't generally useful and they impede aggregation.    You can see them by switching to gdb mode, and some commands support a `-v` (verbose) option.

Instead, gdb4hpc prints the values based on reference ids:

```
dbg all> p ll
b{0..2}: [1] 1:{val = 0, next = [2]} 2:{val = 0, next = [3]} 3:{val = 2, next = 
[4]} 4:{val = 4, next = [5]} 5:{val = 1, next = [6]} 6:{val = 3, next = [7]} 7:{
val = 0, next = [8]} 8:{val = 2, next = [9]} 9:{val = 4, next = [10]} 10:{val = 
1, next = [11]} 11:{val = 3, next = [1]}
```

Which could use a little explanation.

So `ll` is a pointer and gdb4hpc used `[1]` in place of its pointer value.   Then it continues by printing the object it references.   So `1:` is the contents of the object at `[1]`, which in this case is a list node that contains yet another pointer.   And so on, until it links back to first node; i.e. it's a classic cicularly linked list.

The reference indices are not global, so [1] in the next print command is not the same pointer as [1] in this one.  The number of refences the print command will process is controlled by `set print elements`.

## Debugging a job that is already running

Gdb4hpc has the `attach` command to connect to a job that is already running.   The command itself is straightforward :  `attach $a <job-id>`

The hard part is just that the means to find your '<job-id>' depends on the platform.   See `help attach` for more information.

## Debugging gpu applications

Gdb4hpc does handle gpu based debugging; you just have to add the `--gpu` option when launching or attaching so the device specific version of gdb can be started.

For cuda applications, the `cuda` commands gives access to the cuda-gdb specific commands.

# Beyond *Getting Started*

This is not the end of the what's available, but this should have introduced enough to be able to debug real problems.

To learn more:
- Check out gdb tutorials on line; some very good printed texts are also available
- Dig down into gdb4hpc's help system to find what's available
- Checkout out the **Gdb4hpc Tutorial** for more *4hpc* topics:
  - The compare function
  - Assertion scripts
  - Array decompositions

