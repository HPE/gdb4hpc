---
title: "RAJA Support in Gdb4hpc"
---

Gdb4hpc includes support for debugging a parallel program that uses the `RAJA` Performance Portability Layer (https://raja.readthedocs.io/en/develop/).

In addition to what is described in this document, more support for `RAJA` is currently in development.

## Printing `RAJA::View`s

Starting in Gdb4hpc 4.15.0, Gdb4hpc supports printing values wrapped in `RAJA::View`s.

Since data contained in views tends to be large, Gdb4hpc will not print the entire contents of a `RAJA::View` by default. Instead, it will print the bounds of the views and instructions on how to access the view's data with Gdb4hpc's array range `..` syntax.

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

## More Tools

Gdb4hpc treats `RAJA` views in the same way that it treats arrays. This means that
anything described in "Tools for working with array values in gdb4hpc" applies to
`RAJA::View`s too.

For example, you can dump a large `RAJA::View` to a file like this:

```
dbg all> pipe p big_raja_view | cat > big_raja_view.txt
dbg all> shell wc -c big_raja_view.txt
988 myfile.txt
```

See "Tools for working with array values in gdb4hpc" for more.
