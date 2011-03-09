#!/bin/sh
#
# Script to prepare the release for the UMS module in vishnu
# Author K. Coulomb (kevin.coulomb@sysfera.com)
# Date 08/03/11
#

# Create temporary directory
rm -rf /tmp/prepaRel
mkdir /tmp/prepaRel
mkdir /tmp/prepaRel/VISHNU_UMS_1.0

path=/tmp/prepaRel/VISHNU_UMS_1.0

# Copy root cmake list, copyright, README
cp CMakeLists.txt $path/
cp copyright $path/
cp README $path/

# Copy doxygen generator
cp Doxyfile $path/
cp Doxyfile_API $path/

# Copy doxygen documentation file
cp -r doc $path/

# Copy the licence
cp -r Licence $path/

# Copy Cmake find files
cp -r Cmake $path/

# Copy sql scripts
mkdir $path/core
mkdir $path/core/database
cp -r core/database/database_init.sql $path/core/database/
cp -r core/database/postgre_create.sql $path/core/database/

# Copy core sources
mkdir $path/core/src/
mkdir $path/core/src/database
mkdir $path/core/src/exception
mkdir $path/core/src/registry
mkdir $path/core/src/utils
cp core/src/CMakeLists.txt $path/core/src/
cp core/src/database/CMakeLists.txt $path/core/src/database/
cp core/src/exception/CMakeLists.txt $path/core/src/exception/
cp core/src/registry/CMakeLists.txt $path/core/src/registry/
cp core/src/utils/CMakeLists.txt $path/core/src/utils/
cp core/src/database/*pp $path/core/src/database/
cp core/src/exception/*pp $path/core/src/exception/
cp core/src/registry/*pp $path/core/src/registry/
cp core/src/utils/*pp $path/core/src/utils/

# Copy the sendmail script
cp core/src/utils/sendmail.py $path/core/src/utils/

# Copy UMS sources
mkdir $path/UMS
mkdir $path/UMS/src/
cp UMS/src/CMakeLists.txt $path/UMS/src/
cp -r UMS/src/api $path/UMS/src
cp -r UMS/src/cli $path/UMS/src
cp -r UMS/src/client $path/UMS/src
cp -r UMS/src/sed $path/UMS/src
cp -r UMS/src/server $path/UMS/src
cp -r UMS/src/utils $path/UMS/src

cp setCopyright.sh $path/

cd $path

./setCopyright.sh copyright

rm setCopyright.sh

cd -

# Copy emf4cpp files
mkdir $path/core/deps/
cp -r core/deps/emf4cpp $path/core/deps/

cd $path

rm tmp.txt

# remove all ~ files
for i in  $(find . -name "*~") ; do rm $i;  done ;

# remove all bak files
for i in  $(find . -name "*bak") ; do rm $i;  done ;

# remove all build dir
for i in  $(find . -name "*build") ; do rm -rf $i;  done ;

cd ..

# Archive to send
tar -czvf VISHNU_UMS.tar.gz VISHNU_UMS_1.0

# Moving archive in /tmp
mv VISHNU_UMS.tar.gz /tmp/

