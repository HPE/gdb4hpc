#!/bin/bash
#
# runBuild.sh - Build steps for gdb4hpc
#
# Copyright 2010-2024 Hewlett Packard Enterprise Development LP.
#
# Unpublished Proprietary Information.
# This unpublished work is protected to trade secret, copyright and other laws.
# Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
# no part of this work or its content may be used, reproduced or disclosed
# in any form.
#

echo "############################################"
echo "#            Setup environment.            #"
echo "############################################"

top_level=$PWD

source ./external/cdst_build_library/build_lib_gcc

setup_modules

module load cray-cdst-support
check_exit_status

module load cray-cti-devel
check_exit_status

module load cray-mrnet
check_exit_status

module load cray-zmqnet-devel
check_exit_status

module list

# Create install directory
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

if [[ ! -z $BRANCH_NAME ]]; then
  echo "cleaning build env"
  rm -rf /opt/cray/pe/gdb4hpc

  # Ensure our DST working directory is usable (safe) by user other than owner
  echo "Setting git safe directory . . ." 
  git config --global --add safe.directory /home/jenkins
  check_exit_status
fi

mkdir -p $install_dir
check_exit_status

# ensure python3 exists
python_version=''
python_cflags=''
python_ldflags="-L/usr/lib64"
python_loc=$(command -v python3)
if [[ ! -x "$python_loc" ]]; then
    echo "python3 not found on system..."
    exit 1
else
  python_version=$(/usr/bin/python3 --version | cut -d' ' -f2 | cut -d'.' -f1-2 | xargs)
  if [[ "$python_version" == "3.6" ]]; then
    python_cflags="$(pkg-config --cflags python3)"
    python_ldflags="${python_ldflags} $(pkg-config --libs-only-l python3)"
  else
    python_cflags="$(python3-config --includes | cut -d' ' -f1)"
    python_ldflags="${python_ldflags} -lpython${python_version}"
  fi
fi


echo "############################################"
echo "#               Building libmi             #"
echo "############################################"

# ensure submodule is checked out
if [ ! -f $top_level/external/libmi/README ]
then
    echo "libmi submodule not checked out..."
    exit 1
fi

cd $top_level/external/libmi
check_exit_status

autoreconf -ifv
check_exit_status

CFLAGS="-g -O2 -Wall" ./configure --prefix=$install_dir
check_exit_status

make $cdst_j_flags
check_exit_status

make $cdst_j_flags install
check_exit_status

echo "############################################"
echo "#               Building gdb               #"
echo "############################################"
#TODO:  Both the gdb abd cuda gdb builds take longer than we need; however,
#       because of a non-recursive configure, both require make to run from the
#       top in order to generate the makefiles for the gdb directory.  This is
#       an area we could signficiantly speed up the build, but we're waiting on
#       the release process to be defined properly; at that juncture it may be
#       possible that we replace the gdb backend builds here with a separate rpm
#       of gdb backends included in the build prep.

ddt_gdb=ddt_gdb-10.0

# ensure submodule is checked out
if [[ ! -f $top_level/external/$ddt_gdb/README ]]; then
    echo "gdb submodule not checked out..."
    exit 1
fi

cd $top_level/external/$ddt_gdb
check_exit_status

./configure --prefix=$install_dir \
            --with-gdb-datadir=$install_dir/share/gdb \
            --with-python=$python_loc \
            --disable-werror \
            --with-mmalloc \
            --without-babeltrace \
            --without-guile \
            --without-separate-debug-dir \
	CFLAGS="-O2" CXXFLAGS="-O2"
check_exit_status

make $cdst_j_flags
check_exit_status

# gdb is part of binutils. We don't want all of binutils to install, only gdb.
cd $top_level/external/$ddt_gdb/gdb
check_exit_status
make $cdst_j_flags install
check_exit_status

# deliver pretty-printers
cp -a $top_level/external/python_extras/gdb $install_dir/share/.
check_exit_status

# deliver gdb-guard to libexec dir
mkdir -p $install_dir/libexec
check_exit_status

cp -a $install_dir/bin/gdb $install_dir/libexec/gdb-guard
check_exit_status

echo "############################################"
echo "#             Building ompd plugin         #"
echo "############################################"

ompd_dir=$install_dir/share/ompd
ompd_build=$ompd_dir/build

mkdir -p $ompd_build
check_exit_status

mkdir -p $ompd_dir

cd $top_level/external/openmp/libompd/gdb-plugin
check_exit_status

