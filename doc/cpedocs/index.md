<!--
This file is included in-place in the CPEDocs debugging tools top level index.

Since it is part of a larger file, the highest level header level should be 2 (##).

Links are relative to the output of collect_cpedocs.sh. See its implementation for details.
-->
## gdb4hpc

gdb4hpc is a GDB-based parallel debugger used to debug applications compiled
with CCE, PGI, GNU and Intel Fortran, C and C++ compilers.

### Guides

#### [Getting Started Guide](./guides/getting-started.md)

The getting started guide covers the following topics and more:
* The help system
* Debugging basics
* Procsets
* Focusing on a subset of an application's ranks

[Getting Started Guide](./guides/getting-started.md)

#### [Tutorial](./guides/tutorial.md)

The tutorial covers the following topics and more:
* Comparative debugging
* Assertion scripts
* Decompositions
* Shell commands and output piping
* Array slicing

[Tutorial](./guides/tutorial.md)

#### [Handling Arrays](./guides/handling-arrays.md)

Handling arrays covers Gdb4hpc's enhancements on Gdb's array handling tools.

[Handling Arrays](./guides/handling-arrays.md)

#### Parallel Programming Library Support

Gdb4hpc has extra support for some popular parallel programming libraries.

* [Kokkos Support](./guides/kokkos-support.md)
* [RAJA Support](./guides/raja-support.md)

#### [VSCode Extension Guide](./guides/vscode-plugin.md)

A guide covering Gdb4hpc's vscode extension.

[VSCode Extension Guide](./guides/tutorial.md)

### Python Debugging

[Using the gdb Python extension](./guides/python-extension.md)

### Gdb4hpc man Pages and Reference Material

* [gdb4hpc man page](./man/gdb4hpc.1.md)
* [gdb4hpc Command Reference](./man/help.md)
