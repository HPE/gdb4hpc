# Debugging approaches for gdb4hpc

John Vogt
5/19/2026

---

## Check for other launch problems
Verify the the application runs _outside_ of gdb4hpc.  If resources
aren't available or the application has missing dependencies that will
show up on a basic `srun` and you can avoid digging into gdb4hpc

## Other launch issues

Problems where gdb4hpc fails to connect to the application typically
mean looking into the CTI logs.   Set these environment variables to
generate logs:

```
CTI_DEBUG=1
CTI_LOG_DIR=[a directory mounted by the compute nodes]
```

You should also set up the gdb4hpc server logging:
```
CRAY_DBG_LOG_DIR=[a directory mounted by the compute nodes]
```
and use the --debug option during launch or attach.

This typically points to a problem in the CTI connection.   See the
documentation there for tips there.

## Logging internals

The logging print function in dbgsrv is `DVPRINT(const char*, ...)`
which writes a file to `CRAY_DBG_LOG_DIR` if `--debug` is enabled.

# Client side debugging

The client side tends to need less debugging since most of the
complicated processing happens on the server side or in the
communication layer.

If there are problems, you can just run `gdb4hpc` inside gdb and debug
normally.   You just need need to be careful not to try to step over
the main polling calls because that will cause the process to hang
(i.e. `DbgSendCmdAndRecvReply`).   You will probably want to rebuild
gdb4hpc with the `-O0` flag to improve the experience.

You may also resort to just adding print statement to the client code
if you have no sense of irony.

# Attaching to a dbgsrv with gdb manually

It is recommended you rebuild gdb4hpc with `-O0`.   You may be able to
see the values you need with the standard `-g -O2` build, but often it
comes up a little short.

After launching your job like usual, you can ssh to the compute node,
find the pid of the `dbgsrv` process and attach to the process with
gdb.   Quick tip, within gdb4hpc you can use the command `shell -r
hostname` to find you compute nodes.

The experience is better if you access the gdb4hpc source code from
the compute node so you can `dir` to it.   Otherwise, you'll have to
follow along in your source code manually.

Fortunately, most of things you'd need to debug are effectively
reentrant.   If you're stopping inside a `print value` command from
gdb4hpc, you can just keep re-executing the command until you see the
problem.   Note that the client side call will register a failure
since it will see the server side time out after you stop it.

# Debugging gdb4hpc with gdb4hpc

If you are developing on a VM or in a container that doesn't support
SLURM or PBS, there is a mechanism to run a dummied up version of
gdb4hpc in your development environment.   Here are the steps:

- For the functional tests, in tests/function, build the tests with
  `make LOCALHOST=1`
- That builds with gcc and point the include directory to the
  "localMPI" directory to dummy out the basic MPI functions. It also
  fakes the step of the job manager holding the job at start.
- define the environment variable `CTI_WLM_IMPL=localhost`
- note, for some reason this requires your hostname to be explicitly
  defined in `/etc/hosts`.   So on my VM with hostname
  "rh95.novalocal" it has the line `127.0.0.1   rh95.novalocal`
- in gdb4hpc launch your test job like usual and advance as far as the
  command you wish to debug
- in another shell, also define `CTI_WLM_IMPL=localhost`
- find the pid of your gdb4hpc process
- execute `attach $deb [pid of the main gdb4hpc process]
- use the `dir` command to point to the gdb4hpc source if it can't
  find it.

And presto! You're eating your own dog food.  Inside the debugger
debugger, you are almost certainly stopped where it's waiting for a
command from the client.  You'll want to set a breakpoint in the
function you're investigating, e.g. `DbgEvalExpr` for a print command,
then `continue`, otherwise the server will never pick up the message.

Just like the manual gdb case, one thing to watch out for is trying to
step over when it actually executes the gdb/mi call.   It has to wait
on a response on a socket and it will hang.   Just set a breakpoint
at `MICommandResultOK` and continue.

# Running valgrind on the server

Unfortunately, the best set-up for this was lost when we moved the
build from autotools to meson.   You can get it by hacking it into the
dbgsrv launch in `src/dbgclnt/dbgclnt_zmq.cpp`.   Just insert
"valgrind", before "dbgsrv".

# Running valgrind on the client

The client side does a setup and then exec's itself, but you can run
valgrind directly by giving it the right arguments:
```
valgrind --trace-children=yes
--trace-children-skip='*/srun,*/grep,*gdb-guard' --leak-check=full
gdb4hpc -s -b tmp/c_assign.rc --output tmp.out
```
Once trace children is turned on, valgrind will try to trace the many
children that CTI spawns, so you may want to ignore even more:
```
#!/bin/bash
# pass .rc file as the first argument.
 
# the many programs that CTI spawns (and this is only for slurm machines)
SKIPS='*/sh,*/cminfo,*/file,*/grep,*/nm,*/sha1sum,*/cti_fe_daemon,*/srun,*/ln,*/sbcast,*/gdb-guard,*/iconv,*/cti_fe_daemon*,*/bash,*/readelf,*/tr,*/cut,*/sattach,*/ld-linux-x86-64*'
 
rm tmp.out
valgrind --trace-children=yes --trace-children-skip="$SKIPS" --leak-check=full gdb4hpc -s -b "$1" --output tmp.out -e |& tee valgrind.out
```

## Address sanitizer

This is another option for checking, support should be available by
adding arguments to meson.





