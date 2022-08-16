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

import shutil
import avocado
from avocado import Test
from avocado.utils import process as avproc

if False:
    # mock avocado test
    class Test:
        def get_data(self, filename, must_exist):
            return "./tests.py.data/" + filename

        def assertTrue(self, val, msg):
            if not val:
                print(msg)

import sys
import glob
import os
import socket
import platform
import time
import signal

try:
    from cdst_test.useful import detect_wlm
    import cdst_test.useful as useful
    from cdst_test.textcheck import make_regex
    import cdst_test.modules as mods
except ModuleNotFoundError:
    sys.path.insert(0, os.getcwd() + "/cdst-test")
    from cdst_test.useful import detect_wlm
    import cdst_test.useful as useful
    from cdst_test.textcheck import make_regex
    import cdst_test.modules as mods 

import subprocess
import os
import time
import re

SKIP_FORTRAN_TESTS = None
def should_skip_fortran_tests():
    # The current (as recent as cce 14.0.0.1) ftn compiler's fortran debug info
    # is half-broken and missing variables in certain places. Until that is fixed,
    # we'll just skip tests affected by it.
    global SKIP_FORTRAN_TESTS
    if SKIP_FORTRAN_TESTS is not None:
        return SKIP_FORTRAN_TESTS
    loaded = mods.list("cce")
    if "cce/14.0.0" in loaded or "cce/14.0.0.1" in loaded or "cce/14.0.0.x" in loaded:
        # broken versions
        SKIP_FORTRAN_TESTS = True        
    else:
        # assume anything else has correct symbols
        SKIP_FORTRAN_TESTS = False

    return SKIP_FORTRAN_TESTS

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
suppressedCheck = set()
verbose = False

# regular expressions for types of gdb4hpc output
is_procset_re = re.compile(r"^([a-z0-9_]+){([^:}]+)((:[0-9 -]+)?)}:(.*)")
is_batch_re = re.compile(r"^\d+:(.*)")
is_prog_out_re = re.compile(r"^<\$[a-z0-9_]+>:(.*)")
interrupted_proc_out_re = re.compile(r"^<\$[a-z0-9_]+>:.*([ab]{[0-9.:]+}: Breakpoint .*)")
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
        loc = command.find('???')
        if loc > 0:
            global indeterminate; indeterminate = True
            if command[loc+3:]:
                global suppressedCheck; suppressedCheck.add(command[loc+3:])
            command = command[0:loc]
        return command
    else:
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

    global indeterminate
    if indeterminate:
        return None

    if not ref_batch_line == out_line:
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
    launching = False

    with open(ref_file_path, "r") as ref, open(out_file_path) as out:
        outline_num = 0
        for outline in out:
            outline_num += 1
            orig_outline = outline.rstrip()
            outline = anonymize_line(orig_outline)

            # notice app launches
            if outline == "Starting application, please wait...":
                launching = True

            # input line for the batch file
            out_batch_line = is_batch(outline)
            if out_batch_line != None:
                if launching:
                    # found next command before launching completed. launch probably failed.
                    return error("launch never completed", outline_num, orig_outline)

                refline = next_ref_line(ref)
                global indeterminate; indeterminate = False
                if refline == None:
                    return error("extra output", outline_num, orig_outline)
    
                cmp = compare_batch_lines(refline, out_batch_line)
                if cmp:
                    return error(cmp, outline_num, orig_outline)
                continue
            
            if launching:
                # mrnet and zmqnet output differ, so ignore launch output 
                # until "Launch complete." or another command (checked above)
                if outline == "Launch complete.":
                    launching = False
                    # fast forward ref file until it catches up
                    refline = next_ref_line(ref)
                    while refline is not None and refline != "Launch complete.":
                        refline = next_ref_line(ref)
                    if refline is None:
                        return error("extra output", outline_num, orig_outline)
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

    #if socket.gethostname()[0:5] == 'cfa6k':
    #    launch_args = launch_args + non_mpi_arg

    if launch_args:
        launch_re = re.compile(r"(launch [^}]+} *)")

    rcline = re.compile(r"^\d+: ?(.*)")

    with open(runfile,"w") as run:
        for line in open(rcfile, "r"):
            m = rcline.match(line)
            if not m:
                continue
            line = m.group(1)
            qloc = line.find('???')
            if qloc > 0:
                line = line[0:qloc]
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

