

Instrutions for running lgdb test harness:

1. Checkout lgdb test harness from svn

2. There are 3 test lists. 
	1. sanity_list
	2. basic_list
	3. full_list
	4. gpu_list

3. Go to the Aesop website: htp://peperf/aesop/ and login.
4. At the top menu, select Experiments.
5. Select New Single Experiment.
6. Select Load a Saved Configuration.
7. From the tree, select Standard Configurations.
8. Select LGDB Regression Testing
9. Click on the Load button.

The test configurations should be as follows:
Environments
-PE Default
    -Module Managment
      -Pass -1 Load: load the lgdb module you want to test.
      or
      -Module use: load the dev lgdb module you want to test.

      If you are running gpu tests, ensure you load the correct
      accelerator module as well.

Tests / Benchmarks
-Select which set of tests you want to run via the drop-down
 list.
 - The lgdb test harness will divide up the NPES across the 
   number of tests run in each .rc file.  
   So if a given .rc will launch 2 executables, and you enter
   a 6 for npes in Aesop. Each test will run 3 npes.
   
   NOTE: As of 1/30/2015 .rc files launch at most 3 executables.
         Thus you should use a minimum of 6 npes. So each test
         will run 2 npes.
  
   If you are running the gpu_list to test gpu's, then esure
   you load the correct accelerator module under the Environments
   menu.

-Hosts / Resources
  - Select a host upon which to test from the drop-down menu.

  - Select the Resources / Run Arguments if you want to specify
    these.  

-Review & Submission
  - Review test configurations. 
    - You can save this configuration for
      future use by selecting the Save a copy of this configuration
      option button.  If you edit this configuration, you can save your
      edits by selecting the Update this configuration.  
    - Name your configuration in the textbox.
    - Select Submit Experiment.

To view status of the test, in the menu at the top
- Select Experiments
- Select Status
  - Under Recent Events - you can view build and test status seperately.
    Build is selected by default.

    You should be able to see the most recent test run at the top of the
    table, as well as its status.  To view the build log select the 
    Event Action dropdown button. 
    - Select View Event Log
      Error handling is not idea at this stage. But any error description
      will present in the gray box.  You can view the files specified in 
      by the Host Directory.
    - You can also requeue the build from the Event Action dropdown button.
    - You can also Modify/Fix Experiment from the Event Action dropdown
      button.  (Modify test configurations)

If the build completes succefully, you may need to select the Complete 
checkbox at the top of the Recent Events section of the page to view, as
completed builds/test are not selected by default.

To view test status, select Test under the Recent Events heading.
You can monitor the test status in much the same way as you can for the build.
If the test completes successfully, you can view the results from by selecting
Experiments at the top of the webpage. 
- Select Results
 -You should see your test in the Complete Experiments section of the Results
  page.  Select the Test Name to view detailed pass/fail information.

The log files are located in the Host Directory listed in the Aesop Event log.
e.g.
/lus/scratch/kdavis/pth/2015-01-26_16_02_56-LGDB_Test-21034/PE_Default/LGDB_Test_Harness-test104394
/1422288204_build201946/lgdb_test_harness.

lgdb_test_harness log files:
build_log.rep
test_log.rep
results_log.rep
aesop.rep (Aesop uses this to build the results table)

You can view aesop .ini and .pbs files in the Host Directory listed in the 
Aesop Event log.
e.g.
/lus/scratch/kdavis/pth/2015-01-26_16_02_56-LGDB_Test-21034/PE_Default/LGDB_Test_Harness-test104394

drwxr-xr-x 5 kdavis criemp     4096 Jan 29 16:39 ./
drwxr-xr-x 4 kdavis criemp     4096 Jan 26 10:08 ../
drwxr-xr-x 4 kdavis criemp     4096 Jan 26 10:08 1422288204-build201946/
-rw-r--r-- 1 kdavis criemp 34416360 Jan 26 10:08 1422288204-build201946.tar.gz
-rw-r--r-- 1 kdavis criemp      862 Jan 29 15:24 1422288520_aesop_params.ini
-rwxr-xr-x 1 kdavis criemp     1534 Jan 28 12:34 1422288521_aesop_batch.pbs*
-rw-r--r-- 1 kdavis criemp 33785025 Jan 26 10:08 1422288521-test201947.tar.gz
drwxr-xr-x 2 kdavis criemp     4096 Jan 26 10:08 bin/
drwx------ 2 kdavis criemp     4096 Jan 26 10:10 pth_node_data/
-rwx------ 1 kdavis criemp       96 Jan 26 10:10 pth_op.sh*
-rw------- 1 kdavis criemp    16154 Jan 29 15:24 pth_sysinfo_log
-rw------- 1 kdavis criemp     1055 Jan 26 10:10 PTHTLGDBTestHar.o200630
-rwxr-xr-x 1 kdavis criemp    39249 Jan 29 16:39 test_script*


