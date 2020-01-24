#!/bin/bash
#
# gdb4hpc build tool
# A bootstrap script to automagically build guard and all of its dependencies
#
# Copyright 2010-2019 Cray Inc.  All Rights Reserved.
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
        echo "runBuild.sh: error code of $1 from $2"
        exit $1
    fi
}

echo "############################################"
echo "#             Starting Build               #"
echo "############################################"
echo "gcc version: \"$gcc_ver\""
echo "top_level: \"$top_level\""
echo "Parallel build option: \"$j_flags\""

#Ensure we can use modules
source /opt/cray/pe/modules/default/init/bash

#Build with $gcc_ver
module load gcc/$gcc_ver
check_exit_status $? "module load gcc/$gcc_ver"

module load cray-cdst-support
check_exit_status $? "module load cray-cdst-support"

module load cray-cti-devel
check_exit_status $? "module load cray-cti-devel"

source $top_level/release_versioning
gdb4hpc_version="$major.$minor.$revision"
echo "gdb4hpc version: \"$gdb4hpc_version\""
install_dir="/opt/cray/pe/gdb4hpc/$gdb4hpc_version"
echo "Install dir: \"$install_dir\""
mkdir -p $install_dir
check_exit_status $? mkdir

echo "############################################"
echo "#               Building libaif            #"
echo "############################################"

cd $top_level/libaif
check_exit_status $? cd

autoreconf -ifv
check_exit_status $? autoreconf

CFLAGS="-g -O2 -Wall" ./configure --prefix=$install_dir
check_exit_status $? configure

make $j_flags
check_exit_status $? make

make $j_flags install
check_exit_status $? "make install"

echo "############################################"
echo "#               Building libmi             #"
echo "############################################"

# ensure submodule is checked out
if [ ! -f $top_level/third_party/libmi/README ]
then
    echo "libmi submodule not checked out..."
    exit 1
fi

cd $top_level/third_party/libmi
check_exit_status $? cd

autoreconf -ifv
check_exit_status $? autoreconf

CFLAGS="-g -O2 -Wall" ./configure --prefix=$install_dir
check_exit_status $? configure

# parallel builds are broken with libmi
make
check_exit_status $? make

make install
check_exit_status $? "make install"

echo "############################################"
echo "#               Building gdb               #"
echo "############################################"

# ensure python3 exists
python_loc=$(command -v python3)
if [ ! -x "$python_loc" ]
then
    echo "python3 not found on system..."
    exit 1
fi

# ensure submodule is checked out
if [ ! -f $top_level/third_party/ddt_gdb-8.1/README ]
then
    echo "gdb submodule not checked out..."
    exit 1
fi

cd $top_level/third_party/ddt_gdb-8.1
check_exit_status $? cd

./configure --disable-werror --prefix=$install_dir --with-gdb-datadir=$install_dir/share/gdb --without-guile --without-separate-debug-dir --with-mmalloc --with-python=$python_loc --without-babeltrace
check_exit_status $? configure

make $j_flags
check_exit_status $? make

# gdb is part of binutils. We don't want all of binutils to install, only gdb.
cd $top_level/third_party/ddt_gdb-8.1/gdb
make $j_flags install
check_exit_status $? "make install"

# deliver pretty-printers
cp -a $top_level/third_party/python_extras/gdb $install_dir/share/.
check_exit_status $? "cp -a python_extras"

# deliver gdb-guard to libexec dir
mkdir -p $install_dir/libexec
check_exit_status $? mkdir

cp -a $install_dir/bin/gdb $install_dir/libexec/gdb-guard
check_exit_status $? cp

echo "############################################"
echo "#           Building uda plugin            #"
echo "############################################"

cd $top_level/third_party/gdb_uda
check_exit_status $? cd

CC=gcc make
check_exit_status $? make

# deliver the gdb uda support library to libdir
cp -a $top_level/third_party/gdb_uda/plugin/uda-plugin-cray-upc.so $install_dir/lib
check_exit_status $? cp

