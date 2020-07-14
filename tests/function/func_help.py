#
# © Copyright 2020 Cray Inc.
#

import subprocess
from modules import module, required_module

# try to detect a WLM
# 
# returns a string or None
def detect_wlm():
    # try wlm_detect
    r, _ = module("load", "wlm_detect")
    if r == True:
        p = subprocess.run(["wlm_detect"], stdout=subprocess.PIPE)
        if p.returncode == 0:
            module("unload", "wlm_detect")
            return p.stdout.decode("utf-8").strip()

    # try to run srun
    try:
        p = subprocess.run(["srun", "--version"],
            stdout=subprocess.PIPE)
        if p.returncode == 0:
            return "SLURM"
    except FileNotFoundError:
        pass

    # try to run aprun
    try:
        p = subprocess.run(["aprun", "--version"],
            stdout=subprocess.PIPE)
        if p.returncode == 0:
            return "ALPS"
    except FileNotFoundError:
        pass

    # try to run mpiexec
    try:
        p = subprocess.run(["mpiexec", "-h"],
            stdout=subprocess.PIPE)
        if p.returncode == 0:
            return "SSH"
    except FileNotFoundError:
        pass

    # couldn't find anything
    return None
