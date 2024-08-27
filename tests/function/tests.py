'''
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP.
'''

###########################################################################################
### gdb4hpc function tests
#
# This is the top level driver for running the gdb4hpc function tests.  The test information
# is defined in 'tests.py.data/<testname>.in' which defines both the input and output.  The process
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

import avocado
from avocado import Test
from avocado.utils import process as avproc

import sys
import glob
import socket
import platform
import time
import signal
import shutil
import subprocess
import os
import time
import re

import libtests as lt

try:
    from cdst_test.useful import detect_wlm
    import cdst_test.useful as useful
    import cdst_test.modules as mods
except ModuleNotFoundError:
    sys.path.insert(0, os.getcwd() + "/cdst-test")
    from cdst_test.useful import detect_wlm
    import cdst_test.useful as useful
    import cdst_test.modules as mods 

def should_skip_fortran_tests():
    # The current (as recent as cce 14.0.0.1) ftn compiler's fortran debug info
    # is half-broken and missing variables in certain places. Until that is fixed,
    # we'll just skip tests affected by it.
    loaded = mods.list("cce")
    if "cce/14.0.0" in loaded or "cce/14.0.0.1" in loaded or "cce/14.0.0.x" in loaded:
        # broken versions
        return True        
    else:
        # assume anything else has correct symbols
        return False

def onlyMpiSupported():
    wlm = detect_wlm()
    return wlm != "SLURM" and wlm != "LOCALHOST"

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

def getTxt(test, name: str):
    filename = name + ".in"
    path = test.get_data(filename, must_exist = True)
    if not path:
        test.cancel("Couldn't get .in file: " + str(filename))
    return path

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
    tmpdir = getTmpDir()
    rcfile = getTxt(test)
    if (not rcfile):
        test.cancel("could not get rc file")
    penv = "" if test.penv == "CRAY" else ("." + test.penv)
    extra = ("." + test.extra_tag) if test.extra_tag else ""
    runfile = tmpdir + '/' + test.base_name + penv + extra + ".rc"

    launch_re = re.compile(r"(launch .+?{(\d+)} *)")

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

            launch_m = launch_re.match(line)
            if launch_m:
                # build up launcher args from 3 sources:
                # 1. CDST_TESTS_LAUNCHER_ARGS
                # 2. --launcher-args in .in file
                # 3. -g in .in file
                launch_args_list = []

                env_launch_args = os.environ.get("CDST_TESTS_LAUNCHER_ARGS")
                if env_launch_args:
                    launch_args_list.append(env_launch_args)

                in_launch_args_m = re.search('--launcher-args="(.+?)"', line)
                if in_launch_args_m:
                    in_launch_args = in_launch_args_m.group(1)
                    line = line.replace(in_launch_args_m.group(0), "")
                    launch_args_list.append(in_launch_args)

                in_g_launch_args_m = re.search('-g "(.+?)"', line)
                if in_g_launch_args_m:
                    in_g_launch_args = in_g_launch_args_m.group(1)
                    line = line.replace(in_g_launch_args_m.group(0), "")
                    launch_args_list.append(in_g_launch_args)

                all_launch_args = " ".join(launch_args_list)
                if all_launch_args:
                    num_ranks = int(launch_m.group(2))
                    # if not on a slurm machine, translate launch args
                    local_launch_args = lt.convert_slurm_launcher_args(num_ranks, all_launch_args, detect_wlm())
                    line = launch_re.sub(r"\1" + '--launcher-args="' + local_launch_args + '" ', line)

            # launch binary from current programming environment
            line = re.sub(r"bin/CRAY/", f"bin/{test.penv}/", line)
            for repl, substitution in substitutions:
                print('substitute "%s" with "%s" on "%s"' % (repl, substitution,line))
                line = line.replace(repl, substitution)

            run.write(line)

    return runfile

def getTxt(test):
    filename = test.base_name + ".in"
    path = test.get_data(filename, must_exist = True)
    if not path:
        test.cancel("Couldn't get .in file: " + str(filename))
    return path

def getOut(test):
    out = useful.getOut(test, test.base_name)

    if test.proto is not None:
        # remove ".out"
        unaltered = out[:-4]
        out = unaltered + f".{test.proto}.out"

    if test.penv != "CRAY":
        # remove ".out"
        unaltered = out[:-4]
        out = unaltered + f".{test.penv}.out" 

    if test.extra_tag:
        # remove ".out"
        unaltered = out[:-4]
        out = unaltered + f".{test.extra_tag}.out" 

    return out

'''
Run the gdb4phc batch script for a test. Throws subprocess.TimeoutExpired on test timeout
'''
def runScript(test, substitutions = (), interpreter = "console"):
    runfile = makeRunFile(test, substitutions)
    outfile = getOut(test)

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
        test.cancel(f"Invalid timeout values: abs: {absolute_timeout}, timeout: {timeout}")
    print(f"timeout: {timeout}, cleanup: {cleanup_time}")
    
    try:
        args = [
            'gdb4hpc',
            '--silent',
            '--no-init',
            '--echo',
            '--keep-going',
            '--command-timeout', '120',
            '--batch', runfile,
            '--output', outfile
        ]
        if interpreter != "console":
            args.append(f"-i={interpreter}")
        p = subprocess.Popen(args)
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
        
        # throw to caller
        raise

