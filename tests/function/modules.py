#
# © Copyright 2020 Cray Inc.
#

import os, sys, subprocess, re

# import module functionality
try:
    MODULES_HOME = os.environ['MODULESHOME']
except KeyError as e:
    print("Couldn't read %s from environment. Are modules available?" % e,
        file=sys.stderr)
    raise e

# add the module init scripts directory to the system import path
sys.path.insert(0, MODULES_HOME + "/init/")
# "from python" because the module init file for python is called "python.py"
from python import module

# overwrite provided module function - default is too basic (no return codes etc)
# we still import the system module because it sets up some environment variables etc
#
# module - interface to system module binary
# usage: module("arg1", "arg2")
#    or: module(["arg1", "arg2"])
#
# returns a tuple: (success (True or False), module command output (bytes))
def module(*args):
    # format arguments
    if type(args[0]) == type([]):
        args = args[0]
    else:
        args = list(args)

    global MODULES_HOME

    # make sure we can find the modules executable
    if MODULES_HOME == "":
        try:
            MODULES_HOME = os.environ['MODULESHOME']
        except KeyError as e:
            print("Couldn't read %s from environment. Are modules available?" % e,
                file=sys.stderr)
            raise e
    
    # query modulecmd
    p = subprocess.Popen(['%s/bin/modulecmd' % MODULES_HOME, 'python'] + args,
        stdout = subprocess.PIPE, stderr = subprocess.PIPE)
    (output, error) = p.communicate()

    # check for errors
    # the module command sometimes emits legitimate information to stderr, and
    #   doesn't always have a return code != 0 upon error, so we need to manually
    #   parse the stderr output
    success = True
    if re.search("ERROR", error.decode("utf-8")) != None:
        success = False
    else:
        # no errors, execute returned python code
        exec(output)

    return success, output + error

def required_module(*args):
    r, o = module(*args)

    if not r:
        print("Error in module()", file=sys.stderr)
        raise RuntimeError

    return r, o
