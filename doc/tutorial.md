---
title: Gdb4hpc Tutorial
---

This expands on the *Getting Started with Gdb4hpc* to cover some gdb4hpc specific topics:

- Comparative debugging
- Assertion scripts
- Array decompositions
- Hidden features, tips, tricks and work arounds

# Comparative debugging

*I fixed a bug or added a great speed up but now I get a different result!*

This is a super common debugging situation, and these bugs have a tendency to be subtle: a missing negative sign, changes in round-off, etc.    The good news is that you can really leverage having a working version to reference; you don't have to track the entire process, you can just catch the point where the results change.

This is why procsets have names.   Here we launch two applications:
```
dbg all> launch $a{3} --launcher-args="--exclusive" --args="0 40" bd_dt/C++/cpp_arrays.x
Starting application, please wait...
Creating MRNet communication network...
Waiting for debug servers to attach to MRNet communications network...
Timeout in 400 seconds. Please wait for the attach to complete.
Number of dbgsrvs connected: [1];  Timeout Counter: [0]
Number of dbgsrvs connected: [1];  Timeout Counter: [1]
Number of dbgsrvs connected: [3];  Timeout Counter: [0]
Finalizing setup...
Launch complete.
a{0..2}: Initial breakpoint, main at /home/users/jvogt/rpm_install/gdb4hpc/4.14.0.0/tests/bd_dt/C++/cpp_arrays.cpp:8
dbg all> launch $b{3}  --launcher-args="--exclusive" --args="1 40" bd_dt/C++/cpp_arrays.x
Starting application, please wait...
Creating MRNet communication network...
Waiting for debug servers to attach to MRNet communications network...
Timeout in 400 seconds. Please wait for the attach to complete.
Number of dbgsrvs connected: [1];  Timeout Counter: [0]
Number of dbgsrvs connected: [1];  Timeout Counter: [1]
Number of dbgsrvs connected: [3];  Timeout Counter: [0]
Finalizing setup...
Launch complete.
b{0..2}: Initial breakpoint, main at /home/users/jvogt/rpm_install/gdb4hpc/4.14.0.0/tests/bd_dt/C++/cpp_arrays.cpp:8
dbg all>
```

In this case, it's the same executable running with different arguments.

But now we can control the two running applications in tandem.
```
dbg all> b 92
a{0..2}: Breakpoint 1: file /home/users/jvogt/rpm_install/gdb4hpc/4.14.0.0/tests/bd_dt/C++/cpp_arrays.cpp, line 92.
b{0..2}: Breakpoint 1: file /home/users/jvogt/rpm_install/gdb4hpc/4.14.0.0/tests/bd_dt/C++/cpp_arrays.cpp, line 92.
dbg all> c
a{0..2}: Breakpoint 1, main(int, char**) at /home/users/jvogt/rpm_install/gdb4hpc/4.14.0.0/tests/bd_dt/C++/cpp_arrays.cpp:92
b{0..2}: Breakpoint 1, main(int, char**) at /home/users/jvogt/rpm_install/gdb4hpc/4.14.0.0/tests/bd_dt/C++/cpp_arrays.cpp:92
dbg all>
```

And we can print the arrays, or in this case `std::vector`s that we're interested in:
```
dbg all> p distributedVec1
a{0..2}: {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39}
b{0..2}: {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,-100,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39}
dbg all>
```

You can see that gdb4hpc prints the procset name with each output line.   This is an artificial small example so you could spot the difference yourself, but gdb4hpc provides the compare function to find it for you:
```
dbg all> compare $a::distributedVec1 == $b::distributedVec1
{0..2}: Comparison is false.
The difference delta is:
{0..2}: [20]:-120
dbg all>
```

For floating values, you can use `set error` to have it look for only differences greater than a threshold. 

One proviso, in this case the runs are using the same number of ranks, so gdb4hpc's default strategy of comparing the results rank by rank works.  For more complicated cases, you may need to define a *decomposition* to tell it how to interpret the data.  

This hasn't quite found the bug for us, but it has narrowed it down to occurring between this point and the last time this comparison passed.  We can iterate running side by side in ever narrower scopes until we find what is causing different results.

*Spoiler alert*:
```
if (argv[1]) {
    distributedVec1[size/2] = -100;
}
```

# Assertion scripts

The place where you want to check for divergence may be called hundreds, thousands or millions of times, so gdb4hpc lets us perform the check automatically, via an assertion script.

We start by building a script.

```
dbg all> build $check_distributed
> set halt on
> assert $a::distributedVec1@"cpp_arrays.cpp":92 == $b::distributedVec1@"cpp_arrays.cpp":92
> end
Assertion script $check_distributed compiled.
dbg all> 
```

Pro tip: assertion scripts can be rough to try to type in at the command line, you can instead save them in a text file and use the `source` command to pull them in.

