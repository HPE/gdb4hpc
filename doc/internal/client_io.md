# gdb4hpc - Client Input and Output Architecture

Josh Chandler
10/4/2023

---

This document describes a part of the architecture of the gdb4hpc client. It focuses on
how user input is read/interpreted and how output is formatted (but not how the output is
actually generated). The introduction section is meant to be readable from start to
finish. The reference sections are unordered and are meant to be read as needed.

The gdb4hpc client's input and output implementation is organized into layers. In this
document, names of layers are in **bold**. *Italicized* words are important implementation
objects within the layers. Everything discussed in this document lives in the
`/src/client/` directory, and layers roughly map to source code files there. The **main
loop** is in `main.cpp` with calls to `dbg_client.*`, the **CLI** layer is implemented in
`cli_*` files, the **interpreter** layer in `interpreter_*` files, and the **command**
layer in `cmd_*` and `command_*` files. Layers sometimes cross file boundaries a little,
so this mapping is just a rule of thumb.

## Introduction

Starting from the moment that a user presses return on the gdb4hpc command line, the user
input travels though:

1. The **Main Loop** layer
2. The **CLI** layer (input half)
3. The **Interpreter** layer (input half)
4. The **Command** layer

At this point, the **command** layer does an action and/or generates some output. Usually
this is from talking with the **DbgClient** layer, which is the bridge between the gdb4hpc
client and the gdb4hpc server. The **DbgClient** layer is not discussed in this document.

The **command's** generated output goes goes back up through:

5. The **CLI** layer (output half)
6. The **Interpreter** layer (output half)
7. The user's screen

### The **Main Loop**, *Streams*, and the *Current Stream*

The **main loop** is responsible for detecting input, setting the *Current Stream*, and
activating a **CLI** layer implementation to consume the input.

*Streams* are the gdb4hpc client's main input/output workhorse. A *stream* is not a layer,
but a data structure that holds raw input and output sources (file descriptors, `FILE*`,
etc.) and implementations of the **CLI** and **interpreter** layers.

At startup, gdb4hpc creates one or more *streams* and registers them with its **main
loop**.

The **main loop** now starts, looping forever until gdb4hpc exits. It checks each
*stream*'s input source for activity. When input is detected, it promotes the *stream*
containing the input source to the *Current Stream*. Until new input is detected on
another stream, all input and output will flow through the *Current Stream*. 

Now the *Current Stream* points to the implementations of the **CLI** and **interpreter**
layers that are about to be used. The **main loop** now passes control to the **CLI**
layer. Note that the input has not been consumed yet. Consuming input is the job of the
**CLI** layer, and the **main loop** just detects and and picks the appropriate *stream*
to consume it.

### The **CLI** Layer (Input Half)

The **CLI** layer is responsible for consuming data from the *Current Stream*'s input
sources and delivering the data to the *Current Stream's* **interpreter**  `[1]`.

The **CLI** layer checks the *Current Stream* for a **CLI** implementation to use and
calls it, giving the implementation permission to consume data from the *Current Stream*'s
raw input sources.

Different implementations exist for different types of data source. The main types are a
command line interface in a terminal and a file (for the `source` command or batch mode).
There is also an implementation for reading data from a socket, but it is unused `[2]`.

The **CLI** layer might do some minimal processing on the input, like filter out comments
(commands starting with `#`) and manage command history, but its main job is to consume
the input and give it to the **interpreter**.

Once the **CLI** layer has extracted the data from the given input, it gives the data to
the *Current Stream's* **Interpreter**.

> `[1]` "CLI" is a historical name. A better name might be simply "input". The CLI layer
> does have a command line interface implementation, but there are other implementations
> too. From an API perspective, the CLI layer is more about handling input from an
> abstract source than specifically implementing a command line interface.

> `[2]` The socket implementation was used for a proxy client that has been removed.

### The **Interpreter** Layer (Input Half)

