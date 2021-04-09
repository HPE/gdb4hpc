#
# cray_extensions.m4 Cray configure extensions.
#
# Copyright 2011-2021 Hewlett Packard Enterprise Development LP.
#
#     Redistribution and use in source and binary forms, with or
#     without modification, are permitted provided that the following
#     conditions are met:
#
#      - Redistributions of source code must retain the above
#        copyright notice, this list of conditions and the following
#        disclaimer.
#
#      - Redistributions in binary form must reproduce the above
#        copyright notice, this list of conditions and the following
#        disclaimer in the documentation and/or other materials
#        provided with the distribution.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
# BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
# ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#

dnl
dnl read release and library version information from disk
dnl This is not portable due to source...
dnl
AC_DEFUN([cray_INIT],
[
    dnl Pull in the revision information from the $PWD/release_versioning file
    m4_define([BUILD_NUMBER], [m4_esyscmd_s([source $PWD/release_versioning; echo "$build_number"])])
    m4_define([REVISION], [m4_esyscmd_s([source $PWD/release_versioning; echo "$revision"])])

    m4_define([MAJOR], [m4_esyscmd_s([source $PWD/release_versioning; echo "$major"])])
    m4_define([MINOR], [m4_esyscmd_s([source $PWD/release_versioning; echo "$minor"])])

    AC_ARG_VAR([GDB4HPC_BUILD_NUMBER],[Build Number])
    AC_SUBST([GDB4HPC_BUILD_NUMBER],[BUILD_NUMBER])
    if test -z "$GDB4HPC_BUILD_NUMBER" || -n "$GDB4HPC_BUILD_NUMBER" ; then 
        dnl Build number is empty - Building from release branch
        AC_MSG_NOTICE([checking for build status - Building from release branch])
        AC_DEFINE_UNQUOTED([GUARD_VERSION], ["MAJOR.MINOR.REVISION"], [Version number of gdb4hpc package.])	
    else
        dnl Build number is not empty - Building from other than branch
        AC_MSG_NOTICE([checking for build status - Building from other than release branch])
        AC_DEFINE_UNQUOTED([GUARD_VERSION], ["MAJOR.MINOR.REVISION.BUILD_NUMBER"], [Version number of gdb4hpc package.])
    fi

    AC_DEFINE([CRAY_MAJOR], [MAJOR], [Package major version])
    AC_DEFINE([CRAY_MINOR], [MINOR], [Package minor version])
    AC_DEFINE([CRAY_REVISION], [REVISION], [Package revision])
])
