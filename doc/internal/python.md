# Python Support in gdb4hpc

Josh Chandler - 4/12/2024

## gdb Python Extensions in gdb4hpc

In the cpython source tree, there is a [python extension for
gdb](https://github.com/python/cpython/blob/25f6ff5d3e92305659db62e7f7545f823f0dbd05/Tools/gdb/libpython.py).
gdb can `source` this file to gain new commands and pretty printers. The new
commands begin with `py-` and are specific to when gdb is debugging the python
interpreter. The `py-` commands use the extension to transform the internal
state of the python interpreter seen by gdb into information that is relevant
to the python application source.

For example, gdb's `list` command is useless to a python programmer because it
lists the source code of the python interpreter. But the `py-list` command
lists the application's current location in the python source code.

```
dbg all> frame
a{0..2}: #2  time_sleep at ./Modules/timemodule.c:425

dbg all> list
a{0..2}: 425        if (pysleep(timeout) != 0) {
a{0..2}: 426            return NULL;
a{0..2}: 427        }
a{0..2}: 428        Py_RETURN_NONE;
a{0..2}: 429    }
a{0..2}: 430
a{0..2}: 431    PyDoc_STRVAR(sleep_doc,
a{0..2}: 432    "sleep(seconds)\n\
a{0..2}: 433    \n\
a{0..2}: 434    Delay execution for a given number of seconds.  The argument may be\n\

dbg all> py-list
a{0..2}:
  27
  28    def sleep(depth):
  29        if depth >= 10:
  30            return
  31        f = depth**2
 >32        time.sleep(0.5)
  33        sleep(depth + 1)
  34
  35    def main():
  36        nums = get_nums()
  37        rand_nums = scatter(nums)
```

## Requirements for using Python Extensions

The `py-` commands only work correctly when:
1. The application being debugged by gdb is the cpython interpreter (the python
   software foundation's own implementation of python)
2. The python extension has been loaded (`source`d) by gdb
3. The python interpreter has debug info available

## How gdb4hpc Loads the Extension

gdb4hpc has a copy of the python extension at `/src/extensions/python-gdb.py`.
This file is distributed with gdb4hpc at `<gdb4hpc install
dir>/share/py-gdb/python-gdb.py`.

gdb4hpc does not attempt to detect a python interpreter before loading the
extension. When *any* application is launched, gdb4hpc will ship
`python-gdb.py` to the backend via CTI and `source` it at dbgsrv initialization
time. It does this even for non-python applications because the cost of
shipping and loading the extension is not noticeable to a user. Another reason
for this approach is the benefit that it will always work when needed, even if
the user's python interpreter is not named "python". A python interpreter with
a non "python" name is more common than one might think. Python is designed as
a library, so a python HPC app might be built as an ELF binary that does some
setup and then loads and runs python (e.g.
[pynamic](https://github.com/LLNL/pynamic/blob/master/pynamic-pyMPI-2.6a1/mpi4py_main.c)).

## Python Interpreter Debug Info

Getting a python interpreter with debug info is currently an unsolved problem
and is left up to the user.

The python interpreter installed as part of a Linux distribution never has
debug info. Usually the debug info is provided separately by installing a
package with a name like `python3-dbg`. Installing this package might install a
python interpreter with debug info called `python3-dbg` or install standalone
debug info in a location that gdb knows about. Unfortunately, most
datacenter/user systems don't have this package and getting it requires admin
privileges.

If `python-dbg` *is* installed, it is installed in a directory that gdb knows
about. gdb automatically scans certain directories for debug info matching the
current binary build id. Useful links:
- https://developers.redhat.com/articles/2022/01/10/gdb-developers-gnu-debugger-tutorial-part-2-all-about-debuginfo#
  ([archive](https://web.archive.org/web/20240412150226/https://developers.redhat.com/articles/2022/01/10/gdb-developers-gnu-debugger-tutorial-part-2-all-about-debuginfo#reading_debuginfo__a_simple_use_case))
- https://sourceware.org/gdb/current/onlinedocs/gdb.html/Separate-Debug-Files.html
  ([archive](https://web.archive.org/web/20240412154816/https://sourceware.org/gdb/current/onlinedocs/gdb.html/Separate-Debug-Files.html))

Cray provides a python interpreter with some HPC libraries built in. You can
access it via `module load cray-python`. This is popular with users because
it's usually a newer python version than the system install. Whether it has
debug info seems random. For example, pinoak and frontier's cray-python
interpreters have debug info, but pea2k's does not. There might be a pattern
here: pinoak and frontier are sles, and pea2k is rhel. But that's unconfirmed.

Users, especially AI users, also like conda environments, which make getting
debug info very difficult. Conda environments can install any version of
python, but installing a python interpreter with debug info is [not a
feature](https://stackoverflow.com/questions/69313707/can-we-get-a-python-environment-built-in-debug-mode-using-conda).
There are some efforts towards it here and there, but it never became more than
an undocumented feature at best.

It's worth noting that some experimentation shows the python extensions can
handle *some* situations when the python interpreter does not have debug info,
but nothing is guaranteed.

All of this means that a solution that does not rely on python interpreter
debug info is ideal. We provide the python extensions on the side as a
secondary option to the main python solutions. (the "main python solutions" are
not implemented yet)

## Quirks of the Python Extension Commands

In the Python interpreter, a single C frame can contain multiple Python frames.
This is common in recursive functions (but is not limited to them). Most of the
weird behavior of the `py-` commands stem from how they handle this.

### `py-print`

`py-print` cannot evaluate arbitrary expressions like the standard `print`
command. It can only look up the values of named variables.

```
gdbmode> py-print depth + 1
'depth + 1' not found
```

If there are multiple Python frames in the current C frame, `py-print` will
reference the names in the first (deepest) frame.

`py-print` does not plug in to the gdb4hpc's data parsing/processing systems
(AIS etc.) because the way `py-print` emits data is not supported by our data
parser (yet). For example, `py-print` automatically truncates arrays with a
syntax not supported by our parser (yet).

```
dbg all> py-print nums
a{0}: local 'nums' = [[0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 1...(truncated)
```

### `py-locals`

If there are multiple Python frames in the current C frame, `py-locals` prints
the locals from every frame.

```
a{2}: Locals for sleep
depth = 1
f = 1
Locals for sleep
depth = 0
f = 0
Locals for main
nums = None
rand_nums = [20000, 20001, 20002, 20003, 20004, 20005, 20006, 20007, 20008, 20009, 20010, 20011, 20012, 20013, 20014, 20015, 20016, 20017, 20018, 20019, 20020, 20021, 20022, 20023, 20024, 20025, 20026, 20027, 20028, 20029, 20030, 20031, 20032, 20033, 20034, 20035, 20036, 20037, 20038, 20039, 20040, 20041, 20042, 20043, 20044, 20045, 20046, 20047, 20048, 20049, 20050, 20051, 20052, 20053, 20054, 20055, 20056, 20057, 20058, 20059, 20060, 20061, 20062, 20063, 20064, 20065, 20066, 20067, 20068, 20069, 20070, 20071, 20072, 20073, 20074, 20075, 20076, 20077, 20078, 20079, 20080, 20081, 20082, 20083, 20084, 20085, 20086, 20087, 20088, 20089, 20090, 20091, 20092, 20093, 20094, 20095, 20096, 20097, 20098, 20099, 20100, 20101, 20102, 20103, 20104, 20105, 20106, 20107, 20108, 20109, 20110, 20111, 20112, 20113, 20114, 20115, 20116, 20117, 20118, 20119, 20120, 20121, 20122, 20123, 20124, 20125, 20126, 20127, 20128, 20129, 20130, 20131, 20132, 20133, 20134, 20135, 20136, 20137, 20138, 20139, 20140, 20141, 20142, 20143, 20144, 20145, 2...(truncated)
Locals for <module>
```
### `py-up` / `py-down`

The `up`/`down` commands are not smart enough to see the multiple Python frames
within a single C frame and might give error messages in situations where they
seem like they should work.

```
dbg all> py-bt
a{0..2}: #6  <module> at /cray/css/users/chandlej/home/chandlej/Code/cdst-gdb4hpc/tests/function/main3.py:47
a{0..2}: #5  main at /cray/css/users/chandlej/home/chandlej/Code/cdst-gdb4hpc/tests/function/main3.py:38
a{0..2}: #4  sleep at /cray/css/users/chandlej/home/chandlej/Code/cdst-gdb4hpc/tests/function/main3.py:33
a{0..2}: #3  sleep at /cray/css/users/chandlej/home/chandlej/Code/cdst-gdb4hpc/tests/function/main3.py:33
a{0..2}: #2  sleep at /cray/css/users/chandlej/home/chandlej/Code/cdst-gdb4hpc/tests/function/main3.py:33
a{0..2}: #1  sleep at /cray/css/users/chandlej/home/chandlej/Code/cdst-gdb4hpc/tests/function/main3.py:33
a{0..2}: #0  sleep at /cray/css/users/chandlej/home/chandlej/Code/cdst-gdb4hpc/tests/function/main3.py:32

dbg all> py-up
a{0..2}: Debugger error: Unable to locate an older python frame
dbg all> gdb py-up
a{0..2}: Unable to find an older python frame
dbg all> py-down
a{0..2}: Debugger error: Unable to locate a newer python frame
dbg all> gdb py-down
a{0..2}: Unable to find a newer python frame
```
