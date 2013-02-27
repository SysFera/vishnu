#!/bin/bash

if [ $# -lt 2 ];then
    echo "$0 <vishnu admin user> <vishnu machine id>"
    echo "  - vishnu admin user: user with administrative rights in vishnu"
    echo "  - list of vishnu machine ids of the machines on wich this script is executed"
    exit 1
fi

vishnuRunning=`ps aux | grep umssed | grep -v grep | wc -l`
if (( $vishnuRunning == 0 )); then
    echo "Vishnu is not running!"
    echo "Exiting!"
    exit 1
fi

user=$1
machines=${@:2}
SSHSOCKETTEMPLATE="/tmp/ssh-%r@%h:%p"

echo "Creating master ssh, you'll be asked for the \"admin\" password"

TMPLA=`mktemp`
vishnu_connect -u ${user} 2>/dev/null

for i in ${machines}; do
    # get machine hostname
    echo "## Creating tunnel for $i"
    vishnu_machine=$i
    vishnu_list_machines -i ${vishnu_machine} 2>/dev/null | grep Name| awk '{print $2}' > ${TMPLA}
    machineHost=`cat $TMPLA`

    # get all local accounts and create ssh masters
    vishnu_list_local_accounts -a -i ${vishnu_machine} 2>/dev/null | grep ${vishnu_machine} | awk '{print $3}'| sort | uniq > ${TMPLA}
    for i in `cat ${TMPLA}`;do
	nohup ssh ${i}@${machineHost} -N -M -S ${SSHSOCKETTEMPLATE}  &
	pid=`echo $!`
	echo "PID of generated master ssh: $pid"
    done
done

# exit
vishnu_close 2>/dev/null
rm -f ${TMPLA}

