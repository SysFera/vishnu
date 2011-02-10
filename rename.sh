#!/bin/sh
#
# Bash script to rename all files ending with '.$1' by '.$2'
# Author : Kevin Coulomb (kevin.coulomb@sysfera.com)
# Date   : 10/02/2011
# Use    : ./rename.sh oldExtension newExtension
#
for i in  $(find . -name "*\.$1") ; do mv $i $(echo $(echo $i | sed s/\.$1/\.$2/ ));  done ;

