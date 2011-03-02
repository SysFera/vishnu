#! /bin/bash
# Bash script to append the copyright to the beginning of all files
# Author : Ibrahima Cisse (Ibrahima.cisse@sysfera.com)
# Date   : 03/03/2011
# Usage    : ./setCopyright.sh copyright

#

for file in $(find . -name "*.?pp"); do cat $1 $file > tmp.txt && cp tmp.txt $file; done;


