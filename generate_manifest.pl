#!/usr/bin/perl
use strict;
use warnings;

use lib '/cray/css/users/carl/svn/pebit_shared/lib';
use lib '/cray/css/users/carl/svn/async_installer/trunk/lib';

use Pei::ManifestGenerate;

my ($name,$version,$release) = @ARGV;
if ( ! defined $name    || ! defined $version || ! defined $release  )
{
    print <<EOT;
generate_manifest.pl <name> <version> <release>
EOT
    exit 1;
}

my $manifest_filename = sprintf "%s-%s-%s.x86_64.rpm.yaml",$name,$version,$release;

my $mg = Pei::ManifestGenerate->new();
my ($section_id,$all_except,$commands);
$all_except = 'NO';

#----------------------------------------------------------------------------
# New Section (INSTALL)
#----------------------------------------------------------------------------
$section_id = $mg->create_section('INSTALL');
$mg->add_keyword_value($section_id,'SYSTEM_TYPE','NO','HARDWARE');
$mg->add_keyword_value($section_id,'RPM_LIST','NO','NONE');
$mg->add_keyword_value($section_id,'NETWORK_TYPE','YES','SEASTAR');
$mg->add_keyword_value($section_id,'OS_VERSION','YES','2.2,2.1');
$mg->add_keyword_value($section_id,'EMAILS','NO','claustin@cray.com');

#----------------------------------------------------------------------------
# New Section (INSTALL)
#----------------------------------------------------------------------------
$section_id = $mg->create_section('INSTALL');
$mg->add_keyword_value($section_id,'SYSTEM_TYPE','NO','WHITEBOX');
$mg->add_keyword_value($section_id,'HOSTS','YES','chpbld01,chpbld02');
$mg->add_keyword_value($section_id,'RPM_LIST','NO','NONE');
$mg->add_keyword_value($section_id,'OS_VERSION','YES','SLES10');
$mg->add_keyword_value($section_id,'EMAILS','NO','claustin@cray.com');

#----------------------------------------------------------------------------
# New Section (UPDATE)
#----------------------------------------------------------------------------
$section_id = $mg->create_section('UPDATE');
$mg->add_keyword_value($section_id,'SYSTEM_TYPE','NO','HARDWARE');
$mg->add_keyword_value($section_id,'RPM_LIST','NO','NONE');
$mg->add_keyword_value($section_id,'NETWORK_TYPE','YES','SEASTAR');
$mg->add_keyword_value($section_id,'OS_VERSION','YES','2.2,2.1');
$mg->add_keyword_value($section_id,'EMAILS','NO','claustin@cray.com');

#Add UPDATE command to section
$commands = <<"__END_OF_COMMANDS__";
/opt/cray/admin-pe/set_default_files/set_default_cray-lgdb_$version
__END_OF_COMMANDS__

$mg->add_commands_to_section($section_id,$commands);


#----------------------------------------------------------------------------
# New Section (UPDATE)
#----------------------------------------------------------------------------
$section_id = $mg->create_section('UPDATE');
$mg->add_keyword_value($section_id,'SYSTEM_TYPE','NO','WHITEBOX');
$mg->add_keyword_value($section_id,'HOSTS','YES','chpbld01,chpbld02');
$mg->add_keyword_value($section_id,'RPM_LIST','NO','NONE');
$mg->add_keyword_value($section_id,'OS_VERSION','YES','SLES10');
$mg->add_keyword_value($section_id,'EMAILS','NO','claustin@cray.com');

#Add UPDATE command to section
$commands = <<"__END_OF_COMMANDS__";
/opt/cray/admin-pe/set_default_files/set_default_cray-lgdb_$version
__END_OF_COMMANDS__

$mg->add_commands_to_section($section_id,$commands);

#----------------------------------------------------------------------------
# New Section (REMOVE)
#----------------------------------------------------------------------------
$section_id = $mg->create_section('REMOVE');
$mg->add_keyword_value($section_id,'SYSTEM_TYPE','NO','ALL');
$mg->add_keyword_value($section_id,'EFFECTIVE_DATE','NO','2017-09-19');

# Add REMOVE commands to section
$commands = <<"__END_OF_COMMANDS__";
rpm -e cray-lgdb-$version-$release
__END_OF_COMMANDS__

# Specify COMMANDS for this section
$mg->add_commands_to_section($section_id,$commands);

$mg->add_keyword_value($section_id,'EMAILS','NO','claustin@cray.com');

# INSTALL COMMANDS
$commands = <<"__END_OF_COMMANDS__";
DEFAULT=1
RPM_RELEASE=0

#Remove previous versions of this rpm
#causes multiple removals and installs for releases of the same version
#for package in  \$(rpm -qa | egrep "cray-lgdb-$version-")
#do
#  rpm -e \$package
#done

if [ \$DEFAULT -ne 0 ] ; then
  export CRAY_INSTALL_DEFAULT=1
fi

rpm -hiv --oldpackage cray-lgdb-$version-$release.x86_64.rpm

if [ \$DEFAULT -ne 0 ] ; then
  unset CRAY_INSTALL_DEFAULT
fi

#ln -sf /opt/cray/modulefiles/cray-lgdb/$version /opt/cray/modulefiles/cray-lgdb/\$(echo $version | cut -d "." -f1-3).newest
#lets can the newest link business

NEWLINK=/opt/cray/modulefiles/cray-lgdb/\$(echo $version | cut -d "." -f1-3).newest

if [ -f \$NEWLINK ]
 then
  rm \$NEWLINK
fi


find  -L /opt/cray/modulefiles/cray-lgdb -maxdepth 1 -type l -delete
__END_OF_COMMANDS__

$mg->add_commands($commands);

print $mg->generate();

