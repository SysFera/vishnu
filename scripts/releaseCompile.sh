#!/bin/bash
#
# Script to compile Vishnu from release tarball
# Author B. Depardon (benjamin.depardon@sysfera.com)
# Date 29/01/13
#
# 29/01/13 : Compile all modules, consider that all
#            dependencies are available for cmake
#

if [ $# -lt 0 ]; then
  echo "Usage: $0"
  exit 1
fi


if [ ! -f copyright ]; then
  echo "Missing copyright file - please start the script in VISHNU root directory"
  exit 1
fi

# Get common functions and variables
source scripts/common.sh

VISHNUTAR=deliverables/release/VISHNU_v$NO_VERSION/vishnu_v${NO_VERSION}.tgz

if [ ! -f ${VISHNUTAR} ]; then
  echo "Missing release file:  ${VISHNUTAR}"
  echo "Please run ALL_prepareScript.sh first!"
  exit 1
fi

echo "## Trying to compile VISHNU v${NO_VERSION} extracted from ${VISHNUTAR}"



######################################################################
#                            COMPILATION                             #
######################################################################
tmpdir=`mktemp -d /tmp/vishnu_XXXXXX`
run_cmd cp  ${VISHNUTAR} $tmpdir

change_dir $tmpdir

run_cmd tar xvf vishnu_v${NO_VERSION}.tgz >/dev/null 2>&1
change_dir VISHNU_v${NO_VERSION}

run_cmd mkdir build
change_dir build

run_cmd cmake -DCOMPILE_UMS=ON \
-DCOMPILE_FMS=ON \
-DCOMPILE_IMS=ON \
-DCOMPILE_TMS=ON \
-DCOMPILE_CLIENT_CLI=ON \
-DCOMPILE_SERVERS=ON \
-DVISHNU_BATCH=POSIX \
-DENABLE_MYSQL=ON \
-DBUILD_TESTING=ON \
-DENABLE_JAVA=ON \
-DENABLE_PYTHON=ON \
-DENABLE_SWIG=OFF \
-DENABLE_REPORTS=ON .. 
run_cmd make -j 4

# cleanup
change_dir ~
remove_files $tmpdir

# All done
print_exit
