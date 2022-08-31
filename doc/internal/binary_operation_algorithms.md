# `gdb4hpc` Evaluating Binary Operations in Expressions

> Written on 8/29/2022 by Josh Chandler

The strategies and algorithms in this document are implemented in `/src/gram/expr.cpp`.
The relevant code starts at the `apply_binary_op` function.

## Problem Description

In certain commands in `gdb4hpc`, the user can enter expressions that will be
evaluated based on the state of the program being debugged.

```
> print var_a + var_b
$a{0}: <some answer>
```

`gdb4hpc` can debug multiple programs at a time, meaning the name of a variable
in an expression can refer to memory on multiple machines, and each of those
pieces of memory can have a different value.

```
> print $a{0..99}::var_a
$a{0}: 0
$a{1}: 2
...
$a{99}: 198
```

This affects how expressions are evaluated. Because of the possibility of
multiple values mapping to a single piece of the expression, `gdb4hpc` uses a
few different strategies of mapping the values on the left side of the operator
to the right side for evaluation.

## Input

`apply_binary_op` is called, and it fetches the data to be operated on from the
application ranks. The data does not come back as individual packets from the
ranks. It is aggregated in the network tree so each unique value will only be
represented once, tagged with a bitset representing which ranks that particular
value came from.

For example, if the variable `var_a` has a value of `4` on ranks 0 - 4, `5` on
ranks 5 - 7, and `6` on ranks 8 - 9, we would get back something like this:

```
var_a:
({0..4}, 4), ({5..7}, 5), ({8..9}, 6)
```

And `var_b` might look like this:

```
var_b:
({0, 3}, 8), ({1..2}, -1), ({4..8}, 9), ({9}, 10)
```

The problem we are trying to solve is how to effeciently map these values to
each other for evaluation in a way that makes sense to the user and then evaluate
the expression efficiently.

For example, using the data above, the command `compare var_a == var_b` would
be effectively evaluating all of these:

```
$a{0}: 4 == 8
$a{1}: 4 == -1
$a{2}: 4 == -1
$a{3}: 4 == 8
$a{4}: 4 == 9
$a{5}: 5 == 9
$a{6}: 5 == 9
$a{7}: 5 == 9
$a{8}: 6 == 9
$a{9}: 6 == 10
```

## Strategies

Depending on the input data, we choose from three mapping strategies or return
an error if none fit the input data.

### Single

1 value vs. 1 value.

This is the simplest strategy. It is used in the simple case where each
side of the operation only has one value among all of the ranks making up
the side.

```
compare $a{0}::foo == $a{1}::foo
```

Note that this strategy can be used with any amount of ranks on each side,
as long as each rank holds the same value, meaning the aggregated result
will only be one element long.

Given that `foo` is the same on each rank:

```
compare $a{0..50000}::foo == $a{50001..70000}::foo

=> foo == foo
```

A single evaluation.

### Distributed

1 value vs. N values.

A slightly more complex strategy is used when one side of the operation
reports multiple values, but the other side only reports one.

Given that `foo` is different on each rank:

```
compare $a{0}::foo == $a{1..3}::foo

=> $a{0}::foo == $a{1}::foo
&& $a{0}::foo == $a{2}::foo
&& $a{0}::foo == $a{3}::foo
```

The same "single value" rule as in the Single strategy applies.

Given that `foo` is the same on each rank in `$a`, and different on each rank in `$b`:

```
compare $a{0..99}::foo == $b{1..3}::foo

=> $a{0..99}::foo == $b{1}::foo
&& $a{0..99}::foo == $b{2}::foo
&& $a{0..99}::foo == $b{3}::foo
```

### Zipped

N ranks vs. N ranks.

Note that this strategy cares about the number of *ranks*, not the number of
*values*. As long and the number of ranks are the same, it works on any
number of values. It works in the previous two cases, but is much more
complex/expensive, so it is only used as a last resort.

This strategy allows evaluation of expressions like:

```
compare $a{0, 2..3, 5, 7}::foo == $b{1, 4, 6, 8..9}::foo
```

The strategy "zips" the ranks up for evaluation:

```
=> $a{0}::foo == $b{1}::foo
&& $a{2}::foo == $b{4}::foo
&& $a{3}::foo == $b{6}::foo
&& $a{5}::foo == $b{8}::foo
&& $a{7}::foo == &b{9}::foo
```

#### Ordering the Input

Remembering the format of our input:

