#!/bin/bash
#
# runBuildPrep.sh - Preps the build environment
#
# Copyright 2019-2024 Hewlett Packard Enterprise Development LP.
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
        jq \
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
        gmp-devel \
        python3-pip
    check_exit_status

elif [[ "$target_pm" == "$cdst_pm_yum" ]]; then

    if [[ "$target_os" == "$cdst_os_centos8" ]]; then
      # Note the following will be different on build VMs vs DST. Errors are okay.
      yum config-manager --set-enabled powertools
    elif [[ "$target_os" -lt "$cdst_os_centos8" ]]; then
      yum --assumeyes install subscription-manager
      subscription-manager repos --enable codeready-builder-for-rhel-8-$target_arch-rpms
    elif [[ "$target_os" -ge "$cdst_os_rhel93" ]]; then
      yum --assumeyes install subscription-manager
      subscription-manager repos --enable codeready-builder-for-rhel-9-$target_arch-rpms
    fi

    # Building in CI/CD dst library adds some mirrored repos automatically - VM's dont have access
    # to these repos - add them here
    if [[ -z $BRANCH_NAME ]]; then

      mirrored_repo_1=''
      mirrored_repo_2=''
      arch=$(uname -m)

      if [[ "$target_os" -lt "$cdst_os_centos8" ]]; then
        mirrored_repo_1="https://arti.hpc.amslabs.hpecorp.net:443/artifactory/mirror-epel/epel/8/Everything/$arch/"

        if [[ "$target_os" -lt "$cdst_os_rhel88" ]]; then
          mirrored_repo_2="https://arti.hpc.amslabs.hpecorp.net:443/artifactory/rdlabs-os-mirror-remote-cache/redhat/rhel/8.8/latest/CRB/$arch/os/"

        elif [[ "$target_os" -eq "$cdst_os_rhel89" ]]; then
          mirrored_repo_2="https://arti.hpc.amslabs.hpecorp.net:443/artifactory/rdlabs-os-mirror-remote-cache/netinst/rhel/8.9/$arch/latest/CRB/os/"
 
        fi

      elif [[ "$target_os" -ge "$cdst_os_rhel93" ]]; then  
        mirrored_repo_1="https://arti.hpc.amslabs.hpecorp.net:443/artifactory/mirror-epel/epel/9/Everything/$arch/"
                  
        mirrored_repo_2="https://arti.hpc.amslabs.hpecorp.net:443/artifactory/rdlabs-os-mirror-remote-cache/netinst/rhel/9.3/$arch/latest/CRB/os/"

      fi
      sudo yum-config-manager --add-repo $mirrored_repo_1 "mirrored_repo_1"
      sudo yum-config-manager --add-repo $mirrored_repo_2 "mirrored_repo_1"

    fi
    # Install yum based components
    yum --assumeyes --nogpgcheck install \
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
        python3-pip \
        jq \
        rpm-build \
        tcl \
        which \
        texinfo \
        gmp-devel \
        xz-devel
    check_exit_status

    if [[ -z $BRANCH_NAME ]]; then
      yum-config-manager --disable arti.hpc.amslabs.hpecorp.net*
    fi

else
    # Unknown OS! Exit with error.
    echo "Unsupported Package Manager detected!"
    exit 1
fi


# Install the common PE components
install_common_pe

# Install CDST support, CTI, zmqnet, and mrnet
install_zmqnet
check_exit_status
install_mrnet
check_exit_status

## building openmp relies on bdist_wheel - install from arti location
sudo -H pip3 install wheel --trusted-host https://arti.hpc.amslabs.hpecorp.net \
      --extra-index-url https://arti.hpc.amslabs.hpecorp.net:443/artifactory/api/pypi/dst-pip-master-local/simple \
      --index-url https://arti.hpc.amslabs.hpecorp.net:443/artifactory/api/pypi/pypi-remote/simple
check_exit_status

capture_jenkins_build
check_exit_status

echo "############################################"
echo "#          Done with build prep            #"
echo "############################################"

exit_with_status
