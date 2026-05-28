# gdb4hpc structural overview

John Vogt
5/18/2026

---

## General structure

Gdb4hpc connects a client running on the host machine to a gdb
instance connected to every single rank on the compute nodes.   As a
starting point, you think of the client accepting a gdb like command,
forwarding it to all the back-end gdb instances, and then printing the
response.   However, you will find gdb4hpc is much more sophisticated.

## The client layer

The "src/client" directory implements the user facing interface, which
is a text based interaction closely modeled on gdb.   The command set
is a subset of gdb's functions, sometimes with some extensions for
HPC.

The main processing loop is in the `main.cpp` which starts the
process, then accepts lines of inputs and dispatches each command to
its handler.

We'll use the "frame" command to illustrate, the UI code starts
here:
```
void ConsoleCommandsFactory::insert_frame_commands(CommandNode& root) {
    root.addSubCmd({
        .name = "frame", .ptype = proc_required,
        .fn = cmd_frame_console, .help = cmd_frame_help,
        .desc = "Print the currently selected stack frame."});
```

This area of code is still largely in the original C style with a
struct with function pointers.   To code chooses to live with a
variety of C++ styles rather than modifying working code or locking
into obsolete styles.

Structurally, the `ConsoleCommandsFactory` is a creating a tree of
`CommandNodes` which map a command to both it implementation and its
help text.   It's a tree, because gdb has commands like `set` and
`info` that have multiple sub-commands.

The client layer is broad, but not deep.   You should have little
trouble tracing the logic here.

## The dbgclnt layer

The debug client layer provides all of the client side data handling
that makes up the "4hpc" part of gdb4hpc.

The "frame" command here unfortunately opted to choose the name
"GetArgs" for this command, but we are more interested in the calls
and data types.    The dbgclient level code looks like this:

```
// Return arguments of current frame
rep_set_t DbgGetArgs(rec_set_t *set) {
    auto request = CommandToSocProto(DBGCMD_GETARGS, set->getThreads());
    rep_set_t result = DbgSendCmdAndRecvReply(set, request, DBGEV_STRING);
    return result;
}
```

which has all the key parts:

- `rec_set_t` : short for "receiver set".    One of the key "4hpc"
  function is the ability to send command to a _subset_ of the ranks
  which is exposed to the user via the "focus" command.   For the
  code, this is managed via the rec_set which you can think of a bit
  mask over the set of ranks.
- `CommandToSocProto` : this is a form of remote procedure call, so
  the command and all the input arguments have to be marshaled into a
  packet.
- `DbgSendCmdAndRecvReply` : Performs the actual communication.  The
  final argument here defines the type of response that is expected.
  In many cases, this can just be success or failure.
- `rep_set_t` : short for "reply set".   Executing the command
  generated a response from every gdb in the rec_set.    A key part of
  the design is that the reply set is *aggegated*, it stores each
  *unique* response along with the set of ranks that generated it.
- `DbgInvoke` : doesn't appear directly in the interface, it's
  referenced from the receive set and reply sets.    It manages a
  single application run.   A single gdb4hpc client can do multiple
  `launch` or `attach` commands to simultaneously manage several
  independent hpc jobs.  Running two or more job side by side is the
  key to the comparative debugging feature.

Putting that all together, this layer encodes a command as a message,
sends it to a set of back-end processes, reads the response messages,
and stores it in a reply structure.

## The dbgcomm layer

The src/dbgcomm directory has the communication code between the
client and server.  Objects in this directory are included into both
the client and server side binaries.

### zmqnet / MRnet

The backbone of the communication is done by zmqnet, which is a
separate package that is modeled on MRNet.   These are
multicast/reduction networks that create a n-ary tree of
communications to send a command from the client to the set of ranks,
and to gather and aggregate all the response messages.

The key feature of the network is that results are aggregated at every
level of the tree.  Otherwise the client would be overwhelmed with 10K
individual responses.

### dbgevent_t

dbgevent_t is a discriminated union of every possible type returned by
the back-end, including of course failure cases.   Depending on the
function, this can be a simple success/failure, a string or other
primitive, a pre-defined data structure like a stack trace, up to an
ad hoc structure to print a C++ class structure or fortran aggregate.

The bookends of dbgcomm interaction are the functions CmdToSocProto
and EventToSocProto.

### encoding general data

One of the most complicated things gdb4hpc does is to deal with
handling the general application data access via the `print` command
in gdb.   To implementing comparative debugging, gdb4hpc cannot just
return that result as a string but has to parse and understand the
structure.   This means dealing with primitives, arrays, containers,
structs, classes, nested classes anything that might appear in a
program.

The marshaling and unmarshaling is handled via the AIS (Abstract
Information Stream) which translates a data structure into a self
describing string which can then be unpacked and processed on the
client side.

The structure is complicated enough to require a separate section.

### blockmaps

Blockmaps in an HPC environment define how a single large array may be
distributed across multiple ranks but still processed as a single
unified array on the client side.   Similar to the SocProto, the
client and server need to agree so the logic is kept in dbgcomm.

### "Chunked" data

Since we are working in an HPC environment, it is possible for each
node to individually we working with a local large data structure and
trying to process the results of thousands and thousands of ranks
could overwhelm the client.   The AIFStream interface defined here
handle breaking to result into separate chunks.   This allows the user
to get the first (nominally) million entries and then waiting for
confirmation to transmit and display the next million entries.

