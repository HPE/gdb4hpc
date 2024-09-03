---
title: "(Deprecated) Debugging Python Applications with the gdb Python Debugging Extension in gdb4hpc"
---

# (Deprecated) Debugging Python Applications with the gdb Python Debugging Extension in gdb4hpc

> NOTE: These commands are deprecated in favor of python mode. Python mode has
> more functionality and less requirements. See the
> [Debugging Python Applications with Python Mode](./python-mode.md)
> document for more info.

## Background

gdb is a debugger for compiled binaries, not interpreted languages like Python.
This means that when debugging a Python application with gdb, gdb is debugging
the Python interpreter internals and not the Python application code. For
people developing Python applications, this isn't very useful.

In the [source of the cpython
interpreter](https://github.com/python/cpython/blob/main/Tools/gdb/libpython.py),
there is a gdb extension that can be loaded by gdb to help translate data and
state internal to the python interpreter into data and state relevant to the
python source code.

By loading the extension in a gdb session, gdb gains gains Python equivalents
to the gdb `backtrace`, `list`, `up`, `down`, `info locals`, and `print`
commands and pretty printers for internal Python data type.

gdb4hpc supports using this extension by loading it into the gdb instances
attached to each rank and providing aggregated versions of the commands
provided by the extension.

See
[https://docs.python.org/3/howto/gdb_helpers.html](https://docs.python.org/3/howto/gdb_helpers.html)
for more info about the gdb extension for debugging the Python interpreter.

## Requirements

gdb4hpc will automatically load the Python extension for you, but the following
conditions need to be met for the extension to work.

### The Python interpreter being debugged is the [cpython](https://github.com/python/cpython) interpreter

This is true in most situations. cpython is used in your Linux distribution's
Python installation, a virtual environment, a Conda environment, and
cray-python. This list is not exhaustive, and cpython is used in even more
places. If you don't have evidence suggesting otherwise, you are probably using
cpython.

### The Python interpreter being debugged has debug information available

Unfortunately, this is not true by default in most situations.

#### How to Check for Debug Info

To check if your Python interpreter has debug info, run `gdb $(which python3)`
in a terminal and check the output for messages like "no debugging symbols
found".

You can also run `file $(which python3)` and look for "with debug_info".

```
$ file -L $(which python3)
/usr/bin/python3: ELF 64-bit LSB executable, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, BuildID[sha1]=e84005b12f8e1e707681c3c55d5631f67299312f, for GNU/Linux 3.2.0, with debug_info, not stripped
```

> If the output of the `file` command contains `symbolic link to python`,
> use the `-L` flag to tell `file` to follow links before checking for debug
> info.

Note that using `file` is not 100% reliable because there are multiple ways to
provide debug info for the Python interpreter, and this method only checks one
of them. Other methods exist which load the debug info dynamically from a
standalone debug info file, and this method doesn't check that.

If no debug info is found with the above methods, proceed to *How to Get Debug
Info* below.

#### How to Get Debug Info

If you don't have debug info available for your Python interpreter, there are a
few approaches you can take to get it.

##### Install the Python Debug Info Package

If you are using your distribution's Python interpreter (e.g.
`/usr/bin/python3`), most distributions have a package called `python-dbg` or
`python-debuginfo` that you can install. You will need admin privileges to
install it, or will need to request that a system administrator install it.
Once the package is installed, gdb4hpc will automatically detect the debug info
and use it.

Note that installing the package only supplies debug info for the specific
Python interpreter at `/usr/bin/python3` which comes with your Linux
distribution. For info about getting debug info for other Python interpreters
(cray-python, Python compiled from source, custom interpreters in Conda
environments, etc.), see the other sections below.

##### Use cray-python

The Cray Programming Environment comes with a cpython interpreter that is
pre-built with some HPC libraries (mpi4py, numpy, etc). Some installations of
cray-python include debug info.

To load cray-python and check if it has debug info available, run the
following:

```
$ module load cray-python
$ which python3
/opt/cray/pe/python/3.11.7/bin/python3
$ gdb $(which python3)
...
Reading symbols from /opt/cray/pe/python/3.11.7/bin/python3...
```

If you don't see any messages indicating debug symbols are missing (e.g. "no
debugging symbols found"), your cray-python installation has debug info.

##### Compile Python from Source with Debug Info

If none of the above options are available, you can build Python from source.
Building Python from source is fairly straightforward and outlined here:
[https://devguide.python.org/getting-started/setup-building/#unix](https://devguide.python.org/getting-started/setup-building/#unix)

The important part is adding `--with-pydebug` to the `./configure` call. This
has performance implications for the Python interpreter as it adds extra checks
and assertions.

When you build Python, you also build its package manager, `pip3`. You can use
the `pip3` installed in the same location as your built Python to install
libraries to the newly built Python instance.

## Connecting to a Python Application in gdb4hpc

### Via `launch`

Find the path of your current Python interpreter:

```
$ which python3
/opt/cray/pe/python/3.11.7/bin/python3
```

Use it in the `launch` command in gdb4hpc to launch your Python application.

Pass the name of your Python script to the launched Python interpreter with the
`-a` option. This adds command line arguments to the launched job. The below
launch command is the equivalent of `srun
/opt/cray/pe/python/3.11.7/bin/python3 my-python-app.py`.

Use `--non-mpi` to tell gdb4hpc that the binary (the Python *interpreter*) does
not contain a call to `MPI_Init`. You can still debug applications that use MPI
in Python code, e.g. mpi4py, the `--non-mpi` is specifically about the Python
*interpreter* that will be running the mpi4py code.

```
dbg all> launch $pyapp{3} /opt/cray/pe/python/3.11.7/bin/python3 -a "my-python-app.py" --non-mpi
```

> Using `--non-mpi` and debugging Python interpreters are not supported on
> systems with the ALPS or PALS work load managers. `attach` is still available
> on these systems.

### Via `attach`

Launch your Python app as you would normally (e.g. with `srun` or `sbatch` etc.):

```
$ srun -n4 python3 ./main4.py
```

Next, obtain your application's job ID and attach to your job with gdb4hpc.

#### On a Slurm System

Use `squeue -s` to find the job ID.

```
$ squeue -s
     STEPID PARTITION     NAME     USER ST       TIME  NODES NODELIST(REASON)
  9431671.0 allnodes   python3      hpe  R       0:01      1 n023
```

The job ID is in the `STEPID` column. Start gdb4hpc and attach to your application.

```
dbg all> attach $pyapp 9431671.0
```

#### On a Flux System

Use `flux jobs` to find the job ID.

```
$ flux jobs
       JOBID USER     NAME       ST NTASKS NNODES     TIME INFO
 ƒ2BzGNARhpB user     python3    R       2      1   3.021s node1
```

Start gdb4hpc and attach to your application.

```
dbg all> attach $pyapp ƒ2BzGNARhpB
```

> Depending on the environment, Flux job IDs might display with `ƒ` (unicode
> U+0192), not `f` (ascii). This can cause some problems in old terminal
> emulators when copying and pasting. Set the environment variable
> `FLUX_F58_FORCE_ASCII` to 1 to force `flux jobs` to use an ascii f.

#### Obtaining the job ID on Other Systems

For other workload managers, check the help text of the `attach` command in gdb4hpc via the `help attach` command.

## Commands

The Python extension adds pretty printers for Python types and the following
commands.

### `maint pyext bt`

`maint pyext bt` is the Python equivalent of gdb4hpc's `bt` / `backtrace` commands. It
shows all ranks' backtraces merged into a single tree:

```
dbg all> maint pyext bt
a{0..2}: #2  <module> at main4.py:48
a{0..2}: #1  main at main4.py:39
|
|_ a{0,2}: #0  scatter at main4.py:18
|
\_ a{1}: #1  scatter at main4.py:17
   a{1}: #0  sleep
```

This backtrace shows that all ranks started at `<module>` (basically Python's
`_start` equivalent), then diverged in the `scatter` function. Rank 1 is
currently in `sleep`.

### `maint pyext list`

`maint pyext list` is the Python equivalent of gdb4hpc's `list` command.

```
dbg all> maint pyext list
a{0,2}:
  15    def scatter(nums):
  16        if world_rank == 1:
  17            time.sleep(5)
 >18        return comm.scatter(nums)
  19
  20    def get_avg(a):
  21        avg = 0.0
  22        for n in a:
a{1}:
  15    def scatter(nums):
  16        if world_rank == 1:
 >17            time.sleep(5)
  18        return comm.scatter(nums)
  19
  20    def get_avg(a):
  21        avg = 0.0
  22        for n in a:
```

`maint pyext list` takes range arguments similar to gdb4hpc's `list` command that allow
you to list a specific range.

```
dbg all> maint pyext list 10,12
  10        if world_rank == 0:
  11            return [[j * nums_per_rank + i for i in range(nums_per_rank)] for j in range(world_size)]
  12        else:
```

### `maint pyext print` and `maint pyext locals`

`maint pyext print` and `maint pyext locals` are the Python equivalent of gdb4hpc's `print` and
`info locals` commands.

`maint pyext print` can't evaluate arbitrary expressions like gdb4hpc's `print` command
can. It can look up the value of local, global, and builtin names.

`maint pyext locals` shows the local variables in the current Python frame and their
values.

### `maint pyext up` and `maint pyext down`

`maint pyext up` and `maint pyext down` are the Python equivalent of gdb4hpc's `up` and `down`
commands. They navigate the Python call stack and affect the results of
`maint pyext print` and `maint pyext locals`. For example, if you wanted to print a variable in
the top level function of your Python application, you could repeatedly use
`maint pyext up` to climb up the stack to the top level frame, then use `maint pyext locals`.

## Hybrid Applicaitons

The `maint pyext` commands provided by the extension only display information about
Python code. If you are debugging a hybrid application, use the non `py-`
versions of the commands to debug the native code as you usually would.

For example, `maint pyext bt` can be used to show the Python backtrace, but it won't
include any frames from native code. If native code is being run, the usual
`backtrace` command can be used to see it.

You will also see the frames of the Python interpreter itself, which can be
quite large. You can pass an argument to `backtrace` to limit the number of
frames that are displayed. Native extension frames will be at the bottom of the
backtrace stack, so limiting the amount of frames will work nicely to isolate
them.

```
dbg all> backtrace 5
app{0}: #4  step_one at src/C++/cpp_bt.cpp:62
app{0}: #3  step_two at src/C++/cpp_bt.cpp:67
app{0}: #2  rare_branch at src/C++/cpp_bt.cpp:54
app{0}: #1  step_three at src/C++/cpp_bt.cpp:73
app{0}: #0  merge at src/C++/cpp_bt.cpp:58

app{1..4}: #4  loop at src/C++/cpp_bt.cpp:86
app{1..4}: #3  step_one at src/C++/cpp_bt.cpp:62
app{1..4}: #2  step_two at src/C++/cpp_bt.cpp:69
app{1..4}: #1  step_three at src/C++/cpp_bt.cpp:73
app{1..4}: #0  merge at src/C++/cpp_bt.cpp:58
```

## Loading Your Own Extensions

You can use gdb4hpc's `gdbmode` feature to directly control gdb4hpc's many
instances of gdb. If you want to load your own extensions, you can use the
`source` command in gdb mode.

gdb4hpc automatically does this for you and loads its included Python debugging
extension. But if you had your own extension or a newer version that you wanted
to use, you could load it like this:

```
dbg all> gdbmode
Entering gdb pass-thru mode. Type "end" to exit mode...

gdbmode> source path-to-my-python-extension.py
gdbmode> end
```

Or equivalently:

```
dbg all> gdb source path-to-my-python-extension.py
```
