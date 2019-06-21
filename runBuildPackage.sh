#!/bin/bash


scripts_dir=$PWD/scripts
cd $scripts_dir
python $PWD/package_gdb4hpc
return_code=$?

exit $return_code
