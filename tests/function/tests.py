'''
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
'''

###########################################################################################
### gdb4hpc function tests
#
# This is the top level driver for running the gdb4hpc function tests.  The test information
# is defined in 'func_tests.py/<testname>.in' which defines both the input and output.  The process
# when done will leave a tmp/<testname>.rc file with the script to run and a tmp/<testname>.out
#
# The process catches much of the variability but additional flexiblity can be added by
# using the form %<python regular expression>% for more flexible matching.  For example,
# the output of 'show version' could be set to:
# Version %[\d.]+%
# or
# Version %\d+\.\d+\.\d+(\.d+)?%    if you're feeling more pedantic.
# Or, for a more blunt instrument, the string "???" can be added to the end of any batch
# command to disable output checking entirely for that command
##########################################################################################

#Note: if you want to skip a test but run the suite, add the following line at the top of the class definition:
#       @avocado.skip("<optional comment here>")

import avocado
from avocado import Test
from avocado.utils import process

if False:
    # mock avocado test
    class Test:
        def get_data(self, filename, must_exist):
            return "./tests.py.data/" + filename

        def assertTrue(self, val, msg):
            if not val:
                print(msg)

import sys
import os
import socket

#os.environ["CDST_TESTS_LAUNCHER_ARGS"] = "--exclusive"

try:
    from cdst_test.useful import detect_wlm, getOut
    from cdst_test.textcheck import make_regex
except ModuleNotFoundError:
    sys.path.insert(0, os.getcwd() + "/cdst-test")
    from cdst_test.useful import detect_wlm, getOut
    from cdst_test.textcheck import make_regex

import subprocess
import os
import time
import re

# The current (as recent as craype 2.7.12.1) ftn compiler's fortran debug info
# is half-broken and missing variables in certain places. Until that is fixed,
# we'll just skip tests affected by it.
SKIP_FORTRAN_TESTS = True

######################################################
# Functions for creating a script file and verifying
# the output against the expect output
#
# The script file is embedded into the *.in file under data.
# The script lines are preceded by a line number, and is consisent
# with a the output genereated by running gdb4hpc with the
# -s -e -b <batchfile>
# options.
# The checking for gdb4hpc has to be pretty smart to handle
# the variability in the output since in many cases this will depend on
# the timing of responses from the back-ends.

# output from the current command by proc name  
procRefs = {}
indeterminate = False
verbose = False

# regular expressions for types of gdb4hpc output
is_procset_re = re.compile(r"^([a-z]+){([^:}]+)((:[0-9 -]+)?)}:(.*)")
is_batch_re = re.compile(r"^\d+:(.*)")
connected_re = re.compile(r"^Number of dbgsrvs connected: \[(\d+)\]")
is_prog_out_re = re.compile(r"^<\$[a-z]+>:(.*)")
interrupted_proc_out_re = re.compile(r"^<\$[a-z]+>:.*([ab]{[0-9.:]+}: Breakpoint .*)")
command_error_re = re.compile(r"^(Command resulted in an error: )\d+: (.*)")

# regular expressions for anonymizing gdb4hpc output (currently fortan code
# doesn't generated paths for breakpoints)
replace_dir_re = re.compile(r"\S*/bd_dt/")
replace_ftn_dir_re = re.compile(r"\S*/(\w*.f90)\b")
hex_val_re = re.compile(r"0x[0-9a-f]+")
lwp_val_re = re.compile(r"\(LWP \d+\)")
ftn_init_bp_re = re.compile(r"(Initial breakpoint, [a-zA-z_]+ at )(.*/)?(.*\.f90):\d+")

launcher_arg_re = re.compile('--launcher-args="[^"]*" ')
non_mpi_arg = ' --non-mpi '

def getTxt(test, name: str):
    filename = name + ".in"
    path = test.get_data(filename, must_exist = True)
    if not path:
        test.error("Couldn't get .in file: " + str(filename))
    return path

def is_procset(line):
    m = is_procset_re.match(line)
    if m:
        return m.group(1)
    else:
        return None

def expand_proc_set(proc_set):
    procs = []
    if proc_set:
        for x in proc_set.split(','):
            m = re.match(r'(\d+)\.\.(\d+)', x)
            if m:
                procs += range(int(m.group(1)),int(m.group(2))+1)
            else:
                procs.append(int(x))
    return procs

