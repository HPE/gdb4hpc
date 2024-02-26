#!/bin/bash
#
# manage_release_notes.sh
#
# Copyright 2022-2024 Hewlett Packard Enterprise Development LP.
#
# Unpublished Proprietary Information.
# This unpublished work is protected to trade secret, copyright and other laws.
# Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
# no part of this work or its content may be used, reproduced or disclosed
# in any form.
#

changelog_top_level=$PWD/external/changelog
product_top_level=$PWD
release_count_upper_bound=0
release_notes_source_dir=$product_top_level/rpmbuild/SOURCES
release_version=''
debug_mode=False

# print to stdout 
function print_debug(){
  str_debug=$1
  if [[ $debug_mode == True ]]; then
    echo "Debug: $str_debug"
  fi
}

# Generate the changelog from the github history.
# This required install of git-cliff, rust, and cargo
function generate_changelog_from_github_history(){

  # Handle tags
  if [[ -z $BRANCH_NAME ]]; then
    echo "git tag -l | xargs git tag -d"
    git tag -l | xargs git tag -d

    echo "git fetch --all --tags"
    git fetch --all --tags
  else
    echo "git tag --list"
    git tag --list
  fi

  # Turn error checking and verbose on for cargo/rust setup
  set -ex
  if [ ! -d $HOME/.cargo ]; then

    echo "############################################"
    echo "#        Installing changlog deps          #"
    echo "############################################"
    export RUSTUP_UPDATE_ROOT=https://arti.hpc.amslabs.hpecorp.net/artifactory/rust-lang.org-remote/rustup/
    export RUSTUP_DIST_SERVER=https://arti.hpc.amslabs.hpecorp.net/artifactory/rust-lang.org-remote/
    rust_version=1.74.1

    target_arch=$(get_arch)
    if [[ "$target_arch" == "$cdst_arch_x86_64" ]]; then
        arch="x86_64"
    elif [[ "$target_arch" == "$cdst_arch_aarch64" ]]; then
        arch="aarch64"
    else
        echo "Unknown arch: $target_arch"
        return 1
    fi

    (cd /tmp &&
     wget --no-verbose https://arti.hpc.amslabs.hpecorp.net/artifactory/rust-lang.org-remote/dist/rust-${rust_version}-${arch}-unknown-linux-gnu.tar.gz
     tar -xzf rust-${rust_version}-${arch}-unknown-linux-gnu.tar.gz
     sudo ./rust-${rust_version}-${arch}-unknown-linux-gnu/install.sh
    )
    mkdir $HOME/.cargo
    cat > $HOME/.cargo/config.toml << END
# Makes artifactory the default registry and saves passing --registry parameter
[registry]

default = "artifactory"

[registries.artifactory]
index = "sparse+https://arti.hpc.amslabs.hpecorp.net/artifactory/api/cargo/crates.io-remote/index/"


# For sending credentials in git requests.
# Not required if anonymous access is enabled
[net]
git-fetch-with-cli = true


# Add these 2 sections for resolving dependencies from Artifactory
[source.artifactory]
registry = "sparse+https://arti.hpc.amslabs.hpecorp.net/artifactory/api/cargo/crates.io-remote/index/"

[source.crates-io]
replace-with = "artifactory"
END

  else
    # Should be an error instead of just continuing with an unknown cargo install?
    echo "$HOME/.cargo already exists"
    ls -al $HOME/.cargo
  fi

  PATH=$HOME/.cargo/bin:${PATH}
  if ! cargo install --list | grep git-cliff; then
    echo "############################################"
    echo "#            Installing git-cliff          #"
    echo "############################################"
    cargo install --version 2.0.0 git-cliff
    check_exit_status
  fi

  if [ -f $product_top_level/CHANGELOG.md ]; then
    rm -rf $product_top_level/CHANGELOG.md
  fi

  # Generate a new CHANGELOG.md
  echo "############################################"
  echo "#        Generating a new Changelog        #"
  echo "############################################"
  git cliff -c $changelog_top_level/cliff.toml 2>&1 | tee $product_top_level/CHANGELOG.md
  check_exit_status

  # Turn error checking and verbose back off, code outside this function assumes this
  set +ex

  if [[ $debug_mode == True ]]; then
    echo
    echo
    echo "************************************"
    echo "************ DEBUG MODE ************"
    echo "************************************"
    echo
    echo
    echo "************************************"
    echo "************ GIT TAG ***************"
    git tag > $PWD/tag.log
    cat $PWD/tag.log
    echo "************************************"
    echo
    echo
    echo "********************************************************************"
    echo "********** CHANGELOG CONTENT TO INSERT FROM CHANGELOG.MD ***********"
    cat $product_top_level/CHANGELOG.md
    echo "********************************************************************"
  fi

}