def getOut(test, name):
    if test.proto is not None:
        # remove ".out"
        unaltered = useful.getOut(test, name)[:-4]
        return unaltered + f"_{test.proto}.out"

    return useful.getOut(test, name)

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

    # avocado will kill the test after this amount of time. we set a shorter timeout
    # to give us time to do our own cleanup before avocado kills everything
    absolute_timeout = test.params.get("timeout") if test.params.get("timeout") is not None else 140
    cleanup_time = 10 
    # add a 2 second buffer to run final kill commands
    timeout = absolute_timeout - cleanup_time - 2

    if absolute_timeout < 0 or timeout < 0:
        test.fail(f"Invalid timeout values: abs: {absolute_timeout}, timeout: {timeout}")
    print(f"timeout: {timeout}, cleanup: {cleanup_time}")
    
    try:
        p = subprocess.Popen(['gdb4hpc', '-snekb', runfile, '--output', outfile])
        p.wait(timeout)
    except subprocess.TimeoutExpired:
        # send sigterm to gdb4hpc, triggering clean up.
        # returns list of pids the signal was sent to
        descs = avproc.kill_process_tree(p.pid, signal.SIGTERM)
        
        # give gdb4hpc some time to clean up
        time.sleep(cleanup_time)
        
        # send sigkill to gdb4hpc and all previous children (including srun/aprun)
        # in case any of them ignored sigterm
        for pid in descs:
            avproc.safe_kill(pid, signal.SIGKILL)

        test.fail("Timed out")

'''
Driver for generating the gdb4hpc run script, running gdb4hpc, and comparing the output
against the saved reference file.
'''
def runTest(test):
    global suppressedCheck; suppressedCheck.clear()
    runScript(test)
    result, error = compare_files(getTxt(test, test.__class__.__name__), getOut(test, test.__class__.__name__))
    test.assertTrue(result, error)
    if suppressedCheck:
        test.cancel("suppressed checks: " + str(suppressedCheck))

'''
Specialized driver for running a gdb4hpc script with a gpu.
'''
def runGpuTest(test, gpu):
    hostname = socket.gethostname() 
    gpuScript = "./tests.py.data/" + gpu + "/" + hostname + ".sh"
    if not os.path.exists(gpuScript):
        test.cancel("No " + gpuScript + " script")

    substitutions = {}
    testName = test.__class__.__name__
    runfile = makeRunFile(test, substitutions)
    outfile = getOut(test, testName)
 
    try:
        os.remove(outfile)
    except FileNotFoundError as e:
        # already gone
        pass

    print([gpuScript, 'launch','gdb4hpc', '-s', '-e', '-n', '-b', runfile, '--output', outfile, '--keep-going'])
    subprocess.call([gpuScript, 'launch','gdb4hpc', '-s', '-e', '-n', '-b', runfile, '--output', outfile, '--keep-going'])

    result, error = compare_files(getTxt(test, test.__class__.__name__), getOut(test, test.__class__.__name__))
    test.assertTrue(result, error)
    
########### sanity tests ##################
class Gdb4hpcTest(Test):
    def setUp(self):
        # set proto env var
        self.proto = self.params.get("proto")
        if self.proto != None:
            os.environ["GDB4HPC_PROTO"] = self.proto
            print(f"Set proto to {self.proto}")
            # aarch64 machines are too slow to run all tests.
            # if we're on an aarch64 machine, only run default proto tests
            arch = platform.uname()[5] # .processor
            if self.proto != "zmqnet" and arch == "aarch64":
                self.cancel("Skipping non-default proto on aarch machine.")
        else:
            print(f"No proto param") # use gdb4hpc default

        # set startup timeout env var to speed up failures
        os.environ["GDB4HPC_STARTUP_TIMEOUT"] = "20" # seconds

class c_assign(Gdb4hpcTest):
    def test(self):
        runTest(self)

class c_breakpoint(Gdb4hpcTest):
    def test(self):
        runTest(self)

class c_breakpoint_pending(Gdb4hpcTest):
    def test(self):
        runTest(self)
        
class c_cmp_false(Gdb4hpcTest):
    def test(self):
        runTest(self)
        
class c_cmp_true(Gdb4hpcTest):
    def test(self):
        runTest(self)
        
class c_defset(Gdb4hpcTest):
    def test(self):
        runTest(self)
        
