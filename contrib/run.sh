#!/bin/bash

SCRIPT_PATH="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

nohup ${SCRIPT_PATH}/bin/xmssed ${SCRIPT_PATH}/vishnu.cfg > /tmp/vishnu.log &

