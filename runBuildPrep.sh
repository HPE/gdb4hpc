#!/bin/bash
#
# runBuildPrep.sh - Preps the build environment
#
# Copyright 2019-2022 Hewlett Packard Enterprise Development LP.
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
target_arch=$(get_arch)
if [[ "$target_pm" == "$cdst_pm_zypper" ]]; then
    if [[ "$target_arch" == "$cdst_arch_x86_64" ]]; then
      sudo zypper refresh -f
    fi
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
            libuuid-devel \
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
elif [[ "$target_pm" == "$cdst_pm_yum" ]]; then
    if [[ "$target_os" == "$cdst_os_centos8" ]]; then
      # Note the following will be different on build VMs vs DST. Errors are okay.
      yum config-manager --set-enabled powertools
    else
      yum --assumeyes install subscription-manager
      subscription-manager repos --enable codeready-builder-for-rhel-8-x86_64-rpms
    fi
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
            libarchive \
            libtool \
            libuuid-devel \
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

    if [[ "$target_arch" == "$cdst_arch_aarch64" && "$target_os" == "$cdst_os_rhel84" ]]; then
      yum --assumeyes install \
      https://arti.dev.cray.com/artifactory/hpe-rhel-remote/EL8/Update4/GA/CRB/os/Packages/perl-Unicode-EastAsianWidth-1.33-13.el8.noarch.rpm \
      https://vault.centos.org/centos/8/PowerTools/aarch64/os/Packages/texinfo-6.5-6.el8.aarch64.rpm
    else
      yum --assumeyes install texinfo
    fi
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
check_exit_status

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

sudo -H pip3 install wheel
check_exit_status

capture_jenkins_build
check_exit_status

echo "############################################"
echo "#          Done with build prep            #"
echo "############################################"

exit_with_status
