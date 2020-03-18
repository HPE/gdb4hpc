# Packaging definitions
%global pkgversion %(%{_sourcedir}/get_package_data --crayversion)
%global pkgversion_separator -
%global copyright Copyright 2007-2020 Cray Inc. All rights reserved.

# RPM build time
%global release_date %(date +%%B\\ %%Y)

%global product gdb4hpc
%global cray_product %{product}
%global cray_name %{cray_product}

# Path definitions
%global cray_prefix /opt/cray/pe
%global test_prefix /opt/cray/tests

#FIXME: This should be relocatable
%global external_build_dir %{cray_prefix}/%{cray_product}/%{pkgversion}

# modulefile definitions
%global modulefile_name %{cray_name}
%global module_template_name %{modulefile_name}.module.template

# set_default modulefile definitions
%global set_default_command set_default
%global set_default_template_name %{set_default_command}.template
%global set_default_path admin-pe/set_default_files

# lmod modulefiles
%global lmod_template_gdb4hpc template_%{product}.lua

# release info
%global release_info_name release_info
%global release_info_template_name %{release_info_name}.template

# yaml file
%global yaml_template yaml.template
%global removal_date %(date '+%Y-%m-%d' -d "+5 years")

# attributions file
%global attributions_name ATTRIBUTIONS_gdb4hpc.txt

# cdst-support version
%global cdst_support_pkgversion_min %(%{_sourcedir}/get_package_data --cdstversionmin)
%global cdst_support_pkgversion_max %(%{_sourcedir}/get_package_data --cdstversionmax)

# cti version
%global cti_pkgversion_min %(%{_sourcedir}/get_package_data --ctiversionmin)
%global cti_pkgversion_max %(%{_sourcedir}/get_package_data --ctiversionmax)

# Disable debug package
%global debug_package %{nil}

# System strip command may be too old, use current path
%global __strip strip

# Filter requires - these all come from the cdst-support rpm
# These should match what is excluded in the cdst-support rpm specfile!
%global privlibs             libboost.*
%global privlibs %{privlibs}|libmrnet
%global privlibs %{privlibs}|libbitset
%global privlibs %{privlibs}|libmi
%global privlibs %{privlibs}|libssh2
%global privlibs %{privlibs}|libarchive
%global privlibs %{privlibs}|libsymtabAPI
%global privlibs %{privlibs}|libpcontrol
%global privlibs %{privlibs}|libdynElf
%global privlibs %{privlibs}|libdynDwarf
%global privlibs %{privlibs}|libcommon
%global privlibs %{privlibs}|libxplat
%global privlibs %{privlibs}|libedit
%global privlibs %{privlibs}|libdw
%global privlibs %{privlibs}|libebl.*
%global privlibs %{privlibs}|libelf
%global privlibs %{privlibs}|libtbbmalloc*
%global privlibs %{privlibs}|libtbb.*
%global __requires_exclude ^(%{privlibs})\\.so*

# Dist tags for SuSE need to be manually set
%if 0%{?suse_version}
%if 0%{?suse_version} == 1315
%global dist .sles12
%endif
%if 0%{?sle_version} == 120000
%global dist .sles12
%endif
%if 0%{?sle_version} == 120100
%global dist .sles12sp1
%endif
%if 0%{?sle_version} == 120200
%global dist .sles12sp2
%endif
%if 0%{?sle_version} == 120300
%global dist .sles12sp3
%endif
%if 0%{?sle_version} == 120400
%global dist .sles12sp4
%endif
%if 0%{?sle_version} == 150000
%global dist .sles15
%endif
%if 0%{?sle_version} == 150100
%global dist .sles15sp1
%endif
%endif

