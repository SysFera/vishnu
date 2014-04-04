#!/bin/bash

if [ $# -ne 3 ]; then
    echo "Usage: " $0 " <vishnuDir> <destDir> <version>"
    exit 1
fi

# VISHNUPATH
vishnuDir=$1
# Dest
destDir=$2
# Version
version=$3

cd $destDir
mkdir -p Java
mkdir -p Java/impl
mkdir -p Java/impl/VishnuLib

cp -r $vishnuDir/swigAPI/VishnuLib/* Java/impl/VishnuLib/
cp -r $vishnuDir/swigAPI/generated/swig_output/*.java Java/impl/VishnuLib/src/main/java/com/sysfera/vishnu/api/vishnu/internal/

cd Java/impl/VishnuLib
mvn clean
mvn install

cp target/VishnuLib-$version.jar $destDir/

cd $destDir
rm -rf Java


