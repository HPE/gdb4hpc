#!/bin/bash

version=`ls /cray/css/ulib/gdb4hpc/install_testing/sles15/x86_64 | grep gdb4hpc- | cut -d'-' -f2`
name=ulib
pass=OMGwapita55!
server=$1

/usr/bin/expect<<EOD
#set name ulib
#set pass OMGwapita55!
#set server [lindex $argv 0]
set timeout 500
set exp_internal 1
set prompt "$name@$server ~ $ "

spawn ssh -o StrictHostKeyChecking=no $name@$server
match_max 100000
expect "*?assword:*"
send "$pass\r"
expect $prompt
send -- "cd /cray/css/ulib/gdb4hpc/install_testing/sles15/x86_64\r"
expect  $prompt
send -- "module use /cray/css/ulib/gdb4hpc/install_testing/sles15/x86_64/modulefiles\r"
expect  $prompt
send -- "module load gdb4hpc/$version;module load craype-broadwell\r"
expect  $prompt
send -- "cd /cray/css/ulib/gdb4hpc/tests/test_harness; ./test_script.sh sanity_list build\r"
expect  $prompt
send -- "./test_script.sh sanity_list run\r"
expect  $prompt
expect "exit\r"
expect eof
EOD

