# Python Support in gdb4hpc

Josh Chandler - 7/15/2024

This document describes the internals of gdb4hpc's python debugging support.

In summary, the **gdb4hpc python backend**, or **pybe** for short, injects a
shared library into the debugged python interpreter. The shared library
makes use of the python C API to implement debugger commands and sends results
to gdb4hpc's backend (dbgsrv) over a socket.

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

There are some exceptions to this approach to getting the GIL that happen
during breakpoints and right after launch. See "The GIL Should be Invisible
to the User" for a breakdown of all of the GIL situations.

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
- After a breakpoint (stepping/finishing/etc can be thought of as a special
  breakpoints in this case)
- After an attach
- After a halt

##### Getting the GIL after Launch, after Breakpoints, and Avoiding Slippage

After launching, we need to acquire the GIL so the user can immediately start
running commands. After all, the most common use case of a debugger is to start
setting breakpoints right after starting. To implement this, we pick special
entry point locations for python launches. Instead of stopping at main, we stop
just after python initializes itself. At this point, the main python thread has
the GIL but has not run any python code yet.

After launch is one of the exceptions where we don't use our hidden thread to
get the GIL. We can't use the GIL thread because it would cause slippage.

**Slippage** is what we call anything that changes the location of the python
interpreter when it shouldn't. For example, consider a user setting a
breakpoint at filename.py:80. When the application hits that breakpoint and the
user runs a `backtrace` command, they should see that the application is
stopped at filename.py:80 (of course). But often in pybe development
the application suddenly appears at filename.py:81 (or later) even though
it was never started up again after the breakpoint! That's slippage. It's
an extremely common problem in pybe that we need to always be careful about.

Now consider the situation we are in after we connect to a freshly launched
python interpreter and want to set a breakpoint at the very first python line
that will be executed. Our default process of getting the GIL requires that we
start our hidden thread, start the GIL holding thread, and wait a little while
for the holder to release the GIL and our hidden thread to get it. But if we
did this right after launch, we might slip past our first python line that we
wanted to break at since the main thread has to run for a short time!

So we need to break our original rule of not using the main thread's GIL to run
commands. We set that initial breakpoint using the main thread that already has
the GIL. Indeed, this is undesirable for the original reasons that motivated us
to use the hidden thread (unreproducable threading bugs). But through thorough
testing and the fact that we are operating in a fairly low variance situation
(for each version of python, the launch process will stop at the same
location), we get away with it.

Breakpoints are similar. Breakpoints are implemented by registering a tracing
function with python. The running python thread will call our tracing function
before every python line executed. Since it's being called directly from python,
the GIL is held during the trace function. The trace function is where
breakpoints are detected and hit. When a breakpoint is detected, a signal is
raised and the python thread stops, waiting for input from gdb.

We have to avoid slippage for the same reason that was laid out in the
paragraph that defined slippage: once we hit a breakpoint, we can't let the
python interpreter move on until the user says so. But we can't start a thread
to compete for the GIL since the main thread already has it.

Again, we break our rule and directly use the main python thread's GIL. But
this time the situation is more well defined. It's reasonable to assume that
python *expects* the python C API to be used within the tracing function - why
else would it exist? So we simply assume that using the main thread's GIL won't
lead to mysterious reference counting bugs. Hopefully that is never proven to
be a bad assumption. :-)

##### Getting the GIL after Attach or Halt

Attach and halt have the useful property that the user doesn't actually know
where their app should be when it stops. They know that it should stop
*somewhere*, but the file and line number are not predictable. This gives us
some wiggle room to do things behind the scenes before we give control to the
user. We attach/halt as normal and end up in a random state where it's not
known which thread (if any) has the GIL, but we don't give the user control
yet. Now we do our aquire/drop GIL trick. This causes slippage, but the user
never knows about it.

##### Slippage and All Stop Mode

One last note on slippage. It's a particularly tricky problem to deal with in
all stop mode. gdb has two main modes of operation for multithreaded apps: non
stop mode and all stop mode. In non stop mode, gdb has the ability to stop a
single thread (or any arbitrary amount of threads) while others keep running.
This is ideal for how we implement pybe since it lets us run pybe operations at
a very granular level without the whole app starting up and changing state.
In all stop mode, once one thread stops, gdb stops the rest of the threads. The
same applies to starting threads. Crucially, using gdb's `call` command starts
a thread to evaluate its result. This means that in all stop mode the `call`
command starts all threads! This is important to us because we use gdb's `call`
as the basic interface between dbgsrv and pybe.

This means that sometimes, while it would seem more safe to use our hidden
thread to do something, we have to use the main thread to avoid slippage. For
example, setting breakpoints doesn't actually require the GIL. Setting a
breakpoint doesn't use the python C API (which requires the GIL), it just sets
up some pybe internal tracking information. This makes it tempting to always
use our hidden thread for setting breakpoints. But setting breakpoints still
requires a `call` into pybe. If we tried to set a breakpoint using the hidden
thread in all stop mode, the main thread might slip while we do it!

All stop mode is actually the default setting for gdb, but in gdb4hpc we
silently set non stop mode as the default. We do this by running a quick `set
non-stop on` in gdb before we attach to a freshly launched app. The gdb4hpc
user has the option to disable this with the gdb4hpc command `set non-stop
off`.

Notably, the user can't change the non stop/all stop setting after gdb has
launched or attached to an app. This is a gdb limitation.

Double notably, gdb4hpc's `attach` command does not respect the current gdb4hpc
`non-stop` setting. Since all stop is the default gdb setting, all gdb4hpc
attaches run in all stop mode. This is basically an oversight in gdb4hpc and
could be fixed very easily with a quick bugfix PR, but it's the current status
quo and worth keeping in mind.

