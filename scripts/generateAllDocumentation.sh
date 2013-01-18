#!/bin/bash
# Script to generate the documentation for all modules.
# All docs are generated in place
# Author Benjamin Depardon (benjamin.depardon@sysfera.com)
# Date 19/12/12
# It generates the following documents:
# - doxygen documentation
# - adminman, userman and quickstart
# - STB, general specifications, API doc, and CLI doc
# - UMS, IMS, FMS and TMS design doc
# - UMS, IMS, FMS and TMS test reports
# - UMS, IMS, FMS and TMS test plan
# - manpages

if [ ! -f copyright ]; then
  echo "Missing copyright file - please start the script in VISHNU root directory"
  exit 1
fi


######################################################################
#                            VARIABLES                               #
######################################################################
# get complete path to script dir
vishnuDir=`pwd`
scriptDir=$vishnuDir/scripts

# script to generate documentation from a docbook file
gen_pdf_html_sh=${scriptDir}/generatePDF_HTML_fromDocbook.sh

######################################################################
#                           /VARIABLES                               #
######################################################################

######################################################################
#                            FUNCTIONS                               #
######################################################################
function generate_doc () {
    # Generate pdf and html from docbook
    # Usage:  generate_doc <path to directory> <docbook basename without extension>
    # Example: generate_doc UMS/test/testReports reportUMSTest
    # This example will produce reportUMSTest.pdf and reportUMSTest.html

    cur=$PWD
    cd $1
    FILEBASENAME=$2

    # PDF generation
    dblatex -s ${vishnuDir}/core/tools/docbook/dblatex/docbook-sysfera.sty ${FILEBASENAME}.docbook
    mv ${FILEBASENAME}.docbook.pdf ${FILEBASENAME}.pdf
    
    # HTML generation
    xsltproc --param ignore.image.scaling 1 ${vishnuDir}/core/tools/docbook/docbook-xsl/xhtml/docbook.xsl ${FILEBASENAME}.docbook > ${FILEBASENAME}.html

    cd $cur
}

function generate_man () {
    cur=$PWD
    cd $1
    FILEBASENAME=$2

    # Create temporary directory
    rm -r man1
    rm -r man3
    
    # Create man directory  
    mkdir man1
    mkdir man3
    
    xsltproc $PWD/docbook/${FILEBASENAME}.docbook 

    # move of files
    mv *.1 man1
    mv *.3 man3

    cd $cur
}

######################################################################
#                           /FUNCTIONS                               #
######################################################################


# generate doxygen documentation
mkdir doc
rm -rf doc/*
doxygen Doxyfile
doxygen Doxyfile_API

# generate adminman, userman and quickstart
generate_doc core/doc/adminmanual/docbook adminman-gen
generate_doc core/doc/adminmanual/docbook adminman_eng-gen
generate_doc core/doc/usermanual/docbook userman-gen
generate_doc core/doc/quickstart quick-start

# generate STB, general specifications, API doc, and CLI doc
generate_doc core/specs/docbook STB-gen
generate_doc core/specs/docbook SpecGen-gen
generate_doc core/specs/docbook api-gen
generate_doc core/specs/docbook cli-gen

# generate UMS, IMS, FMS and TMS design doc
generate_doc UMS/design/docbook UmsDesign-gen
generate_doc IMS/design/docbook ImsDesign-gen
generate_doc TMS/design/docbook TmsDesign-gen
generate_doc FMS/design/docbook FmsDesign-gen

# generate UMS, IMS, FMS and TMS test reports
generate_doc UMS/test/testReports reportUMSTest
generate_doc IMS/test/testReports reportIMSTest
generate_doc TMS/test/testReports reportTMSTest
generate_doc FMS/test/testReports reportFMSTest

# generate UMS, IMS, FMS and TMS test plan
generate_doc UMS/test/testPlan VISHNU_D2_1b-UMS-PlanTests
generate_doc IMS/test/testPlan VISHNU_D5_1b-IMS-PlanTests
generate_doc TMS/test/testPlan VISHNU_D4_1b-TMS-PlanTests
generate_doc FMS/test/testPlan VISHNU_D3_1b-FMS-PlanTests

# generate manpages
generate_man UMS/doc/man adminman-gen
generate_man UMS/doc/man userman-gen
generate_man IMS/doc/man manIMS-gen
generate_man TMS/doc/man usermanTMS-gen
generate_man FMS/doc/man usermanFMS-gen