cp ../../runtime/src/include/omp-tools.h.var omp-tools.h
check_exit_status
python3 ./setup.py -v bdist_wheel -b $ompd_build -d $ompd_build
check_exit_status
python3 ./setup.py clean --all
check_exit_status
rm -R ompd.egg-info
check_exit_status
python3 -m pip install -U -t $ompd_build/python-module --no-index  . .
check_exit_status
rm omp-tools.h
check_exit_status

cd $ompd_dir
check_exit_status
mkdir -p gdb
check_exit_status
cp $ompd_build/python-module/ompd/*.* gdb
check_exit_status
rm -R build
check_exit_status

echo "############################################"
echo "#             Building gdb4hpc             #"
echo "############################################"

# Get cray-mrnet pkg-config
mrnet_pkgconfig_L_dir=$(pkg-config --libs-only-L cray-mrnet)
if [[ $? -ne 0 ]]; then
  echo "Could not find cray-mrnet in pkgconfig path. cray-mrnet may not be installed. Cannot continue with the build."
  exit 1
fi

mrnet_pkgconfig_I_dir=$(pkg-config --cflags-only-I cray-mrnet)
if [[ $? -ne 0 ]]; then
  echo "Could not find cray-mrnet in pkgconfig path. cray-mrnet may not be installed. Cannot continue with the build."
  exit 1
fi

mrnet_lib_directory=$(echo $mrnet_pkgconfig_L_dir | cut -c 3-)
mrnet_include_directory=$(echo $mrnet_pkgconfig_I_dir | cut -c 3-)

echo "mrnet_pkgconfig_L_dir:    $mrnet_pkgconfig_L_dir"
echo "mrnet_pkgconfig_I_dir:    $mrnet_pkgconfig_I_dir"
echo "mrnet_lib_directory:      $mrnet_lib_directory"
echo "mrnet_include_directory:  $mrnet_include_directory"

mrnet_cflags=$(pkg-config --cflags cray-mrnet)
mrnet_cxxflags=${mrnet_cflags}
mrnet_ldflags="$(pkg-config --libs cray-mrnet) "'-Wl,-rpath,\$$ORIGIN -Wl,--as-needed'

echo "mrnet_cflags:     $mrnet_cflags"
echo "mrnet_cxxflags:   $mrnet_cxxflags"
echo "mrnet_ldflags:    $mrnet_ldflags"

cdst_ldflags=$(pkg-config --libs cray-cdst-support)
mrnet_ldflags="${cdst_ldflags} ${mrnet_ldflags}"

echo "Python cflags:  $python_cflags"
echo "Python ldflags: $python_ldflags"

mrnet_base_directory=$(realpath -s $mrnet_include_directory/../../default)

cd $top_level
check_exit_status

autoreconf -ifv
check_exit_status

PATH="$PATH:$install_dir/libexec" CFLAGS="-g -O2 -Wall -Wextra -Werror ${mrnet_cflags}" CXXFLAGS="-g -O2 -Wall -Wextra -Werror ${mrnet_cxxflags}" LDFLAGS=${mrnet_ldflags} ./configure \
  --prefix=$install_dir \
  --with-rd \
  --with-mrnet=${mrnet_base_directory} \
  --with-zmqnet=yes \
  --with-gdbmi=$install_dir \
  --with-libssh2=${CRAY_CDST_SUPPORT_INSTALL_DIR} \
  --with-elfutils=${CRAY_CDST_SUPPORT_INSTALL_DIR} \
  --with-cray \
  --with-python \
  --with-python-compat=${top_level}/external/pythoncapi-compat \
  --with-tinystl=${top_level}/external/tinystl \
  --with-doctest=${top_level}/external/doctest \
  --enable-doctest

check_exit_status

make $cdst_j_flags
check_exit_status

make $cdst_j_flags install
check_exit_status

# deliver manpage
mkdir -p $install_dir/share/man/man1
check_exit_status
cp -a $top_level/doc/man/gdb4hpc.1 $install_dir/share/man/man1/.
check_exit_status

# deliver doc
mkdir -p $install_dir/share/doc
check_exit_status
cp -a $top_level/doc/guides/*.md $install_dir/share/doc/.
check_exit_status
cp -a $top_level/doc/guides/*.html $install_dir/share/doc/.
check_exit_status

# deliver extensions
mkdir -p "$install_dir/share/py-gdb"
check_exit_status
cp -a "$top_level/src/extensions/python-gdb.py" "$install_dir/share/py-gdb"
check_exit_status

echo "############################################"
echo "#              Done with build             #"
echo "############################################"

exit_with_status
