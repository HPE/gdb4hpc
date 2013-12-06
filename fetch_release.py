#!/usr/bin/python

import sys, os

if len(sys.argv) < 4:
  print "Error: not enough arguments. ./fetch_release.py {rpm_base up to release # -> cray-lgdb-2.0.0.1910-  } {pkgs dir -> /cray/css/pe/pkgs/cray-lgdb/2.0/ }"
  sys.exit(1)

rpm_base = sys.argv[1]
pkgs_dir = sys.argv[2]
rev_num = sys.argv[3]
rpm_tail = ".x86_64.rpm"

itt_list = range(10)
itt_list.reverse()

rval = 0
for i in itt_list:
  tmp_name = pkgs_dir + rpm_base + "-" + str(i) + "_" + rev_num + rpm_tail
  if os.path.isfile(tmp_name):
    rval = i + 1
    break

sys.exit(str(rval))
