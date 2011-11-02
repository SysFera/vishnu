#!/bin/bash
#
# Script to prepare the release for VISHNU source code (All modules)
# Author K. Coulomb (kevin.coulomb@sysfera.com)
# Date 18/04/11
#
# 29/08/11 : Add new directory to prepare in the git version of the depot
#            Merge with WS release
#
#

if [ ! $# -eq 2 ]; then
  echo "Usage: " $0 "<no_version: 1.0.x> <depot_eclipe_path>"
  exit 1
else
  NO_VERSION=$1
  echo "Preparing VISHNU v" $NO_VERSION "..."
fi

if [ ! -f copyright ]; then
  echo "Missing copyright file - please start the script in VISHNU root directory"
  exit 1
fi

eclipse=$2

# Create temporary directory
rm -rf /tmp/prepaRel
mkdir /tmp/prepaRel
path=/tmp/prepaRel/VISHNU_$NO_VERSION
mkdir $path

pathrel=deliverables/src

mkdir -p deliverables/src
mkdir -p deliverables/doc
mkdir -p deliverables/debs
mkdir -p deliverables/tests


rm -rf deliverables/src/*
rm -rf deliverables/doc/*
rm -rf deliverables/debs/*
rm -rf deliverables/tests/*


# Function that copy the dir in the archive and the depot copy
function copy_dir () {
    cp -r $1 $path/$2;
    cp -r $1 $pathrel/$2;
}

function copy_file () {
    cp $1 $path/$2;
    cp $1 $pathrel/$2;
}

# Function that create the dir in the archive and the depot copy
function create_dir () {
    mkdir -p $path/$1;
    mkdir -p $pathrel/$1;
}

function copy_rel_file () {
    cp $1 $pathrel/$2;
}

function copy_ar_file () {
    cp $1 $path/$2;
}

# Copy root cmake list, copyright, README, version
copy_file CMakeLists.txt
copy_file copyright
copy_file README
copy_file ChangeLog
copy_file vishnu_version.hpp.in

# Copy doxygen generator
copy_file Doxyfile
copy_file Doxyfile_API

# Copy doxygen documentation file
copy_dir doc

# Copy the licence
copy_dir Licence

# Copy Cmake find files
copy_dir Cmake

# Copy sql scripts
create_dir core
create_dir core/database

copy_file core/database/database_init.sql core/database/
copy_file core/database/database_update.sql core/database/
copy_file core/database/postgre_create.sql core/database/
copy_file core/database/postgre_update.sql core/database/
copy_file core/database/mysql_create.sql core/database/

# Copy core sources
create_dir core/doc
create_dir core/src
create_dir core/src/config
create_dir core/src/database
create_dir core/src/emfdata
create_dir core/src/exception
create_dir core/src/registry
create_dir core/src/utils
create_dir core/test
create_dir core/test/cfgs
create_dir core/test/src

copy_file core/src/CMakeLists.txt core/src/
copy_file core/test/CMakeLists.txt core/test/
copy_file core/test/src/CMakeLists.txt core/test/src/
copy_file core/test/cfgs/CMakeLists.txt core/test/cfgs/
copy_file core/src/config/\*pp core/src/config/
copy_file core/src/database/\*pp core/src/database/
copy_file core/src/exception/\*pp core/src/exception/
copy_file core/src/registry/\*pp core/src/registry/
copy_dir core/src/emfdata/\* core/src/emfdata/
copy_file core/src/utils/\*pp core/src/utils/
copy_file core/src/utils/\*i core/src/utils/
copy_file core/test/src/\*pp core/test/src/
copy_file core/test/src/\*h.in core/test/src/
copy_file core/test/cfgs/\*cfg.in core/test/cfgs/
copy_dir core/test/third-party core/test


# dependency of emf
create_dir core/deps

copy_file core/deps/CMakeLists.txt core/deps/
copy_dir core/deps/emf4cpp core/deps/


### Documentation html/pdf/docbook
mkdir $path/core/doc/adminmanual
mkdir $path/core/doc/adminmanual/docbook
mkdir $path/core/doc/usermanual
mkdir $path/core/doc/usermanual/docbook

mkdir $pathrel/../doc/adminmanual
mkdir $pathrel/../doc/adminmanual/docbook
mkdir $pathrel/../doc/usermanual
mkdir $pathrel/../doc/usermanual/docbook

cp core/doc/adminmanual/docbook/adminman-gen.docbook $path/core/doc/adminmanual/docbook/vishnu-adminman.docbook
cp core/doc/adminmanual/docbook/adminman-gen.docbook.pdf $path/core/doc/adminmanual/docbook/vishnu-adminman.docbook.pdf
cp core/doc/adminmanual/docbook/adminman-gen.html $path/core/doc/adminmanual/docbook/vishnu-adminman.html

cp core/doc/usermanual/docbook/userman-gen.docbook $path/core/doc/usermanual/docbook/vishnu-userman.docbook
cp core/doc/usermanual/docbook/userman-gen.docbook.pdf $path/core/doc/usermanual/docbook/vishnu-userman.docbook.pdf
cp core/doc/usermanual/docbook/userman-gen.html $path/core/doc/usermanual/docbook/vishnu-userman.html

cp core/doc/adminmanual/docbook/adminman-gen.docbook $pathrel/../doc/adminmanual/docbook/vishnu-adminman.docbook
cp core/doc/adminmanual/docbook/adminman-gen.docbook.pdf $pathrel/../doc/adminmanual/docbook/vishnu-adminman.docbook.pdf
cp core/doc/adminmanual/docbook/adminman-gen.html $pathrel/../doc/adminmanual/docbook/vishnu-adminman.html

cp core/doc/usermanual/docbook/userman-gen.docbook $pathrel/../doc/usermanual/docbook/vishnu-userman.docbook
cp core/doc/usermanual/docbook/userman-gen.docbook.pdf $pathrel/../doc/usermanual/docbook/vishnu-userman.docbook.pdf
cp core/doc/usermanual/docbook/userman-gen.html $pathrel/../doc/usermanual/docbook/vishnu-userman.html



###############################################################################
#                                     W S                                     #
###############################################################################
create_dir WS
create_dir WS/impl
create_dir WS/impl/VishnuLib
create_dir WS/impl/WSAPI
create_dir WS/WSDL

# Getting vishnu jar
copy_dir $eclipse/VishnuLib/src WS/impl/VishnuLib/
copy_file $eclipse/VishnuLib/pom.xml WS/impl/VishnuLib/

# Getting WSAPI jar
copy_dir $eclipse/WSAPI/src WS/impl/WSAPI/
copy_file $eclipse/WSAPI/pom.xml WS/impl/WSAPI/

# Copying wsdl
copy_file $eclipse/WSAPI/wsdl/UMS.wsdl WS/WSDL/
copy_file $eclipse/WSAPI/wsdl/TMS.wsdl WS/WSDL/
copy_file $eclipse/WSAPI/wsdl/FMS.wsdl WS/WSDL/
copy_file $eclipse/WSAPI/wsdl/IMS.wsdl WS/WSDL/

# Regenerating the jar files

cur=$PWD

cd $path

# Constructing the 2 jar to send
cd WS/impl/VishnuLib

mvn clean

mvn install

cp target/VishnuLib-1.0-SNAPSHOT.jar ..
cp target/VishnuLib-1.0-SNAPSHOT.jar $cur/$pathrel/WS/impl/

cd ../WSAPI

mvn clean

mvn assembly:assembly

cp target/WSAPI-1.0-SNAPSHOT-jar-with-dependencies.jar ../WSAPI.jar
cp target/WSAPI-1.0-SNAPSHOT-jar-with-dependencies.jar $cur/$pathrel/WS/impl/WSAPI.jar

cd $cur

###############################################################################
#                                    U M S                                    #
###############################################################################

# Copy the sendmail script
copy_file core/src/utils/sendmail.py core/src/utils/

# Copy UMS sources
create_dir UMS
create_dir UMS/src/

copy_file UMS/src/CMakeLists.txt UMS/src/
copy_dir UMS/src/api UMS/src
copy_dir UMS/src/cli UMS/src
copy_dir UMS/src/client UMS/src
copy_dir UMS/src/sed UMS/src
copy_dir UMS/src/server UMS/src

# Copy UMS man

create_dir UMS/doc
create_dir UMS/doc/man

# Copy tests
create_dir UMS/test
create_dir UMS/test/src

copy_dir UMS/test/src/sql UMS/test/src
copy_file UMS/test/src/\*pp UMS/test/src
copy_file UMS/test/src/\*.in UMS/test/src
copy_file UMS/test/src/CMakeLists.txt UMS/test/src

# Copy man pages
copy_file  UMS/doc/man/CMakeLists.txt UMS/doc/man/
copy_dir UMS/doc/man/man1 UMS/doc/man/
copy_dir UMS/doc/man/man3 UMS/doc/man/

# Copy tests in release
mkdir $pathrel/../tests/report

copy_rel_file UMS/test/testReports/\*\.pdf ../tests/report/
copy_rel_file UMS/test/testReports/\*\.docbook ../tests/report/
copy_rel_file UMS/test/testReports/\*\.html ../tests/report/

###############################################################################
#                                    T M S                                    #
###############################################################################

create_dir TMS
create_dir TMS/src/
copy_file TMS/src/CMakeLists.txt TMS/src/
copy_file TMS/src/config.cmake TMS/src/
copy_dir TMS/src/api TMS/src
copy_dir TMS/src/cli TMS/src
copy_dir TMS/src/client TMS/src
copy_dir TMS/src/sed TMS/src
copy_dir TMS/src/server TMS/src
copy_dir TMS/src/utils_torque TMS/src
copy_dir TMS/src/slave TMS/src
copy_dir TMS/src/slurm_parser TMS/src

# Copy tests
create_dir TMS/test
create_dir TMS/test/src
copy_dir TMS/test/src/sql TMS/test/src
copy_dir TMS/test/src/scripts TMS/test/src
copy_file TMS/test/src/\*pp TMS/test/src
copy_file TMS/test/src/\*.in TMS/test/src
copy_file TMS/test/src/CMakeLists.txt TMS/test/src

# Copy man pages
create_dir TMS/doc/
create_dir TMS/doc/man
copy_file  TMS/doc/man/CMakeLists.txt TMS/doc/man/
copy_dir  TMS/doc/man/man1 TMS/doc/man/
copy_dir  TMS/doc/man/man3 TMS/doc/man/

# Copy tests in release
copy_rel_file TMS/test/testReports/\*\.pdf ../tests/report/
copy_rel_file TMS/test/testReports/\*\.docbook ../tests/report/
copy_rel_file TMS/test/testReports/\*\.html ../tests/report/

###############################################################################
#                                    F M S                                    #
###############################################################################

create_dir FMS
create_dir FMS/src/
copy_file FMS/src/CMakeLists.txt FMS/src/
copy_dir FMS/src/api FMS/src
copy_dir FMS/src/cli FMS/src
copy_dir FMS/src/client FMS/src
copy_dir FMS/src/sed FMS/src
copy_dir FMS/src/server FMS/src

# Copy tests
create_dir FMS/test
create_dir FMS/test/src
copy_dir FMS/test/src/sql FMS/test/src
copy_file FMS/test/src/\*pp FMS/test/src
copy_file FMS/test/src/\*.in FMS/test/src
copy_file FMS/test/src/CMakeLists.txt FMS/test/src

# Copy man pages
create_dir FMS/doc/
create_dir FMS/doc/man
copy_file  FMS/doc/man/CMakeLists.txt FMS/doc/man/
copy_dir FMS/doc/man/man1 FMS/doc/man/
copy_dir FMS/doc/man/man3 FMS/doc/man/

# Copy tests in release
copy_rel_file FMS/test/testReports/\*\.pdf ../tests/report/
copy_rel_file FMS/test/testReports/\*\.docbook ../tests/report/
copy_rel_file FMS/test/testReports/\*\.html ../tests/report/

###############################################################################
#                                    I M S                                    #
###############################################################################

create_dir IMS
create_dir IMS/src/
copy_file IMS/src/CMakeLists.txt IMS/src/
copy_dir IMS/src/api IMS/src
copy_dir IMS/src/cli IMS/src
copy_dir IMS/src/client IMS/src
copy_dir IMS/src/sed IMS/src
copy_dir IMS/src/server IMS/src
copy_dir IMS/src/utils IMS/src

# Copy tests
create_dir IMS/test
create_dir IMS/test/src
copy_dir IMS/test/src/sql IMS/test/src
copy_dir IMS/test/src/scripts IMS/test/src
copy_file IMS/test/src/\*pp IMS/test/src
copy_file IMS/test/src/\*.in IMS/test/src
copy_file IMS/test/src/CMakeLists.txt IMS/test/src

# Copy man pages
create_dir IMS/doc/
create_dir IMS/doc/man
copy_file  IMS/doc/man/CMakeLists.txt IMS/doc/man/
copy_dir IMS/doc/man/man1 IMS/doc/man/
copy_dir IMS/doc/man/man3 IMS/doc/man/

# Copy tests in release
copy_rel_file IMS/test/testReports/\*\.pdf ../tests/report/
copy_rel_file IMS/test/testReports/\*\.docbook ../tests/report/
copy_rel_file IMS/test/testReports/\*\.html ../tests/report/

###############################################################################
#                                  SWIG API                                   #
###############################################################################

create_dir swigAPI
copy_file swigAPI/CMakeLists.txt swigAPI
copy_file swigAPI/vishnu.i swigAPI
copy_dir swigAPI/generated swigAPI
 
###############################################################################
#                                 All modules                                 #
###############################################################################

copy_file scripts/setCopyright.sh 

cd $path

./setCopyright.sh copyright

rm setCopyright.sh

cd - 

cd $pathrel

./setCopyright.sh copyright

rm setCopyright.sh

cd -

# Copy emf4cpp files
copy_dir core/deps/emf4cpp core/deps/

cd $path

rm -f tmp.txt

# Remove .project
rm -f UMS/src/utils/emfdata/UMS_Data/.project
rm -f UMS/src/utils/emfdata/UMS_Data/.cproject
rm core/deps/emf4cpp/.project
rm core/deps/emf4cpp/.cproject

# remove all ~ files
for i in  $(find . -name "*~") ; do rm $i;  done ;

# remove all bak files
for i in  $(find . -name "*bak") ; do rm $i;  done ;

# remove all build dir
for i in  $(find . -name "*build") ; do rm -rf $i;  done ;

cd -

cd $pathrel

# Remove .project
rm -f UMS/src/utils/emfdata/UMS_Data/.project
rm -f UMS/src/utils/emfdata/UMS_Data/.cproject
rm core/deps/emf4cpp/.project
rm core/deps/emf4cpp/.cproject

# remove all ~ files
for i in  $(find . -name "*~") ; do rm $i;  done ;

# remove all bak files
for i in  $(find . -name "*bak") ; do rm $i;  done ;

# remove all build dir
for i in  $(find . -name "*build") ; do rm -rf $i;  done ;

cd $path

cd ..

# Archive to send
tar -czvf vishnu_v${NO_VERSION}.tgz VISHNU_$NO_VERSION >/dev/null

# Moving archive in /tmp
mv vishnu_v${NO_VERSION}.tgz /tmp/

