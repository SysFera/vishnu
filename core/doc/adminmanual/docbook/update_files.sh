#!/bin/sh
#
# description : Script to generate pdf and html from docbook source file
# author      : kevin coulomb (kevin.coulomb@sysfera.com)
# date        : 22/03/2012


# Génération du pdf fr
dblatex -s ../../../tools/docbook/dblatex/docbook-sysfera.sty adminman-gen.docbook

# Génération de l'html fr
xsltproc --param ignore.image.scaling 1 ../../../tools/docbook/docbook-xsl/xhtml/docbook.xsl adminman-gen.docbook > adminman-gen.html

# Génération du pdf eng
dblatex -s ../../../tools/docbook/dblatex/docbook-sysfera.sty adminman_eng-gen.docbook

# Génération de l'html eng
xsltproc --param ignore.image.scaling 1 ../../../tools/docbook/docbook-xsl/xhtml/docbook.xsl adminman_eng-gen.docbook > adminman_eng-gen.html
