#!/usr/bin/env python3

import os, sys

if __name__ == "__main__":
    # load up cdst test library
    sys.path.insert(0, os.getcwd() + "/cdst-test")
    try:
        from test_tool import run_tests
    except ModuleNotFoundError:
        print("Failed to load testing library. Make sure you've initialized submodules.")
        print("Try \"git submodule update --init ./cdst-test\"")
        exit(-1)

    run_tests()
