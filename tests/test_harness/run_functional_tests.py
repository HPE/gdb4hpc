#!/usr/bin/python

import os
import sys
import commands
import select
import paramiko
import time
import threading
import csv
import openpyxl
import datetime
import update_test_log
import errno

port=22
gdb4hpc_version = ""
user='ulib'
pwd='OMGwapita55!'
host='mars'
path = '/cray/css/ulib/gdb4hpc/tests/test_harness'
cmd1='cd /cray/css/ulib/gdb4hpc/install_testing/sles15/x86_64\n'
cmd2='module use /cray/css/ulib/gdb4hpc/install_testing/sles15/x86_64/modulefiles\n'
cmd3='module load'
cmd4='module load craype-broadwell\n'
cmd5='cd /cray/css/ulib/gdb4hpc/tests/test_harness\n'
cmd6='./test_script.sh sanity_list build\n'
cmd7='./test_script.sh sanity_list run\n'

class Testing:
    shell = None
    client = None
    transport = None


    def __init__(self, host, user, pwd):
        try:
            print("Connecting to " + host)
            self.client = paramiko.SSHClient()
            self.client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
            self.client.connect(hostname=host,port=int(port),username=user,password=pwd)
 
            self.transport = paramiko.Transport((host,22))
            self.transport.connect(username=user,password=pwd)
 
            thread = threading.Thread(target=self.process)
            thread.daemon = True
            thread.start()
        except paramiko.ssh_exception.AuthenticationException:
             print("Authentication failed when connecting to %s" % host)
             sys.exit(1)
        except paramiko.ssh_exception.BadAuthenticationType:
             print("Bad authentication type")
             sys.exit(1)
        except paramiko.SSHException as sshException:
             print ("Unable to estblish SSH connection: %s" % sshException)
             sys.exit(1)
        except paramiko.ssh_exception.BadHostKeyException as badHostKeyException:
             print("Unable to verify server's host key: %s" % badHostKeyException)
             sys.exit(1)
        except paramiko.ssh_exception.ChannelException:
             print("attempt to open a new Channel fails")
             sys.exit(1)
        except Exception as e:
             print(e.args)
             sys.exit(1)
        except:
             print("Could not SSH to %s" % host)


    def close_connection(self):
        if(self.client != None):
          self.client.close()
          self.transport.close()       

    def open_shell(self):
        self.shell = self.client.invoke_shell()

   # if there is an error code = 1-29
    def send_shell(self, cmd):
        if(self.shell):
          ret = self.shell.send(cmd + "\n")
          return 0
        else:
          print("Shell was not opened.\n")
          return 1

    def process(self):
        while True:
          if self.shell != None and self.shell.recv_ready():
            alldata = self.shell.recv(1024)
            print alldata

    def get_gdb4hpc_version(self,arch):
         global gdb4hpc_version
         global cmd3
         stdin, stdout, stderr = self.client.exec_command("ls /cray/css/ulib/gdb4hpc/install_testing/sles15/" + arch + " | grep gdb4hpc- | cut -d'-' -f2")
         result = stdout.read()
         version = result.decode()
         gdb4hpc_version = "gdb4hpc/" + version
         cmd3 = cmd3 + " " + gdb4hpc_version + "\n"
         print("Testing " + gdb4hpc_version)


    def analyzeErrorCode(self,code,strCmd):
      if code > 0:
        print "An error: " + str(code) + " has occured with " +  strCmd
        sys.exit(1)



def main(args=None):
    report = Testing(host,user,pwd)
    report.open_shell()
    cmd = 'uname -m'
    status, arch = commands.getstatusoutput(cmd)
    
    report.get_gdb4hpc_version(arch)
    
    resp = report.send_shell(cmd1)
    report.analyzeErrorCode(resp, "cmd1")
    time.sleep(5)
    
    resp = report.send_shell(cmd2)
    report.analyzeErrorCode(resp, "cmd1")
    time.sleep(5)
    
    resp = report.send_shell(cmd3)
    report.analyzeErrorCode(resp, "cmd1")
    time.sleep(5)
    
    resp = report.send_shell(cmd4)
    report.analyzeErrorCode(resp, "cmd1")
    time.sleep(5)
    
    resp = report.send_shell(cmd5)
    report.analyzeErrorCode(resp, "cmd1")
    time.sleep(5)
    
    resp = report.send_shell(cmd6)
    report.analyzeErrorCode(resp, "cmd1")
    time.sleep(60)
    
    resp = report.send_shell(cmd7)
    report.analyzeErrorCode(resp, "cmd1")
    
    time.sleep(4000)
    print "Functional Testing Complete"

    return_value = update_test_log.main(path)
    
    if return_value == 0:
      print "Tests ran successfully and the Test Summary has been emailed."
    else:
      print "There was an error updating or sending the Test Summary."

    # close connection
    report.close_connection()

    return 0

if __name__ == "__main__":
  main(sys.argv[1:])