class c_finish(Gdb4hpcTest):
    def test(self):
        runTest(self)
        
class c_focus(Gdb4hpcTest):
    def test(self):
        runTest(self)
        
class c_frame(Gdb4hpcTest):
    def test(self):
        runTest(self)
        
class c_gdbmode(Gdb4hpcTest):
    def test(self):
        runTest(self)
        
class c_help(Gdb4hpcTest):
    def test(self):
        runTest(self)
        
class c_info(Gdb4hpcTest):
    def test(self):
        runTest(self)
        
class c_kill(Gdb4hpcTest):
    def test(self):
        runTest(self)

class c_list(Gdb4hpcTest):
    def test(self):
        runTest(self)
        
class c_next(Gdb4hpcTest):
    def test(self):
        runTest(self)
        
class c_print(Gdb4hpcTest):
    def test(self):
        runTest(self)

class c_commnode_fanout(Gdb4hpcTest):
    def getLogDir(self):
        return f"{os.path.dirname(os.path.abspath(__file__))}/mrnet_logs_{os.getpid()}"

    def setUp(self):
        super().setUp()
        # create mrnet log dir
        os.mkdir(self.getLogDir())

    def test(self):
        # self.proto == None indicates zmqnet
        if self.proto == None or self.proto != "mrnet":
            self.cancel("Only applicable to mrnet")

        # set fanout env variable to force invocation of mrnet_commnode
        os.environ["GUARD_MRNET_TOP_FANOUT"] = "2"
        os.environ["MRNET_DEBUG_LOG_DIRECTORY"] = self.getLogDir()
        os.environ["MRNET_OUTPUT_LEVEL"] = "3"

        runTest(self)

        found = False
        for file in glob.glob(f"{self.getLogDir()}/FE_*"):
            magic = "Using CTI to start: mrnet_commnode"
            data = open(file, "r").read()
            print(file)
            print(data) # write to job log
            if magic in data:
                print(f"found in {file}")
                found = True
                break

        self.assertTrue(found, f"\"{magic}\" not found in MRNET logs. Check job.log for contents")

    def tearDown(self):
        # cleanup mrnet log dir
        shutil.rmtree(self.getLogDir())

class c_release(Gdb4hpcTest):
    def test(self):
        wlm = detect_wlm()

        # XXX: on ALPS, because of PE-42693, releasing an app causes it to linger and clog our allocation
        if wlm == "ALPS":
            self.cancel("Cancelling on ALPS because of PE-42693")

        runTest(self)
        
class c_set_unset(Gdb4hpcTest):
    def test(self):
        runTest(self)
        
class c_show(Gdb4hpcTest):
    def test(self):
        runTest(self)
        
class c_sizeof(Gdb4hpcTest):
    def test(self):
        runTest(self)
        
class c_step(Gdb4hpcTest):
    def test(self):
        runTest(self)
        
class c_up_down(Gdb4hpcTest):
    def test(self):
        runTest(self)
        
class c_viewset(Gdb4hpcTest):
    def test(self):
        runTest(self)
        
class c_whatis(Gdb4hpcTest):
    def test(self):
        runTest(self)

class c_segv(Gdb4hpcTest):
    def test(self):
        runTest(self)

class c_half_exit(Gdb4hpcTest):
    def test(self):
        runTest(self)

class lgdb_cpp_t1(Gdb4hpcTest):
    def test(self):
        runTest(self)
        
class lgdb_cpp_t2(Gdb4hpcTest):
    def test(self):
        runTest(self)
        
class lgdb_cpp_t3(Gdb4hpcTest):
    def test(self):
        runTest(self)
        
class lgdb_cpp_ds1(Gdb4hpcTest):
    def test(self):
        runTest(self)
        
class lgdb_ftn_t1(Gdb4hpcTest):
    def test(self):
        if should_skip_fortran_tests():
            self.cancel("Skipping test: broken ftn compiler")
        runTest(self)
        
class lgdb_ftn_t2(Gdb4hpcTest):
    def test(self):
        if should_skip_fortran_tests():
            self.cancel("Skipping test: broken ftn compiler")
        runTest(self)
        
class lgdb_ftn_t3(Gdb4hpcTest):
    def test(self):
        if should_skip_fortran_tests():
            self.cancel("Skipping test: broken ftn compiler")
        runTest(self)
        
