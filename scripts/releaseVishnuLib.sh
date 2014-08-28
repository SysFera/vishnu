#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: " $0 " <vishnuDir> <destDir>"
    exit 1
fi

# VISHNUPATH
vishnuDir=$1
# Dest
destDir=$2

cd $destDir
mkdir -p Java/impl/VishnuLib

cp -r $vishnuDir/swigAPI/VishnuLib/* Java/impl/VishnuLib/
cp -r $vishnuDir/swigAPI/generated/swig_output/*.java Java/impl/VishnuLib/src/main/java/com/sysfera/vishnu/api/vishnu/internal/

cd Java/impl/VishnuLib
mvn clean
mvn install

cp target/VishnuLib-*.jar $destDir/

cd $destDir
rm -rf Java