# compare to process sets, return:
# - True if the process sets are identical
# - a string with the remaining process set if out_proc_str is a subset
#       of ref_proc_str
# - False if the process sets are inconsistent
def compare_proc_set(ref_proc_str, out_proc_str, seen):
    ref_procs = expand_proc_set(ref_proc_str)
    out_procs = expand_proc_set(out_proc_str)
    seen_procs = expand_proc_set(seen)

    remaining_procs = [ str(x) for x in ref_procs if x not in out_procs ]
    mismatched = [ x for x in out_procs if x not in ref_procs and x not in seen_procs]
    for p in out_procs:
        if p not in seen_procs:
            seen_procs.append(p)
    seen_procs = [ str(x) for x in seen_procs ]

    if mismatched:
        return False, None, None
    elif remaining_procs:
        return None, ",".join(remaining_procs), ",".join(seen_procs) 
    else:
        return True, None, None

# compare a process set reply with the reference
def compare_procset_lines(ref_line, out_line, seen):
    ref_m = is_procset_re.match(ref_line)
    out_m = is_procset_re.match(out_line)

    if not ref_m or not out_m:
        return False, None, None

    # (procset, procs, value)
    ref = (ref_m.group(1), ref_m.group(2), ref_m.group(5))
    out = (out_m.group(1), out_m.group(2), out_m.group(5))

    # by design ref[0] == out[0] because we are looking in a single proc-set, but
    # check anyway
    if not compare_lines(ref[2],out[2]) or ref[0] != out[0]:
        return False, None, None

    cmp, refset, seen = compare_proc_set(ref[1], out[1], seen)
    if cmp is not None:
        return cmp, None, None
    else:
        return None, ref[0] + "{" + refset + "}:" + ref[2], seen 

# return the command if this is a batch line
def is_batch(line):
    m = is_batch_re.match(line)
    if m:
        command = m.group(1)
        if command[-3:] == '???':
            global indeterminate; indeterminate = True
            command = command[0:-3]
        return command
    else:
        return None

# recognize a "Number of dbgsrvs connected: [1];  Timeout Counter: [0]" line
def is_connected(line):
    m = connected_re.match(line)
    if m:
        return m.group(1)
    return None

# text output generated by the application itself
def is_prog_out(line):
    m = is_prog_out_re.match(line)
    if m:
        return True
    else:
        return False

# compare out_line with a reference line that may include %<re>% wildcards
def compare_lines(ref_line, out_line):
    if verbose:
        print('compare "%s" and "%s"' % (ref_line, out_line))
    percents = ref_line.count('%')
    if percents != 0 and percents & 1 == 0:
        return re.match(make_regex(ref_line), out_line) != None
    else:
        return ref_line == out_line

def compare_batch_lines(ref_line, out_line):
    ref_batch_line = is_batch(ref_line)
    if ref_batch_line == None:
        return "missing output"

    if not compare_lines(ref_batch_line, out_line):
        return "batch lines do not match"

    return None

# non-procset lines are the return from things like "compare"
# that are not associated with a single process set
def compare_non_procset_lines(ref_line, out_line):
    m = command_error_re.match(ref_line)
    if m:
        ref_line = m.group(1) + m.group(2)
    m = command_error_re.match(out_line)
    if m:
        out_line = m.group(1) + m.group(2)
    return compare_lines(ref_line, out_line)

# remove elements from a line that can vary from
# run to run or machine to machine
def anonymize_line(line):
    line = replace_dir_re.sub('./bd_dt/', line)
    line = replace_ftn_dir_re.sub(r"\1", line)
    # 0x0 is special enough that we don't want to anonymize it
    def hex_match(m):
        return "hexval" if m[0] != "0x0" else "0x0"
    line = hex_val_re.sub(hex_match, line)
    line = lwp_val_re.sub('LWP', line)
    line = ftn_init_bp_re.sub(r"\1\3:<start-line>", line)
    line = launcher_arg_re.sub('', line)
    line = line.replace(non_mpi_arg, '')
    m = interrupted_proc_out_re.match(line)
    if m:
        line = m.group(1)
    return line

# read the reference file to get the next comparable line
def next_ref_line(ref):
    while True:
        refline = ref.readline()
        if not refline:
            return None

        refline = anonymize_line(refline.rstrip())
        if indeterminate:
            if is_batch(refline) != None:
                return refline
        elif not is_prog_out(refline):
            return refline

# generate an error message        
def error(message, line_num, line):
    return (False, message + " at " + str(line_num) + " on " + line)

