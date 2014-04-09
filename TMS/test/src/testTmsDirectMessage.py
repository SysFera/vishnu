#!/usr/bin/python
# -*- coding: iso-8859-15 -*-

import zmq

context = zmq.Context()

#  Socket to talk to server
socket = context.socket(zmq.REQ)
socket.connect("tcp://localhost:5565")

#  Do 10 requests, waiting each time for a response
print("Sending request ")
socket.send(R"""{"params":["nulldd","machine_1","#!/bin/bash\n#\n#% vishnu_wallclocklimit=01:00:00\n\n## This script is a simple example showing some SysFera-DS variables.\n# It takes as input the following variables:\n# - name: a string representing the name of the user\n# - file: a file\n\n\nHOSTNAME=`hostname`\n\necho \"Hello ${name}!\"\necho \"You are on machine ${HOSTNAME}\"\n\necho \"The name of the submitted server is: \" $VISHNU_SUBMIT_MACHINE_NAME\n\necho \"TEST OF VISHNU JOB OUTPUT ENVIRONMENT VARIABLES!....\"\necho \"VISHNU_BATCHJOB_ID: \"$VISHNU_BATCHJOB_ID\necho \"VISHNU_BATCHJOB_NAME: \" $VISHNU_BATCHJOB_NAME\necho \"VISHNU_BATCHJOB_NODEFILE:\" $VISHNU_BATCHJOB_NODEFILE\ncat $VISHNU_BATCHJOB_NODEFILE\necho \"VISHNU_BATCHJOB_NUM_NODES: \"$VISHNU_BATCHJOB_NUM_NODES\necho \"VISHNU_SUBMIT_MACHINE_NAME: \"$VISHNU_SUBMIT_MACHINE_NAME\necho \"VISHNU_OUTPUT_DIR: \" $VISHNU_OUTPUT_DIR\n\necho\necho \"Content of input file:\"\ncat ${file}\n\necho\necho \"Goodbye ${name}\"\n\n\n","{\"cputime\":\"1\",\"workid\":3}"],"name":null,"param_count":4""")

#  Get the reply.
message = socket.recv()
print("Received reply [%s]" % (message))
