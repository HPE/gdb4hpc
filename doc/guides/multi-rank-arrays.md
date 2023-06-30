---
title: "Decompositions : treating distributed data as a single array"
---

# Motivation

Commonly in multi-process programming a single conceptual array is
parceled out so that each process stores a portion of the array.   In
a simple case a 100K element array is divided across 100 ranks and
rank 0 has elements 0 to 999, rank 1 has elements 1000 to 1999, etc.

The **decomposition** tool in gdb4hpc lets you define a named mapping
so you treat it like a single giant array.

Decompositions are a flexible tool that lets you:

* Work with data tiles across multiple ranks

* Transform data into an array or table with the rank as one dimension

* Define the shape and size of a C/C++ array

# Tiled data

## One D block decomposition

So for the first case, say you are running on 100 ranks that each work
on 1,000 elements.  You can define a decomposition like this:

~~~~~
dbg all> decomp $d 100000/100
dbg all>
~~~~~

Here "$d" is the name of decomposition, "100000" is the dimension of
the full array, and "/100" indicates it's using a **block
decomposition** over 100 ranks.

If the array of interest is ```my_array``` then you get the composed
view by using it like a function:

~~~~
dbg all> p $d{my_array}
... one really long array...
~~~~

Note that gdb4hpc use curly brackets instead of parentheses to avoid
clashing with program expressions.

For the sake of illustration, we'll use something a lot smaller:

~~~~
dbg all> p block
a{0}: {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39}
a{1}: {40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79}
a{2}: {80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119}
dbg all> decomp $d3 120/3
dbg all> p $d3{block}
{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119}
dbg all>
~~~~

And you can use the expression ```$d3{block}``` like a regular array,
so:

~~~~~
dbg all> p $d3{block}[81]
81
dbg all> p $d3{block}[38..47]
{38,39,40,41,42,43,44,45,46,47}
dbg all> pipe p/1 $d3{$a::block} | grep -n '^107$'
108:107
~~~~~

If we launch the same application running on four ranks, and stop
at the same place:

~~~~~
dbg all> decomp $d4 120/4
dbg all> compare $d3{$a::block} == $d4{$b::block}
Comparison is true.
~~~~~

Which was one of the original use cases.   See the "handling-arrays"
guide for more tools for working with array data.

## Types of decompositions

gdb4hpc doesn't yet try to handle every possible way a program might
divvy up its data, but it does try to handle some common cases.

### Block decomposition

Indicated by ```size / num_ranks``` the array is broken into equal
sized blocks.   Mnemonically, ```rank(idx) = idx /
elems_per_rank```.

| rank 0 | rank 1 | rank 2 |
|--------|--------|--------|
| [0,1,2,3]|[4,5,6,7] |[8,9,10,11] |

Table: **decomp $d 12 / 3**


### Cyclic decomposition

In cyclic decomposition, the elements are assigned round-robin, so
array[0] is on rank 0, array[1] on rank 1, etc. until array[num_ranks]
is again on rank 0.

This is indicated by the format ```size % num_ranks```, with the
mnemonic that ```rank(idx) = idx % num_ranks```.

| rank 0 | rank 1 | rank 2 |
|--------|--------|--------|
| [0,3,6,9]|[1,4,7,10]|[2,5,8,11]|

Table: **decomp $d 12 % 3**


### Block cyclic decomposition