#####################################
### Top level comparison function ###
#####################################
def compare_files(ref_file_path, out_file_path):
    ref_conn = ''

    with open(ref_file_path, "r") as ref, open(out_file_path) as out:
        outline_num = 0
        for outline in out:
            outline_num += 1
            orig_outline = outline.rstrip()
            outline = anonymize_line(orig_outline)

            # input line for the batch file
            out_batch_line = is_batch(outline)
            if out_batch_line != None:
                refline = next_ref_line(ref)
                global indeterminate; indeterminate = False
                if refline == None:
                    return error("extra output", outline_num, orig_outline)
                cmp = compare_batch_lines(refline, out_batch_line)
                if cmp:
                    return error(cmp, outline_num, orig_outline)
                continue

            if indeterminate:
                continue

            # gdb procset based output
            proc = is_procset(outline)
            if proc:
                if not procRefs.get(proc):
                    procRefs[proc] = []
                matched = False
                procIdx = 0
                while not matched:
                    while procIdx >= len(procRefs[proc]):
                        refline = next_ref_line(ref)
                        if refline == None:
                            return error("extra output", outline_num, orig_outline)
                        rproc = is_procset(refline)
                        if rproc:
                            if procRefs.get(rproc) == None:
                                procRefs[rproc] = []
                            procRefs[rproc].append([refline, ""])
                        else:
                            return error("did not find match", outline_num, orig_outline)

                    refline, seen = procRefs[proc][procIdx]
                    cmp, newref, seen = compare_procset_lines(refline, outline, seen)
                    if cmp is not None and cmp == False:
                        procIdx += 1
                    elif cmp is not None and cmp == True:
                        procRefs[proc].pop(procIdx)
                        matched = True
                    else:
                        procRefs[proc][procIdx][0] = newref
                        procRefs[proc][procIdx][1] = seen
                        matched = True
                if not matched:
                    return error("mismatch in return", outline_num, orig_outline)
                continue

            # connecting to dbgsrvs lines
            connected = is_connected(outline)
            if connected:
                while not ref_conn or int(ref_conn) < int(connected):
                    refline = next_ref_line(ref)
                    if refline == None:
                        return error("extra output", outline_num, orig_outline)
                    ref_conn = is_connected(refline)
                    if not ref_conn:
                        return error("did not find connection line", outline_num, orig_outline)
                if ref_conn == connected:
                    ref_conn = None
                continue

            # output printed by the application itself
            prog_out = is_prog_out(outline)
            if prog_out:
                continue

            # other output
            refline = next_ref_line(ref)
            if refline == None:
                return error("extra output", outline_num, orig_outline)
            if not compare_non_procset_lines(refline, outline):
                return error("mismatch", outline_num, orig_outline)

        # make sure every line in the ref file has been accounted for
        extra = next_ref_line(ref)
        if extra != None:
            return (False, f"output ended before match for \"{extra}\"")

    return (True, ' ')



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
def makeRunFile(test, substitutions = ()):
    testName = test.__class__.__name__
    tmpdir = getTmpDir()
    rcfile = getTxt(test,testName)
    if (not rcfile):
        test.fail("could not get rc file")
    runfile = tmpdir + '/' + testName + ".rc"

    launch_re = None
    launch_args = os.environ.get("CDST_TESTS_LAUNCHER_ARGS")

    if launch_args:
        launch_args = '--launcher-args="' + launch_args + '" '

    if socket.gethostname()[0:5] == 'cfa6k':
        launch_args = launch_args + non_mpi_arg

    if launch_args:
        launch_re = re.compile(r"(launch [^}]+} *)")

    rcline = re.compile(r"^\d+: ?(.*)")

    with open(runfile,"w") as run:
        for line in open(rcfile, "r"):
            m = rcline.match(line)
            if not m:
                continue
            line = m.group(1)
            if line[-3:] == '???':
                line = line[0:-3]
            line += '\n'

            if launch_re:
                line = launch_re.sub(r"\1" + launch_args, line)
            for repl, substitution in substitutions:
                print('substitute "%s" with "%s" on "%s"' % (repl, substitution,line))
                line = line.replace(repl, substitution)

            run.write(line)

    return runfile

def getTxt(test, name: str):
    filename = name + ".in"
    path = test.get_data(filename, must_exist = True)
    if not path:
        test.error("Couldn't get .in file: " + str(filename))
    return path

'''
Run the gdb4phc batch script for a test
'''
def runScript(test, substitutions = {}):
    testName = test.__class__.__name__
    runfile = makeRunFile(test, substitutions)
    outfile = getOut(test, testName)

    try:
        os.remove(outfile)
    except FileNotFoundError:
        pass

    subprocess.call(['gdb4hpc', '-s', '-e', '-n', '-b', runfile, '--output', outfile, '--keep-going'])

