#!/usr/bin/env python
import sys, os, re, datetime, time, fnmatch, glob, commands, stat
from os import listdir
from os.path import isfile, join
from distutils.version import LooseVersion
from datetime import date
from datetime import datetime

final   		    = False
product             = "gdb4hpc"
os_sub              = ""
prefix              = "/opt/cray/pe"
prefix_cmd          = ""
osver               = ""
os_name             = ""
arch                = ""
latest_ver          = ""
new_version_str     = ""
release_date        = ""
three_digit_version = ""
two_digit_version   = ""
ver_two_dig_nodot   = ""
parent_dir          = ""
cti_version         = ""
scripts_dir         = ""
cti_lowerBound      = ""
cti_upperBound      = ""


def fetch_os():
  global bld_dir
  global os_sub
  global prefix
  global prefix_cmd
  global os_name

  if os.path.isfile("/etc/os-release"):
    with open("/etc/os-release", "r") as f:
      for line in f:
        if line.find('VERSION="15"') != -1:
          os_sub = '15'
          break
  
  os_sub = os_sub.strip()
  if os_sub == "15":
    os_name = "sles15"
    
  return os_name

def getARCH():
  cmd = 'uname -m'
  str(os.system(cmd))
  status, rd = commands.getstatusoutput(cmd)
  return rd

def getReleaseDate():
  cmd = 'source ' + scripts_dir + '/find_release_date.sh'
  str(os.system(cmd))
  status, rd = commands.getstatusoutput(cmd)
  return rd

def get_three_digit_version():
    global two_digit_version
    global three_digit_version
    global ver_two_dig_nodot
    global parent_dir
    global scripts_dir

    major=''
    minor=''
    revision=''

    parent_dir = os.path.abspath(os.path.join(os.getcwd(),os.pardir))
    scripts_dir = parent_dir + "/scripts"
    if os.path.isfile(parent_dir + "/release_versioning"):
      with open(parent_dir + "/release_versioning", "r") as f:
        for line in f:
          line = line.strip('\n')
          if line.find('major=') != -1:
            temp = re.split('"',line)
            major = temp[1]
          elif line.find('minor=') != -1:
            temp = re.split('"',line)
            minor = temp[1]
          elif line.find('revision=') != -1:
            temp = re.split('"',line)
            revision = temp[1]

      two_digit_version = [major,minor]
      ver_two_dig_nodot = "".join(two_digit_version)
      two_digit_version = ".".join(two_digit_version)
      three_digit_version = [major,minor,revision]
      three_digit_version = ".".join(three_digit_version)
    return three_digit_version

def get_ctiVersion():

    global cti_lowerBound
    global cti_upperBound
    upperBound = ""

    #The latest CTI version was installed and gdb4hpc was built against it.
    #Get the CTI version
    print "Building gdb4hpc against . . ."
    cmd = 'readlink -f /opt/cray/pe/cti/default'
    str(os.system(cmd))
    status, rd = commands.getstatusoutput(cmd)
    cti_version_list = re.split('/',rd)
    cti_ver = cti_version_list[5]
    cti_major = cti_ver[0]
    cti_minor = cti_ver[2]
    cti_lowerBound = cti_major + "." + cti_minor + ".0"
    upperBound = int(cti_major) + 1
    cti_upperBound = str(upperBound) + ".0.0"
    
    return cti_ver

def fetch_newData():

  global two_digit_version
  global timestmp_revnbr
  global pkgs_dir
  global three_digit_version
  global version
  global release_number
  global rpm_name
  global osver
  global ver_two_dig_nodot
  global release_date
  global arch
  global cti_version


  rev_command = "git rev-parse HEAD"
  rev_number  = os.popen(rev_command).read()
  rev_number  = rev_number.rstrip()

  branch = "git rev-parse --abbrev-ref HEAD"
  branch = os.popen(branch).read()
  branch = branch.rstrip() 
  print "Packaging from: " + branch

  time_command = "git log -s -1 --format=%ci | tail -n1 | cut -d' ' -f1-2"
  dt_time = os.popen(time_command).read()
  dt_time = str(dt_time).replace(":","")
  dt_time = str(dt_time).replace("-","")
  dt_time = str(dt_time).replace(" ","")
  dt_time = str(dt_time)[0:12]

  rev_number = rev_number[0:13]
  timestmp_revnbr = str(dt_time) + "." + str(rev_number)

  arch = getARCH()
  osver = fetch_os()
  three_digit_version = get_three_digit_version()
  version = three_digit_version
  base_name = product + "-" + version
  release_date = getReleaseDate() 
  cti_version = get_ctiVersion()
  release_number='0'

  package_name = base_name + "-" + timestmp_revnbr + "-" + release_number + "." + osver
  rpm_name = package_name + "." + arch + ".rpm"

  return


