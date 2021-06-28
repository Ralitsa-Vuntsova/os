#!/bin/bash

if [ $# -ne 2 ]
then
	echo "There must be 2 arguments!"
	exit 1
fi

read -p "Write a message: " MSG
USER=$(cat "${1}" | grep "${2}" | cut -d " " -f 2)
echo "${MSG}" | write "${USER}"