Summary:    Cray gdb4hpc parallel debugger
Name:       %{cray_name}
Version:    %{pkgversion}
# BUILD_METADATA is set by Jenkins
Release:    %(echo ${BUILD_METADATA})%{dist}
Prefix:     %{cray_prefix}
License:    %{copyright}
Vendor:     Cray Inc.
Group:      Development/Tools/Debuggers
Provides:   %{cray_name} = %{pkgversion}
Requires:   set_default_2, cray-cdst-support >= %{cdst_support_pkgversion_min}, cray-cdst-support < %{cdst_support_pkgversion_max}, cray-cti >= %{cti_pkgversion_min}, cray-cti < %{cti_pkgversion_max}
Source0:    %{module_template_name}
Source1:    %{set_default_template_name}
Source2:    %{release_info_template_name}
Source3:    %{attributions_name}
Source4:    %{lmod_template_gdb4hpc}
Source5:    %{yaml_template}

%description
Cray gdb4hpc %{pkgversion}
gdb4hpc is a gdb based parallel debugger used to debug MPI/SHMEM/UPC/Fortran 2008 applications written in C/C++/Fortran.

%prep
# Run q(uiet) with build directory name, c(reate) subdirectory, disable T(arball) unpacking
%setup -q -n %{name} -c -T
%build
# external build
%{__sed} 's|<RELEASE_DATE>|%{release_date}|g;s|<VERSION>|%{pkgversion}|g;s|<RELEASE>|%{release}|g;s|<COPYRIGHT>|%{copyright}|g;s|<CRAY_NAME>|%{cray_name}|g;s|<CRAY_PREFIX>|%{cray_prefix}|g;s|<ARCH>|%{_target_cpu}|g' %{SOURCE2} > ${RPM_BUILD_DIR}/%{release_info_name}
%{__cp} -a %{SOURCE3} ${RPM_BUILD_DIR}/%{attributions_name}

