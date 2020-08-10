'''
 * Copyright 2020 Cray Inc.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
'''

import avocado
from avocado import Test
from avocado.utils import process

# get cdst testing library
import sys
from test_tool_starter import find_cdst_support_prefix

sys.path.insert(0, find_cdst_support_prefix() + "/testing_lib")

from cdst_test.useful import detect_wlm, getOut, getTxt
from cdst_test.textcheck import make_regex, compare_files

import subprocess
import os
import time
import re

###########################################################################################
### gdb4hpc function tests
#
# This is the top level driver for running the gdb4hpc function tests.  For now, at least, these
# are all based on running gdb4phc with a script file 'see func_tests.py.data/*.rc' gathering the 
# output and comparing the result against a reference file (see func_tests.py.data/*.txt) which 
# contain some special mark-ups to account for the expected variations in the output
##########################################################################################

#Note: if you want to skip a test but run the suite, add the following line at the top of the class definition:
#       @avocado.skip("<optional comment here>")

def isSlurm():
    return ( detect_wlm() == "SLURM")

'''
Return a writeable directory for saving the output result for the tests.
'''
def getTmpDir():
    tmpdir = os.getcwd() + '/tmp'
    try:
        os.mkdir(tmpdir)
    except OSError:
        pass
    return tmpdir

'''
Create the batch file for running a specific test in gdb4phc
'''
def makeRunFile(test):
    testName = test.__class__.__name__
    tmpdir = getTmpDir()
    rcfile = test.get_data(testName + ".rc", must_exist=True)
    if (not rcfile):
        test.fail("could not get rc file")
    runfile = tmpdir + '/' + testName + ".rc"

    launchRE = re.compile(r"^\s*launch\s+\$\S+{([0-9]+)\}.*\s([^-]\S+)")
    application = None
    numProcs = 1
    totNumProcs = 0
    for line in open(rcfile,"r"):
        match = launchRE.search(line)
        if match:
            numProcs = int(match.group(1))
            application = match.group(2)
            totNumProcs += numProcs

    if not application:
        test.error("could not find launch in " + rcfile)

    with open(runfile,"w") as run:
        for line in open(rcfile, "r"):
            line = line.replace(r"%%launcher_args%%", os.environ["CDST_TESTS_LAUNCHER_ARGS"])

            # hack: on some systems, specifying --launcher-args="" will break the launcher.
            line = line.replace("--launcher-args=\"\"", "")

            run.write(line)

    return runfile

'''
Run the gdb4phc batch script for a test
'''
def runScript(test):
    testName = test.__class__.__name__
    runfile = makeRunFile(test)
    outfile = getOut(test, testName)

    try:
        os.remove(outfile)
    except FileNotFoundError:
        pass

    subprocess.call(['gdb4hpc', '-s', '-b', runfile, '--output', outfile])

'''
Driver for generating the gdb4hpc run script, running gdb4hpc, and comparing the output
against the saved reference file.
'''
def runTest(test):
    runScript(test)
    result, error = compare_files(getTxt(test, test.__class__.__name__), getOut(test, test.__class__.__name__))
    test.assertTrue(result, error)

'''
A real dummy test just to make sure the avocado framework is working
'''
class Gdb4HpcPass(Test):
    def test(self):
        pass
########### sanity tests ##################
class c_assign(Test):
    def test(self):
        runTest(self)

class c_breakpoint(Test):
    def test(self):
        runTest(self)

class c_breakpoint_pending(Test):
    def test(self):
        runTest(self)
        
class c_cmp_false(Test):
    def test(self):
        runTest(self)
        
class c_cmp_true(Test):
    def test(self):
        runTest(self)
        
class c_defset(Test):
    def test(self):
        runTest(self)
        
class c_finish(Test):
    def test(self):
        runTest(self)
        
class c_focus(Test):
    def test(self):
        runTest(self)
        
class c_frame(Test):
    def test(self):
        runTest(self)
        
class c_gdbmode(Test):
    def test(self):
        runTest(self)
        
class c_help(Test):
    def test(self):
        runTest(self)
        
class c_info(Test):
    def test(self):
        runTest(self)
        
class c_kill(Test):
    def test(self):
        runTest(self)
        
class c_next(Test):
    def test(self):
        runTest(self)
        
class c_print(Test):
    def test(self):
        runTest(self)
        
class c_release(Test):
    def test(self):
        runTest(self)
        
class c_set_unset(Test):
    def test(self):
        runTest(self)
        
class c_show(Test):
    def test(self):
        runTest(self)
        
class c_sizeof(Test):
    def test(self):
        runTest(self)
        
class c_step(Test):
    def test(self):
        runTest(self)
        
class c_up_down(Test):
    def test(self):
        runTest(self)
        
class c_viewset(Test):
    def test(self):
        runTest(self)
        
class c_whatis(Test):
    def test(self):
        runTest(self)
        
class lgdb_cpp_t1(Test):
    def test(self):
        runTest(self)
        
class lgdb_cpp_t2(Test):
    def test(self):
        runTest(self)
        
class lgdb_cpp_t3(Test):
    def test(self):
        runTest(self)
        
class lgdb_cpp_ds1(Test):
    def test(self):
        runTest(self)
        
class lgdb_ftn_t1(Test):
    def test(self):
        runTest(self)
        
class lgdb_ftn_t2(Test):
    def test(self):
        runTest(self)
        
class lgdb_ftn_t3(Test):
    def test(self):
        runTest(self)
        
class c_assert_wodecomp(Test):
    def test(self):
        runTest(self)
        
class c_assert_wdecomp(Test):
    def test(self):
        runTest(self)
        
class ftn_arrays(Test):
    def test(self):
        runTest(self)

class cpp_arrays_compare(Test):
    def test(self):
        runTest(self)

class cpp_decomp(Test):
    def test(self):
        runTest(self)

class ftn_decomp(Test):
    def test(self):
        runTest(self)

class cpp_decomp2(Test):
    def test(self):
        runTest(self)

class cpp_bt(Test):
    def test(self):
        runTest(self)

class cpp_thread_funcs(Test):
    def test(self):
        runTest(self)

class cpp_thread_focus(Test):
    def test(self):
        runTest(self)