class assert_scalars(Gdb4hpcTest):
    def test(self):
        runTest(self)

class assert_arrays(Gdb4hpcTest):
    def test(self):
        runTest(self)
        
class assert_decomp(Gdb4hpcTest):
    def test(self):
        runTest(self)

class assert_deadlock(Gdb4hpcTest):
    def test(self):
        runTest(self)

class assert_interrupted(Gdb4hpcTest):
    def test(self):
        runTest(self)

class assert_errors(Gdb4hpcTest):
    def test(self):
        runTest(self)
        
class ftn_arrays(Gdb4hpcTest):
    def test(self):
        runTest(self)

class cpp_arrays_compare(Gdb4hpcTest):
    def test(self):
        runTest(self)

class cpp_decomp(Gdb4hpcTest):
    def test(self):
        runTest(self)

class ftn_decomp(Gdb4hpcTest):
    def test(self):
        runTest(self)

class cpp_decomp2(Gdb4hpcTest):
    def test(self):
        runTest(self)

class cpp_bt(Gdb4hpcTest):
    def test(self):
        runTest(self)

class cpp_thread_funcs(Gdb4hpcTest):
    def test(self):
        runTest(self)

class cpp_thread_focus(Gdb4hpcTest):
    def test(self):
        self.cancel("need to redo thread focus example to be more determinate")
        #runTest(self)

class cpp_arrays_print(Gdb4hpcTest):
    def test(self):
        runTest(self)

class cpp_attach(Gdb4hpcTest):
    def test(self):
        wlm = detect_wlm()

        # XXX: on ALPS, because of PE-42693, releasing an app causes it to linger and clog our allocation
        if wlm == "ALPS":
            self.cancel("Cancelling on ALPS because of PE-42693")

        # make sure we were able to build the proper binaries for this test
        if not os.path.isfile("./bd_dt/cti_launch.x"):
            self.cancel("./bd_dt/cti_launch.x not present, skipping. When the tests were built, could the compiler find CTI development libraries?")

        # build arguments
        rank_args = None
        if wlm == "SLURM":
            rank_args = ["-n4", "--ntasks-per-node=2"]
            config_args = os.environ.get("CDST_TESTS_LAUNCHER_ARGS")
            if config_args:
                rank_args += config_args.split()
        elif wlm == "ALPS" or wlm == "PALS":
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

class cpp_catch(Gdb4hpcTest):
    def test(self):
        runTest(self)

NOCHECKPOINT_FLAG = "./avocado-virtual-environment/nocheckpoint"

class c_checkpoint0(Gdb4hpcTest):
    def test(self):
        # some systems can't support checkpoints because of multithreaded MPI implementations.
        # this test serves as a meta-test to see if the system can support checkpoints. if
        # it can, we run the full checkpoint tests below.

        # we need to drop a file onto disk to communicate with other tests.
        # this is because avocado runs each test as a new process, so
        # global variables or environment variables wouldn't work.

        # we invert the logic of the flag in case the user runs the full checkpoint tests
        # without running this one first. the full checkpoint tests should always
        # be run unless we know for sure that they can't be.

        if os.path.exists(NOCHECKPOINT_FLAG):
            self.cancel(f"Determined that checkpoints are unsupported via {NOCHECKPOINT_FLAG}")
            return # this test always passes
        
        runScript(self)
        with open(getOut(self, self.__class__.__name__), "r") as out:
            data = out.read()
            if "Debugger error: checkpoint: can't checkpoint multiple threads." in data:
                print("Checkpoints unsupported")
                open(NOCHECKPOINT_FLAG, "a").close()
                self.cancel(f"Determined that checkpoints are unsupported")
            elif "Created checkpoint 1" in data:
                print("Checkpoints supported")

        # this test always passes

class c_checkpoint1(Gdb4hpcTest):
    def test(self):
        if not os.path.exists(NOCHECKPOINT_FLAG):
            runTest(self)
        else:
            self.cancel("System doesn't support gdb checkpoints")

class c_checkpoint2(Gdb4hpcTest):
    def test(self):
        if not os.path.exists(NOCHECKPOINT_FLAG):
            runTest(self)
        else:
            self.cancel("System doesn't support gdb checkpoints")

class rocmGpu(Gdb4hpcTest):
    def test(self):
        runGpuTest(self, "rocm")

class c_run(Gdb4hpcTest):
    def test(self):
        runTest(self)