Then we start the script and it takes over control of the debugger:
```
dbg all> start $check_distributed
Sending continue to application...
Assertion 1: $a::distributedVec1@"cpp_arrays.cpp":92 == $b::distributedVec1@"cpp_arrays.cpp":92 failed. Stopping script.
Assertion script $check_distributed finished.
Use "info script $check_distributed results" to view results.
Sending halt to all involved ranks.
a{0..2}: Application halted in main at /home/users/jvogt/rpm_install/gdb4hpc/4.14.0.0/tests/bd_dt/C++/cpp_arrays.cpp:92
b{0..2}: Application halted in main at /home/users/jvogt/rpm_install/gdb4hpc/4.14.0.0/tests/bd_dt/C++/cpp_arrays.cpp:92
Assertion script $check_distributed stopped.

dbg all>
```

In this case, `set halt on` tells the script to stop the application process as soon as an assertion fails.   You can get the assertion result with the `info` command:

```
dbg all> info script $check_distributed results
Results summary for script $check_distributed, configured with halt:

Total assertion evaluations: 1
Passed: 0, Passed with warning: 0, Failed: 1, Indeterminate: 0
Assertion results:

Assertion 1, configured with stop:
$a::distributedVec1@"cpp_arrays.cpp":92 == $b::distributedVec1@"cpp_arrays.cpp":92
Passes: 0, Passes with warnings: 0, Failures: 1.

dbg all> 
```

This script defined only a single assertion, but a script can contain multiple assertions which need not all be at the same line or even file.  

There are a couple of things to be aware of with assertion scripts:

- The assertion mechanism relies on every rank that is involved in an assertion stopping at their locations at the same time, which might not happen.  The mechanism detects deadlocks, but these will terminate the script.
- You don't actually want to set an assertion that will be hit millions of times.   Each time the line number is hit the process is stopped, the debugger has to extract a value, and that value has to be sent back to gdb4hpc.   That all takes time, and it's probably measured in seconds not milliseconds.

Some basic advice about comparative debugging:

- Assertion scripts are powerful, but relatively expensive.   Start using comparisons on the outermost loops to narrow to the point of divergence.   Once you've narrowed in on the bug, you can add more checking.
- If your check is always within values in the same process, then a conditional breakpoint can be significantly faster.
- If you do want to put a check on a location that is called a lot, and you have the option, just building into you code can be hundreds of times faster.   Your compiled in check can still call some *hook* function so you can just use a plain old breakpoint to stop when it happens.
- An even more potent form of comparative debugging is to build an application with both versions of the computation and compare the result each time.

# CCDB
CCDB (Cray Comparative Debugger) is a GTK based GUI application built using gdb4hpc to make the comparative debugging easy to use.   If your X or VNC connection is fast enough, it has a dialog window to launch two application, a comparison window, and interactive assertion script creation.

It will not be confused with a full featured graphical debugger, but it is good at showing side by side debugging.

See the `cray-ccdb` module for details.

# Decompositions

HPC programs will often distribute a data structure across multiple ranks by decomposing it into pieces.    A simple-minded version might, for example, decompose a 10,000 x 10,000 elements array across 100 ranks by storing the first 100 rows on the first rank, the next hundred rows on the second rank, and so on.    

Gdb4hpc provides `decompositions` to let you work with the data in unified form.    So, for the previous example, this would be the definition:

```
dbg all> decomposition $block10k
> dimension 10000,10000
> distribute block,*
> proc_grid 100,*
> end
```

Where the lines define in order:

