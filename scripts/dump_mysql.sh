#!/bin/sh
#
# Dump the mysql DB $1  with username $2 and put result in $3
#
if [ $# -ne 3 ]; then
    echo "Usage: " $0 " <databse_name> <database_username> <outputdir>"
    exit 1
fi
mysqldump $1 -u $2 -p > $3