%install
# copy files from external install
#
# Cray PE package root
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}
# Information files
%{__cp} -a ${RPM_BUILD_DIR}/%{release_info_name} ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/%{release_info_name}
%{__cp} -a ${RPM_BUILD_DIR}/%{attributions_name} ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/%{attributions_name}
# Libraries
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/lib
%{__cp} -a %{external_build_dir}/lib/libbitset.so ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/lib
%{__cp} -a %{external_build_dir}/lib/libdbgagg.so ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/lib
%{__cp} -a %{external_build_dir}/lib/libmi.so ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/lib
%{__cp} -a %{external_build_dir}/lib/libmrnet.so* ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/lib
%{__cp} -a %{external_build_dir}/lib/libxplat.so* ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/lib
%{__cp} -a %{external_build_dir}/lib/uda-plugin-cray-upc.so ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/lib
# Libraries static
# pkg-config
# Headers
# libexec binaries
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/libexec
%{__cp} -a %{external_build_dir}/libexec/dbgsrv ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/libexec
%{__cp} -a %{external_build_dir}/libexec/dbgsrv_alps ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/libexec
%{__cp} -a %{external_build_dir}/libexec/gdb-guard ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/libexec
%{__cp} -a %{external_build_dir}/libexec/cuda101-gdb-guard ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/libexec
%{__cp} -a %{external_build_dir}/libexec/roc_alpha-gdb-guard ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/libexec
%{__cp} -a %{external_build_dir}/libexec/guard_proxy ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/libexec
%{__cp} -a %{external_build_dir}/libexec/start_proxy.sh ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/libexec
%{__cp} -a %{external_build_dir}/bin/mrnet_commnode ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/libexec
%{__cp} -a %{external_build_dir}/bin/mrnet_topgen ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/libexec
# Binaries
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/bin
%{__cp} -a %{external_build_dir}/bin/gdb4hpc ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/bin
# ddt-gdb Share
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/boost
%{__cp} -a %{external_build_dir}/share/gdb/python/boost/__init__.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/boost
%{__cp} -a %{external_build_dir}/share/gdb/python/boost/printers.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/boost
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb
%{__cp} -a %{external_build_dir}/share/gdb/python/gdb/FrameDecorator.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb
%{__cp} -a %{external_build_dir}/share/gdb/python/gdb/FrameIterator.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb
%{__cp} -a %{external_build_dir}/share/gdb/python/gdb/frames.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb
%{__cp} -a %{external_build_dir}/share/gdb/python/gdb/__init__.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb
%{__cp} -a %{external_build_dir}/share/gdb/python/gdb/printing.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb
%{__cp} -a %{external_build_dir}/share/gdb/python/gdb/prompt.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb
%{__cp} -a %{external_build_dir}/share/gdb/python/gdb/types.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb
%{__cp} -a %{external_build_dir}/share/gdb/python/gdb/unwinder.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb
%{__cp} -a %{external_build_dir}/share/gdb/python/gdb/xmethod.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/command
%{__cp} -a %{external_build_dir}/share/gdb/python/gdb/command/explore.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/command
%{__cp} -a %{external_build_dir}/share/gdb/python/gdb/command/frame_filters.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/command
%{__cp} -a %{external_build_dir}/share/gdb/python/gdb/command/__init__.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/command
%{__cp} -a %{external_build_dir}/share/gdb/python/gdb/command/pretty_printers.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/command
%{__cp} -a %{external_build_dir}/share/gdb/python/gdb/command/prompt.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/command
%{__cp} -a %{external_build_dir}/share/gdb/python/gdb/command/tsx.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/command
%{__cp} -a %{external_build_dir}/share/gdb/python/gdb/command/type_printers.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/command
%{__cp} -a %{external_build_dir}/share/gdb/python/gdb/command/unwinders.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/command
%{__cp} -a %{external_build_dir}/share/gdb/python/gdb/command/xmethods.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/command
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/function
%{__cp} -a %{external_build_dir}/share/gdb/python/gdb/function/as_string.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/function
%{__cp} -a %{external_build_dir}/share/gdb/python/gdb/function/caller_is.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/function
%{__cp} -a %{external_build_dir}/share/gdb/python/gdb/function/__init__.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/function
%{__cp} -a %{external_build_dir}/share/gdb/python/gdb/function/strfns.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/function
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/printer
%{__cp} -a %{external_build_dir}/share/gdb/python/gdb/printer/bound_registers.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/printer
%{__cp} -a %{external_build_dir}/share/gdb/python/gdb/printer/__init__.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/printer
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/libstdcxx
%{__cp} -a %{external_build_dir}/share/gdb/python/libstdcxx/__init__.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/libstdcxx
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/libstdcxx/v6
%{__cp} -a %{external_build_dir}/share/gdb/python/libstdcxx/v6/__init__.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/libstdcxx/v6
%{__cp} -a %{external_build_dir}/share/gdb/python/libstdcxx/v6/printers.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/libstdcxx/v6
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/syscalls
%{__cp} -a %{external_build_dir}/share/gdb/syscalls/*.xml ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/syscalls
%{__cp} -a %{external_build_dir}/share/gdb/syscalls/gdb-syscalls.dtd ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/syscalls
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/system-gdbinit
%{__cp} -a %{external_build_dir}/share/gdb/system-gdbinit/elinos.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/system-gdbinit
%{__cp} -a %{external_build_dir}/share/gdb/system-gdbinit/wrs-linux.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/gdb/system-gdbinit
# cuda-gdb Share
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/python/gdb/FrameDecorator.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/python/gdb/FrameIterator.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/python/gdb/frames.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/python/gdb/__init__.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/python/gdb/printing.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/python/gdb/prompt.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/python/gdb/types.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/python/gdb/unwinder.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/python/gdb/xmethod.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/command
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/python/gdb/command/explore.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/command
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/python/gdb/command/frame_filters.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/command
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/python/gdb/command/__init__.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/command
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/python/gdb/command/pretty_printers.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/command
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/python/gdb/command/prompt.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/command
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/python/gdb/command/tsx.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/command
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/python/gdb/command/type_printers.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/command
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/python/gdb/command/unwinders.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/command
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/python/gdb/command/xmethods.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/command
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/function
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/python/gdb/function/as_string.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/function
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/python/gdb/function/caller_is.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/function
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/python/gdb/function/__init__.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/function
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/python/gdb/function/strfns.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/function
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/printer
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/python/gdb/printer/bound_registers.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/printer
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/python/gdb/printer/__init__.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/printer
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/syscalls
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/syscalls/*.xml ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/syscalls
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/syscalls/gdb-syscalls.dtd ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/syscalls
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/system-gdbinit
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/system-gdbinit/elinos.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/system-gdbinit
%{__cp} -a %{external_build_dir}/cuda_gdb_install/share/cuda-gdb/system-gdbinit/wrs-linux.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/system-gdbinit
# roc-gdb Share
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb
%{__cp} -a %{external_build_dir}/roc_gdb_install/share/roc-gdb/python/gdb/FrameDecorator.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb
%{__cp} -a %{external_build_dir}/roc_gdb_install/share/roc-gdb/python/gdb/FrameIterator.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb
%{__cp} -a %{external_build_dir}/roc_gdb_install/share/roc-gdb/python/gdb/frames.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb
%{__cp} -a %{external_build_dir}/roc_gdb_install/share/roc-gdb/python/gdb/__init__.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb
%{__cp} -a %{external_build_dir}/roc_gdb_install/share/roc-gdb/python/gdb/printing.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb
%{__cp} -a %{external_build_dir}/roc_gdb_install/share/roc-gdb/python/gdb/prompt.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb
%{__cp} -a %{external_build_dir}/roc_gdb_install/share/roc-gdb/python/gdb/types.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb
%{__cp} -a %{external_build_dir}/roc_gdb_install/share/roc-gdb/python/gdb/unwinder.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb
%{__cp} -a %{external_build_dir}/roc_gdb_install/share/roc-gdb/python/gdb/xmethod.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb/command
%{__cp} -a %{external_build_dir}/roc_gdb_install/share/roc-gdb/python/gdb/command/explore.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb/command
%{__cp} -a %{external_build_dir}/roc_gdb_install/share/roc-gdb/python/gdb/command/frame_filters.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb/command
%{__cp} -a %{external_build_dir}/roc_gdb_install/share/roc-gdb/python/gdb/command/__init__.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb/command
%{__cp} -a %{external_build_dir}/roc_gdb_install/share/roc-gdb/python/gdb/command/pretty_printers.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb/command
%{__cp} -a %{external_build_dir}/roc_gdb_install/share/roc-gdb/python/gdb/command/prompt.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb/command
%{__cp} -a %{external_build_dir}/roc_gdb_install/share/roc-gdb/python/gdb/command/type_printers.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb/command
%{__cp} -a %{external_build_dir}/roc_gdb_install/share/roc-gdb/python/gdb/command/unwinders.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb/command
%{__cp} -a %{external_build_dir}/roc_gdb_install/share/roc-gdb/python/gdb/command/xmethods.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb/command
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb/function
%{__cp} -a %{external_build_dir}/roc_gdb_install/share/roc-gdb/python/gdb/function/as_string.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb/function
%{__cp} -a %{external_build_dir}/roc_gdb_install/share/roc-gdb/python/gdb/function/caller_is.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb/function
%{__cp} -a %{external_build_dir}/roc_gdb_install/share/roc-gdb/python/gdb/function/__init__.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb/function
%{__cp} -a %{external_build_dir}/roc_gdb_install/share/roc-gdb/python/gdb/function/strfns.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb/function
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb/printer
%{__cp} -a %{external_build_dir}/roc_gdb_install/share/roc-gdb/python/gdb/printer/bound_registers.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb/printer
%{__cp} -a %{external_build_dir}/roc_gdb_install/share/roc-gdb/python/gdb/printer/__init__.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb/printer
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/syscalls
%{__cp} -a %{external_build_dir}/roc_gdb_install/share/roc-gdb/syscalls/*.xml ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/syscalls
%{__cp} -a %{external_build_dir}/roc_gdb_install/share/roc-gdb/syscalls/gdb-syscalls.dtd ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/syscalls
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/system-gdbinit
%{__cp} -a %{external_build_dir}/roc_gdb_install/share/roc-gdb/system-gdbinit/elinos.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/system-gdbinit
%{__cp} -a %{external_build_dir}/roc_gdb_install/share/roc-gdb/system-gdbinit/wrs-linux.py ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/system-gdbinit
# manpage
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/man/man1
%{__cp} -a %{external_build_dir}/share/man/man1/gdb4hpc.1 ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/share/man/man1
# modulefile
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/modulefiles/%{modulefile_name}
%{__sed} 's|<PREFIX>|%{external_build_dir}|g;s|<CRAY_NAME>|%{cray_name}|g;s|<VERSION>|%{pkgversion}|g' %{SOURCE0} > ${RPM_BUILD_ROOT}/%{prefix}/modulefiles/%{modulefile_name}/%{pkgversion}
# lmod modulefile
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/lmod/modulefiles/core/%{cray_name}
%{__sed} 's|\[@%PREFIX_PATH%@\]|%{prefix}|g;s|\[@%MODULE_VERSION%@\]|%{pkgversion}|g' %{SOURCE4} > ${RPM_BUILD_ROOT}/%{prefix}/lmod/modulefiles/core/%{cray_name}/%{pkgversion}.lua
# set_default scripts
%{__sed} 's|<PREFIX>|%{prefix}|g;s|<CRAY_PRODUCT>|%{cray_product}|g;s|<VERSION>|%{pkgversion}|g;s|<MODULEFILE_NAME>|%{modulefile_name}|g' %{SOURCE1} > ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/%{set_default_command}_%{cray_product}_%{pkgversion}
# set_default into admin-pe
%{__install} -d ${RPM_BUILD_ROOT}/%{prefix}/%{set_default_path}
%{__install} -D ${RPM_BUILD_ROOT}/%{prefix}/%{cray_product}/%{pkgversion}/%{set_default_command}_%{cray_product}_%{pkgversion} ${RPM_BUILD_ROOT}/%{prefix}/%{set_default_path}/%{set_default_command}_%{cray_product}_%{pkgversion}
# yaml file
%{__mkdir} -p %{_rpmdir}/%{_arch}
%{__sed} 's|<PRODUCT>|%{cray_product}|g;s|<VERSION>|%{pkgversion}|g;s|<BUILD_METADATA>|%{release}|g;s|<ARCH>|%{_arch}|g;s|<REMOVAL_DATE>|%{removal_date}|g' %{SOURCE5} > %{_rpmdir}/%{_arch}/%{cray_product}-%{pkgversion}-%{release}.%{_arch}.rpm.yaml

%post
# Add Shasta configuration files for PE content projection
if [ "${RPM_INSTALL_PREFIX}" = "%{cray_prefix}" ]; then
    %{__mkdir_p} /etc/%{cray_prefix}/admin-pe/bindmount.conf.d/
    %{__mkdir_p} /etc/%{cray_prefix}/admin-pe/modulepaths.conf.d/
    echo "%{cray_prefix}/%{product}/%{pkgversion}" > /etc/%{cray_prefix}/admin-pe/bindmount.conf.d/%{cray_name}.conf
    echo "%{cray_prefix}/modulefiles/%{modulefile_name}" > /etc/%{cray_prefix}/admin-pe/modulepaths.conf.d/%{modulefile_name}.conf

    echo -e '#%Modulefile\r\nset  ModulesVersion "%{pkgversion}"' > %{prefix}/lmod/modulefiles/core/%{cray_name}/.version
fi

# Process relocations
if [ "${RPM_INSTALL_PREFIX}" != "%{prefix}" ]
then
    # Modulefile
    %{__sed} -i "s|^\([[:space:]]*set GDB4HPC_BASEDIR[[:space:]]*\)\(.*\)|\1${RPM_INSTALL_PREFIX}/%{cray_product}/%{pkgversion}|" ${RPM_INSTALL_PREFIX}/modulefiles/%{modulefile_name}/%{pkgversion}
    # set default command
    %{__sed} -i "s|^\(export CRAY_inst_dir=\).*|\1${RPM_INSTALL_PREFIX}|" ${RPM_INSTALL_PREFIX}/%{cray_product}/%{pkgversion}/%{set_default_command}_%{cray_product}_%{pkgversion}
else
    ${RPM_INSTALL_PREFIX}/%{set_default_path}/%{set_default_command}_%{cray_product}_%{pkgversion}

    # Don't want to set LD_LIBRARY_PATH or GDB4HPC_INSTALL_DIR if we are not relocating since rpath was set properly
    %{__sed} -i "/^[[:space:]]*prepend-path[[:space:]]*LD_LIBRARY_PATH.*/d" ${RPM_INSTALL_PREFIX}/modulefiles/%{modulefile_name}/%{pkgversion}
    %{__sed} -i "/^[[:space:]]*setenv[[:space:]]*GDB4HPC_INSTALL_DIR.*/d" ${RPM_INSTALL_PREFIX}/modulefiles/%{modulefile_name}/%{pkgversion}
