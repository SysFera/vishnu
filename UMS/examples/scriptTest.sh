#!/bin/sh

#
# This script launches some VISHNU commands. 
# Date : 03/03/2011
# Author : K. COULOMB
#
# Prerequisite
#
# A postgre database must be running and have been initialized 
#
# The VISHNU server side must be running i.e. :
# launch omninames -> omniNames -start (if it fails, check the presence of the omninames generated file and remove them or launch omniNames without parameter)
# launch a master agent of diet -> dietAgent ~/myClient_server/mathOp/MA.cfg,
#   'dietAgent ~/myClient_server/mathOp/MA.cfg' that may contain the 3 following lines : 
#     traceLevel = 1
#     agentType = DIET_MASTER_AGENT 
#     name = MA0
# Launch the VISHNU SeD with a line like 
# ./umssed ~/myClient_server/mathOp/SeD.cfg 1 pg localhost vishnu_user vishnu_user ~/Bureau/sendmail.py
#   'umsed' -> name of the sed program
#   '~/myClient_server/mathOp/SeD.cfg' that may contain the 2 following lines 
#      traceLevel = 10
#      parentName = MA0 (must be the same as the name in the MA)
#   '1' -> The vishnuid to refer in the database
#   'pg' -> To use a postgre database
#   'localhost' -> the database
#   'vishnu_user' -> database username
#   'vishnu_user' -> database password
#   '~/Bureau/sendmail.py' -> A python script used to send the mails, it is provided with the ums package
#
# Export the VISHNU configuration file
# > export VISHNU_CONFIG_FILE=/home/keo/myClient_server/mathOp/client.cfg, with the file that may contain
#   traceLevel  = 10
#   MAName = MA0


# Connection with the user 'root' and password 'vishnu_user'
vishnu_connect root -w vishnu_user

# Connection with the user 'root' and password 'vishnu_user' closing the session on timeout
vishnu_connect root -w vishnu_user -p 1

# Connection with the user 'root' and password 'vishnu_user' closing the session on disconnect
vishnu_connect root -w vishnu_user -p 2

# Display sessions
vishnu_list_sessions

# Display sessions that close on disconnect
vishnu_list_sessions -p 2

# Display sessions that close on timeout
vishnu_list_sessions -p 1

# Add a local account for user admin_1 on machine of id machine_1 with the account login toto  the ssh key is /tmp/mykey with home is /home/toto
vishnu_add_local_account admin_1 machine_1 toto /tmp/mykey /home/toto

# Visualize the account created on the machine machine_1
vishnu_list_local_accounts -i machine_1

# Delete the account of admin_1 on the machine machine_1
vishnu_delete_local_account admin_1 machine_1

# List the account of admin_1 on machine_1
vishnu_list_local_accounts  -i machine_1 -u admin_1

# Add a machine called graal located in paris with a description in french, the public key is in /path and the description is 'To finish the quest Arthur needs the graal'. Warning the description is not optionnal.
vishnu_add_machine graal paris french /path -d "To finish the quest Arthur needs the graal"

# List all the machines
./vishnu_list_machines -a

# Close the session
vishnu_close

