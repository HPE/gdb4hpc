#!/bin/bash

pandoc -s -t html getting-started.md -o getting-started.html
pandoc -s -t html tutorial.md -o tutorial.html
pandoc -s -t html handling-arrays.md -o handling-arrays.html
pandoc -s -t html kokkos-support.md -o kokkos-support.html
pandoc -s -t html raja-support.md -o raja-support.html
pandoc -s -t html vscode-plugin.md -o vscode-plugin.html
pandoc -t html --self-contained --css=./doc.css multi-rank-arrays.md -o multi-rank-arrays.html
