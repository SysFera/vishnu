#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: " $0 " <version_release> <new_version_dev>"
    exit 1
fi
SCRIPT_PATH="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $SCRIPT_PATH/..
POM_VERSION=$2-SNAPSHOT

change_version(){
# Update CMakeLists.txt vishnu_version
    sed -e s/set\(VISHNU_VERSION\ \"[0-9]\.[0-9]\.[0-9]\"/set\(VISHNU_VERSION\ \"$1\"/ CMakeLists.txt > tmp_cmakelists
    mv tmp_cmakelists CMakeLists.txt
# Update pom.xml version
    mvn versions:set -DnewVersion=$2 -f swigAPI/VishnuLib/pom.xml
    git add swigAPI/VishnuLib/pom.xml
    git add CMakeLists.txt
    git commit -m"Setting vishnu version to $1"
}

change_version $1 $1

git tag -s RELEASE_V_$1 -m "version $1"

change_version $2 $POM_VERSION


