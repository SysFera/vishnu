#!/bin/bash
# Script find all Vishnu's sources, without third party software
# Author Benjamin Depardon (benjamin.depardon@sysfera.com)
# Date 07/03/13

if [[ ! -f copyright ]]; then
  echo "Missing copyright file - please start the script in VISHNU root directory"
  exit 1
fi


FIND_DIR="FMS TMS UMS IMS core communication"
EXCLUDE_DIR="TMS/src/slurm_parser TMS/src/deltacloudcommon TMS/src/utils_pbs TMS/src/utils_torque core/deps core/test/third-party core/tools core/src/emfdata/"
EXTENSIONS="cc cpp hpp hh c h"

cmd="find ${FIND_DIR} \\("

for e in ${EXCLUDE_DIR}; do
    cmd+=" \\( -not -regex .*${e}.* \\) -and"
done
cmd=${cmd%-and}

cmd+=" \\) -type f \\( "

for e in ${EXTENSIONS}; do
    cmd+=" -name *.${e} -or"
done
cmd=${cmd%-or}

cmd+=" \\)"

/bin/bash -c "$cmd"



