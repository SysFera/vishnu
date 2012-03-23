#!/bin/sh
#
# description : Script to generate pdf and html from docbook source file
# author      : kevin coulomb (kevin.coulomb@sysfera.com)
# date        : 22/03/2012


# Génération du pdf
dblatex -s ../../tools/docbook/dblatex/docbook-sysfera.sty STB-gen.docbook

# Génération de l'html
xsltproc --param ignore.image.scaling 1 ../../tools/docbook/docbook-xsl/xhtml/docbook.xsl STB-gen.docbook > STB-gen.html
