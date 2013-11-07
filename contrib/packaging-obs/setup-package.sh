#!/bin/bash
# This script allows to setup OBS packages for a given version of VISHNU
# Author : Rodrigue Chakode <Rodrigue.Chakode@sysfera.com>
# Creation : 07/11/2013

# REQUIREMENTS
#
# ** The VISHNU tarball MUST be available at http://www.sysfera.com/download/vishnu/vishnu-<version>.tar.gz
# ** The tarball MUST contain all the source files
# ** To run the script you MUST have osc (OBS CLI tools) installed


VERSION=$1
DEFAULT_VERSION=X.Y.Z
OBS_PROJECT="home:techsysfera"
#OBS_PROJECT_DIR=/opt/software/$OBS_PROJECT
PACKAGE_NAME=vishnu-$VERSION
DEFAULT_PACKAGE=vishnu-$DEFAULT_VERSION
DOWNLOAD_URL="http://www.sysfera.com/download/vishnu/$PACKAGE_NAME.tar.gz"
WORKING_DIR=/tmp/$PACKAGE_NAME

function usage
{
 echo "`basename $0` <version>"
}

if [ -z $VERSION ]; then
  echo "no version set"
  exit 1
fi

# Create the package directory from the default directory
cp -r $DEFAULT_PACKAGE $WORKING_DIR

cd $WORKING_DIR
FILES_TO_ALTER=$(grep -r "$DEFAULT_VERSION" . | awk -F : '{print $1}')

for file in $FILES_TO_ALTER; do
  sed --in-place "s/$DEFAULT_VERSION/$VERSION/g" $file
done

# Download vishnu
wget $DOWNLOAD_URL

if [ $? -ne 0 ]; then
  echo "Can not download the tarball at $DOWNLOAD_URL"
#  exit 1
fi

# Create OBS package
if [ ! -z $OBS_PROJECT_DIR && -d $OBS_PROJECT_DIR ]; then
  cd $OBS_PROJECT_DIR
  osc up
else
  cd /tmp/
  osc co $OBS_PROJECT
  cd $OBS_PROJECT
fi

osc mkpac $PACKAGE_NAME
cp -r $WORKING_DIR/* $PACKAGE_NAME
osc add $PACKAGE_NAME/*
osc commit -m "add package for $PACKAGE_NAME"