fi

%preun
default_link="${RPM_INSTALL_PREFIX}/%{cray_product}/default"

# Cleanup module .version if it points to this version
if [ -f ${RPM_INSTALL_PREFIX}/modulefiles/%{modulefile_name}/.version ]
then
  dotversion=`grep ModulesVersion ${RPM_INSTALL_PREFIX}/modulefiles/%{modulefile_name}/.version | cut -f2 -d'"'`

  if [ "$dotversion" == "%{pkgversion}" ]
  then
    %{__rm} -f ${RPM_INSTALL_PREFIX}/modulefiles/%{modulefile_name}/.version
    echo "Uninstalled version and .version file match = ${default_version}."
    echo "Removing %{modulefile_name} .version file."
    %{__rm} -f ${default_link}
  fi
fi

%postun
if [ $1 == 1 ]
then
  exit 0
fi

# If the install dir exists
if [[ -z `ls ${RPM_INSTALL_PREFIX}/%{cray_product}` ]]; then
  %{__rm} -rf ${RPM_INSTALL_PREFIX}/%{cray_product}
  if [ -f /etc%{prefix}/admin-pe/bindmount.conf.d/%{cray_product}.conf ]; then
    %{__rm} -rf /etc%{prefix}/admin-pe/bindmount.conf.d/%{cray_product}.conf
  fi
  if [ -f /etc%{prefix}/admin-pe/modulepaths.conf.d/%{cray_product}.conf ]; then
    %{__rm} -rf /etc%{prefix}/admin-pe/modulepaths.conf.d/%{cray_product}.conf
  fi
  if [ -d ${RPM_INSTALL_PREFIX}/lmod/modulefiles/core/%{cray_product} ]; then
    %{__rm} -rf ${RPM_INSTALL_PREFIX}/lmod/modulefiles/core/%{cray_product}
  fi
  if [ -d ${RPM_INSTALL_PREFIX}/modulefiles/%{cray_product} ]; then
    %{__rm} -rf ${RPM_INSTALL_PREFIX}/modulefiles/%{cray_product}
  fi
