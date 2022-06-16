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

echo "############################################"
echo "#           Building Unit Tests            #"
echo "############################################"

cd $top_level/tests/unit

make $cdst_j_flags
check_exit_status

echo "############################################"
echo "#            Running Unit Tests            #"
echo "############################################"

cd $top_level/tests/unit

./unit_tests
check_exit_status

echo "############################################"
echo "#                   Done                   #"
echo "############################################"

exit_with_status
