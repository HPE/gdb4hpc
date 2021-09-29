'''
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
'''

import avocado
from avocado import Test
from avocado.utils import process

if False:
    # get cdst testing library
    import sys
    from test_tool_starter import find_cdst_support_prefix

    sys.path.insert(0, find_cdst_support_prefix() + "/testing_lib")

    from cdst_test.useful import detect_wlm, getOut
    from cdst_test.textcheck import make_regex, compare_files

else:
    #####################    mockery   #######################
    import os
    class Test:
        def get_data(self, filename, must_exist):
            return "./tests.py.data/" + filename

        def assertTrue(self, val, msg):
            if not val:
                print(msg)
                   
    os.environ["CDST_TESTS_LAUNCHER_ARGS"] = "--exclusive"
    def getOut(test, name: str):
        return "./tmp2/" + name + ".out"

import subprocess
import os
import time
import re

procRefs = {}
indeterminate = False

is_procset_re = re.compile(r"^([a-z]+){([^:}]+)((:[0-9 -]+)?)}:(.*)")
is_batch_re = re.compile(r"^\d+:(.*)")
connected_re = re.compile(r"^Number of dbgsrvs connected: \[(\d+)\]")
is_prog_out_re = re.compile(r"^<\$[a-z]+>:(.*)")
interrupted_proc_out_re = re.compile(r"^<\$[a-z]+>:.*([ab]{[0-9.:]+}: Breakpoint .*)")
command_error_re = re.compile(r"^(Command resulted in an error: )\d+: (.*)")

replace_dir_re = re.compile(r"\S*/bd_dt/")
hex_val_re = re.compile(r"0x[0-9a-f]+")
lwp_val_re = re.compile(r"\(LWP \d+\)")

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

def compare_proc_set(ref_proc_str, out_proc_str):
    ref_procs = expand_proc_set(ref_proc_str)
    out_procs = expand_proc_set(out_proc_str)

    remaining_procs = [ str(x) for x in ref_procs if x not in out_procs ]
    mismatched = [ x for x in out_procs if x not in ref_procs ]

    if mismatched:
        return False
    elif remaining_procs:
        return ",".join(remaining_procs)
    else:
        return True


def compare_procset_lines(ref_line, out_line):
    ref_m = is_procset_re.match(ref_line)
    out_m = is_procset_re.match(out_line)

    if not ref_m or not out_m:
        return False

    # (procset, procs, value)
    ref = (ref_m.group(1), ref_m.group(2), ref_m.group(5))
    out = (out_m.group(1), out_m.group(2), out_m.group(5))

    # by design ref[0] == out[0] because we are looking in a single proc-set, but
    # check anyway
    if ref[2] != out[2] or ref[0] != out[0]:
        return False

    pcs = compare_proc_set(ref[1], out[1])
    if pcs == True or pcs == False:
        return pcs
    else:
        return ref[0] + "{" + pcs + "}:" + ref[2]


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

def is_connected(line):
    m = connected_re.match(line)
    if m:
        return m.group(1)
    return None

def is_prog_out(line):
    m = is_prog_out_re.match(line)
    if m:
        return True
    else:
        return False

def compare_batch_lines(ref_line, out_line):
    ref_batch_line = is_batch(ref_line)
    if ref_batch_line == None:
        return "missing output"

    if ref_batch_line != out_line:
        return "batch lines do not match"

    return None

def compare_lines(ref_line, out_line):
    m = command_error_re.match(ref_line)
    if m:
        ref_line = m.group(1) + m.group(2)
    m = command_error_re.match(out_line)
    if m:
        out_line = m.group(1) + m.group(2)
    return ref_line == out_line

def anonymize_line(line):
    line = replace_dir_re.sub('./bd_dt/', line)
    line = hex_val_re.sub('hexval', line)
    line = lwp_val_re.sub('LWP', line)
    m = interrupted_proc_out_re.match(line)
    if m:
        line = m.group(1)
    return line
    

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

def compare_files(ref_file_path, out_file_path):
    ref_conn = ''

    with open(ref_file_path, "r") as ref, open(out_file_path) as out:
        outline_num = 0
        for outline in out:
            outline_num += 1
            outline = anonymize_line(outline.rstrip())

            # input line for the batch file
            out_batch_line = is_batch(outline)
            if out_batch_line != None:
                refline = next_ref_line(ref)
                global indeterminate; indeterminate = False
                if refline == None:
                    return (False, "extra output at " + str(outline_num))
                cmp = compare_batch_lines(refline, out_batch_line)
                if cmp:
                    return (False, cmp + " at " + str(outline_num))
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
                            return (False, "extra output at " + str(outline_num))
                        rproc = is_procset(refline)
                        if rproc:
                            if procRefs.get(rproc) == None:
                                procRefs[rproc] = []
                            procRefs[rproc].append(refline)
                        else:
                            return (False, "did not find match for " + outline + " at " + str(outline_num))

                    refline = procRefs[proc][procIdx]
                    pc_cmp = compare_procset_lines(refline, outline)
                    if pc_cmp == False:
                        procIdx += 1
                    elif pc_cmp == True:
                        procRefs[proc].pop(procIdx)
                        matched = True
                    else:
                        procRefs[proc][procIdx] = pc_cmp
                        matched = True
                if not matched:
                    return (False, "mismatch in return at " + str(outline_num))
                continue

            # connection line
            connected = is_connected(outline)
            if connected:
                while not ref_conn or int(ref_conn) < int(connected):
                    refline = next_ref_line(ref)
                    if refline == None:
                        return (False, "extra output at " + str(outline_num))
                    ref_conn = is_connected(refline)
                    if not ref_conn:
                        return (False, "did not find connection line at " + str(outline_num))
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
                return (False, "extra output at " + str(outline_num))
            if not compare_lines(refline, outline):
                return (False, "mismatch at " + str(outline_num))

    return (True, ' ')


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
def makeRunFile(test, additional_substitutions = {}):
    testName = test.__class__.__name__
    tmpdir = getTmpDir()
    rcfile = getTxt(test,testName)
    if (not rcfile):
        test.fail("could not get rc file")
    runfile = tmpdir + '/' + testName + ".rc"

    substitutions = {
        r"%%launcher_args%%" : lambda: os.environ["CDST_TESTS_LAUNCHER_ARGS"]
    }

    rcline = re.compile(r"^\d+: (.*)")

    with open(runfile,"w") as run:
        for line in open(rcfile, "r"):
            m = rcline.match(line)
            if not m:
                continue
            line = m.group(1) + "\n"

            for repl, substitution in additional_substitutions.items():
                line = line.replace(repl, substitution())

            for repl, substitution in substitutions.items():
                line = line.replace(repl, substitution())

            # hack: on some systems, specifying --launcher-args="" will break the launcher.
            line = line.replace("--launcher-args=\"\"", "")

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

    subprocess.call(['gdb4hpc', '-s', '-e', '-b', runfile, '--output', outfile, '--keep-going'])

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

class c_segv(Test):
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

        # build substitution dict to inject appid into .rc file
        subs = {
            r"%%appid%%" : lambda: app_id
        }

        # start gdb4hpc, attach, and generate results
        runScript(self, subs)
        result, error = compare_files(getTxt(self, self.__class__.__name__), getOut(self, self.__class__.__name__))
        self.assertTrue(result, error)

