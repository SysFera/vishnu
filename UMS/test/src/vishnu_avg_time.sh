#!/bin/bash

bench_executable_vishnu_name=$1
vishnu_binary_dir=$2
vishnu_executable_name=$3
bench_ssh_name=$4
ssh_prog_name=$5

vishnu_connect=$vishnu_binary_dir"/vishnu_connect "
vishnu_test_prog=$vishnu_binary_dir"/"$vishnu_executable_name

VISHNU_BENCH=$bench_executable_vishnu_name" "$vishnu_connect" "$vishnu_test_prog

VISHNU_TIMES_FILENAME=~/VishnuProject/vishnu/UMS/test/src/bench/$vishnu_executable_name"-time.txt"
SSH_FILENAME=~/VishnuProject/vishnu/UMS/test/src/bench/"ssh-local-host-"$ssh_prog_name"-time.txt" 
rm $VISHNU_TIMES_FILENAME
rm $SSH_FILENAME

     echo "======== Testing with " $vishnu_executable_name "=========="
     for ((J=0 ; J < 10 ; J++)); do
           # echo -n "$I " >> $FILENAME
	    echo Test number  $J
           $VISHNU_BENCH | grep  "vishnu_time = " | awk '{print $4}' >> $VISHNU_TIMES_FILENAME 
           $bench_ssh_name $ssh_prog_name | grep  "ssh_time = " | awk '{print $4}' >> $SSH_FILENAME
     done
