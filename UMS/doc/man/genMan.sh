#!/bin/sh
#
# Script for generating man pages of the UMS module using userman-gen.docbook et adminman-gen.docbook
# Author Eugène PAMBA CAPO-CHICHI (kevin.coulomb@sysfera.com)
# Date 17/03/11
#

# Create temporary directory

rm -r man1
rm -r man3

# Create man directory  
mkdir man1
mkdir man3

xsltproc $PWD/docbook/adminman-gen.docbook 
xsltproc $PWD/docbook/userman-gen.docbook 

# move of files
mv *.1 man1
mv *.3 man3

#Message: don't forget to commit
echo "Don't forget to commit changes -- git commit and git push" 