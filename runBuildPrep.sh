#!/bin/bash
#
# runBuildPrep.sh - Preps the build environment
#
# Copyright 2019-2020 Cray Inc.  All Rights Reserved.
#
# Unpublished Proprietary Information.
# This unpublished work is protected to trade secret, copyright and other laws.
# Except as permitted by contract or express written permission of Cray Inc.,
# no part of this work or its content may be used, reproduced or disclosed
# in any form.
#

top_level=$PWD
source ./external/cdst_build_library/build_lib

echo "############################################"
echo "#             Installing deps              #"
echo "############################################"
#   If you add pkgs, please insert them in alphabetical order.
zypper --non-interactive install \
                autoconf \
				automake \
				binutils-devel \
				bison \
				bzip2 \
				ctags \
				flex \
				m4 \
				make \
				makeinfo \
				mksh \
				ncurses \
				ncurses-devel \
				libbz2-devel \
				liblzma5 \
				libtool \
				tcl \
				python3-devel \
				which \
				xz-devel
check_exit_status


# Install the common PE components
install_common_pe

# Install CTI and CDST support
install_cti

echo "############################################"
echo "#          Installing cudatoolkit          #"
echo "############################################"
# Note that cudatoolkit rpms depend on readelf; there may be other pkgs it
# requires; we can get everything we need from the various zypper installs
# currently preceding this script, but maybe we should document its
# dependencies...
# TODO: figure out what packages cudatoolkit depends on
# TODO: eventually add cudatoolkit to cdst_build_library

# Install cudatoolkit 10.1
# ensure submodule is checked out
if [[ ! -f $top_level/external/pebuildenv/README.md ]]; then
    echo "$top_level/external/pebuildenv submodule not checked out..."
    exit 1
fi
export product_version="10.1"
export pebuildenv_path=$top_level/external/pebuildenv
$top_level/external/pebuildenv/src/cuda-local.sh
check_exit_status

capture_jenkins_build
check_exit_status

echo "############################################"
echo "#          Done with build prep            #"
echo "############################################"

exit_with_status
