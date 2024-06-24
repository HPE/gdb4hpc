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

source ./external/cdst_build_library/build_lib_gcc

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

function run_unit_tests () {
    # in any jenkins pipeline, the tests can randomly fail for an unknown
    # reason and exit with exit code 141.

    for i in 1 2 3 4 5; do
        $1 --test
        exit_code=$?
        if [ "$exit_code" -eq 141 ]; then
            # weird exit code, try again...
            echo "Try again..."
            sleep 5
        else
            # this exit code is meaningful
            return $exit_code
        fi
    done

    echo "Out of tries."
    return -1
}

echo "############################################"
echo "#       Running Client Unit Tests          #"
echo "############################################"

run_unit_tests $install_dir/bin/gdb4hpc
check_exit_status

echo "############################################"
echo "#       Running Server Unit Tests          #"
echo "############################################"

run_unit_tests $install_dir/libexec/dbgsrv
check_exit_status

echo "############################################"
echo "#    Running Python Backend Unit Tests     #"
echo "############################################"

run_unit_tests $install_dir/libexec/pybe_unittest
check_exit_status

echo "############################################"
echo "# Running Functional Text Match Unit Tests #"
echo "############################################"

pushd "$top_level/tests/function"
./libtests.py
check_exit_status
popd

echo "############################################"
echo "#                   Done                   #"
echo "############################################"

exit_with_status
