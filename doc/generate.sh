#!/bin/bash

source ../man/man1/get_pandoc.sh

echo "Generating..."
pandoc -s -o getting-started.html getting-started.md
pandoc -s -o tutorial.html tutorial.md
echo "Done."
