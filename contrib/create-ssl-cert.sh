#!/bin/bash

set -e
CERT_PASSWD=certpasswd
SERVER_DN="/DC=com/DC=sysfera/CN=mysql-server"
CLIENT_DN="/DC=com/DC=sysfera/CN=mysql-client"

echo "Create clean environment.................................."
rm -rf mysql-ssl
mkdir mysql-ssl && cd mysql-ssl

echo "Generate the server certificate..........................."
openssl req -x509 -newkey rsa:1024 \
       -keyout server-key-enc.pem -out server-cert.pem \
       -subj ${SERVER_DN} -passout pass:${CERT_PASSWD}

echo "Generate the server key..................................."
openssl rsa -in server-key-enc.pem -out server-key.pem \
       -passin pass:${CERT_PASSWD} -passout pass:

echo "Generate the client certificate..........................."
openssl req -x509 -newkey rsa:1024 \
       -keyout client-key-enc.pem -out client-cert.pem \
       -subj ${CLIENT_DN} -passout pass:${CERT_PASSWD}

echo "Generate the client key................................."
openssl rsa -in client-key-enc.pem -out client-key.pem \
       -passin pass:${CERT_PASSWD} -passout pass:

# Combine the client and server certificates into the CA certificates file:
cat server-cert.pem client-cert.pem > ca-cert.pem
        
echo "Verify the certificate................................."
sleep 2
openssl verify -CAfile ca-cert.pem server-cert.pem client-cert.pem
