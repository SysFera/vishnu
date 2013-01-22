#!/bin/bash
#
# Script to prepare the release for VISHNU source code (All modules)
# Author K. Coulomb (kevin.coulomb@sysfera.com)
# Date 18/04/11
#
# 29/08/11 : Add new directory to prepare in the git version of the depot
#            Merge with WS release
# 21/01/13 : use generateAllDocumentation.sh to generate Vishnu docs.
#            Use CMakeLists.txt to retrieve Vishnu version
#

if [ ! $# -eq 1 ]; then
  echo "Usage: " $0 " <depot_eclipe_path>"
  exit 1
fi

if [ ! -f copyright ]; then
  echo "Missing copyright file - please start the script in VISHNU root directory"
  exit 1
fi

eclipse=$1


# Get vishnu version
versionNumber=`egrep -e "VISHNU_VERSION .*" CMakeLists.txt | awk -F'"' '{print $2}' | sed "s/ /_/g"` 
versionComment=`egrep -e "VISHNU_VERSION_COMMENTS .*" CMakeLists.txt | awk -F'"' '{print $2}' | sed "s/ /_/g"`

NO_VERSION="${versionNumber}${versionComment}"
echo "Preparing VISHNU v" $NO_VERSION "..."


######################################################################
#                            VARIABLES                               #
######################################################################
# get complete path to script dir
vishnuDir=`pwd`
scriptDir=$vishnuDir/scripts

# script to generate all docs
gen_doc_sh=${scriptDir}/generateAllDocumentation.sh

nbfailed=0
nbexec=0

######################################################################
#                           /VARIABLES                               #
######################################################################


######################################################################
#                            FUNCTIONS                               #
######################################################################
function run_cmd() {
    $@
    tmprv=$?
    nbexec=$(($nbexec+1))
    if [ $tmprv != 0 ]; then
        nbfailed=$(($nbfailed+1))
    fi
}

# Function that copy the dir in the archive and the depot copy
function copy_dir () {
    run_cmd cp -r $1 $path/$2;
    run_cmd cp -r $1 $pathrel/$2;
}

function copy_file () {
    run_cmd cp $1 $path/$2;
    run_cmd cp $1 $pathrel/$2;
}

# Function that create the dir in the archive and the depot copy
function create_dir () {
    run_cmd mkdir -p $path/$1;
    run_cmd mkdir -p $pathrel/$1;
}

function copy_rel_file () {
    run_cmd cp $1 $pathrel/$2;
}

function copy_rel_dir () {
    run_cmd cp -r $1 $pathrel/$2;
}

function copy_ar_file () {
    run_cmd cp $1 $path/$2;
}

function remove_files () {
    run_cmd rm -rf $@;
}

function change_dir () {
    run_cmd cd $1;
}
######################################################################
#                           /FUNCTIONS                               #
######################################################################



# Create temporary directory
run_cmd rm -rf /tmp/prepaRel
run_cmd mkdir -p /tmp/prepaRel
path=/tmp/prepaRel/VISHNU_$NO_VERSION
run_cmd mkdir -p $path

pathrel=deliverables/src

run_cmd mkdir -p deliverables/src
run_cmd mkdir -p deliverables/doc
run_cmd mkdir -p deliverables/debs
run_cmd mkdir -p deliverables/tests


remove_files deliverables/src/*
remove_files deliverables/doc/*
remove_files deliverables/debs/*
remove_files deliverables/tests/*



# Generate all documentation
run_cmd sh -c $gen_doc_sh



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
copy_dir License

# Copy Cmake find files
copy_dir Cmake

# Copy sql scripts
create_dir core
create_dir core/database

copy_file core/database/database_\*.sql core/database/
copy_file core/database/postgre_\*.sql core/database/
copy_file core/database/mysql_\*.sql core/database/

# Copy contrib
create_dir contrib
copy_file contrib/\* contrib/

# Copy core sources
create_dir core/doc
create_dir core/src
create_dir core/src/config
create_dir core/src/database
create_dir core/src/emfdata
create_dir core/src/exception
create_dir core/src/registry
create_dir core/src/utils
create_dir core/src/authenticator
create_dir core/src/authenticator/ldap
create_dir core/test
create_dir core/test/cfgs
create_dir core/test/src
create_dir core/src/version
create_dir core/src/version
create_dir core/examples/confs/

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
copy_file core/src/authenticator/\*pp core/src/authenticator/
copy_file core/src/authenticator/ldap/\*pp core/src/authenticator/ldap/
copy_file core/test/src/\*pp core/test/src/
copy_file core/test/src/\*h.in core/test/src/
copy_file core/test/cfgs/\*cfg.in core/test/cfgs/
copy_dir core/test/third-party core/test
copy_file core/src/version/\*pp core/src/version/
copy_file core/examples/confs/\* core/examples/confs/

# Copy ZMQ files
create_dir mockZMQ/tests

copy_file mockZMQ/\*pp mockZMQ/
copy_file mockZMQ/\*h mockZMQ/
copy_file mockZMQ/CMakeLists.txt mockZMQ/
copy_file mockZMQ/tests/\*pp mockZMQ/tests
copy_file mockZMQ/tests/CMakeLists.txt mockZMQ/tests


# dependency of emf
create_dir core/deps

copy_file core/deps/CMakeLists.txt core/deps/
copy_dir core/deps/emf4cpp core/deps/


### Documentation html/pdf/docbook
run_cmd mkdir -p $path/core/doc/adminmanual
run_cmd mkdir -p $path/core/doc/usermanual

run_cmd mkdir -p $pathrel/../doc/adminmanual
run_cmd mkdir -p $pathrel/../doc/usermanual

run_cmd cp core/doc/adminmanual/docbook/adminman-gen.docbook $path/core/doc/adminmanual/vishnu-adminman.docbook
run_cmd cp core/doc/adminmanual/docbook/adminman-gen.pdf $path/core/doc/adminmanual/vishnu-adminman.pdf
run_cmd cp core/doc/adminmanual/docbook/adminman-gen.html $path/core/doc/adminmanual/vishnu-adminman.html

run_cmd cp core/doc/usermanual/docbook/userman-gen.docbook $path/core/doc/usermanual/vishnu-userman.docbook
run_cmd cp core/doc/usermanual/docbook/userman-gen.pdf $path/core/doc/usermanual/vishnu-userman.pdf
run_cmd cp core/doc/usermanual/docbook/userman-gen.html $path/core/doc/usermanual/vishnu-userman.html

run_cmd cp core/doc/adminmanual/docbook/adminman-gen.docbook $pathrel/../doc/adminmanual/vishnu-adminman.docbook
run_cmd cp core/doc/adminmanual/docbook/adminman-gen.pdf $pathrel/../doc/adminmanual/vishnu-adminman.pdf
run_cmd cp core/doc/adminmanual/docbook/adminman-gen.html $pathrel/../doc/adminmanual/vishnu-adminman.html

run_cmd cp core/doc/usermanual/docbook/userman-gen.docbook $pathrel/../doc/usermanual/vishnu-userman.docbook
run_cmd cp core/doc/usermanual/docbook/userman-gen.pdf $pathrel/../doc/usermanual/vishnu-userman.pdf
run_cmd cp core/doc/usermanual/docbook/userman-gen.html $pathrel/../doc/usermanual/vishnu-userman.html



###############################################################################
#                                     Java API                                #
###############################################################################
create_dir WS
create_dir WS/impl
create_dir WS/impl/VishnuLib
# create_dir WS/impl/WSAPI
# create_dir WS/WSDL

# Getting vishnu jar
copy_dir $eclipse/VishnuLib/src WS/impl/VishnuLib/
copy_file $eclipse/VishnuLib/pom.xml WS/impl/VishnuLib/

# Getting WSAPI jar
# copy_dir $eclipse/WSAPI/src WS/impl/WSAPI/
# copy_file $eclipse/WSAPI/pom.xml WS/impl/WSAPI/

# Copying wsdl
# copy_file $eclipse/WSAPI/wsdl/UMS.wsdl WS/WSDL/
# copy_file $eclipse/WSAPI/wsdl/TMS.wsdl WS/WSDL/
# copy_file $eclipse/WSAPI/wsdl/FMS.wsdl WS/WSDL/
# copy_file $eclipse/WSAPI/wsdl/IMS.wsdl WS/WSDL/

# Regenerating the jar files

cur=$PWD

change_dir $path

# Constructing the 2 jar to send
change_dir WS/impl/VishnuLib

run_cmd mvn clean

run_cmd mvn install

run_cmd cp target/VishnuLib-1.0-SNAPSHOT.jar ..
run_cmd cp target/VishnuLib-1.0-SNAPSHOT.jar $cur/$pathrel/WS/impl/

# cd ../WSAPI

# mvn clean

# mvn assembly:assembly

# cp target/WSAPI-1.0-SNAPSHOT-jar-with-dependencies.jar ../WSAPI.jar
# cp target/WSAPI-1.0-SNAPSHOT-jar-with-dependencies.jar $cur/$pathrel/WS/impl/WSAPI.jar

change_dir $cur

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
copy_file UMS/test/src/.netrc.in UMS/test/src

# Copy man pages
copy_file  UMS/doc/man/CMakeLists.txt UMS/doc/man/
copy_dir UMS/doc/man/man1 UMS/doc/man/
copy_dir UMS/doc/man/man3 UMS/doc/man/

# Copy tests in release
run_cmd mkdir -p $pathrel/../tests/report

copy_rel_file UMS/test/testReports/\*\.pdf ../tests/report/
copy_rel_file UMS/test/testReports/\*\.docbook ../tests/report/
copy_rel_file UMS/test/testReports/\*\.html ../tests/report/

# Copy test plans in release
run_cmd mkdir -p $pathrel/../doc/concept/images
copy_rel_file UMS/test/testPlan/\*\.pdf ../doc/concept/
copy_rel_file UMS/test/testPlan/\*\.docbook ../doc/concept/
copy_rel_file UMS/test/testPlan/\*\.html ../doc/concept/
copy_rel_file UMS/test/testPlan/images/\* ../doc/concept/images

# Copy design in release
copy_rel_file UMS/design/docbook/\*-gen\.pdf ../doc/concept/
copy_rel_file UMS/design/docbook/\*-gen\.docbook ../doc/concept/
copy_rel_file UMS/design/docbook/\*-gen\.html ../doc/concept/
copy_rel_dir UMS/design/docbook/images/png ../doc/concept/images


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
copy_dir TMS/src/lsf_parser TMS/src

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

# Copy test plans in release
copy_rel_file TMS/test/testPlan/\*\.pdf ../doc/concept/
copy_rel_file TMS/test/testPlan/\*\.docbook ../doc/concept/
copy_rel_file TMS/test/testPlan/\*\.html ../doc/concept/
copy_rel_file TMS/test/testPlan/images/\* ../doc/concept/images

# Copy design in release
copy_rel_file TMS/design/docbook/\*-gen\.pdf ../doc/concept/
copy_rel_file TMS/design/docbook/\*-gen\.docbook ../doc/concept/
copy_rel_file TMS/design/docbook/\*-gen\.html ../doc/concept/
copy_rel_dir TMS/design/docbook/images/png ../doc/concept/images


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

# Copy test plans in release
copy_rel_file FMS/test/testPlan/\*\.pdf ../doc/concept/
copy_rel_file FMS/test/testPlan/\*\.docbook ../doc/concept/
copy_rel_file FMS/test/testPlan/\*\.html ../doc/concept/
copy_rel_file FMS/test/testPlan/images/\* ../doc/concept/images

# Copy design in release
copy_rel_file FMS/design/docbook/\*-gen\.pdf ../doc/concept/
copy_rel_file FMS/design/docbook/\*-gen\.docbook ../doc/concept/
copy_rel_file FMS/design/docbook/\*-gen\.html ../doc/concept/
copy_rel_dir FMS/design/docbook/images/png ../doc/concept/images

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

# Copy test plans in release
copy_rel_file IMS/test/testPlan/\*\.pdf ../doc/concept/
copy_rel_file IMS/test/testPlan/\*\.docbook ../doc/concept/
copy_rel_file IMS/test/testPlan/\*\.html ../doc/concept/
copy_rel_file IMS/test/testPlan/images/\* ../doc/concept/images

# Copy design in release
copy_rel_file IMS/design/docbook/\*-gen\.pdf ../doc/concept/
copy_rel_file IMS/design/docbook/\*-gen\.docbook ../doc/concept/
copy_rel_file IMS/design/docbook/\*-gen\.html ../doc/concept/
copy_rel_dir IMS/design/docbook/images/png ../doc/concept/images


###############################################################################
#                                   SPECIF                                    #
###############################################################################
run_cmd mkdir -p $pathrel/../doc/specif/images
copy_rel_file core/specs/docbook/\*-gen\.pdf ../doc/specif/
copy_rel_file core/specs/docbook/\*-gen\.docbook ../doc/specif/
copy_rel_file core/specs/docbook/\*-gen\.html ../doc/specif/
copy_rel_dir core/specs/docbook/images/png ../doc/specif/images
copy_rel_dir core/specs/docbook/images/svg ../doc/specif/images


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

change_dir $path
run_cmd ./setCopyright.sh copyright
remove_files setCopyright.sh

change_dir - 
change_dir $pathrel

run_cmd ./setCopyright.sh copyright

remove_files setCopyright.sh

change_dir -

# Copy emf4cpp files
copy_dir core/deps/emf4cpp core/deps/

change_dir $path

remove_files tmp.txt

# Remove .project
remove_files UMS/src/utils/emfdata/UMS_Data/.project
remove_files UMS/src/utils/emfdata/UMS_Data/.cproject
remove_files core/deps/emf4cpp/.project
remove_files core/deps/emf4cpp/.cproject

# remove all ~ files
for i in  $(find . -name "*~") ; do
    remove_files $i
done

# remove all bak files
for i in  $(find . -name "*bak") ; do
    remove_files $i
done

# remove all build dir
for i in  $(find . -name "*build") ; do
    remove_files -rf $i
done

change_dir -
change_dir $pathrel

# Remove .project
remove_files UMS/src/utils/emfdata/UMS_Data/.project
remove_files UMS/src/utils/emfdata/UMS_Data/.cproject
remove_files core/deps/emf4cpp/.project
remove_files core/deps/emf4cpp/.cproject

# remove all ~ files
for i in  $(find . -name "*~") ; do
    remove_files $i
done

# remove all bak files
for i in  $(find . -name "*bak") ; do
    remove_files $i
done

# remove all build dir
for i in  $(find . -name "*build") ; do
    remove_files $i
done

change_dir $path
change_dir ..

# Archive to send
run_cmd tar -czvf vishnu_v${NO_VERSION}.tgz VISHNU_$NO_VERSION >/dev/null

# Moving archive in /tmp
run_cmd mv vishnu_v${NO_VERSION}.tgz /tmp/

echo
echo
echo "## Failed / total: $nbfailed / $nbexec"

exit $nbfailed
