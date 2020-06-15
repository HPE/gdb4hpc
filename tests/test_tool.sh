#!/bin/bash

SCRIPT=$(basename "$0")
BUILD=true
TEST_ALL=true
DIR=$(cd `dirname $0` && pwd)

run_tests() {
    # default action - run all tests

    if ! prereq_checks ; then
        echo "Failed prerequisite checks."
        exit 1
    fi

    if [ $BUILD = true ]; then
        if ! build ; then
            echo "Failed to build test program binaries."
            exit 1
        fi
    fi

    cd "$DIR/function"
    ./build_run.sh $@
    cd "$DIR"
}

prereq_checks() {
    # do we have gdb4hpc installed?
    if ! gdb4hpc -v > /dev/null ; then
        echo "gdb4hpc not found."
        return 1
    fi

    # are we in a writable directory?
    if ! [ -w `pwd` ]; then
        echo "Current directory is not writable."
        echo "Run ./$SCRIPT --cp <directory> to install the tests in a writable directory."
        return 1
    fi

    return 0
}

build() {
    local source_files_glob="$DIR/function/bd_dt/c/*.c \
                             $DIR/function/bd_dt/C++/*.cpp \
                            $DIR/function/bd_dt/fortran/*.f90"
    
    # touch source files to force rebuild
    # stat to make sure they're there; otherwise touch *.c would create a file
    # called '*.c'.
    if stat $source_files_glob --printf='' 2>/dev/null ; then
        touch $source_files_glob
    else
        echo "Failed to stat all test program source files. Are some missing?"
        return 1
    fi

    cd "$DIR"
    if ! (autoreconf -ifv && ./configure) ; then
        echo "Failed while running autotools."
        return 1
    fi
    
    cd "$DIR"
    if ! make check ; then
        echo "Failed to compile test program binaries."
        return 1
    fi

    return 0
}

copy_to_usage() { 
    echo "Usage: ./$SCRIPT [-c|--cp] <destination>" 
}
copy_to() {
    if [ "$#" -lt 1 ]; then
        copy_to_usage
        return 1
    fi

    local dest="$1/gdb4hpc-tests"

    echo "Copying tests to $dest."
    if [ -d $dest ]; then
        echo "Couldn't copy: $dest already exists."
        return 1
    fi

    if ! cp -r "$DIR" $dest; then
        echo "Failed to copy."
        return 1
    fi

    echo "Now cd $dest && ./test_tool.sh"

    return 0
}

process_arguments() {
    if [ "$#" -ne 0 ]; then
        # process arguments
        while [ "$#" -gt 0 ]; do
            case $1 in
                -c|--cp) 
                    if [ "$#" -gt 1 ]; then
                        TEST_ALL=false
                        copy_to $2
                    else
                        copy_to_usage
                        return 1
                    fi
                    shift ;;
                -t|--test)
                    if [ "$#" -gt 1 ]; then
                        run_tests $2
                        TEST_ALL=false
                    else
                        echo "Usage: ./$SCRIPT [-t|--test] <test name>"
                        return 1
                    fi
                    shift ;;
                -s|--skip-build)
                    echo "Skipping the build step."
                    BUILD=false
                    ;;
                -h|--help)
                    TEST_ALL=false
                    echo "    ./$SCRIPT [options]"
                    echo "    [-t|--test] <test name> : run a single test. if this option isn't used, all tests are run."
                    echo "    [-c|--cp] <directory>   : copy tests to a directory."
                    echo "    [-s|--skip-build]       : skip the build step."
                    echo "    [-h|--help]             : show this help message."
                    ;;
                *) 
                    echo "Unknown parameter passed: $1"
                    return 1 ;;
            esac
            shift
        done
    fi

    if [ $TEST_ALL = true ]; then
        run_tests
    fi

    return 0
}

process_arguments $@