fi

%files
%defattr(755, root, root)
%dir %{prefix}/%{cray_product}/%{pkgversion}
%dir %{prefix}/%{cray_product}/%{pkgversion}/lib
%dir %{prefix}/%{cray_product}/%{pkgversion}/libexec
%dir %{prefix}/%{cray_product}/%{pkgversion}/bin
%dir %{prefix}/%{cray_product}/%{pkgversion}/share
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/man
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/man/man1
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/gdb
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/syscalls
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/system-gdbinit
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/boost
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/command
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/function
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/printer
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/libstdcxx/v6
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/libstdcxx
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/command
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/function
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/printer
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/syscalls
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/system-gdbinit
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb/command
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb/function
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb/printer
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/syscalls
%dir %{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/system-gdbinit
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/%{release_info_name}
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/%{attributions_name}
%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/lib/libbitset.so
%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/lib/libdbgagg.so
%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/lib/libmi.so
%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/lib/libmrnet.so*
%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/lib/libxplat.so*
%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/lib/uda-plugin-cray-upc.so
%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/libexec/dbgsrv
%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/libexec/dbgsrv_alps
%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/libexec/gdb-guard
%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/libexec/cuda101-gdb-guard
%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/libexec/roc_alpha-gdb-guard
%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/libexec/guard_proxy
%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/libexec/start_proxy.sh
%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/libexec/mrnet_commnode
%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/libexec/mrnet_topgen
%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/bin/gdb4hpc
#gdb share
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/boost/__init__.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/boost/printers.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/FrameDecorator.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/FrameIterator.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/frames.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/__init__.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/printing.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/prompt.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/types.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/unwinder.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/xmethod.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/command/explore.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/command/frame_filters.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/command/__init__.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/command/pretty_printers.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/command/prompt.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/command/tsx.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/command/type_printers.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/command/unwinders.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/command/xmethods.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/function/as_string.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/function/caller_is.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/function/__init__.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/function/strfns.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/printer/bound_registers.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/gdb/printer/__init__.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/libstdcxx/__init__.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/libstdcxx/v6/__init__.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/python/libstdcxx/v6/printers.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/syscalls/*.xml
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/syscalls/gdb-syscalls.dtd
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/system-gdbinit/elinos.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/gdb/system-gdbinit/wrs-linux.py
#cuda-gdb share
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/FrameDecorator.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/FrameIterator.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/frames.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/__init__.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/printing.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/prompt.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/types.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/unwinder.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/xmethod.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/command/explore.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/command/frame_filters.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/command/__init__.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/command/pretty_printers.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/command/prompt.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/command/tsx.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/command/type_printers.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/command/unwinders.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/command/xmethods.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/function/as_string.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/function/caller_is.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/function/__init__.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/function/strfns.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/printer/bound_registers.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/python/gdb/printer/__init__.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/syscalls/*.xml
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/syscalls/gdb-syscalls.dtd
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/system-gdbinit/elinos.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/cuda-gdb/system-gdbinit/wrs-linux.py
#roc-gdb share
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb/FrameDecorator.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb/FrameIterator.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb/frames.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb/__init__.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb/printing.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb/prompt.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb/types.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb/unwinder.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb/xmethod.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb/command/explore.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb/command/frame_filters.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb/command/__init__.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb/command/pretty_printers.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb/command/prompt.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb/command/tsx.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb/command/type_printers.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb/command/unwinders.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb/command/xmethods.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb/function/as_string.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb/function/caller_is.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb/function/__init__.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb/function/strfns.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb/printer/bound_registers.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/python/gdb/printer/__init__.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/syscalls/*.xml
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/syscalls/gdb-syscalls.dtd
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/system-gdbinit/elinos.py
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/roc-gdb/system-gdbinit/wrs-linux.py
#end share
%attr(644, root, root) %{prefix}/%{cray_product}/%{pkgversion}/share/man/man1/gdb4hpc.1
%attr(755, root, root) %{prefix}/%{cray_product}/%{pkgversion}/%{set_default_command}_%{cray_product}_%{pkgversion}
%attr(755, root, root) %{prefix}/%{set_default_path}/%{set_default_command}_%{cray_product}_%{pkgversion}
%attr(755, root, root) %{prefix}/modulefiles/%{modulefile_name}/%{pkgversion}
%attr(644, root, root) %{prefix}/lmod/modulefiles/core/%{cray_name}/%{pkgversion}.lua
