#!/bin/bash

set -e

if [ $(gcc --version | grep -c '\<14\.') != 1 ]; then
    source /opt/rh/gcc-toolset-14/enable
fi    
install_dir=/lus/scratch/jvogt/gutools

# we explicitly set CC/CXX here to their full paths so that the full paths are
# baked into the meson build configuration and meson doesn't use whatever gcc/g++ is
# in PATH on subsequent builds outside of runBuild.sh
CC=$(which gcc) CXX=$(which g++) \
  meson setup \
    -Dprefix=$install_dir \
    -Ddebug=true \
    -Doptimization=2 \
    -Db_ndebug=true \
    ./build

meson compile --verbose -C build

meson install -C build

