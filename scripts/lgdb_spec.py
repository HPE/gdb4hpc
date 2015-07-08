#!/usr/bin/env python
content='''%define namespace [namespace]
%define intranamespace_name [intranamespace_name]
%define two_dig_version [package_branch_two_digit]
%define two_dig_nodot_version [package_branch_two_digit_nodot]

%define man_source /cray/css/compiler/comp_rel/pubs/manpages/lgdb/%{two_dig_version}/xt_lgdb_%{two_dig_nodot_version}.cpio

Summary: Performance Application Programming Interface
Name: %{namespace}-%{intranamespace_name} 
Version: [version] 
Release: [rpm_release]_[package_revision][sles_sub]
License: BSD
Group: Development/System
URL: [url]
Source: [tarball]
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root
Prefix: /opt/cray/pe

#BuildRequires: gcc-fortran

%define __find_requires [script_dir]/find-requires 

%description
CCDB is a comparative debugger.

# version
%define major_version %(echo %{version} | awk -v n=1 'BEGIN { FS = "." } ; { print $n }')
%define minor_version %(echo %{version} | awk -v n=2 'BEGIN { FS = "." } ; { print $n }')

# _prefix
%define _namespace_prefix %{prefix}
%define _name_prefix %{_namespace_prefix}/%{intranamespace_name}
%define _version_prefix %{_name_prefix}/%{version}

# _moduledir
%define _namespace_moduledir %{prefix}/modulefiles
%define _name_moduledir %{_namespace_moduledir}/%{namespace}-%{intranamespace_name}

%prep
%setup

%build
#./ccdb_build_tool.sh --all
#
#pushd ccdb_install
#  mkdir man
#  pushd man
#    cpio -idm < %{man_source}
#    find . -type d -exec chmod 755 {} \;
#    find . -type f -exec chmod 644 {} \;
#  popd
#popd

%install
mkdir -p %{buildroot}%{_version_prefix}
cp -r cray-lgdb/* %{buildroot}%{_version_prefix}/
rm -rf %{buildroot}%{_version_prefix}/modulefiles

install -D modulefile/%{version} %{buildroot}%{_name_moduledir}/%{version}
install -D cray-lgdb/release_info %{buildroot}%{_version_prefix}/
echo "%{version}-%{release}" > %{buildroot}%{_version_prefix}/.%{namespace}_rpm_release
mkdir -p %{buildroot}%{_namespace_prefix}/admin-pe/set_default_files/
install -D set_default_%{namespace}-%{intranamespace_name}_%{version} %{buildroot}%{_namespace_prefix}/admin-pe/set_default_files/set_default_%{namespace}-%{intranamespace_name}_%{version}
install -D set_default_%{namespace}-%{intranamespace_name}_%{version} %{buildroot}%{_version_prefix}/
mv cray-lgdb/docs/ATTRIBUTIONS_lgdb.txt %{buildroot}%{_version_prefix}/ATTRIBUTIONS_lgdb.%{version}.txt

# hardlink duplicate files to save space
%fdupes %{buildroot}

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root,-)
%dir %{_name_moduledir}
%dir %{_namespace_prefix}
%dir %{_name_prefix}
%dir %{_version_prefix}
%dir %{_version_prefix}/bin
%{_version_prefix}/bin/%{intranamespace_name}
%dir %{_version_prefix}/libexec
%{_version_prefix}/libexec/dbgsrv
%{_version_prefix}/libexec/dbgsrv_alps
#%{_version_prefix}/libexec/dlaunch 
%{_version_prefix}/libexec/gdb-guard
%{_version_prefix}/libexec/mrnet_commnode 
%{_version_prefix}/libexec/guard_proxy
%{_version_prefix}/libexec/start_proxy.sh
%{_version_prefix}/libexec/cuda65-gdb-guard
%{_version_prefix}/libexec/cuda55-gdb-guard
%{_version_prefix}/libexec/cuda70-gdb-guard
%{_version_prefix}/libexec/cti_overwatch
%{_version_prefix}/libexec/cti_slurm_util
%{_version_prefix}/libexec/cti_starter
%{_version_prefix}/libexec/cti_dlaunch7.0


%{_version_prefix}/demos
%{_version_prefix}/lib
%{_version_prefix}/man

%{_namespace_prefix}/admin-pe/set_default_files/set_default_%{namespace}-%{intranamespace_name}_%{version}
%{_name_moduledir}/%{version}
%{_version_prefix}/.cray_rpm_release
%{_version_prefix}/set_default_%{namespace}-%{intranamespace_name}_%{version}
%{_version_prefix}/release_info
%{_version_prefix}/ATTRIBUTIONS_lgdb.%{version}.txt
#%{_version_prefix}/docs/GPLv3.txt
#%{_version_prefix}/docs/LGPLv21.txt
#%{_version_prefix}/docs/LICENSE
#%{_version_prefix}/docs/epl-v10.txt
%{_version_prefix}/docs/ATTRIBUTIONS_lgdb.txt


%post

#Set the install path in the modulefile
sed -i "s,\[BASE_INSTALL_DIR\],$RPM_INSTALL_PREFIX,g" $RPM_INSTALL_PREFIX/modulefiles/%{namespace}-%{intranamespace_name}/%{version} 

sed -i \
-e"s,\[install_dir\],$RPM_INSTALL_PREFIX,g" \
-e"s,\[module_dir\],$RPM_INSTALL_PREFIX/modulefiles,g" \
$RPM_INSTALL_PREFIX/admin-pe/set_default_files/set_default_%{namespace}-%{intranamespace_name}_%{version} \
$RPM_INSTALL_PREFIX/%{name}_%{version}


#
# Set as default if no default exists either because this is first install of
# cray-ccdb or CRAY_INSTALL_DEFAULT=1 and previous default was deleted
#

# prevent echo of new directory as it messes up install output
if [ $RPM_INSTALL_PREFIX = "/opt/cray" ]
  then
    if [ ${CRAY_INSTALL_DEFAULT:-0} -eq 1 ] || [ ! -f $RPM_INSTALL_PREFIX/modulefiles/cray-lgdb/.version ]
      then
        $RPM_INSTALL_PREFIX/admin-pe/set_default_files/set_default_%{namespace}-%{intranamespace_name}_%{version}
     else
      echo "%{namespace}-%{intranamespace_name}_%{version} has been installed as non-default."
    fi
fi

%preun
# Cleanup default link if it point to this version
default_link="%{_name_prefix}/default"
if [ -f ${default_link} -a "$(stat -c "%%d:%%i" ${default_link})" == "$(stat -c "%%d:%%i" %{_version_prefix})" ]
then
  rm -f ${default_link}
fi

cd %{_name_moduledir}
version="%{version}"

# Cleanup module .version if it points to this version
pushd %{_name_moduledir} 1> /dev/null
if [ $RPM_INSTALL_PREFIX = "/opt/cray" ]
  then
	if [ -f .version ]
	then
	  dotversion=$(grep ModulesVersion .version | awk 'BEGIN { FS="\\"" } {print $2 }')
	  
	  if [ "${dotversion}" = "${version}" ]
	  then
	    echo "Uninstalled version and .version file match = ${version}."
	    echo "Removing %{intranamespace_name} .version file."
	    rm .version
	  fi
	fi
fi
# pop out of module directory
# prevent echo of new directory as it messes up install output
popd 1> /dev/null

%changelog

'''
