=======================================================================
## [4.14.0] - 2022-04-14
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

