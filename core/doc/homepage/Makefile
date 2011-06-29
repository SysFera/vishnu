all: vishnu-homepage.html

vishnu-homepage.html: vishnu-homepage.docbook
	xsltproc --param ignore.image.scaling 1 ../../tools/docbook/docbook-xsl/xhtml/docbook.xsl vishnu-homepage.docbook > vishnu-homepage.html