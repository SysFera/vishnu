#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: " $0 " <version>"
    exit 1
fi

sed -e s/set\(VISHNU_VERSION\ \"[0-9]\.[0-9]\.[0-9]\"/set\(VISHNU_VERSION\ \"$1\"/ CMakeLists.txt > tmp_cmakelists
mv tmp_cmakelists CMakeLists.txt
git add CMakeLists.txt
git commit -m"Setting vishnu version to $1"

