#!/bin/sh
#
# Script for generating man pages of the IMS module using userman-gen.docbook
# Author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
# Date 17/03/11
#

# Create temporary directory

rm -r man1
rm -r man3

# Create man directory  
mkdir man1
mkdir man3

xsltproc $PWD/docbook/manIMS-gen.docbook 

# move of files
mv *.1 man1
mv *.3 man3

#Message: don't forget to commit
echo "Don't forget to commit changes -- git commit and git push" 
