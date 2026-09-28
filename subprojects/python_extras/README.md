# python extras

This directory contains the pretty printers for boost and std cxx types.  It mostly originates from
files packaged with some version of the standard library packages.  In the ideal world, gdb4hpc would
just get these from those files packaged with the OS, but every time this was tried it was found that while
some data types would be improved, others would break.   This might be a side effect of using different
versions of gdb, or of differences based on using compilers based on LLVM.  The net effect is that this
directoy has an amalgam of difference generations of the pretty printers, along with specific local fixes.

The key test case for exercising these pretty printers is the cpp_ds1 test in the functional suite, which
contains tests for dealing a gamut of C++ data types.

This code was stored in cdst-mirror-github-llvm-mirror-openmp for the HPE CPE releases.   It has been pulled
directly into the gdb4hpc source code to simplify the open source packaging.


