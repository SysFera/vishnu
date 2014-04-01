#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: " $0 " <version>"
    exit 1
fi


git tag -s RELEASE_V_$1 -m "version $1"
