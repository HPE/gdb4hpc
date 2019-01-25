#!/usr/bin/env python
import sys, os, re, datetime, time, fnmatch, glob, commands, stat
from os import listdir
from os.path import isfile, join
from distutils.version import LooseVersion
from datetime import date
from datetime import datetime

final   		    = False
product             = "gdb4hpc"
product_dir         = "/cray/css/pe/pkgs/" + product
ulib_dir            = "/cray/css/ulib/" + product + "/builds"
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



for arg in sys.argv:
  if arg == '-f':
    final = True

def fetch_release(base_name, rev_num,pkgs_dir):
  rpm_tail = ".rpm"
  itt_list = range(20)
  itt_list.reverse()
  rval = 0
  for i in itt_list:
    tmp_name = pkgs_dir + "/" + base_name + "-*." + rev_num + "-" + str(i) + osver + "." + arch + rpm_tail
    if glob.glob(tmp_name):
      rval = i + 1
      break
  return str(rval)  


def fetch_os():
  global bld_dir
  global os_sub
  global prefix
  global prefix_cmd
  global os_name


  if os.path.isfile("/etc/SuSE-release"):
    with open("/etc/SuSE-release", "r") as f:
      for line in f:
        if line.find("VERSION") != -1:
          os_sub = line.replace("VERSION = ", "")
          break
  elif os.path.isfile("/etc/redhat-release"):
    with open("/etc/redhat-release", "r") as f:
      for line in f:
        if line.find("CentOS ") != -1:
          os_sub = 'CS'
          os_name = "el7"
          break
  elif os.path.isfile("/etc/os-release"):
    with open("/etc/os-release", "r") as f:
      for line in f:
        if line.find('VERSION="15"') != -1:
          os_sub = '15'
          break
  
  os_sub = os_sub.strip()
  if os_sub == "CS":
    bld_dir = ulib_dir + "/builds_cs"
  elif os_sub == "11":
    bld_dir = ulib_dir + "/builds_xc"
    prefix = "/opt/cray"
    os_name = "sles11"
    prefix_cmd = "--prefix=/opt/cray" 
  elif os_sub == "12":
    os_name = "sles12"
    if arch == "x86_64":
      bld_dir = ulib_dir + "/builds_xc"
    elif arch == "aarch64":
      bld_dir = ulib_dir + "/aarch64"
  elif os_sub == "15":
    bld_dir = ulib_dir + "/builds_xc"
    os_name = "sles15"
    
  return "." + os_name

def getARCH():
  cmd = 'uname -m'
  str(os.system(cmd))
  status, rd = commands.getstatusoutput(cmd)
  return rd

def release_date():
  cmd = 'source /cray/css/ulib/utilities/find_release_date.sh'
  str(os.system(cmd))
  status, rd = commands.getstatusoutput(cmd)
  return rd


def fetch_packageDir(branch):
    #get a list of sub directories representing the products 2 digit release version
    onlyDirs = os.listdir(product_dir)
    if len(onlyDirs) == 0:
       print "There are no product subdirectories."
       exit(1)
    else:
       if branch == 'master':
         last_dir = '4.0'
       elif branch == 'legacy':
         last_dir = '3.0'
       return last_dir


def get_three_digit_version():


    global two_digit_version
    global three_digit_version
    global ver_two_dig_nodot

    parent_dir = os.path.abspath(os.path.join(os.getcwd(),os.pardir))
    if os.path.isfile(parent_dir + "/release_versioning"):
      with open(parent_dir + "/release_versioning", "r") as f:
        for line in f:
          line = line.strip('\n')
          if line.find("major") != -1:
            temp = line.split("=",1)
            major = temp[1]
          elif line.find("minor") != -1:
            temp = line.split("=",1)
            minor = temp[1]
          elif line.find("fix") != -1:
            temp = line.split("=",1)
            fix = temp[1]
            break

      two_digit_version = [major,minor]
      ver_two_dig_nodot = "".join(two_digit_version)
      two_digit_version = ".".join(two_digit_version)
      three_digit_version = [major,minor,fix]
      three_digit_version = ".".join(three_digit_version)
    return three_digit_version




def fetch_latest_version(final, revision, pkgs_dir):

  onlyfiles = glob.glob(pkgs_dir + "/*" + osver + "." + arch + ".rpm")
  if len(onlyfiles) == 0:
    if final == True:
      version = three_digit_version
    else:
      version = three_digit_version + ".1"
    return version
  else:
    if final == False:
      convert = lambda text: int(text) if text.isdigit() else text
      alphanum_key = lambda key: [ convert(c) for c in re.split('([0-9]+)',key) ]
      onlyfiles.sort(key=os.path.getmtime)
      last_package = onlyfiles.pop()
      last_package_ver = last_package.split(product + "-")[1]
      version = last_package_ver.split("-")[0]
      version_list = version.split(".")
      oldrev = last_package.split("-")[2]
      oldrev=oldrev.split(".")
      oldrev=oldrev[1]

      if len(version) >= 5 and len(version) <= 6:
        latest_ver = "release"
      elif len(version) >= 7:
        latest_ver = "pre-release"

      print "The latest package created was a: " + latest_ver
    
      if latest_ver == "release":
        third_digit = version_list[2]	
        third_digit = str(int(third_digit) + 1)
        version_list.pop()
        version_list.append(third_digit)
        version_list.append("1")
        new_version_str = ".".join(version_list)
      elif latest_ver == "pre-release": 
        if oldrev != revision:
          print "This is a new revision"
          new_last_digit = str(int(version_list[3]) + 1)
          version_list.pop()
          version_list.append(new_last_digit)
          new_version_str = ".".join(version_list)
        else:
          new_version_str = ".".join(version_list)

      return new_version_str
    elif final == True:
      return three_digit_version
         

def fetch_newData():

  global ver_two_dig
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

  release_date = release_date()

  rev_command = "git rev-parse HEAD"
  rev_number  = os.popen(rev_command).read()
  rev_number  = rev_number.rstrip()

  branch = "git rev-parse --abbrev-ref HEAD"
  branch = os.popen(branch).read()
  branch = branch.rstrip() 
  print "Packaging from: " + branch

  if final == True and branch == 'master':
    print "You indicated this should be a final build, but you are packaging from " + branch
    print "Exiting. . ."
    exit(1)
  elif final == True and branch[0:7] == 'release':
    release_date = branch.split('_')[1]
    release_date = release_date[:2] + "-" + release_date[2:4] + "-" + release_date[4:]
    print "Release Date: " + release_date

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
  ver_two_dig = fetch_packageDir(branch)
  pkgs_dir = product_dir + "/" + ver_two_dig
  version = fetch_latest_version(final,rev_number,pkgs_dir)
  base_name = product + "-" + version
  release_number = fetch_release(base_name, rev_number,pkgs_dir)

  package_name = base_name + "-" + timestmp_revnbr + "-" + release_number + osver
  rpm_name = package_name + "." + arch + ".rpm"

  return


