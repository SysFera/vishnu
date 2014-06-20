#!/bin/bash
# This script opens a visualization session on a given VM

set -e
set -u

ONE_SERVER=$1
ONE_VM_ID=$2
ONE_NOVNC_PROXY_PORT=29876
NOVNC_BASE_DIR=/opt/sop/noVNC

firefox "file:///${NOVNC_BASE_DIR}/vnc_auto.html?host=${ONE_SERVER}&port=${ONE_NOVNC_PROXY_PORT}&encrypt=0&true_color=1&path=?token=${ONE_VM_ID}"
