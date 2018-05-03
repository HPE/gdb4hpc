#!/bin/sh

# Create windows executable file properties for GDB on Windows.
# These are visible in context menu Properties / Details on the GDB
# executable on Windows.
#
# Copyright (C) 2013-2016 Free Software Foundation, Inc.
#
# Contributed by Intel Corporation
#
# This file is part of GDB.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

# Create windows_exe_properties.h from given parameters
# Usage:
#    create-windows_exe_properties.sh PATH-TO-GDB-SRCDIR HOST_ALIAS \
#        TARGET_ALIAS OUTPUT-FILE-NAME OPTIONS
#
#    See also environment variables below to customize some of the
#    description fields.
#

# Shell parameters.
srcdir="$1"
host_alias="$2"
target_alias="$3"
output="$4"

if [ $# -ne 4 ] ; then
  echo "usage: $0 PATH-TO-GDB-SRCDIR HOST_ALIAS TARGET_ALIAS OUTPUT-FILE-NAME" >&2
  exit 1
fi

echo > $output

# Default option values for the file property text fields.
version=`cat $srcdir/version.in`
file_description="GNU gdb (GDB)"
if [ -n "$host_alias" ] ; then
  file_description="GNU gdb (GDB) configured as $host_alias"
fi
product_name=""
original_filename="gdb.exe"
copyright="Copyright (C) 2013 Free Software Foundation, Inc."

# Check for environment variables to replace certain file properties.
[ -n "$WINDOWS_EXE_VERSION" ] && version=$WINDOWS_EXE_VERSION
[ -n "$WINDOWS_EXE_FILE_DESCRIPTION" ] && file_description=$WINDOWS_EXE_FILE_DESCRIPTION
[ -n "$WINDOWS_EXE_PRODUCT_NAME" ] && product_name=$WINDOWS_EXE_PRODUCT_NAME
[ -n "$WINDOWS_EXE_ORIGINAL_FILENAME" ] && original_filename=$WINDOWS_EXE_ORIGINAL_FILENAME
[ -n "$WINDOWS_EXE_COPYRIGHT" ] && copyright=$WINDOWS_EXE_COPYRIGHT

# Use GDB external version as product version, but remove "#" at the beginning and remove internal version.
product_version=`echo ${version%-*} | sed -e "s/#//"`
# Use GDB internal version as file version, but remove "#" at the beginning and remove external version and cvs tags.
file_version=`echo ${version#*-} | sed -e "s/#//" | sed -e "s/cvs//"` 
# If there is no internal version, then also use the external version as file version.
[ -z "$file_version" ] && file_version=$product_version

# Split version.
IFS_BAK="$IFS"
IFS='.'
product_array=($product_version)
product_version_major=${product_array[0]}
product_version_minor=${product_array[1]}
product_version_build=${product_array[2]}
file_array=($file_version)
file_version_major=${file_array[0]}
file_version_minor=${file_array[1]}
file_version_build=${file_array[2]}
IFS="$IFS_BAK"
[ -z "$product_version_major" ] && product_version_major=0
[ -z "$product_version_minor" ] && product_version_minor=0
[ -z "$product_version_build" ] && product_version_build=0
[ -z "$file_version_major" ] && file_version_major=0
[ -z "$file_version_minor" ] && file_version_minor=0
[ -z "$file_version_build" ] && file_version_build=0

# Write versions into output file.
echo "/* Original GDB version : $version */" >> $output
echo "#define FP_PRODUCT_VERSION_MAJOR $product_version_major" >> $output
echo "#define FP_PRODUCT_VERSION_MINOR $product_version_minor" >> $output
echo "#define FP_PRODUCT_VERSION_BUILD $product_version_build" >> $output
echo "#define FP_PRODUCT_VERSION \"$product_version_major.$product_version_minor.$product_version_build\"" >> $output
echo "" >> $output
echo "#define FP_FILE_VERSION_MAJOR $file_version_major" >> $output
echo "#define FP_FILE_VERSION_MINOR $file_version_minor" >> $output
echo "#define FP_FILE_VERSION_BUILD $file_version_build" >> $output
echo "#define FP_FILE_VERSION \"$file_version_major.$file_version_minor.$file_version_build\"" >> $output
echo "" >> $output

# Write other file properties into output file.
echo "#define FP_FILE_DESCRIPTION  \"$file_description\""  >> $output
echo "#define FP_COPYRIGHT         \"$copyright\""         >> $output
echo "#define FP_ORIGINAL_FILENAME \"$original_filename\"" >> $output
echo "#define FP_PRODUCT_NAME      \"$product_name\""      >> $output
