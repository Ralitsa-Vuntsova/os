#!/bin/bash

if [ $# -ne 1 ]; then
	echo "There must be one argument!"
	exit 1
fi

while read line; do
	if [ "${1}" == ${line} ]; then
		echo "Found"
		exit 2
	fi
	sleep 1
done < <(users | sed -E 's/ /\n/g')
