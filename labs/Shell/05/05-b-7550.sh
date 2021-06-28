#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Invalid number of arguments!"
	exit 1
fi

USER="${1}"

if [ $(id -u "${USER}") -eq 1 ]; then
	echo "Invalid username!"
	exit 2
fi
 
PS_CNT=0

while read PID; do
	kill -15 "${PID}"
	sleep 1
	kill -9 "${PID}"
	PS_CNT=(($PS_CNT+1))
done < <(ps -u "${USER}" -o pid=)

echo "${PS_CNT} killed processes"
