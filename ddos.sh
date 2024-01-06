#!/bin/bash

if [ "$#" -ne 3 ]; then
	echo "Usage: bash $0 <connections> <server_ip> <port>"
	exit 1
fi

connection_attempts=$1
server_ip=$2
port=$3

for ((i=1; i<=$connection_attempts; i++)); do
	(nc -z $server_ip $port &)
	echo "Connecting to $server_ip:$port - Attempt $i"
done

wait
