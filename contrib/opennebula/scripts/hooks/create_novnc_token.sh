#!/bin/bash
#
# This file should be copied in the location /var/lib/one/remotes/hooks from OpenNebula Server
# Once the file copied, define a hook in /etc/one/oned.conf as follows:
#
# VM_HOOK = [
#   name      = "create_novnc_token",
#   on        = "RUNNING",
#   command   = "create_novnc_token.sh",
#   arguments = "$ID" ]
#

set -e

if [ $# -ne 1 ]; then
  echo "usage: $0 <vmid>"
  exit 1
fi


ONEVM_CMD="onevm"
VMID=$1
HOST=$(${ONEVM_CMD} show ${VMID} | grep "HOST[\ \t]*\:" | tail -1 | tr -d ' ' | awk -F : '{print $2}')
VNCPORT=$(expr 5900 + $VMID)

TOKEN_FILENAME="/var/lib/one/sunstone_vnc_tokens/one-${VMID}"
echo "${VMID}: ${HOST}:${VNCPORT}" > ${TOKEN_FILENAME}
echo "token file created: $TOKEN_FILENAME"