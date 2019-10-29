#!/bin/bash
#
# runBuildPrep.sh - Preps the build environment
#
# Copyright 2019 Cray Inc.  All Rights Reserved.
#
# Unpublished Proprietary Information.
# This unpublished work is protected to trade secret, copyright and other laws.
# Except as permitted by contract or express written permission of Cray Inc.,
# no part of this work or its content may be used, reproduced or disclosed
# in any form.
#

# TODO: Once we move away from monolithic builds, point at the proper repositories
# ./add_rpm_repo.sh http://car.dev.cray.com/artifactory/internal/PE-CDST/ /x86_64/dev/master/

return_code=0
gcc_ver=8.1.0

function check_exit_status(){
    if [ $1 -ne 0 ]
    then
        echo "runBuildPrep.sh: error code of $1 from $2"
	return_code=$1
    fi
}

echo "############################################"
echo "#             Installing deps              #"
echo "############################################"
#   If you add pkgs, please insert them in alphabetical order.
zypper --non-interactive install autoconf \
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

check_exit_status $? sys-pkgs

zypper addrepo http://car.dev.cray.com/artifactory/pe/DST/sle15_pe/x86_64/dev/master/ car-pe-base
check_exit_status $? "zypper addrepo car-pe-base"

zypper --non-interactive --no-gpg-check install cray-gcc-$gcc_ver \
					cray-modules \
					cray-set-gcc-libs \
					craype
check_exit_status $? "zypper install cray-deps"

zypper rr car-pe-base

zypper addrepo http://car.dev.cray.com/artifactory/pe-base/PE-CDST/sle15_premium/x86_64/dev/master/ car-cdst-master
check_exit_status $? "zypper addrepo car-cdst-master"

zypper --non-interactive --no-gpg-check install cray-cdst-support-devel cray-cti-devel
check_exit_status $? "zypper install cray-cdst-deps"

zypper rr car-cdst-master

echo "############################################"
echo "#      Capturing Jenkins Env Vars          #"
echo "############################################"
#Get the Jenkins BUILD_NUMBER from the ENV.
BUILD_NUMBER=$(echo $BUILD_NUMBER)
if [ ! -z $BUILD_NUMBER ]
then
  #update the release_versioning file
  sed -i.bak "s/9999/$BUILD_NUMBER/g" $PWD/release_versioning
  echo "Set version to $BUILD_NUMBER in release_versioning"
else
  echo "Unable to determine Jenkins BUILD_NUMBER"
  return_code=1
fi

echo "############################################"
echo "#          Done with build prep            #"
echo "############################################"
exit $return_code
