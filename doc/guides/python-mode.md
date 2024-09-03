---
title: "Debugging Python Applications with Python Mode"
---

# Debugging Python Applications with Python Mode

In gdb4hpc 4.16.3, gdb4hpc gained initial beta support for debugging HPC Python
applications via a new Python mode.

In Python mode, common debugging commands automatically execute in the context
of Python code (.py files) instead of the internals of the Python interpreter
that you would normally get when attaching gdb to a Python interpreter.

gdb4hpc Python mode can display Python backtraces, print Python expressions,
set Python breakpoins, and step through Python code.

## Python Mode is in Beta

Python mode is brand new and only a small portion of the possible debugging
space has been explored. While its existing functionality has been thoroughly
tested, it is inevitable that there are cases that haven't been encountered
yet. If you encounter a problem, check the troubleshooting section at the end
of this document or submit a bug report.

## Supported Python Versions and gdb4hpc Workflows

Python mode is supported when you are using the CPython Python interpreter for
your HPC application. [CPython](https://github.com/python/cpython) is the name
of the reference implementation of Python maintained by the Python foundation.
Most python interpreters you find in the wild are CPython. The Python
interpreter provided by the cray-python module in the Cray Programming
Environment is an instance of the CPython interpreter.

The following Python versions and debugging workflows are supported:

| Python Version | 3.6 | 3.8 | 3.9 | 3.10 | 3.11 | 3.12 |
| -------------- | --- | --- | --- | ---- | ---- | ---- |
| Launch         | yes | yes | yes | yes  | yes  | yes  |
| Attach         | no* | yes | yes | yes  | yes  | yes  |

You can verify that your version of Python is supported by checking the
version with `python3 --version`.

> Due to limitations in Python 3.6, attach is not supported. gdb4hpc can
> still attach to a Python 3.6 interpreter like any C binary, but will be
> debugging the Python interpreter rather than the Python script that the
> interpreter is running.

> If you are unfamiliar with gdb4hpc's two workflows, launch and attach, see
> the [gdb4hpc Getting Started Guide](./getting-started.md).

## Using Python Mode

### Launching or Attaching to a Python Application

To start debugging a Python application, launch or attach to a Python HPC
application as you would any other. For more information on the launch and
attach commands, see the [gdb4hpc Getting Started Guide](./getting-started.md).

For example:

Launching via the traditional gdb4hpc launch command:

```
dbg all> launch $my_python_app{4} python3 ./src/python/my_python_app.py app_arg_1 app_arg_2
Starting application, please wait...
Launched application...
Enabling python debugging. To disable, use `set python off`
4/4 ranks connected
Creating network... (timeout in 300 seconds)
Created network...
Connected to application...
Launch complete.
my_python_app{0..3}: Initial breakpoint, in pymain_init
```

Launching via the srun launch command:

```
dbg all> srun -n4 python3 ./src/python/my_python_app.py app_arg_1 app_arg_2
Starting application, please wait...
Launched application...
Enabling python debugging. To disable, use `set python off`
4/4 ranks connected
Creating network... (timeout in 300 seconds)
Created network...
Connected to application...
Launch complete.
python3{0..3}: Initial breakpoint, in pymain_init
```

Attaching to a running Python application (Python 3.8 and higher):

```
$ srun python3 ./src/python/my_python_app.py app_arg_1 app_arg_2 &
$ squeue
             JOBID PARTITION     NAME     USER ST       TIME  NODES NODELIST(REASON)
          11251488  allnodes  python3      cpe  R       0:01      1 n022
$ gdb4hpc
dbg all> attach $my_python_app 11251488.0
Attaching to application, please wait...
0/1 ranks connected... (timeout in 300 seconds)
1/1 ranks connected.
Created network...
Connected to application...
Attach complete.
Current rank location:
  (backtrace omitted)
```

> Launching and attaching change depending on the WLM used on your system. See
> the [Getting Started Guide](./getting-started.md) for more details on
> launching and attaching to jobs.

### Enabling Python Mode

gdb4hpc will automatically detect when you are debugging a Python application
and enable Python mode. See the "Enabling python debugging..." messages in the
above examples. However, the automatic detection is not perfect and will
sometimes not detect a Python application. In those situations, enable Python
mode manually with the `set python` command. For example, if you run the
`backtrace` command and get a C backtrace (here showing the internals of the
CPython interpreter):

```
dbg all> backtrace
my_python_app{0}: #15 _start
my_python_app{0}: #14 __libc_start_main
my_python_app{0}: #13 Py_BytesMain
my_python_app{0}: #12 Py_RunMain
my_python_app{0}: #11 _PyRun_AnyFileObject
my_python_app{0}: #10 _PyRun_SimpleFileObject
my_python_app{0}: #9  pyrun_file
my_python_app{0}: #8  run_mod
my_python_app{0}: #7  run_eval_code_obj
my_python_app{0}: #6  PyEval_EvalCode
my_python_app{0}: #5  _PyEval_Vector
my_python_app{0}: #4  _PyEval_EvalFrameDefault
my_python_app{0}: #3  PyObject_Vectorcall
my_python_app{0}: #2  cfunction_vectorcall_O
my_python_app{0}: #1  time_sleep
my_python_app{0}: #0  clock_nanosleep@GLIBC_2.2.5
```

Check if Python mode is enabled with `show python`:

```
dbg all> show python
Python mode for future launches is off.
For current procsets:
my_python_app{0}: Python mode is off.
```

If it's disabled, enable it with `set python on` and the `backtrace` command
will display a Python backtrace.

```
dbg all> set python on
dbg all> backtrace
my_python_app{0}: #2  <module> at my_python_app.py:16
my_python_app{0}: #1  main at my_python_app.py:13
my_python_app{0}: #0  sleep at my_python_app.py:10
```

### Available Commands

Any gdb4hpc command can be run in Python mode, but only specific commands have
Python centric behavior. If there is no Python specific implementation of the
command, behavior will fall back to the default C/C++/Fortran centric version
of the command.

The following examples use the following Python code which calculates the
fibonacci sequence:

```python
import sys

def fibonacci(n):
   if n <= 1:
       return n
   else:
       return(fibonacci(n-1) + fibonacci(n-2))

nterms = int(sys.argv[1])

print("Fibonacci sequence:")
for i in range(nterms):
   print(fibonacci(i))
```

#### `break`, `delete` - Set and Delete Breakpoints in Python Code

Use the `break` command in Python mode to set breakpoints in Python code just
like you would set breakpoints in C/C++/Fortran. You can specify breakpoint
locations as usual with a function name or a file name and line number
combination. When the breakpoint is no longer needed, use the `delete` command
and the breakpoint ID to delete it like any other breakpoint.

For example, launch the example fibonacci Python application and set some
breakpoints:

```
dbg all> srun -n4 python3 ./fib.py 10
...
python3{3}: Initial breakpoint, in pymain_init
dbg all> break fib.py:9
python3{0..3}: Breakpoint 1: file fib.py, line 9.
dbg all> break fibonacci
python3{0..3}: Breakpoint 2: file , line 0 # see troubleshooting and limitations section
```

Use the usual `continue` command as usual to run the application until it gets
to the breakpoints:

```
dbg all> continue
python3{0..3}: Breakpoint 1,  at fib.py:9
dbg all> continue
python3{0..3}: Breakpoint 2, in fibonacci
```

Now that we are stopped in a Python source file, we can omit the file name
portion when we set new breakpoints:

```
dbg all> break 5
python3{0..3}: Breakpoint 3: file fib.py, line 5.
dbg all> continue
python3{0..3}: Breakpoint 3,  at fib.py:5
```

Delete breakpoints as you would any other breakpoint, by using the `delete`
command and the breakpoint ID.

```
dbg all> delete 1
dbg all> delete 2
dbg all> delete 3
dbg all> continue
dbg all> <$python3>: Fibonacci sequence:
<$python3>: 0
<$python3>: 1
<$python3>: 1
<$python3>: 2
<$python3>: 3
<$python3>: 5
<$python3>: 8
<$python3>: 13
<$python3>: 21
<$python3>: 34
python3{0..3}: The application has reached an exit breakpoint.
```

#### `backtrace` - Generate a Python Backtrace

When stopped in a Python source file, use the usual `backtrace` command to
generate a Python backtrace:

```
dbg all> srun -n4 python3 ./fib.py 10
...
python3{3}: Initial breakpoint, in pymain_init
dbg all> break lib:py:5
dbg all> # get a few recursion levels deep
dbg all> continue
python3{0..3}: Breakpoint 1,  at fib.py:5
dbg all> continue
python3{0..3}: Breakpoint 1,  at fib.py:5
dbg all> continue
python3{0..3}: Breakpoint 1,  at fib.py:5
dbg all> continue
python3{0..3}: Breakpoint 1,  at fib.py:5
dbg all> continue
python3{0..3}: Breakpoint 1,  at fib.py:5
dbg all> backtrace
python3{0..3}: #3  <module> at fib.py:13
python3{0..3}: #2  fibonacci at fib.py:7
python3{0..3}: #1  fibonacci at fib.py:7
python3{0..3}: #0  fibonacci at fib.py:5
```

In a backtrace, `<module>` is the "function name" of script level (i.e. code
not contained in a function) Python code.

#### `print` - Print Python Expressions

When stopped in a Python source file, use the usual `print` command to print
Python expressions. As usual with the `print` command, you can reference local
or global variables, function arguments, or arbitrary constant expressions.
Continuing from the location we stopped at in the backtrace example:

```
dbg all> print n
python3{0..3}: 1
dbg all> print locals()
python3{0..3}: {'n': 1}
dbg all> print fibonacci
python3{0}: <function fibonacci at 0x7faaa9298ae0>
python3{1}: <function fibonacci at 0x7f9d4f8e4ae0>
python3{2}: <function fibonacci at 0x7f9633d7cae0>
python3{3}: <function fibonacci at 0x7fa073d38ae0>
dbg all> print fibonacci(10)
python3{0..3}: 55
dbg all> print fibonacci(10) * 10
python3{0..3}: 550
dbg all> print sys
python3{0..3}: <module 'sys' (built-in)>
```

#### `step`, `next`, `finish` - Step Through Python Code Line-by-Line

When stopped in a Python source file, use the usual `step`, `next`, and
`finish` commands to navigate through the Python code in a controlled fashion.
Each command has its usual meaning:

- `step`: Advance forward one line. If the current line contains a function
          call, enter the function and then stop.

- `next`: Advance forward one line. If the current line contains any function
          calls, don't enter them. Continue until the next line in the current scope is
          reached and then stop.

- `finish`: Advance until the current function returns and then stop.

```
dbg all> srun -n4 python3 ./fib.py 10
...
python3{3}: Initial breakpoint, in pymain_init
dbg all> break fib.py:13
python3{0..3}: Breakpoint 1: file fib.py, line 13.
dbg all> continue
python3{0..3}: Breakpoint 1,  at fib.py:13
dbg all> next # doesn't step into the fibonacci call
python3{0..3}: <module> at fib.py:12
dbg all> next
python3{0..3}: Breakpoint 1,  at fib.py:13
dbg all> step # steps into the fibonacci call
python3{0..3}: fibonacci at fib.py:3
dbg all> finish # runs until the function returns
python3{0..3}: <module> at fib.py:12
```

### Temporarily Disabling Python Mode

If you are debugging a Python application but want to see the native
(C/C++/Fortran) backtrace or another native debugging item, Python mode can be
disabled and normal gdb4hpc functionality will be restored.

This is a common situation when debugging native modules that are called from Python code.

```
dbg all> continue
python3{0..3}: Breakpoint 1,  at fib.py:1
dbg all> backtrace
python3{0..3}: #0  <module> at fib.py:1

dbg all> set python off
dbg all> backtrace
python3{0..3}: #15 _start
python3{0..3}: #14 __libc_start_main
python3{0..3}: #13 Py_BytesMain
python3{0..3}: #12 Py_RunMain
python3{0..3}: #11 _PyRun_AnyFileObject
python3{0..3}: #10 _PyRun_SimpleFileObject
python3{0..3}: #9  pyrun_file
python3{0..3}: #8  run_mod
python3{0..3}: #7  run_eval_code_obj
python3{0..3}: #6  PyEval_EvalCode
python3{0..3}: #5  _PyEval_Vector
python3{0..3}: #4  _PyEval_EvalFrameDefault
python3{0..3}: #3  call_trace
python3{0..3}: #2  pybe::trace
python3{0..3}: #1  pybe::PybeImpl::trace_line
python3{0..3}: #0  raise

```

Python mode can be re-enabled via `set python on`:

```
dbg all> set python on
dbg all> backtrace
python3{0..3}: #0  <module> at fib.py:1

```

## Troubleshooting and Known Limitations

This section outlines some known limitations of gdb4hpc's Python mode and
offers some workarounds for the most common issues. If you encounter a problem
that is not listed here, please file a bug!

The issues listed here will be fixed in a future version of gdb4hpc.

### Breakpoints/Step/Next/Finish Only Work in the Main Thread

Due to limitations in Python, breakpoints in Python code are only hit by the
main python thread. The step, next, and finish commands are implemented with
similar techniques to breakpoints and also do not work outside of the main
python thread. If these commands are used on a non-main thread, the application
will act as if a plain `continue` command was used.

#### Workaround

If you can get a non-main thread to stop in a position that you want to debug
(e.g. using the `halt -a` command or setting a breakpoint in native code that
it calls), the `print` command will still work. Python threads are selected as
usual via the `thread` command.

### Breakpoints are Sometimes Missing File Names and Line Numbers

Due to limitations in Python, setting a breakpoint using a function name will
not show file names or line numbers.

#### Workaround

When setting a function name breakpoint, a "breakpoint set..." result is always
returned. gdb4hpc is treating it like a pending breakpoint. The function name
has not been verified to actually exist and gdb4hpc is only keeping an eye out
for it. When it does encounter it, it will break as normal and generate a
message with the actual location.

This means that extra care should be taken when typing function names for
breakpoints. If gdb4hpc doesn't seem to hit the breakpoint, double check that
the function name was spelled correctly.

### Some Features Don't Work Directly After a Launch

After a launch, gdb4hpc stops the Python interpreter before it starts
interpreting Python code to give you the chance to do debugging set up tasks
like setting breakpoints. In fact, in this state, setting breakpoints is the
only thing you can do. Other Python mode features require that the current
thread be stopped in Python code. This requirement is not met directly after a
launch since no Python code is executing yet.

#### Workaround

Setting line number breakpoints without a file name (e.g. `break 17`) will not
work directly after launch because gdb4hpc does not know what the current
Python file is (in fact, there is no current file at all). You can work around
this by specifying the file name e.g. `break my_app.py:17`.

Once you have stopped in the middle of Python code (e.g. via a breakpoint or
`halt`), all Python mode features will work. To get access to all Python
functionality before any code is interpreted, set a breakpoint at an early line
of Python code before any meaningful code is executed. Import statements are
good candidates.

### Step/Next/Finish can Skip Lines in Higher Stack Frames After a Return

Due to limitations in Python, gdb4hpc can only track Python code execution on
the granularity of a line, and each line is only seen once. This has subtle
consequences when stepping in and out of functions with `step`, `next`, and
`finish`.

Consider the following Python code:

```python
def foo():
    return 1

def bar():
    return 2

result = foo() + bar()
print(result)
```

If you are stopped at the `result = ...` line and `step`, gdb4hpc will step
into `foo` as expected. If you then `finish`, you will suddenly find yourself
in `bar`. The usual behavior for debuggers would be to stop at the `result =
...` line for a second time before diving into `bar`.

#### Workaround

Break expressions you are interested in into multiple lines.