# Update the release_notes.md file with parsed content from the changelog
function update_release_notes(){

  # Get branch - Determine the number of releases to include in the release notes
  # Default is 3, but user can override.
  branch_name=$(git rev-parse --abbrev-ref HEAD)
  branch_type=$(echo $branch_name | cut -d'/' -f1)
  if [[ $user_release_upper_bound -gt 0 ]]; then
    if [[ $branch_type == 'release' ]]; then 
      release_count_upper_bound=$((user_release_upper_bound*2 + 1))
    else
      release_count_upper_bound=$((user_release_upper_bound*2 + 3))
    fi
  else
    # Default release count is 3
    if [[ $branch_type == 'release' ]]; then 
      release_count_upper_bound=7
    else
      release_count_upper_bound=9
    fi
  fi

  
  # Get the date of the tag - this info will be used to calculate the release version 
  # which will be inserted into the release notes for each release (e.g. (24.03))
  IFS=$'\n'
  tag_dates=$(git log --tags --simplify-by-decoration --pretty="format:%ci %d" | head -$release_count_upper_bound)
  tag_list=()
  # Parse tag and date from each line in tag_dates
  for line in $tag_dates
  do
    print_debug "Looking at: $line"
    tag_date=$(echo $line | cut -d' ' -f1)
    tag_name=$(echo $line | cut -d' ' -f6)
    print_debug "$tag_date  -  $tag_name"
    if [[ $tag_name == *',' ]]; then
      tag_name=$(echo $tag_name | cut -d ',' -f1 | cut -d'v' -f2)
    elif [[ $tag_name == *')' ]]; then
      tag_name=$(echo $tag_name | cut -d ')' -f1 | cut -d'v' -f2)
    fi
    print_debug "Adding to tag list: $tag_date - $tag_name"
    tag_list+=($tag_date"-"$tag_name)
  done


  # Parse CHANGELOG.md, grab the releases and insert into product release notes
  changelog_file=$product_top_level/CHANGELOG.md
  changelog=$(cat $changelog_file | grep -n "\#\# \[")
  changelog_start_line=1
  changelog_end_line=$(cat CHANGELOG.md | wc -l)

  if [[ $debug_mode == True ]]; then
    echo "**************************************"
    echo "********* CHANGELOG RELEASES *********"
    echo "$changelog"
    echo "**************************************"
    echo "******** RELEASE LINE NUMBER: $changelog_end_line"
    echo
    echo
    echo "*******************************************"
    echo "CHANGELOG START LINE: $changelog_start_line"
    echo "CHANGELOG END LINE: $changelog_end_line"
    echo "*******************************************"
  fi

  # Get start and endline to insert/update release notes
  if [ -f $release_notes_source_dir/release_notes.md ]; then
    rm -f $release_notes_source_dir/release_notes.md
  fi
  cp $release_notes_source_dir/release_notes.md.in $release_notes_source_dir/release_notes.md
  chmod a+w $release_notes_source_dir/release_notes.md
  release_info_file="$release_notes_source_dir/release_notes.md"

  # Get the line number of [AUTO_RELEASE_HISTORY] pattern in the template
  release_info_auto_release_history=$(cat $release_info_file | grep -n "AUTO_RELEASE_HISTORY" | cut -d":" -f1)
  if [ ! -z $release_info_auto_release_history ]; then
    sed -i -e "${release_info_auto_release_history}d" $release_info_file
  fi

  if [[ $changelog_end_line -le 1 ]]; then
    echo "There were no commits since the last release.."
    
  else

    # grap the content from changelog.md in preperation for inserting into the release_notes.in
    changelog_content=$(sed -n "${changelog_start_line},${changelog_end_line}p" $changelog_file)

    if [[ $debug_mode == True ]]; then
      if [ -f $release_notes_source_dir/release_notes.md ]; then
        echo
        echo "*******************************************"
        echo "************ RELEASE INFO FILE ************"
        cat $release_notes_source_dir/release_notes.md
        echo "*******************************************"

        echo
        echo "************************************************************************"
        echo "************ CHANGELOG CONTENT TO BE INSERTED FROM VARIABLE ************"
        echo "************************************************************************"
        echo $changelog_content
      fi
    fi
    
    count=${release_info_auto_release_history}
    divCount=0
    release_count=0
    lp_count=0
    # Loop through each line of changelog content, parse, and insert into release_notes.md
    for LINE in ${changelog_content[@]}; do

      # Commit type header is prepended with ### and type is either features or bug fixes
      commit_type_header=$(echo $LINE | grep -n "\#\#\# ")
      # release divider envelopes the release version - date
      release_divider=$(echo $LINE | grep "=========")

      # Formatting - blank line
      if [[ ! -z $release_divider ]]; then
        # Found a release_divider
        release_count=$((release_count+1))
        if [[ $release_count -eq $release_count_upper_bound ]]; then
          break
        fi
        if [[ $divCount -eq 2 ]]; then
          # Found the second release divider - after the release version - date
          count=$((count+1))
          sed -i -e "${count}i\ " $release_info_file
          divCount=1
        else
          # Did not find a release_divider
          divCount=$((divCount+1))
        fi
      fi

      # Formatting - insert blank line above header
      if [[ ! -z $commit_type_header ]]; then
        # Found commit type header (bug fixes or features)
        count=$((count+1))
        # Insert blank line after commit type header
        sed -i -e "${count}i\ " $release_info_file

        # Insert Type header on next line(e.g.- Bug Fixes/Features)
        count=$((count+1))
        sed -i -e "${count}i${LINE}" $release_info_file

        # Formatting - insert blank line after header
        count=$((count+1))
        sed -i -e "${count}i\ " $release_info_file
      else
        # Insert the actual commit message
        count=$((count+1))
        if [[ $LINE == "* "* ]]; then
          # Found a commit
          pre_message=$(echo $LINE  | cut -d'[' -f1)
          message=$(echo $pre_message | cut -d'(' -f1)
        else
          # Did not find a commit
          if [[ -z $release_divider ]] ; then
            # Did not find a release divider - Insert New Release Header - Date
            if [[ "$LINE" == "## [unreleased]" ]]; then
              message="$LINE"
            else
              # Found a release header
              # Loop through tag_list array for matching tag
              print_debug "Looking at changelog LINE: $LINE"
              for date_tag in ${tag_list[@]}; do
                print_debug "Looking at date_tag: $date_tag"
                rel_tag=$(echo $date_tag | cut -d'-' -f4)
                rel_date=$(echo $date_tag | cut -d'-' -f1-3)
                print_debug "Looking at rel_tag: $rel_tag"
                print_debug "Looking at rel_date: $rel_date"
                if [[ "$LINE" == *"$rel_tag"* ]]; then
                  print_debug "Changelog line contains tag date"
                  pe_release_yr=${rel_date:2:2}
                  pe_release_mo=${rel_date:5:2}
                  pe_release_mo=$(echo $pe_release_mo | sed 's/^0*//')
                  pe_release_mo=$((pe_release_mo+1)) 
                  message="$LINE ($pe_release_yr.$(printf %02d $pe_release_mo))"
                  print_debug "$message"

                  # Get 4 digit year, and 2 digit month 
                  if [[ $lp_count == 0 ]]; then
                    pretty_release_date=$(date -d "$release_date 1 month" +"%B %Y")
                    # Update the release notes with the current release date
                    print_debug "Pretty release date: $pretty_release_date"
                    sed -i -e "s|<RELEASE_DATE>|$pretty_release_date|" $release_info_file
                    lp_count=$((lp_count+1))
                  fi
                  break
                fi
              done

            fi
          else
            message="$LINE"
          fi
        fi
        sed -i -e "${count}i${message}" $release_info_file
      fi 
    done

    # Insert blank lines after as a buffer
    for i in {1..2} ;do
      count=$((count+1))
      sed -i -e "${count}i\ " $release_info_file
    done

    if [[ $debug_mode == True ]]; then
      echo
      echo "****************************************************"
      echo "************ RELEASE NOTES AFTER UPDATE ************"
      echo "****************************************************"
      cat $release_info_file
    fi
  fi

}

OPTIND=1
function showOptions (){
    echo "Usage: $0 [ -c ] [ -r ] [ -s source_dir (optional) ] [ -n release_count (optional) ] [ -d (optional) ] [-h help ]" 1>&2
    echo "    -c: update changelog"
    echo "    -r: update release notes"
    echo "    -s: release_notes source directory"
    echo "    -n: number of releases to include - release count"
    echo "    -d: debug mode - print stages to stdout"
    echo "    -h: help"
    exit 1
}

while getopts "crdhs:n:" opt
do
  case $opt in
    c ) changelog=True;;
    r ) release_notes=True;;
    s ) release_notes_source_dir=$OPTARG;;
    n ) user_release_upper_bound=$OPTARG;;
    d ) debug_mode=True;;
    h ) showOptions;;
    ? ) echo "Unknown option $OPTARG"
        showOptions;;
    : ) echo "No known argument value for option $OPTARG"
        exit 1 ;;
    * ) echo "Unknown error occurred while processing options"
        exit 1 ;;
  esac
done
shift $((OPTIND-1))

if [[ $changelog == True ]]; then
  generate_changelog_from_github_history
fi
if [[ $release_notes == True ]]; then
  update_release_notes 
fi