'''
Driver for generating the gdb4hpc run script, running gdb4hpc, and comparing the output
against the saved reference file.
'''
def runTest(test, substitutions = (), interpreter = "console"):
    # Do prep for non-CRAY PrgEnv test
    if test.penv != "CRAY":
        # Load PrgEnv to ensure PrgEnv-specific libraries link correctly.
        #
        # Loading PrgEnvs usually puts /opt/cray/pe/lib64:/opt/cray/lib64
        # onto the front of LD_LIBRARY_PATH. This is a problem if
        # we have a non-default version of CTI loaded to test against.
        # The system default CTI library in lib64 will replace the library we
        # are trying to test gdb4hpc with. To dodge this, we save any parts
        # of the LD_LIBRARY_PATH that refer to CTI and restore them.

        cti_ld_paths = [
            p for p in os.environ["LD_LIBRARY_PATH"].split(":") if "/cti/" in p
        ]

        if not mods.invoke_successfully("load", f"PrgEnv-{test.penv.lower()}"):
            test.cancel(f"Failed to load PrgEnv-{test.penv.lower()}")

        ld_path = ":".join(cti_ld_paths) + ":" + os.environ["LD_LIBRARY_PATH"]
        os.environ["LD_LIBRARY_PATH"] = ld_path

    # Run gdb4hpc until completion or timeout
    try:
        runScript(test, substitutions, interpreter)
    except (subprocess.TimeoutExpired, avocado.core.exceptions.TestTimeoutInterrupted):
        if not lt.all_launches_successful(getOut(test)):
            # 90% of timeouts are caused by system problems preventing a launch
            # from completing. This means that the test can't test what it was
            # designed for and it shoudln't report a failure.
            test.cancel("Test timed out while launching job")

        # if we didn't find a failed launch, this time out could be a real issue
        test.fail("Timed out at some point after successful launch")

    if not os.path.exists(getTxt(test)):
        test.cancel(f"Missing .in file {getTxt(test)}")

    if not os.path.exists(getOut(test)):
        test.cancel(f"Missing .out file {getOut(test)}")

    # main result check
    result, error = lt.compare_files(getTxt(test), getOut(test))
    test.assertTrue(result, error)

    # If we passed but the test contained some ???PE-XXXXX commands, report them
    if lt.suppressedCheck:
        test.cancel("suppressed checks: " + str(lt.suppressedCheck))

'''
Specialized driver for running a gdb4hpc script with a gpu.
'''
def runGpuTest(test, gpu):
    hostname = socket.gethostname() 
    gpuScript = "./tests.py.data/" + gpu + "/" + hostname + ".sh"
    if not os.path.exists(gpuScript):
        test.cancel("No " + gpuScript + " script")

    res = subprocess.run([gpuScript, 'args'], stdout=subprocess.PIPE)
    gpuArgs = res.stdout.decode('utf-8').rstrip()
    substititions = ( ("<LAUNCH_ARGS>", gpuArgs), )
            
    runfile = makeRunFile(test, substititions)
    outfile = getOut(test)
 
    try:
        os.remove(outfile)
    except FileNotFoundError as e:
        # already gone
        pass

    print([gpuScript, 'launch','gdb4hpc', '-s', '-e', '-n', '-b', runfile, '--output', outfile, '--keep-going'])
    subprocess.call([gpuScript, 'launch','gdb4hpc', '-s', '-e', '-n', '-b', runfile, '--output', outfile, '--keep-going'])

    result, error = lt.compare_files(getTxt(test), getOut(test))
    test.assertTrue(result, error)
    
class Gdb4hpcTest(Test):
    def setUp(self):
        # Base name of test. Determines output files.
        # e.g. self.base_name = "c_assign" will use c_assign.in and generate
        # c_assign.out. The proto and penv variables below will change the
        # name of the .out file further.
        self.base_name = None

        # Overwrite in test to use binary compiled by different compiler
        self.penv = "CRAY"

        # Overwrite in test to use alternate protocol
        self.proto = "zmqnet"

        # Overwrite in test to add extra info to output filename
        self.extra_tag = None

        # set startup timeout env var to speed up failures
        if not os.getenv("GDB4HPC_STARTUP_TIMEOUT"):
            os.environ["GDB4HPC_STARTUP_TIMEOUT"] = "30" # seconds

    def run_under_mrnet(self):
        os.environ["GDB4HPC_PROTO"] = "mrnet"
        self.proto = "mrnet"

