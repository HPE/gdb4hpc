#!/usr/bin/env python
import sys, os, re
from os import listdir
from os.path import isfile, join

primary_version     = "2.4.4" 
ver_two_dig         = ".".join([primary_version[0],primary_version[2]])
ver_two_dig_nodot   = "".join([primary_version[0],primary_version[2]])
ver_three_dig       = ".".join([primary_version[0],primary_version[2],primary_version[4]]) 

if len(sys.argv) == 2 and (sys.argv[1].lower() == "final"):
  final = True
else:
  final = False

def packages_dir(sles_level):
  if sles_level == "11":
    pdir = "/cray/css/pe/pkgs/cray-lgdb/%s" % ver_two_dig
  elif sles_level == "12":
    pdir = "/cray/css/pe/pkgs/cray-lgdb/%s/sles%s" % (ver_two_dig, sles_level)
  return pdir

def build_dir(sles_level):
  if sles_level == "11":
    bld_dir = "/cray/css/ulib/lgdb/builds/latest/lgdb_install/"
  elif sles_level == "12":
    bld_dir = "/cray/css/ulib/lgdb/builds/sles12/latest/lgdb_install/"
  return bld_dir

def fetch_release(bld_version, sles, rev_num):
  rpm_base = "cray-lgdb-" + bld_version
  pkgs_dir = packages_dir(sles)  

  rpm_tail = ".x86_64.rpm"
  itt_list = range(20)
  itt_list.reverse()
  rval = 0
  for i in itt_list:
    tmp_name = pkgs_dir + rpm_base + "-" + str(i) + "_" + rev_num  + rpm_tail
    if os.path.isfile(tmp_name):
      rval = i + 1
      break
  return str(rval)  

def release_date():
  rd = os.popen('/cray/css/ulib/utilities/find_release_date','r').readline()
  rd = rd.replace('"','').replace('\n','')
  return rd

def fetch_version(primary_ver, final, pkgs_dir):

  if final:
    new_version_str = primary_ver
  else:
    #need to escape the decimal points in the version string
    primary_list = primary_ver.split(".")
    clean_version = "\.".join(primary_list)
    reg_str = clean_version + '\.[0-9]{1}'
    reg_x = re.compile(reg_str)

    #only grab 4 digit version numbers / pre-releases
    onlyfiles = [ f for f in listdir(pkgs_dir) if isfile(join(pkgs_dir,f)) and reg_x.search(f) ]

    #if this is the first in this branch
    if len(onlyfiles) == 0:
      new_version_str = primary_ver + ".1"
    else: 
      #sort the file list, grab the last one
      onlyfiles.sort()
      last_package = onlyfiles.pop()
      #determine the 4 digit version number
      last_package_ver = last_package.replace("cray-lgdb-","")
      last_package_ver = last_package_ver.split("-")[0]
      version_list = last_package_ver.split(".")
      #figure out the new last digit, regex above ensures 4 digits
      new_last_digit = str(int(version_list[3]) + 1)
      #replace the last digit of the version number
      version_list.pop()
      version_list.append(new_last_digit)
      new_version_str = ".".join(version_list)

  return new_version_str

def fetch_revision(bld_dir):
  rev_command = "/opt/cpkg/current/subversion/current/bin/svn info %sdemos" % bld_dir
  rev_info       = os.popen(rev_command).read()
  svn_list       = rev_info.split("\n")
  for i in svn_list:
    if i.find("Revision:") != -1:
      rev_number = i.replace("Revision: ","")
      break
  return rev_number