In this case, the data parceled out round-robin, but in chunks of a
fixed size.   It's indicated by the form: ```size / num_ranks %
block_size```.

Here's an example: ```decomp $bc 120/6%5```   It cycles
over the 6 ranks using chunks of 5 elements each.   So rank 0 has the
elements (0,1,2,3,4,30,31,32,33,34,60,61,62,63,64,90,91,92,93,94).

So ```rank(idx) = (idx / block_size) % num_ranks```.

| rank 0 | rank 1 | rank 2 |
|--------|--------|--------|
| [0,1,6,7]|[2,3,8,9]|[4,5,10,11]|

Table: **decomp $d 12 / 3 % 2**

### Distributed

This is just an array that is not split up, and is just represented by a
simple integer.   To anticipate the next sections, it's used for
multi-dimension data and for artificially shaping data.

## Multi-dimension arrays

The decomposition is not limited to a single dimension, a
decomposition can cover a full n-dimensional array where each
dimension is decomposed independently.

Syntactically, this just means the decomposition definition is a comma
separated list:

~~~~
decomp $d2 1000/10, 1000
~~~~

Defines a 1000x1000 array which is broken into 10 100 x 1000 arrays.

Here's the arrangement of data in a much smaller example:

Table: **decomp $d 6/3, 2**

| rank 0                        | rank 1                        | rank 2                        |
| :---------------------------: | :---------------------------: | :---------------------------: |
| {[0][0],[0][1],[1][0],[1][1]} | {[2][0],[2][1],[3][0],[3][1]} | {[4][0],[4][1],[5][0],[5][1]} |

Which should be read that element [2][1] of the full array comes from
the second element of rank 1; or in gdb4hpc notation:
```$d{$a::x}[2][1] == $a{1}::x[1]```.   You might note that this is
showing the entries on each rank as a 1D array.   If we're working with
doubles, the data can be stored as ```double*, double[4], double[2][2],
std:vector<double>, std::array<double,4>, Kokkos::View``` and other
variants.

## Index ordering

Decompositions are defined assuming the natural ordering of the data,
so the interpretation depends on its context.   In more concrete
terms:

* the *rightmost* index is the fastest varying for C/C++ arrays
* the *leftmost* index is the fastest varying for fortran arrays

In each case, gdb4hpc is *trying* to make it intuitive for the user.


## Matrices decomposed in multiple directions

A multi-d array may be tiled in more than one direction.   For example,
if you are computing using 120 ranks, you might give each rank a
100x100 array to work with:

~~~~
decomp $tile 1200/12, 1000%10
~~~~

Which gives you a 12x10 array of *processes*.   Which begs the
question if the layout is row major or column major.   The default is
is use the same ordering as the language.   So if this is
C/C++, rank 0 has the elements [0..99][{0,10,20..,990}], and rank 1 has
the elements [0..99][{1,10,21,..,991}].

The ordering of the *process grid* can be overridden by adding an
explicit ```order()``` clause:

~~~~~
decomp $tile_T 1200/12 order(1), 1000%10 order(2)
~~~~~

Though to reduce strain to the brain, there no reason not to make it
explicit:

~~~~~
decomp $tile 1200/12 order(2), 1000%10 order(1)
~~~~~

As a concrete example, here is a 4 x 6 C style array tiled over a 2 x 3 grid of processes.

Table: **decomp $d 4/2 order(2), 6/3 order(1)**

| rank 0                        | rank 1                        | rank 2                        |
| :---------------------------: | :---------------------------: | :---------------------------: |
| {[0][0],[0][1],[1][0],[1][1]} | {[0][2],[0][3],[1][2],[1][3]} | {[0][4],[0][5],[1][4],[1][5]} |
| **rank 3**                    | **rank 4**                    | **rank 5**                    |
| {[2][0],[3][1],[2][0],[3][1]} | {[2][2],[3][3],[2][2],[3][3]} | {[2][4],[3][5],[2][4],[3][5]} |

Which means each rank contributes four elements; the table show the location of each element.

Or in fortran:

Table: **decomp $d 6/3 order(1), 4/2 order(2)**

| rank 0                        | rank 1                        | rank 2                        |
| :---------------------------: | :---------------------------: | :---------------------------: |
| ((1,1),(2,1),(1,2),(2,2))     | ((3,1),(4,1),(3,2),(4,2))     | ((5,1),(6,1),(5,2),(6,2))     |
| **rank 3**                    | **rank 4**                    | **rank 5**                    |
| ((1,3),(2,3),(1,4),(2,4))     | ((3,3),(4,3),(3,4),(4,4))     | ((5,3),(6,3),(5,4),(6,4))     |


## Natural ordering within each data block

Note that the "order" clause **only** affects the tiling order of the
ranks.  Within each rank, it always follows the natural layout of the
data.   You can think of the debugger iterating over the elements of
the combined array, and at each step it chooses only a rank and always
takes the next available element on that rank.

It does not support transposing an array or otherwise shuffling the
access - at least not yet.

# Treating multi-rank data as an array

## Adding the rank as an additional dimension of an array

The default behavior for printing values in gdb4hpc is to print
results by rank.

~~~~~
dbg all> p $a::block
$a{0}: {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39}
$a{1}: {40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79}
$a{2}: {80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119}
~~~~~

Which, when aggregating identical values, is typically
the most concise way to present the values.  In some contexts it
may be more convenient to handle it as a 2d array, say to output a
comma separated table for graphing.

A decomposition can turn an nD array into a (n+1)D array.

Using the same ```$a::block``` array, we can define:

~~~~~
dbg all> decomp $block1_3to2 3/3, 40
dbg all> p $block1_3to2{$a::block}
{{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39},{40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79},{80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119}}
dbg all>
~~~~~

Alternatively, you might want it grouped by index instead:

~~~~~
dbg all> decomp $block1_3to2T 40, 3/3
dbg all> p $block1_3to2T{$a::block}
{{0,40,80},{1,41,81},{2,42,82},{3,43,83},{4,44,84},{5,45,85},{6,46,86},{7,47,87},{8,48,88},{9,49,89},{10,50,90},{11,51,91},{12,52,92},{13,53,93},{14,54,94},{15,55,95},{16,56,96},{17,57,97},{18,58,98},{19,59,99},{20,60,100},{21,61,101},{22,62,102},{23,63,103},{24,64,104},{25,65,105},{26,66,106},{27,67,107},{28,68,108},{29,69,109},{30,70,110},{31,71,111},{32,72,112},{33,73,113},{34,74,114},{35,75,115},{36,76,116},{37,77,117},{38,78,118},{39,79,119}}
~~~~~

Remember the order is reversed when working with fortran:

~~~~~
dbg all> decomp $block1_3to2 40, 3/3
dbg all> p $block1_3to2{$a::block}
((0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39),(40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79),(80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119))
~~~~~

## Scalars

The same tool can be used to turn a scalar value into a 1d array
indexed by rank:

~~~~~
dbg all> p x
$a{0}: 1.0
$a{1,3}: -1.0
$a{2}: 2.0
dbg all> decomp $scalar 4/4
dbg all> p $scalar{x}
{1.0, -1.0, 2.0, -1.0}
dbg all> pipe p/csv $scalar{x} | cat > x.txt
~~~~~

# Using a decomposition to define a size and shape

The decomposition can also be used without tiling just to define the
shape of an array, which can be particularly useful when working with
allocated C/C++ arrays:

~~~~
dbg all> whatis localArray1
a{0..2}: int *
dbg all> p localArray1
a{0}: (int*) <1>
  <1>:0
a{1}: (int*) <1>
  <1>:160
a{2}: (int*) <1>
  <1>:320
dbg all> decomp $d2 10,4
dbg all> p $d2{localArray1}
a{0}: {{0,2,4,6},{8,10,12,14},{16,18,20,22},{24,26,28,30},{32,34,36,38},{40,42,44,46},{48,50,52,54},{56,58,60,62},{64,66,68,70},{72,74,76,78}}
a{1}: {{160,162,164,166},{168,170,172,174},{176,178,180,182},{184,186,188,190},{192,194,196,198},{200,202,204,206},{208,210,212,214},{216,218,220,222},{224,226,228,230},{232,234,236,238}}
a{2}: {{320,322,324,326},{328,330,332,334},{336,338,340,342},{344,346,348,350},{352,354,356,358},{360,362,364,366},{368,370,372,374},{376,378,380,382},{384,386,388,390},{392,394,396,398}}
~~~~~

Notice that since every dimension is distributed, the result is still
shown by rank.

Since ```localArray1``` is an ```int*``` the debugger has no way to know that it's an
array or what the length might be.   You can use the gdb "@" operator to
get the value: ```p localArray1@40```, but using a
decomposition allows you treat it as multi-dimension array.

Since the debugging information doesn't include any size information,
there is no bounds checking.   It will simply treat the pointer as an
array of the necessary length.

# Miscellaneous

The ```info decomp``` or ```i decomp``` command print the available
decompositions.   Or you can print a single decomposition using
```info decomp $name```.

If you have decompositions you use frequently, you can save them in a
text file and use the ```source``` command to define them in a new
session, or save it ~/.gdb4hpc/gdb4hpc_init

The actual name of the command is "decomposition", but gdb4hpc lets
you shorten it to anything unambiguous.

The single line decomposition form is new to version 4.15.1, older
versions used a multi-line form:

~~~~~
decomposition $block2_4
dimension 10,12
distribute *,block
proc_grid *,4
end
~~~~~

Which is equivalent to ```decomp $block2_4 10, 12/4```.   Use ```help
decomposition``` for details.
