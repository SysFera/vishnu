#!/bin/bash
# Copyright SysFera SA
# Last update : 15-05-2013

# Before running this scrit, adapt the following environment variables
# to suit your needs.

CERT_PASSWD=certpasswd                          # Sets the certificate password
SERVER_DN="/DC=com/DC=sysfera/CN=mysql-server"  # Sets the server distinguish name
OUTPUT_DIR=cert                          # Sets the output directory

set -e
echo "Create clean environment.................................."
rm -rf ${OUTPUT_DIR}
mkdir ${OUTPUT_DIR} && cd ${OUTPUT_DIR}

echo "Generate the server certificate..........................."
openssl req -x509 -newkey rsa:1024 \
       -keyout server-key-enc.pem -out server-cert.pem \
       -subj ${SERVER_DN} -passout pass:${CERT_PASSWD}

echo "Generate the server key..................................."
openssl rsa -in server-key-enc.pem -out server-key.pem \
       -passin pass:${CERT_PASSWD} -passout pass:

# Create the CA certification file:
cat server-cert.pem > ca.pem
        
echo "Verify the certificate................................."
sleep 2
openssl verify -CAfile ca.pem server-cert.pem
