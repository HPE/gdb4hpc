---
title: "Kokkos Support in Gdb4hpc"
---

## Printing kokkos::Views

gdb4hpc 4.15 lets you print the contents of the most common kokkos::View arrays, at least those with the usual "left" or "right" layouts.    

Since these arrays tend to be large, the handling is built around getting ranges of values.   So printing a kokkos::View object gives the bounds of the matrix:

~~~~~
dbg all> launch $a{4} --non-mpi ./TeamVectorLoop.host
Starting application, please wait...
Launched application...
0/4 ranks connected... (timeout in 299 seconds)
4/4 ranks connected.
Created network...
Connected to application...
Launch complete.
a{0..3}: Initial breakpoint, main at /home/jvogt/Kokkos/kokkos-tutorials/Exercises/team_vector_loop/Solution/team_vector_loop_solution.cpp:56
dbg all> b 149
a{0..3}: Breakpoint 1: file /home/jvogt/Kokkos/kokkos-tutorials/Exercises/team_vector_loop/Solution/team_vector_loop_solution.cpp, line 149.
dbg all> c
  ... program output ...
a{0..3}: Breakpoint 1, main(int, char**) at /home/jvogt/Kokkos/kokkos-tutorials/Exercises/team_vector_loop/Solution/team_vector_loop_solution.cpp:149
dbg all> p y
a{0..3}: Kokkos::View use (0..1023,0..255) for full contents
dbg all> 
~~~~~

".." is the range operator in gdb4hpc.  It's use isn't limited to kokkos, it can also be used for C arrays, C++ containers, for example: `p my_array[4..6]`.   You also see it in the procset result `a{0..3}` which is telling you the View is the same size in all four ranks.

And of course you can change the limits to look at a slice of the array:

```
dbg all> p y(0..5,11..16)
a{0..3}: ((10, 11, 12, 13, 14, 15), (20, 21, 22, 23, 24, 25), (30, 31, 32, 33, 34, 35), (40, 41, 42, 43, 44, 45), (50, 51, 52, 53, 54, 55), (60, 61, 62, 63, 64, 65))
dbg all> 
```

or

```
dbg all> p y(1023,200..255)
a{0..3}: (3, 13, 23, 33, 43, 53, 63, 73, 83, 93, 3, 13, 23, 33, 43, 53, 63, 73, 83, 93, 3, 13, 23, 33, 43, 53, 63, 73, 83, 93, 3, 13, 23, 33, 43, 53, 63, 73, 83, 93, 3, 13, 23, 33, 43, 53, 63, 73, 83, 93, 3, 13, 23, 33, 43, 53)
dbg all> p y(4,5)
a{0..3}: 54
dbg all> 
```

(*note that team_vector_loop_solution.cpp has been modified to set each element to `i%10 + 10 * (j%10)`; in the original program it would have been all zeros at this point*)

## Decompositions

gdb4hpc also lets you treat the result on each rank as a piece of a single much larger array.   Again, this applies to any array; see the tutorial for other examples.   

So here we'll just show a simple example that combines this value on two ranks into a single 2048x255 array:

```
dbg all> decomposition $d
> dimension 2048,255
> distribute block,*
> proc_grid 2,*
> end
dbg all> p $d{y(0..1023,0..255)}(0..2047,16)
(60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 60, 61, 62, 63, ..., 60, 61, 62)
dbg all>
```

In this case, the "..." in the output is for the purpose of this document, gdb4hpc would print 2048 values.

If you're not familiar with gdb4hpc decomposition, the from ```$d{<expression}``` applies the decomposition to whatever is in the expression.   

The special note for this release, to get at the contents of the view, as opposed to the just the header, you need to use the full range expression ```y(0..1023,0..255)```.    The same requirement applies for the comparative debugging operations ```compare``` or ```assert```.   ```$d{y}``` is expected to be available in the future. 

## Sending the results to a file

To write the contents of a kokkos::View to a file use this:

```dbg all> pipe p y(0..1023,0..255) | cat > myfile.txt```

If you guessed that the ```pipe``` command can be applied to *any* gdb4hpc command and that it can be sent to *any* shell command ("grep", "count", "sort", etc.) you'll begin to appreciate the possibilities.   See the tutorials for more examples.

## Analyzing with python tools

gdb4hpc 4.15.0 has the first installment of python support in gdb4hpc.   A quick example should show off the usage:

```
dbg all> python
Entering python interpreter, use exit to leave.
> val = gdb.e('y(0..5,11)')
> print(val)
{'a{0}': (10.0, 11.0, 12.0, 13.0, 14.0, 15.0)}
> val2 = gdb.e('y(0..3,11..12)')
> print(val2)
{'a{0}': ((10.0, 11.0, 12.0, 13.0), (20.0, 21.0, 22.0, 23.0))}
> exit

Exiting python interpreter.
dbg all> 
```

Which shows off the key capability.   You get a python3 interpreter with the preloaded module "gdb" with the lone command "e" (for eval) that evaluates a program expression and turns it into a python dictionary mapping each procset to its result value.

Importing numpy to analyze the data will be covered in a separate tutorial, but for now will be "left as an exercise for the reader".
