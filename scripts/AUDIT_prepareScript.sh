#!/bin/sh
#
# Script to prepare the release for VISHNU source code for the AUDIT
# Author B. Isnard
# Date 17/05/11
#
# NOTES;
# - does not contain the EMF4CPP library sources
# - does not contain the generated EMF4CPP files
# - does not contain the TORQUE C files
# - does not apply the license file (to avoid changing line nbs)

if [ ! $# -eq 1 ]; then
  echo "Usage: " $0 "<no_version: 1.0.x>"
  exit 1
else
  NO_VERSION=$1
  echo "Preparing VISHNU v" $NO_VERSION "..."
fi

# Create temporary directory
rm -rf /tmp/prepaAudit
mkdir /tmp/prepaAudit
path=/tmp/prepaAudit/VISHNU_$NO_VERSION
mkdir $path

# Copy core sources
mkdir $path/core
mkdir $path/core/src/
mkdir $path/core/src/config
mkdir $path/core/src/database
mkdir $path/core/src/exception
mkdir $path/core/src/registry
mkdir $path/core/src/utils
mkdir $path/core/test
mkdir $path/core/test/cfgs
mkdir $path/core/test/src

cp core/src/config/*pp $path/core/src/config/
cp core/src/database/*pp $path/core/src/database/
cp core/src/exception/*pp $path/core/src/exception/
cp core/src/registry/*pp $path/core/src/registry/
cp core/src/utils/*pp $path/core/src/utils/
cp core/test/src/*pp $path/core/test/src/

###############################################################################
#                                    U M S                                    #
###############################################################################

# Copy UMS sources
mkdir $path/UMS
mkdir $path/UMS/src/

cp -r UMS/src/api $path/UMS/src
cp -r UMS/src/cli $path/UMS/src
cp -r UMS/src/client $path/UMS/src
cp -r UMS/src/sed $path/UMS/src
cp -r UMS/src/server $path/UMS/src
# cp -r UMS/src/utils $path/UMS/src

###############################################################################
#                                    T M S                                    #
###############################################################################

mkdir $path/TMS
mkdir $path/TMS/src/
cp -r TMS/src/api $path/TMS/src
cp -r TMS/src/cli $path/TMS/src
cp -r TMS/src/client $path/TMS/src
cp -r TMS/src/sed $path/TMS/src
cp -r TMS/src/server $path/TMS/src
# cp -r TMS/src/utils_torque $path/TMS/src
cp -r TMS/src/slave $path/TMS/src

###############################################################################
#                                 All modules                                 #
###############################################################################

cd $path
rm -f tmp.txt

# remove all .txt files
for i in  $(find . -name "*txt") ; do rm $i;  done ;

# remove all .kdev files
for i in  $(find . -name ".kdev*") ; do rm $i;  done ;

# remove all ~ files
for i in  $(find . -name "*~") ; do rm $i;  done ;

# remove all bak files
for i in  $(find . -name "*bak") ; do rm $i;  done ;

# remove all build dir
for i in  $(find . -name "*build") ; do rm -rf $i;  done ;

cd ..

# Archive to send
tar -czvf VISHNU_AUDIT_v${NO_VERSION}.tgz VISHNU_$NO_VERSION >/dev/null

# Moving archive in /tmp
mv VISHNU_AUDIT_v${NO_VERSION}.tgz /tmp/

