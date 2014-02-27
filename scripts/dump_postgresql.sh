#!/bin/sh
#
# Dump the pgsql DB $1  with username $2 and put result in $3
#
if [ $# -ne 3 ]; then
    echo "Usage: " $0 " <databse_name> <database_username> <outputdir>"
    exit 1
fi
pg_dump $1 -U $2 > $3