```
$a: ({0, 5}, x), ({2..3}, y), ({7}, z)
$b: ({1, 6}, x), ({4, 9}, y), ({8}, z)
```

Note that the ranks need not overlap. The complexity of this strategy comes from
creating a mapping of the nth rank in the left set to the nth rank in the right
set and still evaluating as little expressions as possible.

Given the expressions:

```
4 == 8
4 == 9
5 == 9
5 == 9
5 == 9
```

Note how we only need to generate 3 answers. The answers to `4 == 8`, `4 == 9` and `5 == 9`.
Comparison logic, especially for large arrays, is expensive, so we want to keep it to a minimum.

#### Priority Queue

For now, assume that the two sides of the expression have data that *does* come from
the same ranks. This does not mean the values are the same, but for now we can
assume that the bitsets on each side are compatible for bitset operations.

For example:

```
compare $a{0..2}::foo == $b{0..2}::bar

$a           $b
$a{0..1}: 5  $b{0, 2}: 7
$a{2}   : 7  $b{1}   : 5
```

Since we know the two halves' ranks match, we know that rank 0 of `$a` will be
compared with rank 0 of `$b`, etc. Our task is to find the sets containing the ranks
we need.

Sorting the ranks is a good start, but it won't match everything for us. The data
in the previous example is already sorted by lowest bit on each side, and the only
overlap on any side-by-side set is `{0}`, on the first set in each list.

In fact, `{0}` matching is guaranteed, and it's the only match guaranteed by sorting.

We can use this fact to come up with a starting algorithm:

```
0. While there is still data in the lists:
    1. Sort each side by lowest rank in set
        - O(nlogn)
    2. First set in each list will match. Evaluate, record result
    3. Remove lowest rank from each set, removing it from the list if empty
        - O(n) when removing from list
```

The while loop is repeated once for each of n pairs. This algorithm is O(n^2 logn).

It's easy to improve those big O times by swapping out the list for a priority
queue where the priority of each element is the value of the lowest set bit:

```
0. Put data into priority queues
    - O(nlogn)
1. While there is still data in the queues:
    2. Evaluate match on top of queue, record result
    3. Remove lowest rank from each set,
       removing it from the queue if empty, re-inserting it if not
        - O(logn)
```

Again, the while loop is repeated once for each of n pairs. This algorithm is O(nlogn).

Bringing back the idea from before of how pairs with identical values only need
to be evaluated once each, we can make one more improvement:

```
0. Put data into priority queues
    - O(nlogn)
1. While there is still data in the queues:
    2. Evaluate match on top of queue,
       record result **for all ranks in the pair that happen to match, not only the first**
    3. Remove matched ranks from each set,
       removing the set from the queue if empty, re-inserting it if not
        - O(logn)
```

Now the number of iterations in the loop is not the number of pairs, but the
number of unqiue pairs. Worst case, it's still O(nlogn), as each rank could report
a unique result, but average case is better.

#### Mapping

The above algorithm requires that the bitsets from each queue "line up", so that
bitset operations on them are meaningful.

If our bitsets don't come from the same ranks, they won't always match in the algorithm above.

We solve this by doing our matching with "mapped bitsets".

Remember that we are not interested in the absolute rank when we want to match
pairs. We are interested in matching the nth piece of data on the left with the
nth piece of data on the right.

Before we run the algorithm above, we create a "rank map" for the ranks involved
in each side of the evaluation, and then do our bitset matching with condensed
bitsets that will always match up with each other. A rank map is just a list
where the data at index i is the ith set bit in the corresponding bitset.

Given the following:

```
compare $a{0,4,8}::foo == $b{1..3}::bar

$a          $b
$a{2,4}: 5  $b{1, 3}: 7
$a{8}  : 7  $b{2}   : 5
```

Our desired pairs are:

```
$a{2} == $b{1}
$a{4} == $b{2}
$a{8} == $b{3}
```

Creating a rank map for `$a` yields `[2, 4, 8]`. Now when we need to know the
ith set bit in `$a`, we just look up `rank_map[i]`. We use the rank map to order
the priority queue, and then we can use the above algorithm as normal.

```
$a                            $b
$a{2,4}, mapped to {0..1}: 5  $b{1, 3}, mapped to {0, 2}: 7
$a{8}  , mapped to {2}   : 7  $b{2}   , mapped to {1}   : 5
```

Reduces to:

```
$a           $b
$a{0..1}: 5  $b{0, 2}: 7
$a{2}   : 7  $b{1}   : 5
```

from before.