kdavis@kaibab:/lus/scratch/kdavis/pth/2015-01-26_16_02_56-LGDB_Test-21034/PE_Default/LGDB_Test_Harness-test104394> cat 1422288520_aesop_params.ini
[test]
bucket=sanity_list
build_201946_bucket=sanity_list
build_201946_dir=1422288204-build201946
build_201946_lgdb_test_harness_conf_id=21
build_201946_npes=6
build_201946_state=0
build_201946_tarball=1422288204-build201946.tar.gz
build_201946_test_id=21034
dependencies=build_201946
exec_log=test_log.rep
exec_time=2756
executable=null
inputs_repository=/lus/scratch/aesop/pth_inputs
is_build=N
lgdb_test_harness_conf_id=21
npes=6
pat_ap2_file=null
pat_report_file=null
pat_xf_dir=null
results_file=lgdb_test_harness/aesop.rep
runner=aprun
runops=null
stage=testing
stage_tstart=1422563912
state=0
test_cmplt=1422566670
test_id=21034
test_start=1422563912
test_status=C
#!### AESOP TEST Parameter File - Created 2015-01-26 16:08:40 ###



kdavis@kaibab:/lus/scratch/kdavis/pth/2015-01-26_16_02_56-LGDB_Test-21034/PE_Default/LGDB_Test_Harness-test104394> cat 1422288521_aesop_batch.pbs
#!/bin/bash
#PBS -l walltime=2:00:00
#PBS -N PTHTLGDBTestHar
#PBS -j oe
#PBS -l select=6:ncpus=1:mpiprocs=1:vntype=cray_compute
#PBS -l place=scatter

cd /lus/scratch/kdavis/pth/2015-01-26_16_02_56-LGDB_Test-21034/PE_Default/LGDB_Test_Harness-test104394

. /opt/modules/default/init/bash
export OMP_NUM_THREADS=1


export PATH="${PATH}:/lus/scratch/kdavis/pth/2015-01-26_16_02_56-LGDB_Test-21034/PE_Default/LGDB_Test_Harness-test104394/bin"

eval module load cray-lgdb/2.4.0.7


export PTH_RDIR="/lus/scratch/kdavis/pth/2015-01-26_16_02_56-LGDB_Test-21034/PE_Default/LGDB_Test_Harness-test104394"
export PTH_SYSINFO="/lus/scratch/kdavis/pth/2015-01-26_16_02_56-LGDB_Test-21034/PE_Default/LGDB_Test_Harness-test104394/pth_sysinfo_log"
export PTH_BIN="/lus/scratch/kdavis/pth/2015-01-26_16_02_56-LGDB_Test-21034/PE_Default/LGDB_Test_Harness-test104394/bin"
export PTH_CC="cc"
export PTH_FC="ftn"
export PTH_CXX="CC"
export PTH_FFLAGS=""
export PTH_CFLAGS=""
export PTH_CXXFLAGS=""
export PTH_LDFLAGS=""
export PTH_LIBS=""


bin/pth_ini 1422288520_aesop_params.ini set test_start `date +'%s'`
bin/pth_ini 1422288520_aesop_params.ini set test_status R

### work for dependency directory 1422288204-build201946 ###
cd 1422288204-build201946
export PAT_RT_EXPFILE_DIR=`pwd`"/perftools"
mkdir -p perftools
../test_script $PTH_RDIR/1422288520_aesop_params.ini build_201946
cd ..

bin/pth_get_sysinfo `bin/pth_ini 1422288520_aesop_params.ini get executable` 6 > $PTH_SYSINFO
bin/pth_ini 1422288520_aesop_params.ini set test_cmplt `date +'%s'`
