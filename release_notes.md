gdb4hpc 4.16.7
================================================================================

Product Description
---------------------------
- gdb4hpc is a GDB-based parallel debugger used to debug applications compiled with
  CCE, AOCC, GNU, and Intel Fortran, C and C++ compilers. It allows programmers to
  either launch an application or attach to an already running application that
  was launched on the system. Additionally, it provides comparative debugging
  technology that enables programmers to compare data structures between two
  executing applications. Comparative debugging should be used in conjunction
  with the CCDB GUI tool accessed by loading the cray-ccdb module.

- Some features of gdb include:
  * Command line parallel debugger allows for launching/attaching applications.
  * Utilizes process sets to operate on a subset of application ranks.
  * GDB-like feel, also implements a gdbmode to enable a true parallel gdb.
  * GPU debugging is supported for OpenMP 4.0+ GPU directives; OpenMP 5.0 on AMD
    and Nvidia GPU's.
  * Workload manager support via Common Tools Interface (cray-cti).

Changelog
---------------------------

=======================================================================
## [4.17.0] Open Source Version
=======================================================================

### License changes

* This is the first version released under open-source.
* The key dependency zmqnet is being released under the same license.
* The CTI was already open-source, but now has an improved stand-alone build.

### Build changes

* Enable building gdb4hpc without hpe submodules.
* Set up build to work against either open source or HPE package versions
   of zmqnet and cti
* Open source build do not build ddt-gdb.   They will rely on gdb being
   available in the the current PATH.

=======================================================================
## [4.16.4]
=======================================================================
 
### Bug Fixes
 
* Printing of large std::arrays and help for apropos 
* Subscripting for std::vector and std::tuple 
* --gpu option for launch and attach was broken 
* CPE-11530: add rust functional tests and fix bug with rust-gdb spawning 
* CPE-10692: Parse quoted arguments following equals in longopt  
* Ignore quotes around the --env argument in launch 
* CPE-9018: Survive segv in message queue debugging shared libraries 
* CPE-9350: Fix incorrect pending breakpoint response in mi mode 
 
### Features
 
* CPE-11464: Add Rust Strings 
* CPE-11215: add Utf32Char type, Rust vector and array improvements. 
* Remove dependence on cdst-support package 
* CPE-9081: Add info mpi pending command 
* Add support for float16 and bfloat 
* CPE-8683: Add --gdbinit launch option to enable reading .gdbinit files in backend 
 
=======================================================================
## [4.16.3]
=======================================================================
 
### Bug Fixes
 
* CPE-5960: Fix failure to launch application when using certain acceleration runtimes 
* CPE-7944: Work around gdb bug when finishing during startup sequence on aarch/python 
* CPE-8161: print an error message when trying to print a dereferenced null pointer 
* CPE-7930: Don't print parse error to stderr  
 
### Features
 
* CPE-8868: Remove MRNet network protocol 
* CPE-8194: Better node management when using the run command to relaunch jobs  
 
=======================================================================
## [4.16.4]
=======================================================================
 
### Bug Fixes
 
* CPE-6196: Allow CUDA breakpoints on kernel launch  
* Print NaN and infinity values 
* Add RUNPATH for unit tests to libexec/dbgsrv 
* CPE-5982: Fix cuda command failures in some cases  
* CPE-5960: Fix failure to launch application when using certain acceleration runtimes 
* No not generate missing MPI breakpoint if a program use mpi_init_thread 
* PE-52432: Load MRNet filter library from complete path  
* PE-54147: Use main function from debug information  
* Fix printing std::unique_ptr with newer stdc++ 
* Fix print of character arrays shorted by set print elements 
* Fix backend gdb crash on vector<vector<string>>
 
### Features
 
* CPE-7286: Support basic list command in python mode 
* CPE-7395: Support 'catch throw' and 'catch raise' commands in python mode 
* CPE-6731: Apply pager setting to results of `info threads` command 
* CPE-5688: In python mode, stop the application on uncaught exceptions 
* CPE-635: Verify python breakpoint locations 
* CPE-6345: Add --pending option to break  
* CPE-5686: Add frame and up/down commands to python mode 
* CPE-6994: Handle unregistered breakpoints and watchpoints  
* CPE-6094: Support setting breakpoints in multithreaded python apps 
* Add rhel 9.5, remove rhel 8.9 and sles15sp4 
* CPE-6664: Support debugging python 3.5, python 3.7, python 3.13 
* CPE-6561: Improve CUDA API error reporting  
* CPE-6513: Show cuda-gdb cuda API warnings 
* CPE-5690: Change command prompt when in python mode 
* CPE-6037: Add GPU debugging tutorial 
* CPE-615: Add python mode documentation 
* CPE-2316: Add "what's new in 4.16.3" document 
* CPE-633: support finish command in python mode 
* CPE-2350: Autocomplete struct members for `print`  
* PE-53102: Set python breakpoints and step/next in python 
* Support rhel 8.10 
* Add builds for rhel 9.4 and sles15sp6 
* PE-53101: Evaluate python statements with the print command 
* PE-55035: Python mode and python backtraces 
* PE-53105: Launch applications with the same syntax as an srun command 
* PE-54824: Support non-MPI PALS launches  
* PE-54073: Print job output on failed launch  
* PE-53771: Pass arguments to launched app on the end of the launch command 
 
=======================================================================
## [4.16.2]
=======================================================================
 
### Bug Fixes
 
* PE-52737: Fix backend crash when debugging Intel OneAPI GPUs 
 
### Features
 
* PE-47910: Run job cleanup in background  
* PE-53716: Added user guide for using the Python gdb extension 
* PE-51960: Added support for rhel89/93 & gcc13  
* PE-53828: Implement Flux attach  
* Automatically load python extensions when debugger starts 
* PE-53100: Support for aggregated gdb python extension commands py-print and py-locals if underlying gdb has python extensions loaded 
* PE-53100: Support for aggregated gdb python extension commands py-bt, py-list, py-up, and py-down if underlying gdb has python extensions loaded 
* Handle "~" in the --debug launch argument 
* PE-52589: Tweak default assertion script behavior - now assertion scripts stop iff an assertion fails default 
* PE-51936: Added tab completion for the print command 
 
 

OS Dependencies
---------------------------
- HPE CSM systems with SLES 15 SP5 - SP6
- HPE HPCM systems with SLES 15 SP5 - SP6 or RHEL 8.10 - 9.4
- HPE Apollo systems with RHEL 8.10

Product Dependencies
---------------------------
- cray-cti installed

Documentation
---------------------------
- Type `man gdb4hpc` with the gdb4hpc module loaded to read the gdb4hpc(1) man page.

  Simple usage examples are provided at the end.

  Also, type `help` at the gdb4hpc command line for more information on commands.

Legal Notices
---------------------------
Certain components, files or programs contained within this package or product are Copyright 2007-2026 Hewlett Packard Enterprise Development LP.

Attribution notices for open source licensed software for this package are detailed in the file:
  ATTRIBUTIONS_gdb4hpc.txt

