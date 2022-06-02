#!/bin/bash

source get_pandoc.sh

echo "Generating..."
pandoc --standalone -r markdown+line_blocks+definition_lists -w man -o gdb4hpc.1 gdb4hpc.md
echo "Done."