The **interpreter** layer is responsible for interpreting text and interfacing with
gdb4hpc. Different interpreter implementations exist for different input formats. For
example, the *Console Interpreter* accepts human readable commands, while the *MI
Interpreter* accepts less human readable commands formatted in the
[GDB/MI](https://sourceware.org/gdb/onlinedocs/gdb/GDB_002fMI.html) format.

An **interpreter** tightly coupled a *stream*. It can be thought of as part of the
*stream* itself.

99% of the time, **interpreter** functionality is implemented with the **command** layer.

### The **Command** Layer

#### History

From the perspective of the **CLI**, the **command** layer is an invisible implementation
detail of the **interpreter** layer. But historically, the **command** layer existed
before **interpreters**, and the **CLI** would talk directly to the **commands**. After
**interpreters** were implemented, the **command** layer did indeed become an
implementation detail of **interpreters**, but **commands** inherited a lot of their old
structure, which distinguishes **commands** enough to be considered their own layer.

Notably, most **commands** are **interpreter** agnostic and the two layers are not coupled
with each other. Commands exist as independent functions that are attached to
**interpreters** at **interpreter** creation time.

#### Description

From an API perspective, **interpreters** accept text as input and generate text and side
effects as output. The text that **interpreters** accept is usually in the form of
commands, like any debugger. gdb4hpc's **command** layer is what takes text with a
command-like structure and dispatches it to gdb4hpc functions.

In other words, when talking about the **command** layer, this document is talking about
the code that translates text into a function call, and the effects of the resulting
function call. All **interpreters** must translate command text into function calls
somehow, and the **command** layer is what all current **interpreters** use to do it. But
in theory, an **interpreter** is not required to use the **command** layer to do this. For
example, the *MI interpreter* has some mi-specific commands that are implemented in the
*MI interpreter* itself, but it uses the **command** layer for everything else. In
practice, the **command** layer is used in 99% of cases across all **interpreters**.

After processing the command text, the **command** layer finally causes something to
happen inside gdb4hpc. A **command** causes some effect to happen by talking to the
**DbgClient** layer. Eventually a **command** generates some output.

Output is generated, but can't be printed just yet. Different **interpreters** have
different output requirements, but **commands** don't know which interpreter is listening.
The output is passed back to the **CLI Output** layer, which does know.

### The **CLI** Layer (Output Half)

Now the **CLI** layer has some unformatted output data from a **command**, and has to
print it somehow. But on the way down to the **command** layer, the output destination
(*stream*) was lost because the **interpreter** does not tell the **command** which
*stream* it is part of.

The **CLI** layer reconstructs this information by finding the *Current Stream* and asking
its **interpreter** to accept the output for formatting.

### The **Interpreter** Layer (Output Half)

The **interpreter** formats the output as needed with its *printer*. Now the data is
finally ready for the world to see!

When **interpreters** are created, they know where their owning *stream* is. So after
formatting, the **interpreter** sends the output to its owning *stream* and the *stream*
delivers it to its final destination.

### Roundtrip Example

A user types `break source_code.c:99` into the gdb4hpc command line prompt and presses
enter:

1.  The **main loop** detects activity on the stdin file descriptor.
2.  The **main loop** looks up which *stream* is associated with stdin and declares it the
    *Current Stream*.
3.  The **main loop** calls the entry function in the **CLI** layer.
4.  The **CLI** layer looks up the *Current Stream*, finds its command line interface
    implementation, and uses it to consume the input on stdin.
5.  Inside of the command line interface implementation, the **CLI** layer checks if the
    text is a comment (it's not), records the text in the command history that it manages,
    and passes the text to the *Current Stream's* **interpreter** implementation.
6.  The **interpreter** is a *Console Interpreter*, which immediately passes the text to
    the **command** layer it uses to parse text.
7.  The **command** layer examines the text `break source_code.c:99` and traverses down
    its command tree structure to the node labeled "break".
8.  The **command** layer examines the text `source_code.c:99` and sees that it does not
    need to traverse the command tree structure any further.
9.  The **command** layer calls `cmd_break`.
10. The `cmd_break` **command** uses the **DbgClient** layer to send the breakpoint
    request to gdb4hpc and receives a sucessfull reply.
11. `cmd_break` wants to print the reply. It was not given information about which
    interpreter called it, so it calls `PrintOutMsg`, which is in the **CLI** layer.
12. The **CLI** layer looks up the *Current Stream's* **interpreter** and then gives the
    message to the **interpreter's** *printer*.
13. The *printer* checks to see if it needs to format anything (it doesn't, but an *MI
    Interpreter* would have to), and then sends the message out of its owning *stream's*
    output `FILE*`.

For an even lower level trace, check the reference section.

## Implementation Details Reference

This section goes into more depth about certain layers and their implementations. Items
are listed in no particular order.

### Async Output

Async output happens when the gdb4hpc server emits data that isn't in immediate response
to a user command.

The most common example is a "breakpoint hit" message. The user initiates the continuation
of the program, but gdb4hpc has no way to know when (or if) the program will stop. Rather
than potentially wait forever, gdb4hpc gives control back to the user and the breakpoint
hit message will be handled asynchronously.

#### Async Messages have a Different Printing Strategy (`#AsyncStreams`)

The strategy applied to the *Current Stream* selection is problematic when it comes to
async messages, especially in clients with multiple streams, like CCDB (see "How CCDB uses
Streams").

In CCDB, the following sequence of events can happen:

1. The user issues a continue through the CCDB GUI (The MI stream is now the *Current
   Stream*)
2. The user types something in the console (The console stream is now the *Current
   Stream*)
3. The breakpoint is hit, and the gdb4hpc server sends an asynchronous message to the
   gdb4hpc client.

If we used the stream switching strategy outlined in the introduction, the printing
functions would select the *Current Stream* as the output, which is currently the console
stream. This is problematic because the MI stream would not get any data, and the CCDB GUI
would not react to the change.

For reasons like this, async output is handled differently. Instead of emitting output on
the *Current Stream*, the event is sent to every stream. In the previous example, this
causes the CCDB GUI to update like it's supposed to.

However, it also causes an async message to print in the console stream. This is not
necessarily good. If the user is interacting with gdb4hpc with the CCDB GUI only, they
might expect the console to stay silent. But since our model necessitates notifying every
stream of an async message, the console makes some noise.

On the other hand, if the user is interacting with gdb4hpc with the CCDB console, they
probably expect the GUI to update in reaction to their commands, so broadcasting the event
to every stream is beneficial in that case.

#### Printing Async Messages: `PrintOutMsg` vs `printer.print_message` (`#AsyncPrinting`)

Async event handlers are implemented as part of an **interpreter**. This raises a
question: once the async event is handled, how should the **interpreter** print it? There
are two options, `PrintOutMsg` and the **interpreter** member *printer*'s `print_message`.

The answer is always use `print_message`. The different requirements for async message
printing documented above in Alternative Handling of Async Messages, we want to bypass the
*Current Stream* when handling async messages. `PrintOutMsg` goes through the *Current
Stream* by definition, while we want to output the async message to the **interpreter's**
containing *stream*.

#### **Interpreters** that Ignore Async Messages (`#IgnoreAsync`)

**Interpreters** can be created in a state where they do not receive async messages.

This is because some **interpreters** use others as implementation details. For example,
the *MI Interpreter* supports all *Console Interpreter* commands. To implement this
feature, it contains a private *Console Interpreter* that it uses to handle console
commands.

But this private *Console Interpreter* should not print async events when they happen. It
is a hidden implementation detail of the containing *MI interpreter*. The containing *MI
interpreter* creates its private *console interpreter* without connections to async events
to avoid printing the async event twice, once in the *MI interpreter* and once in the
private *console interpreter*.

### *Stream* Registration and Connection to the **Main Loop** (`#StreamInput`)

The **main loop** does not actually know what a *stream* is. What the **main loop** sees
are special *input* objects. The *inputs* are a special kind of glue object that link the
**main loop** to the **CLI** layer.

An *input* holds a file descriptor, a pointer to a *stream*, and a function pointer. The
**main loop** monitors the file descriptor for activity. When activity is found, the
**main loop** sets the *Current Stream* to the *stream* held in the *input*, and then
calls the function pointer. The function pointer points to an entry function in the
**CLI** layer.

Certain commands can create new streams. CCDB uses the `new-ui` to create a different
stream for MI commands. The `source` command creates a new temporary stream that reads
commands from a file.

### A Labelled Execution Trace

This is an execution trace starting from **main** and drilling down to the moment an
invocation of the  `frame` command prints a result under the *Console Interpreter*. Layers
are labelled, and layer transitions are marked with "->".

| Function Name                                  | Source File             | Layer                                 |
| ---------------------------------------------- | ----------------------- | ------------------------------------- |
| `main`                                         | main.cpp                | **main loop**                         |
| `DbgDispatchEvents`                            | dbg_client.cpp          | **main loop**                         |
| `inpcb_t::invoke`                              | dbg_client.cpp          | **main loop** -> **CLI**              |
| `cli_handle_input`                             | cli_funcs.cpp           | **CLI**                               |
| `cli_readline_handle_char`                     | cli_readline.cpp        | **CLI**                               |
| `rl_callback_read_char`                        | libedit.so              | **CLI**                               |
| `rl_handle_line`                               | cli_readline.cpp        | **CLI**                               |
| `record_and_execute_line`                      | cli_readline.cpp        | **CLI**                               |
| `DbgInterpret`                                 | cli_funcs.cpp           | **CLI** -> **interpreter**            |
| `ConsoleInterpreter::interpret`                | interpreter_console.cpp | **interpreter** -> **command**        |
| `CommandNode::execute`                         | cmd.cpp                 | **command**                           |
| `Command::Operator()`                          | command_invoke.h        | **command**                           |
| `Command::Impl<...>::call`                     | command_invoke.cpp      | **command**                           |
| `std::function<...>::operator()`               | std::...                | **command**                           |
| `std::_Function_handler<...>::_M_invoke`       | std::...                | **command**                           |
| `std::invoke_r<...>`                           | std::...                | **command**                           |
| `std::invoke_impl<...>`                        | std::...                | **command**                           |
| `cmd_frame_console`                            | cmd_frame.cpp           | **command**                           |
| `PrintFrameSet`                                | cmd_frame.cpp           | **command** -> **CLI**                |
| `PrintMsgWithSetAndThreadInfo`                 | procset.cpp             | **CLI**                               |
| `_print_with_set_info`                         | procset.cpp             | **CLI**                               |
| `vPrintOutMsg`                                 | cli_funcs.cpp           | **CLI** -> **interpreter**            |
| `ConsoleInterpreter::Printer::print_message_v` | interpreter_console.cpp | **interpreter** *printer* -> *stream* |
| `cli_out_t::vprintf`                           | cli_funcs.cpp           | *stream* -> raw `FILE*`               |

### How CCDB uses *Streams*

When CCDB starts a brand new gdb4hpc session, it starts gdb4hpc "normally" with a *console
interpreter* and immediately issues a `new-ui` command to create a new *stream* with an
*mi interpreter*. It connects its console window to the *console interpreter* and uses the
*mi interpreter* under the hood to make the CCDB GUI functional.

The CCDB dashboard can attach a new browser window to an existing CCDB session, in which
case it reuses the initial two streams created. The CCDB windows filter each stream for
content relevant to the window.