### Getting Results Back to dbgsrv - The Socket

**Problem 4**: After generating the result data with the python C API, we need
a way to get it back to dbgsrv.

Since we are running our functions inside of the python interpreter, not
dbgsrv, we can't simply `return` the generated struct to the caller. The caller
is some function in some thread in the python interpreter and if we returned it
there that python debug data we worked so hard to collect would disappear into
the ether forever.

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
about network trouble.

#### Linux Socket vs. zmq Sockets

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
(especially if it doesn't have to be optimal). But in this case, the two
sides communicating are not using the same standard libraries.

The pybe shared library is written in C++, but without the C++ standard
library. We do this to reduce the chance that injecting pybe into a user's
python interpreter would disrupt anything. If we included/used C++ shared
library symbols in the pybe shared library and a user was using a python
extension that used a different version of the C++ shared library, symbol
conflicts could happen. The pybe shared library uses TINYSTL to replace some
common C++ shared library containers (strings, vectors, etc). This is
statically linked and won't interfere with user apps. pybe also uses some
of the C++ shared library that is templatized and completely implemented in
headers, e.g. smart pointers. Since they are templatized on pybe internal
types, they won't collide with any symbols in the wild. And even if they did,
they are compiled as private symbols in pybe so they wouldn't collide anyway.

So with dbgsrv using the C++ standard library and pybe using TINYSTL, how do we
write the serialization library in such a way that we only have to maintain a
single codebase instead of two copies, one with each shared library?

The pybe communications library is templatized on the basic C++ standard
library containers. When dbgsrv is compiled, it plugs in the C++ standard
library. When pybe is compiled, it plugs in TINYSTL. TINYSTL is intentionally
similar enough to the C++ standard library that STL strings/vectors/etc can
more or less be used interchangeably with TINYSTL strings/vectors/etc. Since
the data is first serialized into raw bytes before contacting the other side,
each side is free to use whatever string (etc.) implementation it wants.

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
suite will try to find different versions of python available on the system and
test against them. See avocado_config_python.yaml and tests.py.

#### Changing The Supported Set Of Python Versions

To add or remove a python version, the following need to be updated:

1. The python-headers repository
   (https://github.hpe.com/hpcde/hpc-cdst-python-headers) needs to be updated
   with the new set of python headers for each supported version and the submodule
   must be updated in gdb4hpc.
2. The list of python versions in meson.build needs to be updated with the new
   set of versions.

Confirm 1 and 2 by building gdb4hpc and checking the build directory for the
expected .so files. (API changes/breakages might make this take longer than you
bargained for, see step 5)

3. The various lists of libgdb4_pybe_3.XX.so files in the .spec file need to
   be updated with the new set of versions.

The list should match the list of built libgdb4_pybe_3.X.so files. (Ignore
libgdb4_pybe_insurance.so). Confirm by building the rpm. Make sure you have run
meson install and be careful that you don't have any old versions of
libgdb4_pybe_3.X.so in the installation directory that might be accidentally
picked up.

4. avocado_config_python.yaml needs to be updated with the new set of versions.

Confirm by checking that running the test suite generates tests against every
supported python version (skipped tests are okay). To check, run:

```
$ ./test_tool.py -st python -c avocado_config_python.yaml
```

5. (sometimes) dbgsrv code.

In theory, you can end at step 4. gdb4hpc knows the list of supported python
versions from the build system you updated in step 2, and it automatically
detects and loads the right version at runtime so no dbgsrv code changes will
be needed for version selection. But adding a new python version will build
libgdb4_pybe against the new python C API, which might have breaking changes.
In general we try to use stable parts of the python C API as much as possible,
but for some debugging purposes we have to read unstable struct members etc,
which might cause an issue during the update process. Give yourself some extra
time for this possibility when you are scheduling time to do the update.

You might also need to find a new entry point function. We rely on private
python symbols so we can set entry breakpoints for launch (see the section
about keeping the GIL invisible to the user). These symbols can disappear
between versions or simply not be called by the newest python version.

Similarly, detecting the special case for using the GIL at launch (see the
section about slippage) relies on private python function names and might need
to be changed.

In general, the test suite will confirm if you need to investigate anything.
Passing tests on the new version mean no action is necessary.

### Compiler/Packaging Quirks of libgdb4_pybe.3.X.so

**Problem 7**: We do some weird stuff to make this all work and compilers/rpm
aren't expecting it.

In some places in pybe, we rely on information that compilers assume doesn't
get relied on. As the end of the last section alludes to, we rely on certain
symbols being present in stack traces to determine how to use the GIL.
Compilers love to inline functions calls when they can, which can break things.

Similar things can happen to local variables that appear unused in the source
code but are modified by gdb commands.

These are all solved by compiler attributes/keywords, be sure to keep an eye
out for them! If you add one, add a comment explaining why it's there.

Another note is that since dbgsrv communicates with the pybe shared library
with gdb, the shared library requires debug info. This is easy enough to tell
the build system, but rpms have their own assumptions. The default process of
packaging a binary in an rpm ends with stripping the binary of all debug info.
The intent is that debug info will be offered as a separate package - but we
don't do that. So we need to figure out a way to disable the stripping.
Stripping can be disabled globally, but for a more granular approach we do a
trick. We package the pybe shared library as a compressed gzip file and
uncompress it at package installation time. Properly packaging this requires
some uncommon rpm macros (e.g. `%ghost`), but it works.
