#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: " $0 " <version>"
    exit 1
fi


mvn versions:set -DnewVersion=$1 -f swigAPI/VishnuLib/pom.xml
git add swigAPI/VishnuLib/pom.xml
git commit -m"Change VishnuLib version to $1"