## dbgsrv

`dbgsrv` is the process deployed on the compute nodes by gdb4hpc.  On
one end it connect to the zmqnet tree via a socket using the dbgcomm
tools, and on the other end it connects via a socket to the gdb
process and communicates via the GDB/MI protocol via the libmi
library.

The `main` for `dbgsrv` is defined in src/dbgsrv/dbgsrv.cpp.  All the
steps to start a gdb process, connect to the application process,
connect the GDB/MI socket and get to a stable state is complicated
enough to have a separate dbgsrv_init.cpp file.   The rest of `main` is
the main processing loop where it waits for message on the command
channel of the zmqnet connection.

### DbgGdbMI

The command dispatch logic is still largely in the original C code
form, a big "switch" statement is about as good for the job as
anything, but the actual working code has largely been gather into a
`DbgGdbMI` class which is the single place to maintain all of the
state for the back-end process.    Going back to our "frame" example
will show the kind of work these function need to do:


```
dbgevent_t *DbgGdbMI::getFrame(bool verbose, const char* threads) {
    DVPRINT("*** Dbg %s, %s\n", __FILE__, __func__);
    dbgevent_t *e;

    if (threads && threads[0]) {
        auto op = std::string{threads} + " frame";
        return threadEventToFrame(threadApply(op.c_str()));
    }

    // Get the selected frame
    auto cmd = MIStackInfoFrame();
    SendMICommand(GDBMISession, cmd);

    if (!MICommandResultOK(cmd)) return NewMIErrorEvent(cmd, FREE_CMD);

    // process the result
    auto mi_frame_list = MIGetFrameInfo(cmd);

    if (mi_frame_list == NULL) return NewMIErrorEvent(cmd, FREE_CMD, DBGERR_NOSTACK);

    // get the frame
    MIListSet(mi_frame_list);
    auto mi_frame = (MIFrame *)MIListGet(mi_frame_list);

    if (!mi_frame) {
        MIListFree(mi_frame_list, NULL);
        return NewMIErrorEvent(cmd, FREE_CMD, DBGERR_NOSTACK);
    }

    // create the result
    auto result = MakeFrameFromMIFrame(mi_frame, verbose);

    // free the MIFrame list
    MIListFree(mi_frame_list, (void (*)())MIFrameFree);
    MICommandFree(cmd);

    // create the event with the frame information
    e = NewEvent(DBGEV_FRAME);
    e->ev_frame = result;

    return e;
}
```

The biggest component of this is working the libmi API to dispatch a
command to the gdb and then process the result, which in this case
means translating an `MIFrame` object into a `dbg_event` with a
`dbg_frame_t` and doing the manual cleanup because libmi is a C API.

### dbgsrv_typeparser

For the most part, the possible event types for a gdb command are
simple, the MI result is a status, a string, or at most a set of stack
frames which is just a list of simple structures.

The big exception is the `evalExpr` command which is exercised when
via `print someVariable` in the gdb interface.   In that case, gdb
gives us human readable string that needs to be parsed into simple
elements for transmission and processing.

The `dbgsrv_typeparser.cpp` is a beast because it has to translate the
string gdb gives us for the type of a value into the AIS structure
describing it.   So it may get something like `std::vector<StudentId>`
and use additional "whatis" or "ptype" calls to determine that
`StrudentId` is a C++ class with a "_name" field of type `std::string`
and "_id" field that is yet another nested struct.   This will map
onto an ais::Vector object whose `m_elemtype` is an ais::Struct and so
on.

### dbgsrv_libmi2aif.cpp

This is another large and relatively complicated component of the
debug server.   Much of the lowest level parsing is handled by ais,
but this file has to manage the data caching and type handling.  

It also contains the support for breaking very large arrays into
manageable sized chunks to prevent the client from being overwhelmed
by terabytes of data.   The `SubArray` type here is specialized with
methods to tell gdb we want the second million elements of an array.

## src/gram

gdb4hpc evaluates expressions for syntactic correctness before sending
them to the back-ends.    The `expr.y` file here defines the valid
grammar.  The result is not foolproof since gdb4hpc can potentially
reference C, C++ or fortran expressions, and also because the client
side does not know whether a particular identifier is a variable name
or a type.   The key correctness it has to enforce is to be sure gdb
isn't being give a partial expressing that would make it hang
expecting the rest of the expression, e.g. `p (a+b`.

`val.cpp` is also in this directory which handles the procset form of
variable reference as in `print $a{0..2}::b` which will print the
value of the variable `b` in the first three ranks of the application
referenced by procset `$a`.   To evaluate that expression, gdb4hpc
will have to make a custom `rec_set` referring to those three ranks
and them evaluate `b` within that set.

Logic for other gdb4hpc expression supplements like blockmaps and
array slicing are here as well.

## src/script

This implements the assertion scripts.   It's been rewritten as a
reasonably straightforward implementation of the feature.  The best
reference here is the user level documentation of assertion scripts.

## frontend gdb

When launching an hpc job, gdb4hpc starts an instance of gdb on the
host node that it can use for some operations that do not require a
running process, for example `list` or checking for symbols.

No frontend instance is created on an `attach`