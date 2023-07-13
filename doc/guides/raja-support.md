---
title: "RAJA Support in Gdb4hpc"
---

Gdb4hpc includes support for debugging a parallel program that uses the `RAJA` Performance
Portability Layer (https://raja.readthedocs.io/en/develop/).

## Printing `RAJA::View`s

Starting in Gdb4hpc 4.15.0, Gdb4hpc supports printing values wrapped in `RAJA::View`s.

Since data contained in views tends to be large, Gdb4hpc will not print the entire
contents of a `RAJA::View` by default. Instead, it will print the bounds of the views and
instructions on how to access the view's data with Gdb4hpc's array range `..` syntax.

```
# a 2D RAJA::View
dbg all> whatis Aview
a{0}: RAJA::View<double, RAJA::detail::LayoutBase_impl<camp::int_seq<long, 0l, 1l>, int, -1l> >

dbg all> p Aview
a{0}: RAJA::View; Use (0..3, 0..3) for full contents.

dbg all> p Aview(0..3, 0..3)
a{0}: {{1,1,1,1},{2,2,2,2},{3,3,3,3},{4,4,4,4}}
```

Like other array-like objects, the `..` range syntax can be used to view a subset of the data:

```
dbg all> p Aview(2..3, 0..3)
a{0}: {{3,3,3,3},{4,4,4,4}}

dbg all> p Aview(0..3, 2..3)
a{0}: {{1,1},{2,2},{3,3},{4,4}}
```

Ranges are inclusive.

*Unlike* other array-like objects, views are indexed with parenthesis `()` instead of
brackets `[]`. This more closely matches how `RAJA::View`s work in actual source code.

### Supported Variations

Gdb4hpc supports printing the following types of `RAJA::View`s.

#### Normal Views

Gdb4hpc supports printing normal views, created without any special permutation or offset syntax.

##### Example

```cpp
// in a .cpp file
RAJA::View< int, RAJA::Layout<2, int> > view_2D(a, Nx, Ny);
```

```
# in gdb4hpc

dbg all> whatis Aview
a{0}: RAJA::View<double, RAJA::detail::LayoutBase_impl<camp::int_seq<long, 0l, 1l>, int, -1l> >

dbg all> p Aview
a{0}: RAJA::View; Use (0..3, 0..3) for full contents.

dbg all> p Aview(0..3, 0..3)
a{0}: {{1,1,1,1},{2,2,2,2},{3,3,3,3},{4,4,4,4}}
```

#### Permuted Views

Gdb4hpc supports printing permuted views.

##### Example

```cpp
// in a .cpp file
std::array<RAJA::idx_t, 3> perm3a {{2, 1, 0}};
RAJA::Layout< 3, int > perm3a_layout =
  RAJA::make_permuted_layout( {{Nx, Ny, Nz}}, perm3a);
RAJA::View< int, RAJA::Layout<3, int> > perm3a_view_3D(a, perm3a_layout);
```

```
# in gdb4hpc

dbg all> whatis perm3a_view_3D
a{0}: RAJA::View<int, RAJA::detail::LayoutBase_impl<camp::int_seq<long, 0l, 1l, 2l>, int, -1l> >

dbg all> p perm3a_view_3D
a{0}: RAJA::View; Use (0..2, 0..4, 0..1) for full contents.

dbg all> p perm3a_view_3D(0..2, 0..4, 0..1)
a{0}: {{{0,15},{3,18},{6,21},{9,24},{12,27}},{{1,16},{4,19},{7,22},{10,25},{13,28}},{{2,17},{5,20},{8,23},{11,26},{14,29}}}
```

#### Offset Views

Gdb4hpc supports printing offset views.

##### Example

```cpp
// in a .cpp file
RAJA::OffsetLayout<2, int> offlayout_2D =
  RAJA::make_offset_layout<2, int>( {{-1, -5}}, {{2, 5}} );

RAJA::View< int, RAJA::OffsetLayout<2, int> > aoview_2Doff(ao,
                                                           offlayout_2D);
```

```
# in gdb4hpc

dbg all> whatis aoview_2Doff
a{0}: RAJA::View<int, RAJA::OffsetLayout<2ul, int> >

dbg all> p aoview_2Doff
a{0}: RAJA::View; Use (-1..1, -5..4) for full contents.

dbg all> p aoview_2Doff(-1..1, -5..4)
a{0}: {{0,1,2,3,4,5,6,7,8,9},{10,11,12,13,14,15,16,17,18,19},{20,21,22,23,24,25,26,27,28,29}}
```

## Using `RAJA::View`s in Decompositions

Gdb4hpc has a decomposition feature which allows the user to logically combine and divide
data that is in reality distributed across multiple ranks. Gdb4hpc supports using
`RAJA::View`s with decompositions.

### Example

For the following example, suppose you have a 4-rank application. Each rank has a one
dimensional `RAJA::View` 60 elements long. The name of the view is `view_1D`. Rank 1
stores the numbers `{0, 1, 2, ..., 59}`, rank 2 stores `{0, 10, 20, ..., 590}`, rank 3
`{0, 100, ...}`, etc.

We can use the Gdb4hpc `decomposition` command to concatenate each array into a 240
element long logical array:

```
# in gdb4hpc

# (printout abbreviated)
dbg all> p view_1D(0..59)
a{0}: {0,1,2,3,4,5,etc...}
a{1}: {0,10,20,30,40,50,etc...}
a{2}: {0,100,200,300,400,500,etc...}
a{3}: {0,1000,2000,3000,4000,5000,etc...}

# create a decomposition called "concat" that is 240 elements long, split across 4 ranks
dbg all> decomposition $concat 240/4

# apply the decomposition to view_1D. note that the (0..59) suffix is no longer required
# (printout abbreviated)
dbg all> p $concat{view_1D}
{0,1,2,3,4,5,6, ... ,57,58,59,0,10,20,30,40,50,60, ... ,570,580,590,0,100,200,300,400,500,600, ... ,5700,5800,5900,0,1000,2000,3000,4000,5000,6000, ... ,57000,58000,59000}
```

Decompositions can used to handle data in more ways than what was shown here, and
`RAJA::View`s are supported in all of them. See the Tutorial for more details on using
decompositions.

## Printing RAJA Reductions

RAJA reductions are used to reduce large vectors into single values. Common operations are
min, max, and sum.

Gdb4hpc supports printing RAJA reductions.

### Example

```cpp
// in a .cpp file

RAJA::ReduceSum<REDUCE_POL1, int> seq_sum(0);
RAJA::ReduceMin<REDUCE_POL1, int> seq_min(std::numeric_limits<int>::max());
RAJA::ReduceMax<REDUCE_POL1, int> seq_max(std::numeric_limits<int>::min());
RAJA::ReduceMinLoc<REDUCE_POL1, int> seq_minloc(std::numeric_limits<int>::max(), -1);
RAJA::ReduceMaxLoc<REDUCE_POL1, int> seq_maxloc(std::numeric_limits<int>::min(), -1);

RAJA::forall<EXEC_POL1>(arange, [=](int i) {
  seq_sum += a[i];

  seq_min.min(a[i]);
  seq_max.max(a[i]);

  seq_minloc.minloc(a[i], i);
  seq_maxloc.maxloc(a[i], i);
});

std::cout << "\tsum = " << seq_sum.get() << std::endl;
std::cout << "\tmin = " << seq_min.get() << std::endl;
std::cout << "\tmax = " << seq_max.get() << std::endl;
std::cout << "\tmin, loc = " << seq_minloc.get() << " , "
                             << seq_minloc.getLoc() << std::endl;
std::cout << "\tmax, loc = " << seq_maxloc.get() << " , "
                             << seq_maxloc.getLoc() << std::endl;
```

```
# in gdb4hpc

dbg all> p seq_sum
a{0}: {RAJA::reduce::detail::BaseReduceSum<int, RAJA::detail::ReduceSeq> = {RAJA::reduce::detail::BaseReduce<int, RAJA::reduce::sum, RAJA::detail::ReduceSeq> = {c = {RAJA::reduce::detail::BaseCombinable<int, RAJA::reduce::sum<int>, RAJA::detail::ReduceSeq<int, RAJA::reduce::sum<int> > > = {parent = (RAJA::reduce::detail::BaseCombinable<int, RAJA::reduce::sum<int>, RAJA::detail::ReduceSeq<int, RAJA::reduce::sum<int> > >*) 0x0, identity = 0, my_data = 1}}}}}

dbg all> p seq_sum.get()
a{0}: 1

dbg all> p seq_min
a{0}: {RAJA::reduce::detail::BaseReduceMin<int, RAJA::detail::ReduceSeq> = {RAJA::reduce::detail::BaseReduce<int, RAJA::reduce::min, RAJA::detail::ReduceSeq> = {c = {RAJA::reduce::detail::BaseCombinable<int, RAJA::reduce::min<int>, RAJA::detail::ReduceSeq<int, RAJA::reduce::min<int> > > = {parent = (RAJA::reduce::detail::BaseCombinable<int, RAJA::reduce::min<int>, RAJA::detail::ReduceSeq<int, RAJA::reduce::min<int> > >*) 0x0, identity = 2147483647, my_data = -100}}}}}

dbg all> p seq_min.get()
a{0}: -100

dbg all> p seq_minloc
a{0}: {RAJA::reduce::detail::BaseReduceMinLoc<int, long, RAJA::detail::ReduceSeq> = {RAJA::reduce::detail::BaseReduce<RAJA::reduce::detail::ValueLoc<int, long, true>, RAJA::reduce::min, RAJA::detail::ReduceSeq> = {c = {RAJA::reduce::detail::BaseCombinable<RAJA::reduce::detail::ValueLoc<int, long, true>, RAJA::reduce::min<RAJA::reduce::detail::ValueLoc<int, long, true> >, RAJA::detail::ReduceSeq<RAJA::reduce::detail::ValueLoc<int, long, true>, RAJA::reduce::min<RAJA::reduce::detail::ValueLoc<int, long, true> > > > = {parent = (RAJA::reduce::detail::BaseCombinable<RAJA::reduce::detail::ValueLoc<int, long, true>, RAJA::reduce::min<RAJA::reduce::detail::ValueLoc<int, long, true> >, RAJA::detail::ReduceSeq<RAJA::reduce::detail::ValueLoc<int, long, true>, RAJA::reduce::min<RAJA::reduce::detail::ValueLoc<int, long, true> > > >*) 0x0, identity = {val = 2147483647, loc = -1}, my_data = {val = -100, loc = 500000}}}}}}

dbg all> p seq_minloc.get()
a{0}: {val = -100, loc = 500000}

dbg all> p seq_minloc.getLoc()
a{0}: 500000

dbg all> p a[seq_minloc.getLoc()]
a{0}: -100
```

## Printing `RAJA::LocalArray`s

`RAJA::LocalArray`s are used to to store data in CPU stack-allocated or GPU thread local
memory. Most often, they are used for tiling operations.

Gdb4hpc supports printing `RAJA::LocalArray` objects.

`RAJA::LocalArray`s are implemented as `RAJA::View` objects, so working with them in
Gdb4hpc is the same as working with `RAJA::View`s.

### Example

See the RAJA Tiled Matrix Transpose with Local Array example for more details.

https://raja.readthedocs.io/en/develop/sphinx/user_guide/tutorial/matrix_transpose_local_array.html

```cpp
// in a .cpp file

using TILE_MEM =
  RAJA::LocalArray<int, RAJA::Perm<0, 1>, RAJA::SizeList<TILE_DIM, TILE_DIM>>;
TILE_MEM Tile_Array;

using SEQ_EXEC_POL_I =
  RAJA::KernelPolicy<
    RAJA::statement::Tile<1, RAJA::tile_fixed<TILE_DIM>, RAJA::loop_exec,
      RAJA::statement::Tile<0, RAJA::tile_fixed<TILE_DIM>, RAJA::loop_exec,

        RAJA::statement::InitLocalMem<RAJA::cpu_tile_mem, RAJA::ParamList<2>,

        RAJA::statement::ForICount<1, RAJA::statement::Param<0>, RAJA::loop_exec,
          RAJA::statement::ForICount<0, RAJA::statement::Param<1>, RAJA::loop_exec,
            RAJA::statement::Lambda<0>
          >
        >,

        RAJA::statement::ForICount<0, RAJA::statement::Param<1>, RAJA::loop_exec,
          RAJA::statement::ForICount<1, RAJA::statement::Param<0>, RAJA::loop_exec,
            RAJA::statement::Lambda<1>
          >
        >

        >
      >
    >
  >;

RAJA::kernel_param<SEQ_EXEC_POL_I>(
  RAJA::make_tuple(RAJA::TypedRangeSegment<int>(0, N_c),
                   RAJA::TypedRangeSegment<int>(0, N_r)),

  RAJA::make_tuple((int)0, (int)0, Tile_Array),

  [=](int col, int row, int tx, int ty, TILE_MEM &Tile_Array) {
    Tile_Array(ty, tx) = Aview(row, col);
  },

  [=](int col, int row, int tx, int ty, TILE_MEM &Tile_Array) {
    Atview(col, row) = Tile_Array(ty, tx);
  }
);
```

```
# in gdb4hpc

dbg all> p Tile_Array
a{0}: RAJA::View; Use (0..15, 0..15) for full contents.

dbg all> p Tile_Array(1, 0..15)
a{0}: {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}

dbg all> p Tile_Array(0..15, 0..15)
a{0}: {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},{3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},{4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4},{5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5},{6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},{7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7},{8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8},{9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9},{10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10},{11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11},{12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12},{13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13},{14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14},{15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15}}
```

## More Tools

Gdb4hpc treats `RAJA` views in the same way that it treats arrays. This means that
anything described in "Handling Arrays" applies to `RAJA::View`s too.

For example, you can dump a large `RAJA::View` to a file like this:

```
dbg all> pipe p big_raja_view | cat > big_raja_view.txt
dbg all> shell wc -c big_raja_view.txt
988 myfile.txt
```

See "Handling Arrays" for more.