'''
Driver for generating the gdb4hpc run script, running gdb4hpc, and comparing the output
against the saved reference file.
'''
def runTest(test):
    runScript(test)
    result, error = compare_files(getTxt(test, test.__class__.__name__), getOut(test, test.__class__.__name__))
    test.assertTrue(result, error)

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

class c_list(Test):
    def test(self):
        runTest(self)
        
class c_next(Test):
    def test(self):
        runTest(self)
        
class c_print(Test):
    def test(self):
        runTest(self)

class c_commnode_fanout(Test):
    def test(self):
        # .in file should not contain the text "mrnet_commnode", or check below
        # will always succeed
        commnode_binary = "mrnet_commnode"

        run = makeRunFile(self)
        with open(run, "r") as rc:
            if commnode_binary in rc.read():
                self.fail(f"\"{commnode_binary}\" already in {run}, test is invalidated")

        # set fanout env variable to force invocation of mrnet_commnode
        os.environ["GUARD_MRNET_TOP_FANOUT"] = "2"
        runTest(self)

        # .out file should contain the text "mrnet_commnode" in output of ps ux
        outfile = getOut(self, self.__class__.__name__)
        with open(outfile) as out:
            if commnode_binary not in out.read():
                self.fail(f"\"{commnode_binary}\" not found in {outfile}")

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

class c_segv(Test):
    def test(self):
        runTest(self)

class c_half_exit(Test):
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
        if SKIP_FORTRAN_TESTS:
            self.cancel("Current Cray ftn compiler has broken debug symbols")
        runTest(self)
        
class lgdb_ftn_t3(Test):
    def test(self):
        runTest(self)
        
class assert_scalars(Test):
    def test(self):
        runTest(self)

class assert_arrays(Test):
    def test(self):
        runTest(self)
        
class assert_decomp(Test):
    def test(self):
        runTest(self)

class assert_deadlock(Test):
    def test(self):
        runTest(self)

class assert_interrupted(Test):
    def test(self):
        runTest(self)

class assert_errors(Test):
    def test(self):
        runTest(self)
        
class ftn_arrays(Test):
    def test(self):
        if SKIP_FORTRAN_TESTS:
            self.cancel("Current Cray ftn compiler has broken debug symbols")
        runTest(self)

class cpp_arrays_compare(Test):
    def test(self):
        runTest(self)

class cpp_decomp(Test):
    def test(self):
        runTest(self)

class ftn_decomp(Test):
    def test(self):
        if SKIP_FORTRAN_TESTS:
            self.cancel("Current Cray ftn compiler has broken debug symbols")
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
        self.cancel("need to redo thread focus example to be more determinate")
        #runTest(self)

class cpp_arrays_print(Test):
    def test(self):
        runTest(self)

class cpp_attach(Test):
    def test(self):
        # make sure we were able to build the proper binaries for this test
        if not os.path.isfile("./bd_dt/cti_launch.x"):
            self.cancel("./bd_dt/cti_launch.x not present, skipping. When the tests were built, could the compiler find CTI development libraries?")

        # build arguments
        wlm = detect_wlm()
        rank_args = None
        if wlm == "SLURM":
            rank_args = ["-n4", "--ntasks-per-node=2"]
        elif wlm == "ALPS":
            rank_args = ["-n4", "-N2"]
        else:
            self.error("Failed to detect WLM")

        # launch job
        job = subprocess.Popen(
            ["./bd_dt/cti_launch.x", *rank_args, "./bd_dt/C++/cpp_attach.x", "60"],
            stderr=subprocess.PIPE
        )

        app_id = None
        # wait for job to start and report app id
        for line in iter(job.stderr.readline, b''):
            # first line printed will be the app id, and it will come back in bytes
            app_id = line.decode("utf-8").rstrip()
            break

        # wait a few seconds to make sure cpp_attach gets into its loop
        time.sleep(3)

        # build substitution tuple to inject appid into .rc file
        #   previously this used %%appid%% but rather than add a special rule,
        #   look for the patter that is being used to match the --echo output.
        #   If the same pattern were added elsewhere in the file, this would
        #   need to be changed to match just the line of interest
        subs = ( ("%[0-9.]+%", app_id), )

        # start gdb4hpc, attach, and generate results
        runScript(self, subs)
        result, error = compare_files(getTxt(self, self.__class__.__name__), getOut(self, self.__class__.__name__))
        self.assertTrue(result, error)

class cpp_catch(Test):
    def test(self):
        runTest(self)

class c_checkpoint1(Test):
    def test(self):
        runTest(self)

class c_checkpoint2(Test):
    def test(self):
        runTest(self)

