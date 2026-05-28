# Running functional tests

The overall process for running functional test for any CDST product is as follows:

## Step One: Install and Use Product RPMs

The system will need the product RPMs installed. They can be installed
in a custom directory, the base system, or a mixture of both, as long
as the versions match. The tests are designed to test whatever version
of the rpm/module is currently loaded. The RPMs you will need are: 

- The base RPM of the product
- The tests RPM of the product

Master branch RPMS can be found on arti.dev.cray.com under the path
pe-base-rpm-master-local/dev/master/<flavor>/<x86_64|aarch64> or they
can be manually built and transferred to the machine that you wish to
test on.

### How Modules are Loaded by the Test Tool

If using a custom install location, be sure to module use the location
of your custom RPMs. The test tool will try its best to automatically
load the required modules for building tests, but this means that if
there are old versions of the product installed on the base system and
you want to test a development/newer version and you don't module use
the location of the newer modules, the test tool will grab the system
version. If you already have a version of a product loaded, the test
tool won't try to load anything and will use the currently loaded
version. 

The good rule of thumb is that the test tool will load the same
version of a module that gets loaded when you manually do a module
load. So if you need to test a version of an rpm that's not installed
in the default module location, you will need to module use the
correct modulefiles.

## Step Two: Run the Testing Tool

The tests rpm will create a tests directory in the location that the
base RPM is installed into. Inside the tests directory, there will be
a script called `test_tool.py`. To build and run all tests, execute
test_tool.py with no arguments. Execute test_tool.py -h to see other
options you might find useful, such as skipping the build step or
copying tests to another location. In this document, "the test tool"
refers to this script. 

### Meeting the Requirements to Run the Test Tool

The current directory must be writable by you. The test tool will try
to do things like install an avocado virtual environment and build
test binaries. In the case where tests RPMs are installed system wide,
a normal user might not have permissions to modify the directory the
tests are installed in. In such a case, use `test_tool.py -c <location>`
to copy all tests to a location that you have write access to. 

PrgEnv-cray (or similar) must be loaded. To build mpi apps on any
system, the test tool defers the responsibility of including/linking
mpi libraries to the cray compiler wrapper. This means that in order
to build any mpi tests, the compiler you're using must be Cray
cc/CC/ftn. If you get errors when building related to mpi.h or
similar, double check that the PrgEnv-cray module is loaded and that
cc etc. refer to the Cray CPE compilers.

### Changing Launcher Arguments

Sometimes, certain systems need special arguments on the job launch
program (srun, aprun, etc). The test tool tries to detect the WLM in
use and pick sane defaults, but they aren't always enough. 

To change the launcher arguments, edit the `test_tool_config.yaml`
file in the same directory in the test tool. Near the bottom of the
file, you can find a launch section with launcher arguments. One
option is to edit this section.

A better option would be to make a system-specific section so that the
test tool automatically detects the needed changes. To make a system
specific section, make a section with an initial key that matches the
hostname of the system you need to change. Under that, write any
patches that are specific to that system, and the test tool will
automatically pick them up and apply them. Here is an example for a
system called cfa6k:

If you discover a necessary change, be sure to submit a pull request
to the relevant product repository so that next time it will be picked
up automatically.

```
# launcher config defaults
launch:
  args:
    slurm: "-n4 --ntasks-per-node=2"
    alps: "-n4 -N2"
    ssh: "-n4 --ntasks-per-node=2"

# system specific overides
# start with the hostname
# mimic the structure of the default options
cfa6k: 	
  launch: 
    args:
      slurm: "-n4 --ntasks-per-node=2 --mpi=cray_shasta"
```      

## Running Tests - Detail
When running tests, you can run them "locally" or "remotely". Running
tests locally means running a suite of tests on a single machine that
you are logged in to. Running tests remotely means running a suite of
tests on either a single machine or multiple machines in parallel
which you are not necessarily logged in to.

### Running Tests Locally

More options are available than shown here. For more details, see
https://github.hpe.com/hpe/hpc-pe-cdst-cdst-test/blob/main/README.md#test_toolpy-usage

### Running Tests from a tests RPM

Install the RPM and tests RPM of the product you want to test (or use
a product that's already installed) 

Master or release branch RPMS can be found on arti.dev.cray.com under
the path pe-base-rpm-master-local/dev/master/<flavor>/<x86_64|aarch64>
or they can be manually built and transferred to the machine that you
wish to test on. 

- Load the product you are testing with module use and/or module load
- Navigate to the /tests/ directory of the installed RPMs.
- Ensure the tests are in a directory writable by you.
- If you are trying to run tests installed system wide (in /opt), run $ cp -r . <destination> and run tests from the newly copied-to location.
- Run ./test_tool.py

### Running Tests from a cloned git repository

- Install the RPM of the product you want to test (or use a product that's already installed)
- Clone the git repository of the product you are testing
- Navigate to /tests/function in the cloned repository
- Load the product you are testing with module use and/or module load
- Run ./test_tool.py


## Diagnosing a Failed Test
If you come across a test that fails, here are ways to discover why.

### The Job Log

Every time avocado runs, it outputs a debug log of all jobs that were
run to a job log. This log can contain some insightful information
about why a test failed. When avocado runs, it will print out the
location of the job log at the top of the testing interface, right
above the result of the first test. The job log will usually live in a
path similar to tests/avocado-virtual-environment/job-results. The job
log is a little messy, but you should be able to find the section of
the test that failed. Searching for the test name should bring you to
it. 

A common testing pattern is to run a product capture the output, and
then check to see that the output is correct. For tests that use this
pattern, the output is saved to a file called `<test name>.out` and is
matched against a file called `<test name>.txt`. If a test following
this pattern fails, you can find the .out file in a local directory
named /tmp in the same directory as tests.py (not the system /tmp
dirctory). .txt files are found in the tests.py.data/ directory. The
.txt files support some markup to make writing flexible tests
easier. 

## Running a Single Test Again

To run a single test again, execute:
```
$ test_tool_starter.py -t <Test name> -s
```

The -t option specifies a test to run, and the -s option skips the
build step, since all tests have presumably been built at this
point. Use the -h option to see more options. 
