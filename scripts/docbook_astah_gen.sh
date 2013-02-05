#!/bin/bash
#
# Script to generate a docbook another docbook using ecore data
# Author K. Coulomb (kevin.coulomb@sysfera.com)
# Date 05/02/13
#

# example :
# SpecGen-template
# ./docbook_astah_gen.sh /home/keo/Téléchargements/eclipse/plugins/ /home/keo/Bureau/depot_git_eclipse/eclipse/ /home/keo/Bureau/depot_git_edf/vishnu /home/keo/Bureau/depot_git_edf/vishnu/core/specs/docbook/SpecGen-template.docbook 
#
# STB-template
# ./docbook_astah_gen.sh /home/keo/Téléchargements/eclipse/plugins/ /home/keo/Bureau/depot_git_eclipse/eclipse/ /home/keo/Bureau/depot_git_edf/vishnu /home/keo/Bureau/depot_git_edf/vishnu/core/specs/docbook/STB-template.docbook 
#
# design
#
## ./docbook_astah_gen.sh /home/keo/Téléchargements/eclipse/plugins/ /home/keo/Bureau/depot_git_eclipse/eclipse/ /home/keo/Bureau/depot_git_edf/vishnu/ /home/keo/Bureau/depot_git_edf/vishnu/TMS/design/docbook/TmsDesign-template.docbook 
#
## ./docbook_astah_gen.sh /home/keo/Téléchargements/eclipse/plugins/ /home/keo/Bureau/depot_git_eclipse/eclipse/ /home/keo/Bureau/depot_git_edf/vishnu /home/keo/Bureau/depot_git_edf/vishnu/IMS/design/docbook/ImsDesign-template.docbook 
#
## ./docbook_astah_gen.sh /home/keo/Téléchargements/eclipse/plugins/ /home/keo/Bureau/depot_git_eclipse/eclipse/ /home/keo/Bureau/depot_git_edf/vishnu /home/keo/Bureau/depot_git_edf/vishnu/FMS/design/docbook/FmsDesign-template.docbook 
#
## ./docbook_astah_gen.sh /home/keo/Téléchargements/eclipse/plugins/ /home/keo/Bureau/depot_git_eclipse/eclipse/ /home/keo/Bureau/depot_git_edf/vishnu /home/keo/Bureau/depot_git_edf/vishnu/UMS/design/docbook/UmsDesign-template.docbook 



# Path to the directory containing the eclipse plugins jars
jarpath=$1
# Path to the root of the repositories containing the generators
generatorpath=$2
# Directory containing apim and ecore
rootmodeldir=$3
# Template file
templatefile=$4

# NOTE : KEEP 2.0.0, otherwize generation will be wrong for python lists, 2.0.0 makes generator set list as return values
version=2.0.0

/usr/lib/jvm/java-6-sun-1.6.0.26/bin/java -Dfile.encoding=UTF-8 -classpath $jarpath/org.eclipse.core.runtime_3.6.0.v20100505.jar:$jarpath/org.eclipse.emf.ecore.xmi_2.5.0.v20100521-1846.jar:$jarpath/org.apache.commons.cli_1.0.0.v20080604-1500.jar:$generatorpath/com.sysfera.codegen.api/bin:$generatorpath/com.sysfera.codegen.docbook/bin:$generatorpath/com.sysfera.codegen.docbook/libs/astah-pro.jar:$generatorpath/com.sysfera.codegen.docbook/libs/org.kermeta.docbook.jar:$generatorpath/com.sysfera.codegen.docbook/libs/org.eclipse.emf.ecore_2.6.1.v20100914-1218.jar:$generatorpath/com.sysfera.codegen.docbook/libs/org.eclipse.emf.common_2.6.0.v20100914-1218.jar com.sysfera.codegen.docbook.DocBookDocumentGenerator -I $rootmodeldir $templatefile $version
