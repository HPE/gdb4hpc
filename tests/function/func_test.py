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

'''
Get the name of the work load manager
'''
def getWLM():
    wlm = subprocess.check_output("module load wlm_detect; wlm_detect", shell=True)
    return wlm

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
    subprocess.call(['cp', rcfile, runfile]);
    return runfile

'''
Return the location to save the output from a test
'''
def getOutFile(testName, rm=False):
    tmpdir = getTmpDir()
    outfile = tmpdir + '/' + testName + '.out'
    if rm:
        try:
            os.remove(outfile)
        except OSError:
            pass
    return outfile

'''
line is a line of the reference file that can mix literals and regular expressions
delimited by %, for example:
gdb version %[0-9.]+% all rights reserved.
this function translates this into a regular expression for the whole line.
'''
def makeRegex(line):
    # break the line into characters
    chars = [char for char in line]
    reChars =['^']

    # regular expression for literals that need to be escaped in the re
    spchars = re.compile(r"[.^$*+?{}\\[\]|()]")
    inRe = False
    for c in chars:
        if inRe:
            if c == '%':
                c = ''
                inRe = False
        elif c == '%':
            c = ''
            inRe = True
        elif spchars.match(c):
            c = "\\" + c
        elif c == '\n':
            c = "\\n"
        elif c == '\t':
            c = "\\t"
        reChars.append(c)
    reChars.append('$')

    # put back together into a string
    reLine = ''.join(reChars)
    return reLine

# regular expression for recognizing a "connecting line", the number of these
# that get generated depends on the speed of the launch so we will be ignoring
# all the intermediate ones
connectRE = re.compile(r'^Number of dbgsrvs connected: \[(\d+)\]')

'''
Determine if a reference line (potentially containing mark-ups) matches a line
from the output.
Return a pair where the first element is true if the lines match and the second
element is True if the reference line is consumed.    The result (True,False) is
for cases like "Number of dbgsrvs connected" where a single reference line can
match an arbitrary number of output lines.
'''
def compareLine(refline, outline):
    testdir = os.getcwd()
    refline = refline.replace("%testdir", testdir)
    refConn = connectRE.match(refline)
    outConn = connectRE.match(outline)
    NOMATCH = (False,False)

    if refConn and outConn:
        refN = int(refConn.group(1))  #dbgsrvs connected
        outN = int(outConn.group(1))
        if outN == refN:
            return (True,True)   # we are now fully connected
        elif outN < refN:
            return (True,False)  # an intermediate progress line
        elif outN > refN:
            return NOMATCH   # *more* connections on the run - wrong!
    elif refline.find("%") != -1:
        reRefline = makeRegex(refline) # line contains a regular expression
        if not re.match(reRefline, outline):
            return NOMATCH
    elif refline != outline:
        return NOMATCH

    return (True,True)

'''
Return true if the outline matches a pattern that is ignored for comparison
'''
def ignoreLine(outline, ignores):
    print("ignores = " + ';'.join(ignores))
    for ignore in ignores:
        if compareLine(ignore, outline)[0]:
            return True
    return False

'''
Read the next line or lines of reference input.   Some output from gdb4phc can 
be generated in an undefined order which is indicated in the .ref file with the 
construct %< ... %- ... %>
This function returns a list of list of strings, which each top level 
element is a sequence of lines that must appear in order (a section), but the lines
in each section can be interleaved with lines from difference section.
On input, ref is the FILE to read from, and ignores is a list of regular regular
expressions for lines that should be ignored.
Return an empty list if there are no more lines in the reference file
'''
def readRef(ref, ignores):
    refline = ref.readline()

    # handle ignores definitions which are delimited by %[  %]
    while len(refline) >= 2 and refline[0:2] == "%[":
        refline = ref.readline()
        while refline and ( len(refline) < 2 or refline[0:2] != "%]" ):
            ignores.append(refline)
            refline = ref.readline()
        refline = ref.readline()

    if not refline:
        return []  # end of file
    elif len(refline) < 2 or refline[0:2] != "%<":
        return [ [ refline ] ]  # a single next line to match

    # read in the set of section until we get to the ending %>
    refLines = []
    section = []

    while True:
        refline = ref.readline()
        if not refline:
            break
        elif len(refline) >= 2 and refline[0:2] == "%-":
            if section > 0:
                refLines.append(section)
                section = []
        elif len(refline) >= 2 and refline[0:2] == "%>":
            if section:
                refLines.append(section)
            break
        else:
            section.append(refline)

    return refLines
    
'''
Compare the refernce and generated result for a test
Generate an error and return False if the results do not match.
'''
def compareResults(test):
    testName = test.__class__.__name__
    outfile = getOutFile(testName, rm=False)
    reffile = test.get_data(testName + ".txt", must_exist=True)
    if not reffile:
        return

    testdir = os.getcwd()

    shortOutfile = outfile.replace(testdir, ".")
    shortReffile = reffile.replace(testdir, ".")

    refLines = []
    ignores = []

    with open(reffile, 'r') as ref:
        outln = 0
        for outline in open(outfile, 'r'):
            outln += 1
            if not refLines:
                refLines = readRef(ref, ignores)
                if not refLines:
                    test.fail("output has additional content at %s:%d got %s" % (shortOutfile,outln,outline))
                    return

            if ignoreLine(outline, ignores):
                continue
            
            matched = False
            consumed = False

            for section in refLines:
                (matched,consumed) = compareLine(section[0], outline)
                if not matched:
                    continue
                elif consumed:
                    section.pop(0)
                    if section:
                        consumed = False
                    break

            if not matched:
                refline = '|'.join([ opt[0].rstrip() for opt in refLines ])
                test.fail("mismatch at %s:%d vs reference %s, got %s expected %s" % (shortOutfile, outln, shortReffile, outline, refline))
                return
            elif consumed:
                refLines.remove([])
        # the outfile has fully matched - mark sure we have finished up the reference file
        if not refLines:
            refLines = readRef(ref, ignores)
        if refLines:
            refline = '|'.join([ opt[0] for opt in refLines ])
            test.fail("output ends prematurely, expected file %s" % refline)

'''
Run the gdb4phc batch script for a test
'''
def runScript(test):
    testName = test.__class__.__name__
    runfile = makeRunFile(test)
    outfile = getOutFile(testName, rm=True)
    subprocess.call(['gdb4hpc', '-s', '-b', runfile, '--output', outfile])

'''
Driver for generating the gdb4hpc run script, running gdb4hpc, and comparing the output
against the saved reference file.
'''
def runTest(test):
    runScript(test)
    compareResults(test)

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
        
