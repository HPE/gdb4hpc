#!/bin/bash
#
# runBuildPrep.sh - Preps the build environment
#
# Copyright 2019-2021 Hewlett Packard Enterprise Development LP.
#
# Unpublished Proprietary Information.
# This unpublished work is protected to trade secret, copyright and other laws.
# Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
# no part of this work or its content may be used, reproduced or disclosed
# in any form.
#

top_level=$PWD
source ./external/cdst_build_library/build_lib

echo "############################################"
echo "#             Installing deps              #"
echo "############################################"
target_pm=$(get_pm)
target_os=$(get_os)
if [[ "$target_pm" == "$cdst_pm_zypper" ]]; then
    # Install zypper based dependencies
    zypper --non-interactive install \
            autoconf \
            automake \
            binutils-devel \
            bison \
            bzip2 \
            ctags \
            cmake \
            flex \
            libtool \
            m4 \
            make \
            makeinfo \
            mksh \
            ncurses \
            ncurses-devel \
            libbz2-devel \
            liblzma5 \
            libtool \
            python3-devel \
            rpm-build \
            tcl \
            which \
            xz-devel \
            python3-pip
    check_exit_status
elif [[ "$target_pm" == "$cdst_pm_yum" && "$target_os" == "$cdst_os_centos8" ]]; then
    # Note the following will be different on build VMs vs DST. Errors are okay.
    yum config-manager --set-enabled PowerTools
    yum config-manager --set-enabled dst-remote-centos8-PowerTools
    # Install yum based components
    yum --assumeyes install \
            autoconf \
            automake \
            binutils-devel \
            bison \
            bzip2 \
            bzip2-devel \
            ctags \
            cmake \
            environment-modules \
            flex \
            libtool \
            m4 \
            make \
            mksh \
            ncurses \
            ncurses-devel \
            python3-devel \
            rpm-build \
            tcl \
            which \
            xz-devel
    check_exit_status
elif [[ "$target_pm" == "$cdst_pm_yum" && "$target_os" == "$cdst_os_rhel83" ]]; then
    # Note the following will be different on build VMs vs DST. Errors are okay.
    # Install yum based components
    yum --assumeyes install \
            autoconf \
            automake \
            binutils-devel \
            bison \
            bzip2 \
            bzip2-devel \
            ctags \
            cmake \
            environment-modules \
            flex \
            libtool \
            m4 \
            make \
            mksh \
            ncurses \
            ncurses-devel \
            python3-devel \
            rpm-build \
            tcl \
            which \
            xz-devel
    check_exit_status
else
    # Unknown OS! Exit with error.
    echo "Unsupported Package Manager detected!"
    exit 1
fi

# Install the common PE components
install_common_pe

# Install CTI and CDST support
install_cti

arch=$(get_arch)
if [[ "$arch" == "$cdst_arch_aarch64" ]]; then
    echo "############################################"
    echo "#    aarch64, cudatoolkit not supported    #"
    echo "############################################"
else
    echo "############################################"
    echo "#          Installing cudatoolkit          #"
    echo "############################################"
    # ensure submodule is checked out
    if [[ ! -f $top_level/external/pebuildenv/README.md ]]; then
        echo "$top_level/external/pebuildenv submodule not checked out..."
        exit 1
    fi
    export product_version="11.2"
    export pebuildenv_path=$top_level/external/pebuildenv
    $top_level/external/pebuildenv/src/cuda-local.sh
    check_exit_status
fi

## building openmp relies on bdist_wheel
pip3 install wheel
check_exit_status

capture_jenkins_build
check_exit_status

echo "############################################"
echo "#          Done with build prep            #"
echo "############################################"

exit_with_status
