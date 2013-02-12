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
if [[ ! -f copyright ]]; then
  echo "Missing copyright file - please start the script in VISHNU root directory"
  exit 1
fi

if [ $# -lt 1 ]; then
  echo "Usage: " $0 " <generator_dir>"
  echo " <generator_dir> path to sysfera's git vishnuGenerator containing the docbook generators."
  exit 1
fi



# Get common functions and variables
source scripts/common.sh

MANPAGESXSL=${vishnuDir}/core/tools/docbook/docbook-xsl/manpages/docbook.xsl
HTMLXSL=${vishnuDir}/core/tools/docbook/docbook-xsl/xhtml/docbook.xsl
PDFSTY=${vishnuDir}/core/tools/docbook/dblatex/docbook-sysfera.sty
GENERATORDIR=$1

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
    run_cmd dblatex -s ${PDFSTY} -o ${FILEBASENAME}.pdf ${FILEBASENAME}.docbook
    
    # HTML generation
    run_cmd xsltproc -o ${FILEBASENAME}.html --param ignore.image.scaling 1 ${HTMLXSL} ${FILEBASENAME}.docbook

    cd $cur
}

function generate_man () {
    cur=${PWD}
    cd $1

    # Create temporary directory
    run_cmd rm -rf man1
    run_cmd rm -rf man3
    
    # Create man directory  
    run_cmd mkdir man1
    run_cmd mkdir man3
    
    for i in ${@:2}; do
        FILEBASENAME=$i
        run_cmd xsltproc ${MANPAGESXSL} $PWD/docbook/${FILEBASENAME}.docbook 

        # move of files
        run_cmd mv *.1 man1
        run_cmd mv *.3 man3
    done

    cd $cur
}

function generate_docbook () {
    path=$1
    template=$2
    run_cmd ${scriptDir}/docbook_astah_gen.sh $GENERATORDIR $path $template
}

######################################################################
#                           /PREPARATION                             #
######################################################################
#
### generate gen files from templates
#

# Generate STB from template
generate_docbook ${PWD} ${PWD}/core/specs/docbook/STB-template.docbook

# Generate SpecGen from template
generate_docbook ${PWD} ${PWD}/core/specs/docbook/SpecGen-template.docbook

# Generate design from template
generate_docbook ${PWD} ${PWD}/TMS/design/docbook/TmsDesign-template.docbook
generate_docbook ${PWD} ${PWD}/FMS/design/docbook/FmsDesign-template.docbook
generate_docbook ${PWD} ${PWD}/IMS/design/docbook/ImsDesign-template.docbook
generate_docbook ${PWD} ${PWD}/UMS/design/docbook/UmsDesign-template.docbook

# Generate adminman from template
generate_docbook ${PWD}/core/model ${PWD}/core/doc/adminmanual/docbook/adminman-template.docbook

# Generate userman from template
generate_docbook ${PWD}/core/model ${PWD}/core/doc/usermanual/docbook/userman-template.docbook

# Generate manpage from template
generate_docbook ${PWD}/core/model ${PWD}/UMS/doc/man/docbook/userman-template.docbook
generate_docbook ${PWD}/core/model ${PWD}/UMS/doc/man/docbook/adminman-template.docbook
generate_docbook ${PWD}/core/model ${PWD}/TMS/doc/man/docbook/usermanTMS-template.docbook
generate_docbook ${PWD}/core/model ${PWD}/FMS/doc/man/docbook/usermanFMS-template.docbook
generate_docbook ${PWD}/core/model ${PWD}/IMS/doc/man/docbook/manIMS-template.docbook

# Generate cli from template
generate_docbook ${PWD}/core/model ${PWD}/core/specs/docbook/cli-template.docbook

# Generate api from template
generate_docbook ${PWD}/core/model ${PWD}/core/specs/docbook/api-template.docbook


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
# generate_doc core/doc/adminmanual/docbook adminman_eng-gen
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
generate_doc UMS/test/testPlan UMS-PlanTests
generate_doc IMS/test/testPlan IMS-PlanTests
generate_doc TMS/test/testPlan TMS-PlanTests
generate_doc FMS/test/testPlan FMS-PlanTests

# generate manpages
generate_man UMS/doc/man adminman-gen userman-gen
generate_man IMS/doc/man manIMS-gen
generate_man TMS/doc/man usermanTMS-gen
generate_man FMS/doc/man usermanFMS-gen


# All done
print_exit

