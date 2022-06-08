# Automated Tests

## Running tests from a tests rpm

```
$ ./test_tool.py --cp <destination> # if tests are installed in a directory that root owns
$ ./test_tool.py
```

## Running tests from a git clone

```
$ git submodule update --init --remote ./cdst-test
$ ./test_tool.py
```

## A note on the updated gdb4hpc-specific textcheck functions

This version of the test process is designed to automatically handle
most of the variability in the output from gdb4hpc.
This includes that:
- results from multiple launches can return in any order,
- results from some ranks may be delayed,
- addresses and LWP vary from run to run and rank to rank,
- the directory for breakpoints depends on you run location,
- "connecting to dbgsrv" line may vary from run to run,
- the initial breakpoint in fortran programs is indeterminate,
- output printed within to application itself isn't consistent enough
to really check

It now uses a single input file will incorporate both the batch and
the result.   The convention dovetails with --echo, so lines starting
with linenumber: are pulled out as the script lines to run.

After running the test, the .rc and .out files will be available in
"tmp" as always.

To create a new .in file, you can start with a batch file, and run
gdb4hpc as : gdb4hpc -s -o tests.py.data/testname.in -b testname.rc -e

Typically the result will need very little additional massaging

- procset results need to be canicalized.   So replace:
   a{0,2}: 1
   a{1}: 1
   with:
   a{0..2}: 1

- break up lines where the "Breakpoint" message may have happened in
the middle of program output

- In some cases, add a regualar expression inside %'s to match results
that can change, e.g.:
34: show version
a{0..2}: Version %[\d.]+%

- in desperate circumstances, the function output can me marked as
indeterminate by adding three question marks, e.g.:
34:info thread -v???
