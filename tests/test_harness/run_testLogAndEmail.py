#!/usr/bin/python

import os
import sys
import select
import paramiko
import time
import threading
import csv
import openpyxl
import datetime
import update_test_log

port=22
gdb4hpc_version = ""
user='ulib'
pwd='OMGwapita55!'
host='mars'
path = '/cray/css/ulib/gdb4hpc/tests/test_harness'
cmd1="cd /cray/css/ulib/gdb4hpc/install_testing/sles15/x86_64\n"
cmd2='module use /cray/css/ulib/gdb4hpc/install_testing/sles15/x86_64/modulefiles\n'
cmd3='module load'
cmd4='module load craype-broadwell\n'
cmd5='cd /cray/css/ulib/gdb4hpc/tests/test_harness\n'
cmd6='./test_script.sh sanity_list build\n'
cmd7='./test_script.sh sanity_list run\n'
strdata=''
fulldata=''

class Testing:


    def __init__(self, host, user, pwd):
        print "Running Functional Tests"

def main(args=None):
    return_value = update_test_log.main(path)
    if return_value == 0:
      print "Tests ran successfully and the Test Summary has been emailed."
    else:
      print "There was an error updating or sending the Test Summary."

    return

if __name__ == "__main__":
  main(sys.argv[1:])

