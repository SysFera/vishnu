#!/bin/bash

# -------------------------------------------------------------------------- #
# Copyright 2002-2013, OpenNebula Project (OpenNebula.org), C12G Labs        #
#                                                                            #
# Licensed under the Apache License, Version 2.0 (the "License"); you may    #
# not use this file except in compliance with the License. You may obtain    #
# a copy of the License at                                                   #
#                                                                            #
# http://www.apache.org/licenses/LICENSE-2.0                                 #
#                                                                            #
# Unless required by applicable law or agreed to in writing, software        #
# distributed under the License is distributed on an "AS IS" BASIS,          #
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   #
# See the License for the specific language governing permissions and        #
# limitations under the License.                                             #
#                                                                            #
# Script alterted to suit VISHNU needs                                       #
# Last update :21/10/2013                                                    #
# Rodrigue Chakode <rodrigue.chakode@sysfera.com>                            #
#--------------------------------------------------------------------------- #


if [ -f /mnt/context/context.sh ]
then
  . /mnt/context/context.sh
fi


echo $HOSTNAME > /etc/HOSTNAME
hostname $HOSTNAME

if [ -n "$IP_PUBLIC" ]; then
	ifconfig eth0 $IP_PUBLIC
fi
 
if [ -n "$NETMASK" ]; then
	ifconfig eth0 netmask $NETMASK
fi


if [ -f /mnt/context/$ROOT_PUBKEY ]; then
	cat /mnt/context/$ROOT_PUBKEY >> /root/.ssh/authorized_keys
fi

if [ -n "$USERNAME" ]; then
	adduser -s /bin/bash -D $USERNAME
	if [ -f /mnt/context/$USER_PUBKEY ]; then
		mkdir -p /home/$USERNAME/.ssh/
		cat /mnt/context/$USER_PUBKEY >> /home/$USERNAME/.ssh/authorized_keys
		chown -R $USERNAME /home/$USERNAME/.ssh
		chmod -R 600 /home/$USERNAME/.ssh
	fi
fi


# Mounting the data repository, 
# Before check that nfs-client is installed
#
NFS_CLIENT=`which mount.nfs`

if [ -z ${NFS_CLIENT} ]; then

    echo "nfs-client is not available"
    exit 1

else    
    if [ -z ${VISHNU_CLOUD_NFS_SERVER} ] || [ -z ${VISHNU_CLOUD_NFS_MOUNT_POINT} ]; then

	echo "The data repository parameters are not suitable set"
	exit 1
    else
	mkdir -p ${VISHNU_CLOUD_NFS_MOUNT_POINT}
	mount -t nfs -o rw,rsize=8192,wsize=8192 \
        ${VISHNU_CLOUD_NFS_SERVER}:${VISHNU_CLOUD_NFS_MOUNT_POINT} ${VISHNU_CLOUD_NFS_MOUNT_POINT}
    fi
fi
