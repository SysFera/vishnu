#!/bin/sh
#
# description : Script to generate pdf and html from docbook source file
# author      : kevin coulomb (kevin.coulomb@sysfera.com)
# date        : 22/03/2012


# Génération du pdf
dblatex -s ../../../core/tools/docbook/dblatex/docbook-sysfera.sty TmsDesign-gen.docbook

# Génération de l'html
xsltproc --param ignore.image.scaling 1 ../../../core/tools/docbook/docbook-xsl/xhtml/docbook.xsl TmsDesign-gen.docbook > TmsDesign-gen.html