class UnitTests(Gdb4hpcTest):
    def run_unit_tests(test: Gdb4hpcTest, binary: str):
        p = subprocess.run([binary, "--has-tests"], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        if p.returncode == 0:
            p = subprocess.run([binary, "--test"], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
            if p.returncode != 0:
                with open(getOut(test), "w") as f:
                    f.write(p.stdout.decode())
                test.fail("Failed unit tests")
        else:
            test.cancel("Not compiled with unit tests")

    def test_gdb4hpc(self):
        self.run_unit_tests("gdb4hpc")

    def test_dbgsrv(self):
        dbgsrv = os.path.dirname(shutil.which("gdb4hpc")) + "/../libexec/dbgsrv"
        self.run_unit_tests(dbgsrv)

class FunctionTests(Gdb4hpcTest):
    def test_c_assign(self):
        self.base_name = "c_assign"
        runTest(self)

    def test_c_assign_decomp(self):
        self.base_name = "c_assign_decomp"
        runTest(self)

    def test_c_breakpoint(self):
        self.base_name = "c_breakpoint"
        runTest(self)

    def test_c_breakpoint_save(self):
        self.base_name = "c_breakpoint_save"
        runTest(self)

    @avocado.skipUnless(lambda t: t.params.get("mrnet_flag", default=False), "mrnet_flag not set")
    def test_c_breakpoint_mrnet(self):
        self.base_name = "c_breakpoint"
        self.run_under_mrnet()
        runTest(self)

    def test_c_breakpoint_pending(self):
        self.base_name = "c_breakpoint_pending"
        runTest(self)
        
    def test_c_cmp_false(self):
        self.base_name = "c_cmp_false"
        runTest(self)
        
    def test_c_cmp_true(self):
        self.base_name = "c_cmp_true"
        runTest(self)
        
    def test_c_defset(self):
        self.base_name = "c_defset"
        runTest(self)
        
    def test_c_finish(self):
        self.base_name = "c_finish"
        runTest(self)
        
    def test_c_focus(self):
        self.base_name = "c_focus"
        runTest(self)
        
    def test_c_frame(self):
        self.base_name = "c_frame"
        runTest(self)

    @avocado.skipUnless(lambda t: t.params.get("mrnet_flag", default=False), "mrnet_flag not set")
    def test_c_frame_mrnet(self):
        self.base_name = "c_frame"
        self.run_under_mrnet()
        runTest(self)
        
    def test_c_gdbmode(self):
        self.base_name = "c_gdbmode"

        # before running the test, copy gdb-guard to ./tmp with a different name in order
        # to test --gdb launcher arg

        gdb4_root = os.getenv("GDB4HPC_INSTALL_DIR")
        if gdb4_root is None:
            self.cancel("Couldn't find gdb4hpc install dir")

        guard = f"{gdb4_root}/libexec/gdb-guard"

        shutil.copy2(guard, "./tmp/custom_gdb")

        data_dir = './tmp/custom_data_dir'
        shutil.rmtree(data_dir, ignore_errors=True)
        shutil.copytree(os.environ['GDB4HPC_INSTALL_DIR'] + '/share/gdb', data_dir)

        runTest(self)
        
    def test_c_help(self):
        self.base_name = "c_help"
        runTest(self)
        
    def test_c_info(self):
        self.base_name = "c_info"
        runTest(self)
        
    def test_c_kill(self):
        self.base_name = "c_kill"
        runTest(self)

    def test_c_list(self):
        self.base_name = "c_list"
        subs = ( ("CURRENT_DIRECTORY", os.getcwd()), )
        runTest(self, subs)
        
    def test_c_next(self):
        self.base_name = "c_next"
        runTest(self)
        
    def test_c_print(self):
        self.base_name = "c_print"
        runTest(self)

    @avocado.skipUnless(lambda t: t.params.get("prgenvs_flag", default=False), "prgenvs_flag not set")
    @avocado.skipUnless(os.path.isfile(f"./bin/GNU/c/c_type_test.x"), "Missing binary")
    def test_c_print_gnu(self):
        self.base_name = "c_print"
        self.penv = "GNU"
        runTest(self)

    @avocado.skipUnless(lambda t: t.params.get("prgenvs_flag", default=False), "prgenvs_flag not set")
    @avocado.skipUnless(os.path.isfile(f"./bin/AMD/c/c_type_test.x"), "Missing binary")
    def test_c_print_amd(self):
        self.base_name = "c_print"
        self.penv = "AMD"
        runTest(self)

    @avocado.skip("PE-43876")
    @avocado.skipUnless(lambda t: t.params.get("prgenvs_flag", default=False), "prgenvs_flag not set")
    @avocado.skipUnless(os.path.isfile(f"./bin/NVIDIA/c/c_type_test.x"), "Missing binary")
    def test_c_print_nvidia(self):
        self.base_name = "c_print"
        self.penv = "NVIDIA"
        runTest(self)

    @avocado.skipUnless(lambda t: t.params.get("prgenvs_flag", default=False), "prgenvs_flag not set")
    @avocado.skipUnless(os.path.isfile(f"./bin/INTEL/c/c_type_test.x"), "Missing binary")
    def test_c_print_intel(self):
        self.base_name = "c_print"
        self.penv = "INTEL"
        runTest(self)

    def test_c_release(self):
        self.base_name = "c_release"
        runTest(self)
        
    def test_c_set_unset(self):
        self.base_name = "c_set_unset"
        runTest(self)
        
    def test_c_show(self):
        self.base_name = "c_show"
        runTest(self)
        
    def test_c_sizeof(self):
        self.base_name = "c_sizeof"
        runTest(self)
        
    def test_c_step(self):
        self.base_name = "c_step"
        runTest(self)
        
    def test_c_up_down(self):
        self.base_name = "c_up_down"
        runTest(self)
        
    def test_c_viewset(self):
        self.base_name = "c_viewset"
        runTest(self)
        
    def test_c_whatis(self):
        self.base_name = "c_whatis"
        runTest(self)

    def test_c_segv(self):
        self.base_name = "c_segv"
        runTest(self)

    def test_c_half_exit(self):
        self.base_name = "c_half_exit"
        runTest(self)

    def test_c_signal(self):
        self.base_name = "c_signal"
        runTest(self)

    @avocado.skipIf(detect_wlm() != "SLURM", "Not slurm")
    def test_c_nolib(self):
        self.base_name = "c_nolib"
        runTest(self)

    @avocado.skipIf(detect_wlm() != "SLURM", "Not slurm")
    def test_c_mpmd(self):
        self.base_name = "c_mpmd"
        runTest(self)

    @avocado.skipIf(not os.path.isfile("bin/CRAY/c/c_ompd.x") , "No OMPD")
    def test_c_ompd(self):
        self.base_name = "c_ompd"
        runTest(self)

    @avocado.skipIf(not os.path.isfile("bin/CRAY/c/c_shmem.x") , "No shmem")
    def test_c_shmem(self):
        self.base_name = "c_shmem"
        runTest(self)

    def test_cpp_t1(self):
        self.base_name = "cpp_t1"
        runTest(self)
        
    def test_cpp_t2(self):
        self.base_name = "cpp_t2"
        runTest(self)
        
    def test_cpp_t3(self):
        self.base_name = "cpp_t3"
        runTest(self)
        
    def test_cpp_ds1(self):
        self.base_name = "cpp_ds1"
        runTest(self)
        
    @avocado.skipIf(should_skip_fortran_tests(), "Broken ftn compiler")
    def test_ftn_t1(self):
        self.base_name = "ftn_t1"
        runTest(self)
        
    @avocado.skipIf(should_skip_fortran_tests(), "Broken ftn compiler")
    def test_ftn_t2(self):
        self.base_name = "ftn_t2"
        runTest(self)
        
    @avocado.skipIf(should_skip_fortran_tests(), "Broken ftn compiler")
    def test_ftn_t3(self):
        self.base_name = "ftn_t3"
        runTest(self)

    def test_assert_scalars(self):
        self.base_name = "assert_scalars"
        runTest(self)

    def test_assert_arrays(self):
        self.base_name = "assert_arrays"
        runTest(self)

    @avocado.skipUnless(lambda t: t.params.get("mrnet_flag", default=False), "mrnet_flag not set")
    def test_assert_arrays_mrnet(self):
        self.base_name = "assert_arrays"
        self.run_under_mrnet()
        runTest(self)

    def test_assert_decomp(self):
        self.base_name = "assert_decomp"
        runTest(self)

    def test_assert_deadlock(self):
        self.base_name = "assert_deadlock"
        runTest(self)

    def test_assert_interrupted(self):
        self.base_name = "assert_interrupted"
        runTest(self)

    def test_assert_errors(self):
        self.base_name = "assert_errors"
        runTest(self)

    def test_assert_expressions(self):
        self.base_name = "assert_expressions"
        runTest(self)

    def test_assert_stopstart(self):
        self.base_name = "assert_stopstart"
        runTest(self)

    def test_assert_twice(self):
        self.base_name = "assert_twice"
        runTest(self)

    def test_assert_invalidated(self):
        self.base_name = "assert_invalidated"
        runTest(self)

    def test_assert_save(self):
        self.base_name = "assert_save"
        runTest(self)

    def test_ftn_arrays(self):
        self.base_name = "ftn_arrays"
        runTest(self)
    
    @avocado.skipUnless(lambda t: t.params.get("mrnet_flag", default=False), "mrnet_flag not set")
    def test_ftn_arrays_mrnet(self):
        self.base_name = "ftn_arrays"
        self.run_under_mrnet()
        runTest(self)
    
    @avocado.skipUnless(lambda t: t.params.get("prgenvs_flag", default=False), "prgenvs_flag not set")
    @avocado.skipUnless(os.path.isfile(f"./bin/GNU/fortran/ftn_arrays.x"), "Missing binary")
    def test_ftn_arrays_gnu(self):
        self.base_name = "ftn_arrays"
        self.penv = "GNU"
        runTest(self)

    @avocado.skipUnless(lambda t: t.params.get("prgenvs_flag", default=False), "prgenvs_flag not set")
    @avocado.skipUnless(os.path.isfile(f"./bin/AMD/fortran/ftn_arrays.x"), "Missing binary")
    def test_ftn_arrays_amd(self):
        self.base_name = "ftn_arrays"
        self.penv = "AMD"
        runTest(self)
 
    @avocado.skip("PE-43876")
    @avocado.skipUnless(lambda t: t.params.get("prgenvs_flag", default=False), "prgenvs_flag not set")
    @avocado.skipUnless(os.path.isfile(f"./bin/NVIDIA/fortran/ftn_arrays.x"), "Missing binary")
    def test_ftn_arrays_nvidia(self):
        self.base_name = "ftn_arrays"
        self.penv = "NVIDIA"
        runTest(self)

    @avocado.skipUnless(lambda t: t.params.get("prgenvs_flag", default=False), "prgenvs_flag not set")
    @avocado.skipUnless(os.path.isfile(f"./bin/INTEL/fortran/ftn_arrays.x"), "Missing binary")
    def test_ftn_arrays_intel(self):
        self.base_name = "ftn_arrays"
        self.penv = "INTEL"
        runTest(self)

    def test_cpp_arrays_compare(self):
        self.base_name = "cpp_arrays_compare"
        runTest(self)

    def test_cpp_arrays_chunk(self):
        self.base_name = "cpp_arrays_chunk"
        os.environ["GDB4HPC_BLOCK_ELEMENTS"] = "4"
        runTest(self)

    def test_cpp_decomp(self):
        self.base_name = "cpp_decomp"
        runTest(self)

    def test_ftn_decomp(self):
        self.base_name = "ftn_decomp"
        runTest(self)

    def test_cpp_decomp2(self):
        self.base_name = "cpp_decomp2"
        runTest(self)

    def test_decomp_save(self):
        self.base_name = "decomp_save"
        runTest(self)

    def test_cpp_bt(self):
        self.base_name = "cpp_bt"
        runTest(self)

    def test_cpp_thread_funcs(self):
        self.base_name = "cpp_thread_funcs"
        runTest(self)

    @avocado.skip("need to redo thread focus example to be more determinate")
    def test_cpp_thread_focus(self):
        self.base_name = "cpp_thread_focus"
        runTest(self)

    def test_cpp_arrays_print(self):
        self.base_name = "cpp_arrays_print"
        runTest(self)

    @avocado.skipUnless(lambda t: t.params.get("mrnet_flag", default=False), "mrnet_flag not set")
    def test_cpp_arrays_print_mrnet(self):
        self.base_name = "cpp_arrays_print"
        self.run_under_mrnet()
        runTest(self)

    @avocado.skipUnless(lambda t: t.params.get("prgenvs_flag", default=False), "prgenvs_flag not set")
    @avocado.skipUnless(os.path.isfile(f"./bin/GNU/C++/cpp_arrays_print.x"), "Missing binary")
    def test_cpp_arrays_print_gnu(self):
        self.base_name = "cpp_arrays_print"
        self.penv = "GNU"
        runTest(self)

    @avocado.skipUnless(lambda t: t.params.get("prgenvs_flag", default=False), "prgenvs_flag not set")
    @avocado.skipUnless(os.path.isfile(f"./bin/AMD/C++/cpp_arrays_print.x"), "Missing binary")
    def test_cpp_arrays_print_amd(self):
        self.base_name = "cpp_arrays_print"
        self.penv = "AMD"
        runTest(self)
 
    @avocado.skip("PE-43876")
    @avocado.skipUnless(lambda t: t.params.get("prgenvs_flag", default=False), "prgenvs_flag not set")
    @avocado.skipUnless(os.path.isfile(f"./bin/NVIDIA/C++/cpp_arrays_print.x"), "Missing binary")
    def test_cpp_arrays_print_nvidia(self):
        self.base_name = "cpp_arrays_print"
        self.penv = "NVIDIA"
        runTest(self)

    @avocado.skipUnless(lambda t: t.params.get("prgenvs_flag", default=False), "prgenvs_flag not set")
    @avocado.skipUnless(os.path.isfile(f"./bin/INTEL/C++/cpp_arrays.print.x"), "Missing binary")
    def test_cpp_arrays_print_intel(self):
        self.base_name = "cpp_arrays_print"
        self.penv = "INTEL"
        runTest(self)

    @avocado.skipUnless(os.path.isfile("./bin/CRAY/cti_launch.x"), "Missing cti_launch.x")
    def test_cpp_attach(self):
        self.base_name = "cpp_attach"
        wlm = detect_wlm()

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
            self.cancel("Failed to detect WLM")

        # launch job
        job = subprocess.Popen(
            [f"./bin/CRAY/cti_launch.x", *rank_args, f"./bin/CRAY/C++/cpp_attach.x", "60"],
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
        try:
            runScript(self, subs)
        except subprocess.TimeoutExpired:
            self.cancel("Timed out")

        result, error = lt.compare_files(getTxt(self), getOut(self))
        self.assertTrue(result, error)

    def test_cpp_catch(self):
        self.base_name = "cpp_catch"
        runTest(self)

    NOCHECKPOINT_FLAG = "./avocado-virtual-environment/nocheckpoint"
 
    @avocado.skipIf(os.path.exists(NOCHECKPOINT_FLAG),
        f"Determined that checkpoints are unsuppored via {NOCHECKPOINT_FLAG}")
    def test_c_checkpoint0(self):
        self.base_name = "c_checkpoint0"
        # some systems can't support checkpoints because of multithreaded MPI implementations.
        # this test serves as a meta-test to see if the system can support checkpoints. if
        # it can, we run the full checkpoint tests below.

        # we need to drop a file onto disk to communicate with other tests.
        # this is because avocado runs each test as a new process, so
        # global variables or environment variables wouldn't work.

        # we invert the logic of the flag in case the user runs the full checkpoint tests
        # without running this one first. the full checkpoint tests should always
        # be run unless we know for sure that they can't be.

        try:
            runScript(self)
        except subprocess.TimeoutExpired:
            self.cancel("Timed out")

        with open(getOut(self), "r") as out:
            data = out.read()
            if "Debugger error: checkpoint: can't checkpoint multiple threads." in data:
                print("Checkpoints unsupported")
                open(self.NOCHECKPOINT_FLAG, "a").close()
                self.cancel(f"Determined that checkpoints are unsupported")
            elif "Created checkpoint 1" in data:
                print("Checkpoints supported")

        # this test always passes

    @avocado.skipIf(lambda t: os.path.exists(t.NOCHECKPOINT_FLAG),
        "System doesn't support gdb checkpoints")
    def test_c_checkpoint1(self):
        self.base_name = "c_checkpoint1"
        runTest(self)

    @avocado.skipIf(lambda t: os.path.exists(t.NOCHECKPOINT_FLAG),
        "System doesn't support gdb checkpoints")
    def test_c_checkpoint2(self):
        self.base_name = "c_checkpoint2"
        runTest(self)

    @avocado.skipIf(not os.path.exists("./tests.py.data/rocm/" + socket.gethostname() + ".sh"))
    def test_rocmGpu(self):
        self.base_name = "rocmGpu"
        runGpuTest(self, "rocm")

    @avocado.skipIf(not os.path.exists("./tests.py.data/rocm/" + socket.gethostname() + ".sh"))
    def test_ftn_rocmGpu(self):
        self.base_name = "ftn_rocmGpu"
        runGpuTest(self, "rocm")

    @avocado.skipIf(not os.path.exists("./tests.py.data/cuda/" + socket.gethostname() + ".sh"))
    def test_cudaGpu(self):
        self.base_name = "cudaGpu"
        runGpuTest(self, "cuda")

    def test_c_run(self):
        self.base_name = "c_run"
        runTest(self)

    @avocado.skipIf(detect_wlm() != "PALS", "Not PALS")
    def test_c_run_not_mpi(self):
        self.base_name = "c_run_not_mpi"
        runTest(self)

    @avocado.skipIf(detect_wlm() != "SLURM", "Not slurm")
    def test_sbatch(self):
        self.base_name = "sbatch"

        launch_args = os.environ.get("CDST_TESTS_LAUNCHER_ARGS")

        # write sbatch file
        with open(getTmpDir() + "/sbatch.sh", "w") as batchf:
            batchf.write("#!/bin/bash\n")

            batchf.write("#SBATCH -n2\n")
            batchf.write("#SBATCH -N2\n")

            if launch_args:
                for arg in launch_args.split():
                    batchf.write(f"#SBATCH {arg}\n")

            batchf.write("srun ./bin/CRAY/C++/cpp_attach.x 60\n")

        runTest(self)

    def test_source(self):
        self.base_name = "source"
        runTest(self)

    def test_command_timeout(self):
        self.base_name = "command_timeout"
        runTest(self)

    def test_source(self):
        self.base_name = "source"
        runTest(self)

    # these skip comments on the raja tests about "non-slurm" systems might really only
    # mean "ALPS" systems, but these tests haven't been run on HPCM or PALS yet, so
    # they'll stay slurm-only for now.
    @avocado.skipIf(onlyMpiSupported(), "RAJA test programs are not MPI programs. Non-MPI launches are unsupported on non-slurm systems")
    def test_raja_views(self):
        if not os.path.exists(f"./bin/{self.penv}/raja/view-layout_solution.x"):
            self.cancel("RAJA view-layout_solution.x not built")
        self.base_name = "raja_views"
        runTest(self)

    @avocado.skipIf(onlyMpiSupported(), "RAJA test programs are not MPI programs. Non-MPI launches are unsupported on non-slurm systems")
    def test_raja_tile(self):
        if not os.path.exists(f"./bin/{self.penv}/raja/kernel-matrix-transpose-local-array_solution.x"):
            self.cancel("RAJA kernel-matrix-transpose-local-array_solution.x not built")
        self.base_name = "raja_tile"
        runTest(self)

    @avocado.skipIf(onlyMpiSupported(), "RAJA test programs are not MPI programs. Non-MPI launches are unsupported on non-slurm systems")
    def test_raja_tile2(self):
        # the tile method in launch-matrix-transpose-local-array_solution is blocked by
        # PE-47549.
        self.cancel("PE-47549")
        if not os.path.exists(f"./bin/{self.penv}/raja/launch-matrix-transpose-local-array_solution.x"):
            self.cancel("RAJA launch-matrix-transpose-local-array_solution.x not built")
        self.base_name = "raja_tile2"
        runTest(self)

    @avocado.skipIf(onlyMpiSupported(), "RAJA test programs are not MPI programs. Non-MPI launches are unsupported on non-slurm systems")
    def test_raja_reductions(self):
        if not os.path.exists(f"./bin/{self.penv}/raja/reductions_solution.x"):
            self.cancel("RAJA reductions_solution.x not built")
        self.base_name = "raja_reductions"
        runTest(self)

    # this binary *is* an mpi app so we can run it on any wlm
    def test_raja_decomp(self):
        if not os.path.exists(f"./bin/{self.penv}/raja/view-layout-decomp_solution.x"):
            self.cancel("RAJA view-layout-decomp_solution.x not built")
        self.base_name = "raja_decomp"
        runTest(self)

    @avocado.skipIf(onlyMpiSupported(), "non-mpi not supported on wlm")
    def test_kokkos(self):
        self.base_name = "kokkos"
        if not os.path.exists(f"./bin/{self.penv}/kokkos/TeamVectorLoop.host"):
            self.cancel("kokkos executable is not available")

        runTest(self)

    def test_save_all(self):
        self.base_name = "save_all"
        runTest(self)

    def test_mi_backtrace(self):
        self.base_name = "mi_backtrace"
        runTest(self, interpreter="mi")

    def test_mi_basics_prompt(self):
        self.base_name = "mi_basics_prompt"
        runTest(self, interpreter="mi")

    def test_mi_basics_results(self):
        self.base_name = "mi_basics_results"
        runTest(self, interpreter="mi")

    def test_mi_basics_errors(self):
        self.base_name = "mi_basics_errors"
        runTest(self, interpreter="mi")

    def test_mi_basics_prefixes(self):
        self.base_name = "mi_basics_prefixes"
        runTest(self, interpreter="mi")

    def test_mi_basics_fallback(self):
        self.base_name = "mi_basics_fallback"
        runTest(self, interpreter="mi")

    def test_mi_basics_exec(self):
        self.base_name = "mi_basics_exec"
        runTest(self, interpreter="mi")

    def test_mi_basics_console_exec(self):
        self.base_name = "mi_basics_console_exec"
        # this test does *not* use an mi interpreter.
        # it tests interpreter-exec mi from a console interpreter.
        runTest(self)

    def test_mi_ccdb_breakpoint(self):
        self.base_name = "mi_ccdb_breakpoint"
        runTest(self, interpreter="mi")

    def test_mi_ccdb_step_next_finish(self):
        self.base_name = "mi_ccdb_step_next_finish"
        runTest(self, interpreter="mi")

    def test_mi_ccdb_watch_mouse(self):
        self.base_name = "mi_ccdb_watch_mouse"
        runTest(self, interpreter="mi")

    def test_mi_ccdb_compare(self):
        self.base_name = "mi_ccdb_compare"
        runTest(self, interpreter="mi")

    def test_mi_ccdb_assert(self):
        self.base_name = "mi_ccdb_assert"
        runTest(self, interpreter="mi")

    def test_completions_nolaunch(self):
        self.base_name = "completions_nolaunch"
        # make dummy filesystem to test filesystem completions
        os.makedirs(getTmpDir() + "/completions_fs", exist_ok=True)
        open(getTmpDir() + "/completions_fs/c_inc.x", "a").close()
        open(getTmpDir() + "/completions_fs/c_inc.o", "a").close()
        runTest(self)

    def test_completions_launch(self):
        self.base_name = "completions_launch"
        runTest(self)

    def test_launch_from_path(self):
        self.base_name = "launch_from_path"
        os.environ["PATH"] = f"{os.environ['PATH']}:{os.getcwd()}/bin/{self.penv}/c"
        runTest(self)

    def test_positional_args(self):
        self.base_name = "positional_args"
        runTest(self)

    def test_srun_simple(self):
        # this test can be run on any WLM since gdb4hpc will translate basic srun options to any WLM
        self.base_name = "srun_simple"
        runTest(self)

    @avocado.skipIf(detect_wlm() != "SLURM", "Not slurm")
    def test_srun_twice(self):
        # this test uses slurm-specific syntax so it must be run on slurm
        self.base_name = "srun_twice"
        runTest(self)

# given "Python 3.11.3" return "3.11"
def parse_python_minor_version(full_vers: str) -> str:
    # if caller's `which python3` failed
    if len(full_vers) == 0:
        return ""

    # assume "Python 3.xx.yy"
    vers = full_vers[full_vers.find("3") : full_vers.rfind(".")]
    return vers

class PythonTest(Gdb4hpcTest):
    def setUp(self):
        super().setUp()

        self.python_version = self.params.get("python_version", default="3.6")
        self.extra_tag = self.python_version

        # try to find a version of python that matches

        # does the default python3 in PATH match?
        # if no python3 is in path (for some reason?) this will just parse and
        # return an empty string which parse_python_minor_version handles just fine
        path_vers = parse_python_minor_version(
            subprocess.run(f"python3 --version", stdout=subprocess.PIPE, shell=True).stdout.decode().strip()
        )

        if path_vers == self.python_version:
            self.python_path = subprocess.run(f"which python3", stdout=subprocess.PIPE, shell=True).stdout.decode().strip()
            return

        # try cray python
        which = subprocess.run(f"module load cray-python/{self.python_version} && which python{self.python_version}", stdout=subprocess.PIPE, shell=True)
        if which.returncode == 0:
            self.python_path = which.stdout.decode().strip()
            return

        # try an explicitly versioned python binary
        which = subprocess.run(f"which python{self.python_version}", stdout=subprocess.PIPE, shell=True)
        if which.returncode == 0:
            self.python_path = which.stdout.decode().strip()
            return

        # give up :-(
        self.cancel(f"Could not locate a python{self.python_version} installation")


    # debugging the python interpreter doesn't work without special args on anything but slurm
    @avocado.skipIf(detect_wlm() != "SLURM", "Not slurm")
    def test_python_launch_halt_bt_active_nonstop(self):
        self.base_name = "python_launch_halt_bt_active_nonstop"
        subs = (
            ("%.*python3.*%", self.python_path),
        )
        runTest(self, substitutions = subs)

    # debugging the python interpreter doesn't work without special args on anything but slurm
    @avocado.skipIf(detect_wlm() != "SLURM", "Not slurm")
    def test_python_launch_halt_bt_active_allstop(self):
        self.base_name = "python_launch_halt_bt_active_allstop"
        subs = (
            ("%.*python3.*%", self.python_path),
        )
        runTest(self, substitutions = subs)

    # debugging the python interpreter doesn't work without special args on anything but slurm
    @avocado.skipIf(detect_wlm() != "SLURM", "Not slurm")
    def test_python_launch_halt_bt_dormant_nonstop(self):
        self.base_name = "python_launch_halt_bt_dormant_nonstop"
        subs = (
            ("%.*python3.*%", self.python_path),
        )
        runTest(self, substitutions = subs)

    # debugging the python interpreter doesn't work without special args on anything but slurm
    @avocado.skipIf(detect_wlm() != "SLURM", "Not slurm")
    def test_python_launch_halt_bt_dormant_allstop(self):
        self.base_name = "python_launch_halt_bt_dormant_allstop"
        subs = (
            ("%.*python3.*%", self.python_path),
        )
        runTest(self, substitutions = subs)

    @avocado.skipIf(detect_wlm() != "SLURM", "Not slurm")
    @avocado.skipUnless(os.path.isfile("./bin/CRAY/cti_launch.x"), "Missing cti_launch.x")
    def test_python_attach_halt_bt_active_allstop(self):
        self.base_name = "python_attach_halt_bt_active_allstop"

        if self.python_version == "3.6":
            # the only version <= 3.6 we test *is* 3.6 so we can get
            # away with simple string equality testing instead of fancy
            # version number range extraction etc.
            self.cancel("Attach is not supported <= 3.6")

        # launch job
        job = subprocess.Popen(
            [f"./bin/CRAY/cti_launch.x", self.python_path, "./src/python/spin.py", "300"],
            stderr=subprocess.PIPE
        )

        app_id = None
        # wait for job to start and report app id
        for line in iter(job.stderr.readline, b''):
            # first line printed will be the app id, and it will come back in bytes
            app_id = line.decode("utf-8").rstrip()
            break

        # wait a few seconds to make sure spin.py gets into its loop
        time.sleep(3)

        subs = ( ("%[0-9.]+%", app_id), )

        runTest(self, substitutions = subs)

    @avocado.skipIf(detect_wlm() != "SLURM", "Not slurm")
    @avocado.skipUnless(os.path.isfile("./bin/CRAY/cti_launch.x"), "Missing cti_launch.x")
    def test_python_attach_halt_bt_dormant_allstop(self):
        self.base_name = "python_attach_halt_bt_dormant_allstop"

        if self.python_version == "3.6":
            # the only version <= 3.6 we test *is* 3.6 so we can get
            # away with simple string equality testing instead of fancy
            # version number range extraction etc.
            self.cancel("Attach is not supported <= 3.6")

        # launch job
        job = subprocess.Popen(
            [f"./bin/CRAY/cti_launch.x", self.python_path, "./src/python/sleep.py", "300"],
            stderr=subprocess.PIPE
        )

        app_id = None
        # wait for job to start and report app id
        for line in iter(job.stderr.readline, b''):
            # first line printed will be the app id, and it will come back in bytes
            app_id = line.decode("utf-8").rstrip()
            break

        # wait a few seconds to make sure sleep.py gets into its loop
        time.sleep(3)

        subs = ( ("%[0-9.]+%", app_id), )

        runTest(self, substitutions = subs)

    @avocado.skipIf(detect_wlm() != "SLURM", "Not slurm")
    @avocado.skipUnless(os.path.isfile("./bin/CRAY/cti_launch.x"), "Missing cti_launch.x")
    def test_python_attach_detect_36(self):
        self.base_name = "python_attach_detect_36"

        if self.python_version != "3.6":
            self.cancel("This test is only relevant to 3.6")

        # launch job
        job = subprocess.Popen(
            [f"./bin/CRAY/cti_launch.x", self.python_path, "./src/python/spin.py", "300"],
            stderr=subprocess.PIPE
        )

        app_id = None
        # wait for job to start and report app id
        for line in iter(job.stderr.readline, b''):
            # first line printed will be the app id, and it will come back in bytes
            app_id = line.decode("utf-8").rstrip()
            break

        # wait a few seconds to make sure sleep.py gets into its loop
        time.sleep(3)

        subs = ( ("%[0-9.]+%", app_id), )

        runTest(self, substitutions = subs)

    # debugging the python interpreter doesn't work without special args on anything but slurm
    @avocado.skipIf(detect_wlm() != "SLURM", "Not slurm")
    def test_python_print(self):
        self.base_name = "python_print"
        subs = (
            ("%.*python3.*%", self.python_path),
        )
        runTest(self, substitutions = subs)

    # debugging the python interpreter doesn't work without special args on anything but slurm
    @avocado.skipIf(detect_wlm() != "SLURM", "Not slurm")
    def test_python_launch_print(self):
        self.base_name = "python_launch_print"
        subs = (
            ("%.*python3.*%", self.python_path),
        )
        runTest(self, substitutions = subs)

    # debugging the python interpreter doesn't work without special args on anything but slurm
    @avocado.skipIf(detect_wlm() != "SLURM", "Not slurm")
    def test_python_launch_break_bt_print(self):
        self.base_name = "python_launch_break_bt_print"
        subs = (
            ("%.*python3.*%", self.python_path),
        )
        runTest(self, substitutions = subs)

    # debugging the python interpreter doesn't work without special args on anything but slurm
    @avocado.skipIf(detect_wlm() != "SLURM", "Not slurm")
    def test_python_launch_break_set(self):
        self.base_name = "python_launch_break_set"
        subs = (
            ("%.*python3.*%", self.python_path),
        )
        runTest(self, substitutions = subs)

    # debugging the python interpreter doesn't work without special args on anything but slurm
    @avocado.skipIf(detect_wlm() != "SLURM", "Not slurm")
    def test_python_launch_step_next_finish(self):
        self.base_name = "python_launch_step_next_finish"
        subs = (
            ("%.*python3.*%", self.python_path),
        )
        runTest(self, substitutions = subs)

class c_commnode_fanout(Gdb4hpcTest):
    def getLogDir(self):
        return f"{os.path.dirname(os.path.abspath(__file__))}/mrnet_logs_{os.getpid()}"

    def setUp(self):
        super().setUp()
        # create mrnet log dir
        os.mkdir(self.getLogDir())
        self.run_under_mrnet()

    @avocado.skipUnless(lambda t: t.params.get("mrnet_flag", default=False), "mrnet_flag not set")
    def test(self):
        self.base_name = "c_commnode_fanout"
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

