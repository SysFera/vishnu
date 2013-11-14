#!/bin/bash
# This script allows to add packages to Debian repositories defines in the current directory
# Author : Rodrigue Chakode <Rodrigue.Chakode@sysfera.com>
# Creation : 12/11/2013

# REQUIREMENTS
#
# ** Your Debian packages MUST be available a the given directory on your local machine (deb, dsc, src, diff)
# ** The directory must contains a sub directory for any considered distribution. Currently: wheezy, squeeze
# ** You MUST install reprepro tool
# ** You MUST run the script from the current directory
# ** The script assume that 

PACKAGE_BIN_DIR=$1

if [ $# -ne 1 ]; then
  echo "basename $0 <package_bin_dir>"
  exit 1
fi

DISTS="squeeze wheezy"
for dist in $DISTS; do
  reprepro -Vb . includedeb $dist $PACKAGE_BIN_DIR/$dist/*.deb
  reprepro -Vb . includedsc $dist $PACKAGE_BIN_DIR/$dist/*.dsc
done
