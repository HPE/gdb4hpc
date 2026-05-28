# Developer notes

General guidelines for coding in gdb4hpc

## Formatting

- 4 space tabs with spaces
- curly brace placement : not worth fighting over
- snake case vs camel case : try to match the surroundings
- comments : when they matter, when there's something to explain

## Warning

Keep a clean build with `-Wall`.
           
## C++ style

This code was originally in C, by 2019 it was built with C++ but
didn't have a single occurence of the word "class".

Since this is has been developed with two drives:

- Try to use the most modern C++ style available
- But don't rewrite code without a good reason

The trend has been to redevelop modules or structs when other major
changes were needed.   So if you see a struct with function pointers
instead of virtual functions, you're seeing a piece of code that been
working without significant issue a decade or more.

That does mean you'll need to a little more mindful, a lot of new code
is self managing with smart pointers, but you will encounter some that
still need a manual delete, or even a free.

## Some things to keep in mind when programming gdb4hpc

In many ways the operation of gdb4hpc are very similar to working in
gdb or other debuggers, but there are design considerations unique to
this context.

### Data scale

Always imagine the application gdb4hpc is working with is 10K ranks
and you're looking at a large array:

- Attempting to store a result on the client will blow out the
  available memory.
- Attempting to print the full result will scroll until doomsday.
- Sending a command to one rank at a time will take hours.

Consequently:

- Prefer to keep as much computation on the servers as possible.   The
  application is using distributed processing, the debugging probably
  will as well.
- Provide ways to either search through the data or at least to focus
  on small subsets

### Data structures on multiple processes

- When printing data structures in C/C++ gdb relies heavily on
  printing pointer values.   That approach is not useful with a
  distributed program.   It's also not at all useful for comparative
  debugging.   That's why gdb4hpc tends to derefence and expand rather
  than showing a jumble of pointer values.

- When showing output of any kind, be wary of printing an overwhelming
  amount of information.   A common approach is to have the basic
  version show a heavily aggregated summary.

- However, the user also needs to be able to focus all the way down to
  the finest detail to find a bug.   Thus, many functions may print a
  summary by default, but include a "-v" option to show everything.
  The expectation is that the user might use the summary to narrow in
  on a problem rank, then use the verbose version after narrowing the
  focus.

### The gdb model and the gdb mode back-up.

- When implementing an interface command in gdb4hpc always prefer
  copying the interface from gdb whenever possible.   You add
  extensions that make sense.   For example, gdb4hpc copies the
  "shell" command from gdb, but adds the "-r" option to tell it to run
  the command on the compute nodes instead of the host node.

- Gdb4hpc should directly implement the most common and useful
  options, but can elect to ignore other feature that can be handled
  by using the "gdb" command to pass a command directly to the back
  end gdb instance.

### Distance lag

- There is a round-trip overhead to any command sent to the back-ends.
  If an operation involves multiple gdb commands, it is preferable to
  roll that into a custom operation on the back end that can be
  triggered by a single message sent across the network.

- When adapting GUI client from gdb for gdb4hpc, care must be taken
  because these will tend to assume that some operations are cheap,
  for example evaluating every local variable when the program stops
  in a function.






  







