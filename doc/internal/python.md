# Python Support in gdb4hpc

Josh Chandler - 7/15/2024

This document describes the internals of gdb4hpc's python debugging support.

There are two ways that gdb4hpc implements python debugging:

1. The **gdb python extensions**. gdb4hpc can make use of 3rd party gdb
   extensions designed for debugging python and the python interpreter. Using
   python extensions requires that a few requirements be met. Most notably, the
   python interpreter needs to be compiled with debug information, which is
   uncommon.

2. The **gdb4hpc python backend**, or **pybe** for short. gdb4hpc injects a
   shared library into the debugged python interpreter. The shared library
   makes use of the python C API to implement debugger commands and sends results
   to gdb4hpc's backend (dbgsrv) over a socket.

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

### Requirements for using Python Extensions

The `py-` commands only work correctly when:
1. The application being debugged by gdb is the cpython interpreter (the python
   software foundation's own implementation of python)
2. The python extension has been loaded (`source`d) by gdb
3. The python interpreter has debug info available

### How gdb4hpc Loads the Extension

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

### Python Interpreter Debug Info

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

### Quirks of the Python Extension Commands

In the Python interpreter, a single C frame can contain multiple Python frames.
This is common in recursive functions (but is not limited to them). Most of the
weird behavior of the `py-` commands stem from how they handle this.

#### `py-print`

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

#### `py-locals`

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

#### `py-up` / `py-down`

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

## gdb4hpc's Python Backend (pybe)

gdb4hpc's python backend (pybe) refers to a shared library that is injected
into the python interpreter and some support code used by gdb4hpc's backend
(dbgsrv).

### The Python C API

**Problem 1**: The python interpreter state is not visible to gdb by default.

When we attach gdb to the python interpreter, we are not debugging the python
script that it is interpreting. We are debugging the C code that makes up the
python interpreter itself. This is useless to a user who is trying to figure
out where their pytorch application is hanging. In theory, the python state
could be reconstructed by looking at the right debug info in the interpreter,
but most users will be debugging a python interpreter built without debug info.

Python exports a [C API](https://docs.python.org/3/c-api/index.html) available
for interacting with the python interpreter state. pybe uses this API to
implement python versions of common gdb4hpc commands. For example, implementing
the backtrace command looks something like this (psuedocode):

```cpp
auto pybe_get_backtrace() {
  auto* intr = PyInterpreterState_Head();
  auto* tstate = PyInterpreterState_ThreadHead(intr);
  auto result = Backtrace{};

  for (auto frame : PyThreadState_GetFrames(tstate)) {
    auto code = PyFrame_GetCode(frame.ptr);

    auto funcname = PyUnicode_AsUTF8(code->co_name);
    auto filename = PyUnicode_AsUTF8(code->co_filename);
    auto linenum = PyFrame_GetLineNumber(frame.ptr);

    result.frames.emplace_back(
      Frame{
        .function_name = funcname.data(), funcname.size(),
        .file_name = filename.data(), filename.size(),
        .line_number = linenum
      }
    );
  }

  // ...
}
```

### Injecting The pybe Shared Library into Python

**Problem 2**: The python API can only be used within the python interpreter
itself. gdb4hpc can't call it directly.

The API is designed to be used in a context where it is loaded by the python
interpreter itself (native modules or embedding python in another app).

One way we could deal with this would be to ship a custom python interpreter
with gdb4hpc functions built in, but most users prefer to use their own python
interpreter. Conda and other python environment managers mean users can install
arbitrary python interpreter versions at arbitrary locations on disk. Our
solution needs to work with any python interpreter that the user happens to be
using.

To address this, we create a shared library, `libgdb4_pybe.so`, that we inject
into the user's python interpreter dynamically. The actual injection is
trivial. All it takes is a gdb command: `call dlopen("libgdb4_pybe.so", ...)`.

Once the library is injected, we can use gdb's `call` command again to call our
custom functions, e.g. `call pybe_get_bracktrace()`. At this point, we have
access to the python API and arbitrary code that we author.

### Getting the Global Interpreter Lock

**Problem 3**: The python API can only be used when the caller holds python's
global interpreter lock.

The python interpreter is infamously not thread safe. Only one thread can be
executing python code at once (more or less). There are multiple reasons for
this, but the one most interesting to us is that the python interpreter handles
dynamic memory management via reference counting, and the reference counting is
not thread safe (making it thread safe created too much of a performance
penalty). If multiple threads use the same reference counted object at once, it
can lead to memory leaks or use-after-frees. The python C API returns reference
counted objects. This means that even the low level python C API is not thread
safe. The only safe way to use the python C API is to do it from a thread that
holds the Global Interpreter Lock (GIL).

It might be tempting to simply stop the python interpreter while it already has
the GIL and run pybe functions from that stopped location via the gdb `call`
command. This would lead to thread bugs. The `call` command is essentially a
fancy jump instruction. No locks are released and no cleanup happens when you
`call` something. From the perspective of GIL management, stopping the python
interpreter in a thread which holds the GIL and then jumping to an arbitrary
pybe function is identical to a thread preemption and context switch from the
python thread to an imaginary pybe thread. It behaves as if there were no lock
at all. This is a fast path to subtle and unreproducible threading bugs. 

To properly get the GIL, we have to spawn a real thread, start competing for
the GIL, and wait until it is obtained. This guarantees that the main python
thread is in a state where it already thinks another thread has the GIL and it
is not in the middle of any thread unsafe internal operation.

To set this up, we spawn a dedicated thread for that we will later use to get
the GIL. This thread spins forever in (essentially) a `while (true) {}` and is
started when the shared library is injected. When we want to use the python C
API, we use gdb to switch to that thread, `call grab_the_GIL()`, then continue
all threads and wait. Eventually our thread will have the GIL and notify gdb.
At that point we can call our pybe functions safely. When we are done calling
functions, we release the lock and drop back down into the infinite loop, ready
for next time.

Getting the GIL never takes very long. The python interpreter actively monitors
the GIL for contention after essentially every python opcode and will actively
give up the GIL if it has been holding it for too long. The user can actually
[configure the amount of
time](https://docs.python.org/3/library/sys.html#sys.setswitchinterval) that
python waits before giving up the GIL.

#### The GIL in Python 3.6

In versions of python <= 3.6, the GIL system is not created by default. Python
expects the interpreter bootstrapper (a.k.a. YOU!) to call a special thread
initialization function. But the thread initialization function comes with a
quite large caveat: it's not safe to call if you don't know what thread holds
the GIL.

...wait, what? To initialize the GIL, you need to know which thread holds the
GIL? Before the GIL exists? Before the GIL has been created, it's not possible
to programatically check which thread holds the GIL...

This is where things get hairy and I'll spare the details. The important thing
is that because of how python <= 3.6 is set up, *the thread initialization
function must be called before python starts interpreting python code*.

This is doable with careful selection of entry breakpoint locations in python
<= 3.6. We get to an entry breakpoint before python starts interpreting and
call the function.

But **it's not possible when attaching to python <= 3.6** since it has already
started interpreting code. Trying to initialize the GIL now results in either
python crashing or stealing the GIL from the thread that already legitimately
has it, which results in a python interpreter segfault a short time later.

#### The GIL Should be Invisible to the User

By "invisible" we mean that the application should not advance in secret when
we are handling the GIL under the hood. For example, running a print command
should not cause the application to suddenly be in a different location because
of threads that ran competing for/releasing the GIL. We actually make an even
stronger guarantee - running a print command should not cause *any user thread*
to change locations. This is tricky to pull off when the very act of acquiring
the GIL may require two threads to spin for a moment.

But consider: if *no* thread holds the GIL, getting the GIL only requires
running one thread, and we already have one hidden thread started for the exact
purpose of getting the GIL. Additionally, our thread is not important to the
user so we can let it slip as much as we want. This lets us do some tricks
under the hood to ensure that any time the user gains control, the application
is already in a state where getting the GIL is trivial (requires only one
thread).

Note that we stop short of actually grabbing *and keeping* the GIL. **The GIL is
never actually *held* between commands**. The idea is to manufacture a state
such that when we return control to the user, the application will not actually
hold the GIL, but will be in a state where it's **trivial** to get the GIL
again as needed. It is acquired on a per-command basis and immediately dropped
when the command is done (but no threads run yet!). This has the benefit of
never ending up in a situation where we accidentally hold on to the GIL too
tightly and block the user's app as well as not needing to have any special
"but do I already have it?" GIL logic to prevent deadlocks. **As long as
acquiring the GIL is trivial (i.e. requires only running our hidden thread), we
can consider it "held" for our purposes here**.

In gdb4hpc code, we usually arrange a trivial GIL by actually grabbing the GIL
and then immediately dropping it. We can arrange for an invisible GIL by being
selective of *when* we actually do this GIL grabbing/dropping. There are not
actually that many situations that we need to worry about. The only times the
user ever gains control of their app are:

- After a launch
- After an attach
- After a halt
- After a breakpoint (stepping/finishing/etc can be thought of as a special
  breakpoints in this case)

We need to acquire the GIL right after launch so the user can immediately start
running commands. The most common use case is to start setting breakpoints
right after the launch completes. To implement this, we pick special entry
point locations for python launches. Instead of stopping at main, we stop just
after python initializes itself and drops the GIL for a moment. Stopping here
means the GIL is trivial to grab and the user can run commands. We don't actually
need to lock the GIL at any point here, just stop at a point where it's trivial
to do so.

Attach and halt have the useful property that the user doesn't actually know
where their app should be when it stops. They know that it should stop
*somewhere*, but the file and line number are not predictable. This gives us
some wiggle room to do things behind the scenes before we give control to the
user. We attach/halt as normal and end up in a random state where it's not
known which thread (if any) has the GIL, but we don't give the user control
yet. Now we do our aquire/drop GIL trick. This causes slippage, but the user
never knows about it.

Breakpoints are easy - the python tracing functions we use to implement
breakpoints already guarantee the GIL.

> Note: breakpoints are not actually implemented yet!

### Getting Results Back to dbgsrv - The Socket

**Problem 4**: After generating the result data with the python C API, we need
a way to get it back to dbgsrv.

Since we are running our functions inside of the python interpreter, not
dbgsrv, we can't simply `return` the generated struct to the caller. The caller
is the GIL grabbing thread and if we returned it there that backtrace we worked
so hard to build would disappear into the ether forever.

Since we used gdb's `call` command to initiate the pybe functions and `call`
does print the return value, it might be tempting to try an approach where we
`call` (or `print`) a pybe function and then parse the return value from the
gdbmi output. This seems straightforward at first, but it has mutliple
pitfalls:

- The printed return value respects the `set print elements` setting and we
  would have to manage that setting on every call.
- gdb pretty printers (and/or the things they are pretty printing) change
  often. Our result parser might break next time we upgrade gdb.
- We would potentially have to write multiple parsers for the output of other
  gdb distributions, e.g. rocgdb and cudagdb.
- A lot of pybe functions allocate memory to store their results. Once the
  result is returned (to gdb), how do we deallocate it?

For the reasons above, we opt to do the extra work of creating a dedicated
result socket when we inject the shared library. We send the results over this
socket and dbgsrv receives them. This bypasses `set print elements`, won't
change when gdb versions change, and memory management is obvious.

It does introduce some new problems we have to solve. e.g. we are now
vulnerable to blocking forever if something goes wrong. But the new problems
are problems that only need to be solved once instead of for every gdb
release/distribution. The socket is a unix domain socket so there is no worry
about network trouble. #### Linux Socket vs. zmq Sockets

We use a plain old Linux socket. But we already use zmq in zmqnet, which is a
lot nicer. Why don't we use zmq in pybe too?

zmq is multithreaded. To do its magic, it spawns a background thread. The
background thread is what does the actual sending over the network.

This is problematic when your basic strategy for implementing commands is

1. Stop everything
2. `call` something

If the zmq thread gets stopped in step one, step two will never complete
because the result will never actually be sent until the background thread is
started again.

In general, the less threads we have to wrangle, the better. So we stick to a
single threaded solution. There are caveats with this approach, in particular
that avoiding blocking forever on a socket operation becomes tricky. But this
is managed well enough with non blocking sockets and timeouts.

### [De]serializing Results With[out] the C++ Standard Library

**Problem 5**: We need a serialization/deserialization strategy for results
that is easy to maintain and can be shared by the dbgsrv and pybe shared
library.

Usually writing a serialization/deserialization library is pretty easy
(especially if it doesn't happen to be optimal). But in this case, the two
sides communicating are not using the same standard libraries.

The pybe shared library is written in C++, but without the C++ standard
library. We do this to reduce the chance that injecting pybe into a user's
python interpreter would disrupt anything. If we included/used C++ shared
library symbols in the pybe shared library and a user was using a python
extension that used a different version of the C++ shared library, symbol
conflicts could happen. The pybe shared library uses TINYSTL to replace some
common C++ shared library containers (strings, vectors, etc). This is
statically linked and won't interfere with user apps.

So with dbgsrv using the C++ standard library and pybe using TINYSTL, how do we
write the serialization library in such a way that we only have to maintain a
single codebase instead of two copies, one with each shared library?

The pybe communications library is templatized on the basic C++ standard
library containers. When dbgsrv is compiled, it plugs in the C++ standard
library. When pybe is compiled, it plugs in TINYSTL. TINYSTL is (intentionally)
similar enough to the C++ standard library that strings and vectors can more or
less be used interchangeably. Since the data is first serialized into raw bytes
before contacting the other side, each side is free to use whatever string
(etc.) implementation it wants.

### Supporting Multiple Python Versions

**Problem 6**: The Python C API is not ABI compatible across Python minor
releases.

Code compiled against a particular minor version of the python C API is not
compatible with a python interpreter of a different minor version. For example,
code compiled against the python 3.10.x API will (most likely) not work with a
python 3.11.x interpreter. However, code compiled against python 3.10.x is
guaranteed to work against all python 3.10.y.

The python C API exposes something called the limited API, which *is*
guaranteed to work for all future python 3 versions than the one that it was
compiled against. Unfortunately, the limited API is extremely small and high
level. It doesn't expose the low level python internals needed for a debugger.

With the limited API unavailable, our only other option is to build multiple
versions of the code that uses the python API, one against each python version
we support and pick the correct one at runtime.

We test multiple versions using avocado's parameters/variant feature. The test
suite will search for all minor versions of cray-python on the system and test
against them - plus the system python e.g. /usr/bin/python3.

#### Changing The Supported Set Of Python Versions

To add or remove a python version, the following need to be updated:

1. The python-headers repository
   (https://github.hpe.com/hpcde/hpc-cdst-python-headers) needs to be updated
   with the new set of python headers for each supported version and the submodule
   must be updated in gdb4hpc.
2. The list of python versions in meson.build needs to be updated with the new
   set of versions.

Confirm 1 and 2 by building gdb4hpc and checking the build directory for the
expected .so files.

3. The various lists of libgdb4_pybe_3.XX.so files in the .spec file need to
   be updated with the new set of versions.

The list should match the list of built libgdb4_pybe_3.X.so files. (Ignore
libgdb4_pybe_insurance.so). Confirm by building the rpm. Make sure you have run
meson install and be careful that you don't have any old versions of
libgdb4_pybe in the installation directory that might be accidentally picked
up.

4. avocado_config_python.yaml needs to be updated with the new set of versions.

Confirm by checking that running the test suite generates tests against every
supported python version (skipped tests are okay). To check, run:

```
$ ./test_tool.py -st python -c avocado_config_python.yaml
```

In theory, that's all you need to do. gdb4hpc automatically detects and loads
the right version at runtime so no dbgsrv code changes should be needed. But
adding a new python version will build libgdb4_pybe against the new python C
API, which might have breaking changes. In general we try to use stable parts
of the python C API as much as possible, but for some debugging purposes we
have to read unstable struct members etc, which might cause an issue during the
update process. Give yourself some extra time for this possibility when you
are scheduling time to do the update.

You might also need to find a new entry point function. We rely on private
python symbols so we can set entry breakpoints for launch (see the section
about keeping the GIL invisible to the user). These symbols can disappear
between versions or simply not be called by the newest python version. The test
suite will confirm if you need to investigate this. Passing tests on the new
version mean no action is necessary.
