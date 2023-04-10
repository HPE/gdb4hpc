---
title: Tools for working with array values in gdb4hpc
---

This covers tools and techniques for working with arrays, matrices, and C++ collections.   

## The range operator

Gdb4hpc recognizes the ".." operator in many contexts to indicate a range of indices.

~~~~~
dbg all> p localArray1[0..9]
a{0}: {0,2,4,6,8,10,12,14,16,18}
dbg all>
~~~~~

Or for a couple of examples with 2d C arrays:

~~~~
dbg all> whatis array2d
a{0}: int [40][20]
dbg all> p array2d[4]
a{0}: {8,7,6,5,4,3,2,1,0,-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-11}
dbg all> p array2d[1..2]
a{0}: {{2,1,0,-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-11,-12,-13,-14,-15,-16,-17},{4,3,2,1,0,-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-11,-12,-13,-14,-15}}
dbg all> p array2d[1..2][6]
a{0}: {-4,-2}
dbg all>
~~~~

It works as well for ```std::vector```, ```std::array``` and other containers.   Of course since C/C++ doesn't track the size of an allocated array, you'll have to infer the limits from other program variables.  So possibly:

~~~~
dbg all> p localArray1[0..localSize-1]
a{0}: {0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,52,54,56,58,60,62,64,66,68,70,72,74,76,78,80,82,84,86,88,90,92,94,96,98,100,102,104,106,108,110,112,114,116,118,120,122,124,126,128,130,132,134,136,138,140,142,144,146,148,150,152,154,156,158,160,162,164,166,168,170,172,174,176,178,180,182,184,186,188,190,192,194,196,198,200,202,204,206,208,210,212,214,216,218,220,222,224,226,228,230,232,234,236,238}
dbg all> 
~~~~

The range operator also works for fortran arrays, though it's not quite as flexible as the ":"; it requires a begin and end index.

## The @ operator

gdb4hpc recognizes ```@``` as a repeat operator like gdb does.   

~~~~
dbg all> p array2d[4]@3
a{0}: {{8,7,6,5,4,3,2,1,0,-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-11},{10,9,8,7,6,5,4,3,2,1,0,-1,-2,-3,-4,-5,-6,-7,-8,-9},{12,11,10,9,8,7,6,5,4,3,2,1,0,-1,-2,-3,-4,-5,-6,-7}}
dbg all>
~~~~

## limiting the number of elements to print

By default gdb4hpc prints the entire contents of the array, no matter how long, which can lead to a lot of scrolling.   You can limit the number of elements printed with:

~~~~
dbg all> set print elements 5
dbg all> p array2d[0]
a{0}: {0,-1,-2,-3,-4...}
dbg all> p array2d[5]
a{0}: {10,9,8,7,6...}
dbg all> 
~~~~

To go back to printing the entire contents, use:

```set print elements unlimited```

## Sending output to file, method 1

gdb4hpc implements the "logging" feature like gdb does, and this affects all debugger output:

~~~~
dbg all> set logging file myfile.txt
dbg all> set logging redirect on
dbg all> set logging enabled on
~~~~

Will send all output to myfile.txt until you disable it with ```set logging enabled off```.

Use ```help set logging``` for details.

## Sending output to a file, method 2

This form lets you redirect the output to a file on demand without setting a mode:

~~~~
dbg all> pipe p array2d | cat > myfile.txt
dbg all> shell wc -c myfile.txt
2418 myfile.txt
dbg all>
~~~~

Which is also showing off the convenience ```shell``` command which lets you use shell commands from within gdb4hpc.    (```shell -r <command>``` lets you run commands on the compute nodes which can be very useful).

Of course we could have done this in one step:

~~~~
dbg all> pipe p array2d | tee myfile.txt | wc -c
2418
dbg all> 
~~~~

## Format specifiers

If you've used gdb, you might be familiar with the ```/x```,```/o``` and ```/t``` operators to print values in hex, octal or binary: as in ```print/x val``` or ```p/o val```.   These are supported.

Gdb4hpc extends these with two additional types to make redirecting more useful:

* ```/1``` print out values one per line (that's number one not lower case 'L')
* ```/csv``` prints out as comma separated values

~~~~
dbg all> p/csv array2d[0..5]
"a{0}",0,-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-11,-12,-13,-14,-15,-16,-17,-18,-19
"a{0}",2,1,0,-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-11,-12,-13,-14,-15,-16,-17
"a{0}",4,3,2,1,0,-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-11,-12,-13,-14,-15
"a{0}",6,5,4,3,2,1,0,-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-11,-12,-13
"a{0}",8,7,6,5,4,3,2,1,0,-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-11
"a{0}",10,9,8,7,6,5,4,3,2,1,0,-1,-2,-3,-4,-5,-6,-7,-8,-9
dbg all>
~~~~

The intention is to make it easier to read the data into some other application for further analysis.   The csv form doesn't extend past 2D very so well, so mileage may vary.

The ```/1``` option makes the data amenable to shell utilities, for some examples:

~~~~
dbg all> pipe p/1 array2d | sort -n | head -4
-19
-18
-17
-17
dbg all> pipe p/1 array2d | sort -n | tail -4
76
76
77
78
dbg all>
~~~~

To find the lowest and highest values in the array.   Or perhaps:

~~~~
dbg all> pipe p/1 array2d | grep -n 61 
622:61
644:61
666:61
688:61
710:61
732:61
754:61
776:61
798:61
dbg all> 
~~~~

To find every occurrence of a value (remember to subtract 1 to get the index).

## processing with python

gdb4hpc 4.15.0 includes the first support for accessing python tools from within gdb4hpc directly.   This example shows a start of the possibilities:

~~~~
dbg all> p floatarray
a{0}: {1.1,2.2,3.3,4.4,5.5}
dbg all> python
Entering python interpreter, use exit to leave.
> x = gdb.e('floatarray')
> print(x)
{'a{0}': (1.100000023841858, 2.200000047683716, 3.299999952316284, 4.400000095367432, 5.5)}
> y = x['a{0}']
> print(y)
(1.100000023841858, 2.200000047683716, 3.299999952316284, 4.400000095367432, 5.5)
> import numpy as np
> ny = np.array(y)
> print(ny)
[1.10000002 2.20000005 3.29999995 4.4000001  5.5       ]
> print(np.average(ny))
3.300000023841858
> exit
~~~~

While we expect to refine this in the next releases, it supports the key functionality:

* The ```python``` commands drops you into a python3 interpreter
* The python instance is preloaded with "import gdb4hpc as gdb"
* It supports the evaluator function 'e(program-expression)' which evaluates a program expression and returns its python representation.

The program value may not have the same value on every rank, so the result is a dictionary mapping a procset string to its value.  For example:

~~~~
dbg all> python
Entering python interpreter, use exit to leave.
> rank = gdb.e('rank')
> print(rank)
{'a{0}': 0, 'a{1}': 1, 'a{2}': 2, 'a{3}': 3}
> 
~~~~

