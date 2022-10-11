#!/bin/bash
#
# runBuildUnitTest.sh - Run gdb4hpc unit tests
#
# Copyright 2021 Hewlett Packard Enterprise Development LP.
#
# Unpublished Proprietary Information.
# This unpublished work is protected to trade secret, copyright and other laws.
# Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
# no part of this work or its content may be used, reproduced or disclosed
# in any form.

# It is assumed that runBuild.sh has been run before this script.

echo "############################################"
echo "#            Setup environment.            #"
echo "############################################"

top_level=$PWD

source ./external/cdst_build_library/build_lib

setup_modules

source $top_level/release_versioning
gdb4hpc_version=
branch_name=$(get_branch_info)
branch_type=$(echo "$branch_name" | cut -d'/' -f1)
if [ "$branch_type" != "release" ]; then
  gdb4hpc_version="$major.$minor.$revision.$build_number"
else
  gdb4hpc_version="$major.$minor.$revision"
fi
echo "gdb4hpc version: \"$gdb4hpc_version\""
install_dir="/opt/cray/pe/gdb4hpc/$gdb4hpc_version"
echo "Install dir: \"$install_dir\""

echo "############################################"
echo "#       Running Client Unit Tests          #"
echo "############################################"

$install_dir/bin/gdb4hpc --test
check_exit_status

echo "############################################"
echo "#       Running Server Unit Tests          #"
echo "############################################"

$install_dir/libexec/dbgsrv --test
check_exit_status

echo "############################################"
echo "#                   Done                   #"
echo "############################################"

exit_with_status