echo "############################################"
echo "#              Building MRNet              #"
echo "############################################"

cti_pkgconfig_L_dir=$(pkg-config --libs-only-L common_tools_fe)
if [ $? -ne 0 ]; then
    echo "Could not find CTI in pkgconfig path. CTI may not be installed. Cannot continue with MRNet build."
    exit 1
fi

cti_pkgconfig_I_dir=$(pkg-config --cflags-only-I common_tools_fe)
if [ $? -ne 0 ]; then
    echo "Could not find CTI in pkgconfig path. CTI may not be installed. Cannot continue with MRNet build."
    exit 1
fi

cti_lib_directory=`echo $cti_pkgconfig_L_dir | cut -c 3-`
cti_include_directory=`echo $cti_pkgconfig_I_dir | cut -c 3-`

echo "cti_pkgconfig_L_dir:    $cti_pkgconfig_L_dir"
echo "cti_pkgconfig_I_dir:    $cti_pkgconfig_I_dir"
echo "cti_lib_directory:      $cti_lib_directory"
echo "cti_include_directory:  $cti_include_directory"

if [ ! -d ${CDST_SUPPORT_INSTALL_DIR} ]; then
    echo "Could not find cray-cdst-support install path."
    exit 1
fi

# ensure submodule is checked out
if [ ! -f $top_level/third_party/mrnet/README ]
then
    echo "MRNet submodule not checked out..."
    exit 1
fi

cd $top_level/third_party/mrnet
check_exit_status $? cd

mrnet_cflags=$(pkg-config --cflags cray-cdst-support)
mrnet_cxxflags=${mrnet_cflags}
mrnet_ldflags="$(pkg-config --libs cray-cdst-support) "'-Wl,-rpath,\$$ORIGIN'
mrnet_soflags="-Wl,-rpath,/opt/cray/pe/gcc-libs"

echo "mrnet_cflags:     $mrnet_cflags"
echo "mrnet_cxxflags:   $mrnet_cxxflags"
echo "mrnet_ldflags:    $mrnet_ldflags"
echo "mrnet_soflags:    $mrnet_soflags"

CFLAGS=${mrnet_cflags} CXXFLAGS=${mrnet_cxxflags} LDFLAGS=${mrnet_ldflags} SOFLAGS=${mrnet_soflags} ./configure --prefix=$install_dir --with-startup=cray-cti --enable-shared --with-craycti-inc=${cti_include_directory} --with-craycti-lib=${cti_lib_directory} --with-boost=${CDST_SUPPORT_INSTALL_DIR}
check_exit_status $? configure

# parallel builds are broken with mrnet
make
check_exit_status $? make

make install
check_exit_status $? "make install"

# deliver extra config files
cp -a $top_level/third_party/mrnet/build/*/xplat_config.h $install_dir/include/.
check_exit_status $? cp
cp -a $top_level/third_party/mrnet/build/*/mrnet_config.h $install_dir/include/.
check_exit_status $? cp

echo "############################################"
echo "#             Building gdb4hpc             #"
echo "############################################"

cd $top_level/guard
check_exit_status $? cd

autoreconf -ifv
check_exit_status $? autoreconf

PATH="$PATH:$install_dir/libexec" CFLAGS="-g -O2 -Wall" CXXFLAGS="-g -O2 -Wall" LDFLAGS=$(pkg-config --libs cray-cdst-support) ./configure --prefix=$install_dir --with-aif=$install_dir --with-rd --with-mrnet=$install_dir --with-gdbmi=$install_dir --with-libssh2=${CDST_SUPPORT_INSTALL_DIR} --with-cray
check_exit_status $? configure

make $j_flags
check_exit_status $? make

make $j_flags install
check_exit_status $? "make install"

# deliver manpage
mkdir -p $install_dir/share/man/man1
check_exit_status $? mkdir
cp -a $top_level/man/man1/gdb4hpc.1 $install_dir/share/man/man1/.
check_exit_status $? cp

echo "############################################"
echo "#              Done with build             #"
echo "############################################"
exit $return_code
