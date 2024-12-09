<!--
This file is included in-place in the CPEDocs debugging tools top level index.

Since it is part of a larger file, the highest level header level should be 2 (##).

Links are relative to the output of collect_cpedocs.sh. See its implementation for details.
-->
## gdb4hpc

gdb4hpc is a GDB-based parallel debugger used to debug applications compiled
with CCE, PGI, GNU and Intel Fortran, C and C++ compilers.

- [What's New in gdb4hpc 4.16.3](./whatsnew/new-in-041603.md)

### Guides and Tutorials

#### Getting Started Guide

The getting started guide covers the following topics and more:
* The help system
* Debugging basics
* Procsets
* Focusing on a subset of an application's ranks

[Getting Started Guide](./guides/getting-started.md)

#### HPC Features Tutorial

The tutorial covers the following unique HPC-centric features of gdb4hpc and more:
* Comparative debugging
* Assertion scripts
* Decompositions
* Shell commands and output piping
* Array slicing

[HPC Features Tutorial](./guides/tutorial.md)

#### Debugging an MPI/CUDA GPU Application Tutorial

This tutorial shows how to use gdb4hpc to debug a multinode MPI application
that uses CUDA compute kernels. The tutorial is written with CUDA/NVIDIA GPUs
in mind, but the concepts apply to HIP/AMD GPUs as well.

[Debugging an MPI/CUDA GPU Application Tutorial](./guides/cuda-debugging.md)

#### Handling Arrays

Handling arrays covers gdb4hpc's enhancements on Gdb's array handling tools.

[Handling Arrays](./guides/handling-arrays.md)

#### Parallel Programming Library Support

gdb4hpc has extra support for some popular parallel programming libraries.

* [Kokkos Support](./guides/kokkos-support.md)
* [RAJA Support](./guides/raja-support.md)

#### [VSCode Extension Guide](./guides/vscode-plugin.md)

A guide covering gdb4hpc's vscode extension.

[VSCode Extension Guide](./guides/vscode-plugin.md)

#### Python Debugging

gdb4hpc has extra support for debugging Python applications.

- [Using Python Mode](./guides/python-mode.md)

### gdb4hpc man Pages and Reference Material

* [gdb4hpc man page](./man/gdb4hpc.1.md)
* [gdb4hpc Command Reference](./man/help.md)
