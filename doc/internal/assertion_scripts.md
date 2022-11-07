# `gdb4hpc` Assertion Scripts Implementation Overview

Josh Chandler

11/4/2022

---

`gdb4hpc` has a feature called assertion scripts where a user can write a set of
assertions about the program, run the program, and have the assertions checked as the
program runs.

This document contains an overview of the implementation of this feature.

The implementation of this feature is in `src/script`.

## Writing a Script

The user starts writing a script by using the `build` command. The command changes
`gdb4hpc` into script writing mode where a different set of commands are available. When
the user enters script mode, an empty pending script is created. This empty script will
be modified by `assert` and `set` commands.

### `assert`

The `assert` command creates a new assertion. An assertion is an expression to be
evaluated at a specific time in a program on specific ranks. For example:

```
assert $a::uc@"c_type_test.c":173 < $b{0}::uc@"c_type_test.c":180
```

This sets an assertion that check that the value of each instance of variable `uc` in
ranks in `$a` are less than the value of variable `uc` on rank 0 of `$b`. The check will
be performed when all ranks in `$a` are stopped at `c_type_test:173` and the `$b{0}` rank
is stopped at `c_type_test:180`.

The expression in the command is parsed for correctness as much as possible and then
saved. To parse for correctness, the expression is checked to be a binary compare
operation, procsets are validated, and the given locations are checked to make sure that
there is no location overlap such that it would be impossible to stop the program at both
at the same time.

Examples of invalid assertions:

```
assert $non_existent::uc@"c_type_test.c":173 < $b{0}::uc@"c_type_test.c":180
       ^
# $non_existent does not exist

assert $a::uc@"c_type_test.c":173 * $b{0}::uc@"c_type_test.c":180
                                  ^
# assertion must be a comparison

assert $a::uc@"c_type_test.c":173 == $a::uc@"c_type_test.c":180
                              ^                             ^
# the program referred to by $a can't be at line 173 and 180 at the same time
```

Each assertion is saved in the pending script until the user calls `end`.

### `set`

The `set` command changes the behavior of assertions defined after the command.
When an assertion is created with `assert`, the current settings of the script are
"stamped" onto the newly created assertion and saved. If the script settings change later
with a `set` command, any already-created assertions are unchanged. Only future assertions
receive the new "stamp".

For example, a user could use the `set stop on` command, write a single assert, and then
issue `set stop off`. The resulting behavior, stopping the assertion script if the
assertion fails, would only apply to that specific assertion.

Some settings don't follow this "stamp" model because they are settings for the script in
its entirety. For example, `set halt on` tells the assertion script to send a halt
(SIGINT) to all ranks when the assertion script is complete. This behavior doesn't relate
to any given assertion, so only the last invocation of `set halt` will determine the
behavior.

### `end`

When the user is finished writing an assertion script, they exit script writing mode with
the `end` command. The assertion script is "compiled", and the user can start it later
with the `start` command.

No action actually happens to "compile" the script. All that happens is a script runner
(see below) is created and saved into an internal list. The usage of the word "compile"
stems from legacy assertion script behavior where a script would be compiled into a state
machine.

## Running a Script

To run a script, the user executes the `start` command. This sets the script in motion and
runs it until completion. Completion is defined as every assertion being evaluated at
least once, an unexpected breakpoint being hit, or a rank exiting.

An assertion script is run like this:

1. Reset the internal state of the script runner and re-verify the assertions.
2. Set breakpoints mapping to each program location in the assertion script.
3. Install event handlers into `gdb4hpc`'s main loop to track when these break points are
   hit.
4. Send `continue` to the application.
5. As breakpoint events come in, match them to assertions in the assertion script.
6. When all of the ranks in an assertion are stopped at the correct location, evaluate the
   assertion and store the result.
7. Send `continue` to the stopped ranks.
8. Goto 5 until a stopping condition is met.
9. Uninstall event handlers and un-set breakpoints.

### Reset

> 1. Reset the internal state of the script runner and re-verify the assertions.

Each time an assertion script is run, the assertions inside need to be re-verified in the
same way that they were verified when they were first written. This is because `gdb4hpc`
supports the following workflow:

1. Write and then run an assertion script.
2. Restart the program with the `run` command.
3. Goto 1.

In order to find the ranks associated with an assertion and validate it, its expression is
parsed (`src/gram/*`). During parsing, pointers to procsets are generated. However, the
`run` command restarts the app, erasing these procsets and replacing them with new ones.
This invalidates the old procset pointers that were valid when the user wrote the script.

Assuming all assertions are still valid, all assertions get an "assertion tracker" created
for them. This tracker keeps track of when the program enters a state in which the
assertion can be evaluated.

A rank tracker object is also created to track the state of the program as a whole. This
is mainly for preventing deadlocks.

### Install

> 2. Set breakpoints mapping to each program location in the assertion script.
> 3. Install event handlers into `gdb4hpc`'s main loop to track when these break points
>    are hit.

When an assertion tracker is created, it extracts the locations from the expression it was
created from and saves them. When the script is installed, the assertion tracker acts on
these locations and sets breakpoints corresponding to them.

Once the breakpoints are successfully set, the script is ready to start. The script client
sets the event handlers in each `DbgInvoke` involved in the script to the script client's
specialized breakpoint event handler. The old handlers are kept track of in order to
restore them when the script stops.

The script client also sets a flag visible by `gdb4hpc`'s main loop to indicate that there
is a script installed and that script events should be checked for.

### Run

> 4. Send `continue` to the application.
> 5. As breakpoint events come in, match them to assertions in the assertion script.
> 6. When all of the ranks in an assertion are stopped at the correct location, evaluate
>    the assertion and store the result.
> 7. Send `continue` to the stopped ranks.
> 8. Goto 5 until a stopping condition is met.

`continue` is sent to the ranks, and the internal rank tracker is updated so that no ranks
are marked as blocked.

Eventually, a rank will hit a breakpoint. When that happens, the installed event handler
will receive the event and react to it. The only events that the script is interested in
are breakpoint hits and exits. Regardless of the event, after the script handles it, it is
forwarded to the old event handler.

When a breakpoint is hit, each assertion tracker is notified of it. If the breakpoint is
relevant to the assertion, its internal state is updated regarding the breakpoint. If an
assertion becomes ready, a flag is set in the script indicating so.

If the breakpoint is not relevant to any assertion, the script stops.

The ranks tracker is updated to keep track of which ranks are stopped.

`gdb4hpc`'s main loop is constantly asking if the script has any assertions ready to
evaluate. Once the script has one, the main loop tells it to evaluate any assertions that
are ready. For each ready assert, an evaluation function is called for its stored
expression. The result of the comparison is stored in the script's results.

After all ready asserts are evaluated, they are released (set running again with
`continue`).

This repeats until a stopping condition is met.

### Uninstall

> 9. Uninstall event handlers and un-set breakpoints.

When the script stops, it uninstalls itself by restoring old event handlers and deleting
the breakpoints it set when it was installed. It also unsets the flag telling the main
loop that there is script installed.