1. The name of the decomposition, once defined it lasts for the lifetime of gdb4hpc
2. The size of the full array
3. The distribution strategy (this one is aimed for a C/C++ application, so it's using row/column and row-major ordering)
- The rows are distributed using the *block* strategy, which divided it into equal-sized blocks of data
- `*` means the columns are not distributed.    Each rank will have the full row.
4. The number of ranks to use for the distribution, in this case the rows are divided across 100 ranks.

The decomposition can then be used as an adapter, so if each rank refers to its portion via the variable `matrixSlice`.   You could get the full result via:
```
dbg all> p $block10k{matrixSlice}
```

Though I wouldn't recommend it, unless you really want to see 100 million values scroll past.   For larger arrays, the more practical application is:
```
dbg all> compare $block10k{$a::matrixSlice} == $block10k{$b::matrixSlice}
```

Decompositions also support the *cyclic* and *block-cyclic* distributions which parcel out the values in round-robin, or round-robin by chunk respectively.   The command `usage distribute` shows all the details.

When a multi-dimension arrays are distributed in each direction, a `dim_order` entry is required to define the mapping from the data chunks to the ranks:
```
decomposition $block2_21
dimension 12,10
distribute block,block
proc_grid 3,2
dim_order 2,1
end
```
Which is mapping a 12 x 10 array data onto a 3 x 2 array of ranks.   So this will imply that the first value on the second rank will be referring to array[0][5], and the last value will be array[3][9].    

`help decomposition` does give a good man-page level description of details for the various permutations that are available.   The mechanism covers common cases, but can't, for example, transpose data in a matrix or deal with non-uniform decompositions.

For this document, we're trying to show the idea of a decomposition, and we'll bring up some less obvious features:

- It can be used to compare results when a program is run with different numbers of ranks, or different decomposition strategies.   That is, the values in the compare function can use different decompositions.
- A decomposition can supply the missing length of C/C++ primitive arrays- though just using something like `val[0]@100` (or `val[0..99]`) is quicker if you just want the values.
- The decomposition can be used to give the 2 or 3, (or more)-d formatting for something in a flat array or STL container.  
- It can be used in the opposite direction to print an n-d array as a 1-d array.
- The decomposition can be defined fully distributed.  That is, the `distribute` line is all `*`'s.  There is a potentially different result for each rank; the decomposition is just reshaping the array.
- A decomposition can let you interpret a scalar as array of length num_ranks.    (This used to actually be required to compare scalars in assertion scripts).

# Lesser gdb4hpc features

These are some gdb4hpc features that you aren't obvious, but that can simplify your life.

## History
At the gdb4hpc prompt, the up and down arrows cycle through the command history and the '!' escapes are available.

## Quote escape
Gdb4hpc understands most, but not all possible expressions you might want to print.   Enclosing an expression in double quotes sends it directly to the gdb interpreter:
```
dbg all> p distributedArray1[0]@4
a{0..2}: {0,4,8,12}
dbg all> p distributedArray1[0]@size
syntax error, unexpected SYM, expecting STRING or INT
dbg all> p "distributedArray1[0]@size"
a{0..2}: {0,4,8,12,16,20,24,28,32,36,40,44,48,52,56,60,64,68,72,76,80,84,88,92,96,100,104,108,112,116,120,124,128,132,136,140,144,148,152,156}
dbg all> p $a{1}::"distributedArray1[0]@size"
a{1}: {0,4,8,12,16,20,24,28,32,36,40,44,48,52,56,60,64,68,72,76,80,84,88,92,96,100,104,108,112,116,120,124,128,132,136,140,144,148,152,156}
dbg all> p "$a{1}::distributedArray1[0]@size"
a{0..2}: No symbol "$a{1}::distributedArray1(0)@size" in current context
dbg all>
```
*Note in the last line that the gdb doesn't know anything about procsets.   The quoted should start after the procset.*

Gdb4hpc won't fully understand these results, so the any comparisons would be done strictly by comparing the results as a string.

## Array slicing
You can use `..` to print out a portion of an array

```
dbg all> p distributedVec1
a{0..2}: {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39}
dbg all> p distributedVec1[3..10]
a{0..2}: {3,4,5,6,7,8,9,10}
dbg all>
```

## The source command and running in batch
The `source` commands reads any text files and treats it as if it was entered at the prompt.   This can be particularly handy for executing launch commands, setting up breakpoint lists, or defining decomposition and assertion scripts.    This is similar to the *batch mode* command line option, but it leaves you in the normal interactive mode when it finishes.

Both batch and the source command are not synchronized with the debugger.   Thus, if a script continues to a breakpoint and prints a value, the print statement will execute before the breakpoint is hit.   The command `maint set sync on` can be used to change that behavior, but should be used with some care.

## ~/.gdb4hpc/gdb4hpc_init
This file `source`'d whenever gdb4hpc starts.  Typically, this would be used for setting preferences. 

## Using gdbmode to access some functionality that gdb4hpc doesn't *currently* support directly
You can use gdbmode to:

- define conditional breakpoints or set an ignore count on a breakpoint.   Note that gdb and gdb4hpc will use different values for the breakpoint index.
- Set a hardware watchpoints
- Modify the value of a variable with `=`

Some operations unfortunately don't work like one might hope:

- The `shell` command in gdb mode doesn't print any results
- Adding a `command` to a breakpoint also won't print any result.

## Thread handling
Except when using cuda-gdb which doesn't support it, gdb4hpc defaults to running in non-stop mode.   That is, when a thread hits a breakpoint, the other threads continue to run.   This scales better and mimics how processes act in gdb4hpc.   The main change as a user can be that you need to use `c -a` and `halt -a` when you want these to apply to *all* threads.

Gdb4hpc, by default, aggressively aggregates threads.    Especially in gpu circumstances this will be eliminating the lane information in `info thread` to give a more compact result.  The `-v` option is available for the full verbose result.

Gdb4hpc supports a extended form thread sets for most contexts that take a thread id that can include sequences, ranges, or regular expressions to select a set of theads.   See `help thread` for more details.

There is also a lesser used extension for procset that incorporates threads.    So `$a{1..3:1}`  references thread 1 in three processes.   You can use this to focus on a set of threads to apply a series of operations.   Though `thread apply` is typically easier to use.

## Procset aliases
The defset command lets you define an alias for a set of processes.  This can help when the application divides set of ranks into separate roles: e.g. "earth" and "sea" nodes for a weather simulation.

## --debug
Launch and attach drop gdb4hpc logging files into the directory defined by the environment variable CRAY_DBG_LOG_DIR.   If there are launch problems these files can give some insight.   It will it at least show the work load manager commands that it is trying to call.   

