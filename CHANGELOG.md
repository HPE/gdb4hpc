=======================================================================
## [26.03.0]
=======================================================================

### Bug Fixes

* Printing of large std::arrays and help for apropos (#981)  [762ca54]
* Subscripting for std::vector and std::tuple (#980)  [7c39b51]
* --gpu option for launch and attach was broken (#979)  [8b8a99f]
* CPE-11530: add rust functional tests and fix bug with rust-gdb spawning (#973)  [52facf6]
* CPE-10692: Parse quoted arguments following equals in longopt  [0314678]
* Ignore quotes around the --env argument in launch (#964)  [f392222]
* CPE-9018: Survive segv in message queue debugging shared libraries (#953)  [2e6b9d2]
* CPE-9350: Fix incorrect pending breakpoint response in mi mode (#949)  [cd3872d]

### Features

* CPE-11464: Add Rust Strings (#975)  [0a38dcc]
* CPE-11215: add Utf32Char type, Rust vector and array improvements. (#971)  [3d7aca8]
* Remove dependence on cdst-support package (#963)  [8649a9e]
* CPE-9081: Add info mpi pending command (#957)  [9b34e6e]
* Add support for float16 and bfloat (#956)  [e905daa]
* CPE-8683: Add --gdbinit launch option to enable reading .gdbinit files in backend (#950)  [d591c6d]

=======================================================================
## [4.16.3]
=======================================================================

### Bug Fixes

* CPE-5960: Fix failure to launch application when using certain acceleration runtimes (#904)  [0d207e4]
* CPE-7944: Work around gdb bug when finishing during startup sequence on aarch/python (#942)  [7d14656]
* CPE-8161: print an error message when trying to print a dereferenced null pointer (#940)  [5935212]
* CPE-7930: Don't print parse error to stderr  [285a898]

### Features

* CPE-8868: Remove MRNet network protocol (#946)  [c57465e]
* CPE-8194: Better node management when using the run command to relaunch jobs  [1a643fe]

=======================================================================
## [4.16.4]
=======================================================================

### Bug Fixes

* CPE-6196: Allow CUDA breakpoints on kernel launch  [2d25c8f]
* Print NaN and infinity values (#915)  [b95a2fe]
* Add RUNPATH for unit tests to libexec/dbgsrv (#912)  [168c8e0]
* CPE-5982: Fix cuda command failures in some cases  [ea1ee75]
* CPE-5960: Fix failure to launch application when using certain acceleration runtimes (#904)  [db6eef2]
* No not generate missing MPI breakpoint if a program use mpi_init_thread (#890)  [c050796]
* PE-52432: Load MRNet filter library from complete path  [77b87b1]
* PE-54147: Use main function from debug information  [ce6fb4c]
* Fix printing std::unique_ptr with newer stdc++ (#869)  [ad35290]
* Fix print of character arrays shorted by set print elements (#867)  [1511024]
* Fix backend gdb crash on vector<vector<string>>[][][] (#865)  [a72c48f]

### Features

* CPE-7286: Support basic list command in python mode (#933)  [dfd6be3]
* CPE-7395: Support 'catch throw' and 'catch raise' commands in python mode (#931)  [8ae5df4]
* CPE-6731: Apply pager setting to results of `info threads` command (#928)  [be5d2ab]
* CPE-5688: In python mode, stop the application on uncaught exceptions (#927)  [f82f4aa]
* CPE-635: Verify python breakpoint locations (#926)  [09154fe]
* CPE-6345: Add --pending option to break  [478cfe1]
* CPE-5686: Add frame and up/down commands to python mode (#924)  [05f1816]
* CPE-6994: Handle unregistered breakpoints and watchpoints  [79bb08d]
* CPE-6094: Support setting breakpoints in multithreaded python apps (#921)  [0ff9641]
* Add rhel 9.5, remove rhel 8.9 and sles15sp4 (#920)  [bedbb69]
* CPE-6664: Support debugging python 3.5, python 3.7, python 3.13 (#919)  [4f88519]
* CPE-6561: Improve CUDA API error reporting  [c1ca8ef]
* CPE-6513: Show cuda-gdb cuda API warnings (#914)  [6f3a0d6]
* CPE-5690: Change command prompt when in python mode (#908)  [af631ce]
* CPE-6037: Add GPU debugging tutorial (#907)  [352ea63]
* CPE-615: Add python mode documentation (#899)  [d3f1f64]
* CPE-2316: Add "what's new in 4.16.3" document (#901)  [052eb31]
* CPE-633: support finish command in python mode (#896)  [24e1325]
* CPE-2350: Autocomplete struct members for `print`  [a2ead53]
* PE-53102: Set python breakpoints and step/next in python (#892)  [eb3ed7d]
* Support rhel 8.10 (#893)  [c3c6572]
* Add builds for rhel 9.4 and sles15sp6 (#891)  [fd8ef1d]
* PE-53101: Evaluate python statements with the print command (#889)  [28da921]
* PE-55035: Python mode and python backtraces (#885)  [ebc5ca0]
* PE-53105: Launch applications with the same syntax as an srun command (#882)  [17a694d]
* PE-54824: Support non-MPI PALS launches  [9e1b76e]
* PE-54073: Print job output on failed launch  [e7a07d3]
* PE-53771: Pass arguments to launched app on the end of the launch command (#864)  [0c93292]

=======================================================================
## [4.16.2]
=======================================================================

### Bug Fixes

* PE-52737: Fix backend crash when debugging Intel OneAPI GPUs (#849)  [ee6f283]

### Features

* PE-47910: Run job cleanup in background  [46126f0]
* PE-53716: Added user guide for using the Python gdb extension (#859)  [f9bd7e5]
* PE-51960: Added support for rhel89/93 & gcc13  [83b6f4e]
* PE-53828: Implement Flux attach  [f420064]
* Automatically load python extensions when debugger starts (#857)  [cee05d9]
* PE-53100: Support for aggregated gdb python extension commands py-print and py-locals if underlying gdb has python extensions loaded (#855)  [cae19dd]
* PE-53100: Support for aggregated gdb python extension commands py-bt, py-list, py-up, and py-down if underlying gdb has python extensions loaded (#854)  [7689a0b]
* Handle "~" in the --debug launch argument (#852)  [639ac18]
* PE-52589: Tweak default assertion script behavior - now assertion scripts stop iff an assertion fails default (#847)  [1e1dd32]
* PE-51936: Added tab completion for the print command (#844)  [52cf47d]

=======================================================================
## [4.16.1]
=======================================================================

### Bug Fixes

* PE-52387: Fix segfault from list command (#841)  [d0b57bf]
* PE-51694: Fix occasional crash when operating on a subset of ranks (#826)  [34d634c]
* PE-44112: Track job status while running for quicker timeout and exit  [0c83e3f]
* Fix recursive source command behavior (#814) (PE-51342)  [100cad9]
* Properly format /csv output with data over 1 million items (#809)  [b738859]
* PE-51165: Query ZMQNet for correct rank and target PID  [1f88fce]

### Features

* PE-51935: Tab completion with file names when setting breakpoints (#835)  [516367a]
* PE-51935: Tab completion with function names when setting breakpoints (#832)  [101d582]
* PE-50319: Improve feedback when switching command modes (#829)  [b72aaa2]
* PE-51749: Tab completion for help, set, show commands (#827)  [40872a9]
* PE-51752: Moved vscode plugin to gdb4hpc and packaged executable (#825)  [e4e34a2]
* PE-51598: Add save all command (#820)  [b4076b7]
* PE-51254: Save and restore assertion scripts and decompositions (#818)  [c58b6ae]
* PE-51253: Save and restore breakpoints (#816)  [f2015eb]
* Update included hpcc_demo (#815)  [60e85c2]
* Add set print value-caching (#813) (PE-50620)  [4aa4c1c]
* Save decomposition values as convenience variables (PE-50819) (#812)  [d16eef1]

=======================================================================
## [4.16.0]
=======================================================================

### Bug Fixes

* PE-50810: Support storing chunked values in client side variables (#807)  [900e2a0]
* Fix failure with p 1+1+1 (#805)  [01462cd]
* Fix formatting when printing very large arrays and vectors (#804)  [2df7f8b]
* PE-50510: Set CCE 16 Fortran line number workaround as optional  [3ad4ddd]
* Crash where gdb cannot list a line (#785)  [d2a400b]
* PE-49611: Update ZMQNet submodule  [a806965]

### Features

* PE-50840: Update ZMQNet submodule  [afed9f6]
* PE-50503: Add set variable command (#803)  [ee3ef54]
* PE-50619: Save values on the gdb4hpc client as convenience variables with $foo = my_variable syntax (#802)  [61f00fd]
* PE-50671: Support scoped assignment in print statements (#800)  [7c68922]
* Support --debug=<dir> in launch and attach (#793)  [a00ab95]
* Use dots for progress when connecting (#791)  [bec626e]
* PE-49938: Support Slurm MPMD launch  [1c92f9c]
* PE-46238: Send signals other than SIGKILL to application  [35d5cd0]
* Move to gcc toolset 12 on redhat (#778)  [4210f0a]

=======================================================================
## [4.15.1]
=======================================================================

### Bug Fixes

* Remove error message when calling a void function (#762)  [fc457a7]
* Fix failure to launch at 50K ranks.   (PE-48636)  [956dd2b]
* Handle -gdb= options where the debugger relies on LD_LIBRARY_PATH (#761)  [c0a05a1]
* PE-48404 - Fix misprinting of types like int *(**)[2] (#758)  [33f13fe]
* Remove load of cray-mrnet module (#757)  [7466ee6]
* PE-48366 new nvidia driver version was not recognized by --gpu on launch and attach (#756)  [09000e0]
* Catch evaluation exceptions in more cases (#755)  [0818320]
* PE-48481: Update attach help for PALS (#749)  [e1912e8]
* Correctly print references to arrays (#748)  [17df659]
* Fix printing multi-d arrays truncated by set print elements (#741)  [0cf68e0]
* Printing std::string with embedded quotes in PrgEnv-gcc (#730)  [b4b6f46]
* Set print elements interacts poorly with gdbmode (#713)  [4258936]

### Features

* PE-47564: Update RAJA Documentation (#763)  [4981f32]
* Pretty printing for std::list and std::tuple (#753)  [2ed20ea]
* Use native g++ on sles (#752)  [b21e01f]
* Handle std::pair (#745)  [e62694c]
* Print derived class types inside C++ smart pointers (#743)  [0355495]
* Configurable client-side paging for large data (#731)  [bd19475]
* Support decomposing RAJA views without requiring an explicit slice (#739)  [650a507]
* Add directory and set substitute-path commands to find application source code (#738)  [b38a113]
* Support --tty command line option (#735)  [4baea63]
* Allow open ranges for array slicing: minIdx.., ..maxIdx, and .. (#734)  [15bb2c6]
* Add support std::map, std::multimap, std::unordered_map, and std::unordered_multimap (#732)  [4dd1f41]
* Use C style array ordering for kokkos::Views (#729)  [956faaa]
* Add --gdb-data-dir option to launch and attach (#723)  [64c106c]
* Support decomposing RAJA::Views (#720)  [8bec80f]
* Add tutorial for decompositions (#717)  [69ead12]
* Support RAJA reductions (#716)  [7bf754c]
* More support for RAJA::Views (#714)  [7ec2be7]
* Support RAJA::Views of structs (#708)  [ab8e8fb]
* One line decompostion definition  [a19cb01]

=======================================================================
## [4.15.0]
=======================================================================

### Bug Fixes

* Handle anonymous namespaces in C++ (#692)  [4caa280]
* Use the file of the current frame when setting a breakpoint on a line number (#686)  [20e56af]
* Info sources was not working with binaries built with the rocm compilers (#664)  [d649bf7]
* Fix applications getting stuck when creating new threads  [e2d150b]
* Fix install path exported by lmod modulefile on relocated install (#653)  [638508b]
* Strip quotes from path arguments in launch command (#652)  [6a464dd]
* Fix range operator .. for multi-dimension arrays (#641)  [d7547a0]
* Double evaluation of expressions in print (#637)  [d887173]
* Fix launch --non-mpi (#634)  [78cca8f]
* Add support for multiplication in expressions (#633)  [bae1251]
* Fix double prompt printed after an error message (#632)  [0cba378]
* PE-44973: Print error on CTI attach failure  [9df7c73]
* Do not traverse pointer values in a union (#622)  [17dd68f]
* Fix incorrect handling of some fortran types (#621)  [260bebd]
* Handle classed with virtual functions (#620)  [6f6ca86]
* Fix parsing of scoped identifiers in C++ (#611)  [707f0f3]

### Features

* Add documentation for RAJA support (#698)  [2121471]
* Add documentation for handling arrays and kokkos support (#697)  [5d3b432]
* Add experimental python interpreter mode (#695)  [1e5c018]
* Support RAJA::Views (#684)  [82d5a5b]
* Support slicing of decomposed kokkos views (#683)  [bef0667]
* Set command-timeout command (#680)  [b67c501]
* Use --gdb option for the front end gdb (#674)  [db5de86]
* Build with gcc-toolset-10 on redhat (#671)  [e62c34a]
* Allow cancellation of pending commands and exit with ctrl-c (#660)  [6a113cd]
* Add -r flag to maint sleep for remote sleep (#663)  [ebf0933]
* Add initial support for Kokkos::View (#659)  [632657a]
* PE-45055: Support attaching to Valgrind4hpc VGDB servers  [b3123f7]
* Allow redefinition of decompositions (#650)  [190dbc9]
* Handle range slicing for decomposed arrays (#646)  [3a78e90]
* Update tutorial with new features (#639)  [569a947]
* Expand the expressions recognized by the expression parser (#617)  [2e5fea8]
* Let gdbmode command (a.k.a "gdb") take optional arguments to execute a single gdb command (#623)  [c57b500]
* Add conditional breakpoints (#613)  [7de9e42]

=======================================================================
## [4.14.6]
=======================================================================

### Bug Fixes

* Fix memory leaks (#606)  [2cc655c]
* Fix hang on exit when a back-end gdb has a fatal error (#603)  [a299da4]
* Improve aggregation in info threads with AMD gpu threads (#600)  [a3f465b]
* Fix printing of signed char and long double on aarch (#597)  [a21d408]
* Handle C++ anonymous classes (#591)  [4eee26a]
* Check that the dimension of a decomposed array is a multiple of the number of ranks (#588)  [3e1764a]
* Don't kill jobs launched in gdb4hpc when using the release command (#580)  [7b846ad]
* Fix list function when front end gdb fails (#582)  [d55a9cc]

### Features

* Add shell mode (#608)  [380271f]
* Add set print pretty to enable printing indented structs (#598)  [4ffecaa]
* Add hit and ignore counts to info breakpoints (#594)  [bae86e4]
* Add ignore count to breakpoints (#581)  [37828ba]
* Separate procsets from the data by a comma in p/csv (#573)  [d2ddbd6]

=======================================================================
## [4.14.5]
=======================================================================

### Bug Fixes

* Printing csv for multi-dimension decomposed arrays (#569)  [75aadd8]
* Improve handling typedefs in nested scopes, static member variables, and lambda generated functions (raja) (#568)  [4eeb2ee]
* Handle @ tags in gpu variable types (#561)  [9531100]
* Fix stale references to expired procsets in assertion scripts (#555)  [eaccb6b]

### Features

* Add the pipe command (#565)  [c72a7c3]
* Aggregate cuda thread info (#567)  [558eb31]
* Assertion scripts can contain arbitrary expressions (#563)  [a127f92]
* Add /1 and /csv to print for one-per-line, and comma separated values (#556)  [eaed4d4]
* Remove outdated ddt_cuda-gdb from gdb4hpc package  (#560)  [ced4455]

=======================================================================
## [4.14.4]
=======================================================================

### Bug Fixes

* Fix situations where evaluation of binary operations would fail if source ranks did not overlap (#547)  [faa35d5]
* Output was disabled after finishing a "source" command (#551)  [7442663]
* Fix use of deleted procsets in some cases (#532)  [7c0f066]

### Features

* Add support for print formats /x /o /t and /z (#554)  [a55304f]
* Added capability to run shell comamnds on backend (#545)  [226337d]
* Update cuda-gdb build to cudatoolkit 11.6.2;  add --gdb option to attach (#538)  [ff93c79]
* Add launch option --sbatch for submitting Slurm batch scripts  [ec44b8c]
* PE-42153: Add --qsub script submission option for PALS launch  [648e992]
* PE-41769 Add set logging subcommand  [32d1992]

=======================================================================
## [4.14.3]
=======================================================================

### Bug Fixes

* Prevent launched application from exiting before network is cleaned up (#530)  [b08f886]
* Fix segfault after trying to re-launch a procset that doesn't exist (#527)  [de28a65]

### Features

* Add new-ui and set interior-tty (#536)  [3fbf45b]
* Added gdbgui compatibility (#523)  [7385853]
* Emit error message if remote debugger unexpectedly exits (#529)  [c8ec932]

=======================================================================
## [4.14.2]
=======================================================================

### Bug Fixes

* Handle printing enum values (#525)  [45a97df]
* Handle breakpoints with multiple locations (#524)  [e80d12e]
* Allow program stopped at exit() to exit gracefully (#522)  [14eb31c]
* Print vector.size() and other function calls with no arguments(#513)  [991c128]
* Remove extra blank line from ptype command (#514)  [45a42d6]
* Fix uncaught exception after SIGINT during launch  [cebd819]
* Update testing Makefile to work around PE-41446 (#505)  [a3cc7f3]
* Use correct program arguments when re-launching an app with the run command (#503)  [90b8162]
* Handle printing fortran structs (#502)  [45d8783]
* Hang if gdb initialization fails (#501)  [e2a7f37]
* Removed internal mrnet build - building with external cray-mrnet - PE-41651 (#500)  [10342ef]
* Correct indices in the difference vector when comparing decomposed fortran arrays (#497)  [a467d5e]
* Handle array(lower:upper) form for fortran arrays (#493)  [792ef68]
* Fixed hang when working with very large arrays (#495)  [e256348]
* Improve parsing of templated classes (#492)  [58d59b9]
* Make functional test cleanup smarter in the case of a test time out (#491) - PE-41086  [ed0279d]
* Printing of arrays of fortran complex values (#486)  [0b2937e]
* Handle printing a corrupted vector (#484)  [a420c26]
* Printing of unique_ptr  (#485)  [3738af5]

### Features

* Handle SIGINT during launch. (#515)  [eaae606]
* Print types of pointers; make pointer references more readable (#516)  [befd27b]
* Support info cuda, set cuda, show cuda and help cuda (#508)  [5f065b7]
* Extend the run interface and add documention (#511)  [950cd58]
* Frame command only shows program counter with -v option (#506)  [0c9a99a]
* Show feedback during a job launch while using the zmqnet network protocol. (#499) - PE-41538  [8916bae]
* Add info args (#498)  [3407f33]
* Add local shell command (#496)  [e96dec6]
* Add getting started guide and tutorial (#489)  [3917b8d]

=======================================================================
## [4.14.1]
=======================================================================

### Bug Fixes

* Assertion script was continuing in halt mode (#483)  [f939025]
* Fix mrnet failure message on exit on PALS (#474)  [5865736]
* Fix launch on PALS with default gdb4hpc internal gdb (#473)  [3c75ca2]
* Error trying to finish outermost frame (#472)  [91bf471]
* Launch on some cray-pals systems using --gdb=gdb work-around (#471)  [7619a5f]
* Implement --non-mpi launch with zmqnet (#469)  [66765c6]
* Fix --non-mpi launch on some PALS systems (#466)  [4cb0dab]
* Fix inconsistent results with comparisons over one million elements (#464)  [a9fe64b]
* Fix print for typedef values, null char* and function pointers (#460)  [38dc328]
* Fix array comparison for arrays over 1 million elements (#458)  [6e00568]

### Features

* Added support for rhel86 - PE-41140 #476  [381af11]
* Add ability to re-launch applications with new "run" command, preserving breakpoints between launches (#467) - PE-16867  [116e175]
* Added support for sles15sp4 x86 - PE-40983 (#463)  [0a6c660]
* Added autogen changelog/release notes functionality - PE-40699 #457  [c37e757]

=======================================================================
## [4.14.0]
=======================================================================

### Bug Fixes

* Support "array[0]@n" syntax without requiring the expression to be quoted. (#453) - PE-40413  [c39ef85]
* Add reference to man cti PE-40399 (#452)  [9935953]
* Set up structure for defining gpu tests PE-40398 (#449)  [27b8e66]
* Gdb4hpc is crashing after an interrupted launch (#451)  [b85680f]
* Crash during list command (#450)  [b39641f]
* Fixed certain functional tests reporting failure despite actually passing on certain machines. (#445) - PE-39508  [9e6b008]
* Print out char* as string values (#443)  [f93f9bf]
* Print <optimized out> when values are not available. (#440)  [2dd3bcf]

### Features

* Added optional full suite to functional tests allowing tests to be run against every protocol at once. (#448) - PE-39901  [7a83d4a]
* Overhaul Comparative Debugging and Assertion Scripts (#439)  [8c8a288]
* Added support for rhel85 - PE-40102 (#441)  [67437a0]
* Add --non-mpi option for launch.   Allows gdb4hpc to find the initial breakpoint without WLM support. (#437)  [13d48cc]

