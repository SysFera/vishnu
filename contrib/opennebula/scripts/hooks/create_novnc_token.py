#!/usr/bin/python
#
# This file should be copied in the location /var/lib/one/remotes/hooks from OpenNebula Server
# Once the file copied, define a hook in /etc/one/oned.conf as follows:
#
# VM_HOOK = [
#   name      = "create_novnc_token",
#   on        = "RUNNING",
#   command   = "create_novnc_token.py",
#   arguments = "$ID" ]
#

import sys
import socket

if len(sys.argv) != 2 :
  print 'usage: {0} <vmid>'.format(sys.argv[0])
  exit(-1)

vmId = str(sys.argv[1])
vncPort = 5900 + int(vmId)
oneServer = socket.gethostname()

fileName = '/var/lib/one/sunstone_vnc_tokens/one-{0}'.format(vmId)
file = open(fileName, 'w')
file.write('{0}: {1}:{2}'.format(vmId, oneServer, vncPort));

print 'token file writed:', fileName
