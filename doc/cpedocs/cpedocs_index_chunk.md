<!--
This file is included in-place in the CPEDocs debugging tools top level index.

Since it is part of a larger file, the highest level header level should be 2 (##).
Assume that gdb4hpc content will live in a directory called ./cpedocs_gdb4hpc for linking.
The cpedocs_gdb4hpc directory is created by the collect_cpedocs.sh script.
-->
## gdb4hpc

gdb4hpc is a GDB-based parallel debugger used to debug applications compiled
with CCE, PGI, GNU and Intel Fortran, C and C++ compilers.

### Guides

#### [Getting Started Guide](./cpedocs_gdb4hpc/guides/getting-started.md)

The getting started guide covers the following topics and more:
* The help system
* Debugging basics
* Procsets
* Focusing on a subset of an application's ranks

[Getting Started Guide](./cpedocs_gdb4hpc/guides/getting-started.md)

#### [Tutorial](./cpedocs_gdb4hpc/guides/tutorial.md)

The tutorial covers the following topics and more:
* Comparative debugging
* Assertion scripts
* Decompositions
* Shell commands and output piping
* Array slicing

[Tutorial](./cpedocs_gdb4hpc/guides/tutorial.md)

#### [Handling Arrays](./cpedocs_gdb4hpc/guides/handling-arrays.md)

Handling arrays covers Gdb4hpc's enhancements on Gdb's array handling tools.

[Handling Arrays](./cpedocs_gdb4hpc/guides/handling-arrays.md)

#### Parallel Programming Library Support

Gdb4hpc has extra support for some popular parallel programming libraries.

* [Kokkos Support](./cpedocs_gdb4hpc/guides/kokkos-support.md)
* [RAJA Support](./cpedocs_gdb4hpc/guides/raja-support.md)

### Gdb4hpc man pages

* [gdb4hpc man page](./cpedocs_gdb4hpc/man/gdb4hpc.1.md)
