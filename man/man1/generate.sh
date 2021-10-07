#!/bin/bash

pandoc --standalone -r markdown+line_blocks+definition_lists -w man -o gdb4hpc.1 gdb4hpc.md
