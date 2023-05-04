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

## Test Sizes

Each individual test should be designed so that it can be run in a 4 node
allocation. A 4 node limit was chosen because it is a requirement that is easy
to meet by any small machine, and it allows big machines to assume each test
will use 4 nodes for easy scaling math to run tests in parallel.

Specify the node count in the test's launcher args such that the total amount
of nodes in use never exceeds 4. Even single job launches should specify a node
count if they launch more than 4 ranks so that they can be assumed to only use
4 nodes. This makes for easy parallel test math (`num_parallel_tests =
total_allocation_nodes / 4`).

For example, the following launch would likely fail on a 4 node allocation
because the default action in a launch is to use as many nodes as possible.
Combined, these two launches require 6 nodes.

```
launch $a{3} ./bin/CRAY/c/c_inc.x
launch $b{3} ./bin/CRAY/c/c_inc.x
```

Replace it with the following to keep it under the 4 node limit.

```
launch $a{3} ./bin/CRAY/c/c_inc.x --launcher-args="-N2"
launch $b{3} ./bin/CRAY/c/c_inc.x --launcher-args="-N2"
```

## Launcher arguments in .in files

Some .in files have launcher arguments in their launch line:

> `launch $a{3} ./bin/CRAY/c/c_inc.x --launcher-args="-N2"`

This can be problematic. We only maintain one .in file for each test, but we
test across multiple WLMs. `-N4` in slurm means something different than `-N4`
on ALPS, and another wlm might not even support a `-N` option.

To deal with this, the test harness makes the assumption that .in files are
written with slurm launcher arguments. This is the most common kind of internal
system and thus the easiest to support the "run gdb4hpc with -e -> paste into
.in file" test creation workflow.

The translation implementation if not complete and only supports the cases
needed by current tests. Check libtest.py and the unit tests in libtest.py to
see how much is implemented.
