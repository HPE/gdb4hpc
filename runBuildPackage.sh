#!/bin/bash
#
# runBuildPackage.sh - Package steps for CTI
#
# Copyright 2019 Cray Inc. All Rights Reserved.
#
# Unpublished Proprietary Information.
# This unpublished work is protected to trade secret, copyright and other laws.
# Except as permitted by contract or express written permission of Cray Inc.,
# no part of this work or its content may be used, reproduced or disclosed
# in any form.
#
gcc_ver=8.1.0
return_code=0
top_level=$PWD
j_flags=$(rpm -E %{?_smp_mflags})

function check_exit_status(){
    if [ $1 -ne 0 ]
    then
        echo "runBuildPackage.sh: error code of $1 from $2"
        exit $1
    fi
}

#Ensure we can use modules
source /opt/cray/pe/modules/default/init/bash

#Ensure gdb4hpc is build with $gcc_ver
module load gcc/$gcc_ver
check_exit_status $? "module load gcc/$gcc_ver"

#Ensure dep modules are loaded
module load cray-cdst-support
check_exit_status $? "module load cray-cdst-support"
module load cray-cti
check_exit_status $? "module load cray-cti"

echo "############################################"
echo "#             Creating rpm                 #"
echo "############################################"
rpmbuilddir=${top_level}/rpmbuild
cd ${rpmbuilddir}
check_exit_status $? cd
rpmbuild -bb -D "_topdir ${rpmbuilddir}" SPECS/gdb4hpc.spec
check_exit_status $? rpmbuild

echo "############################################"
echo "#          Done with packaging             #"
echo "############################################"

exit $return_code
