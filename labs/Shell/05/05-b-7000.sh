#!/bin/bash

if [ $# -eq 0 ]; then
	echo "There must be at least one file!"
	exit 1
fi

read -p "Enter string: " STRING

while [ $# -ne 0 ]; do
	i="${1}"
	if [ ! -f "${i}" ]; then 
		echo "Not a file!"
	elif [ ! -r "${i}" ]; then
		echo "File is not readable!"
	else
		MATCHES=$(cat "${i}" | grep "${STRING}" | wc -l)
		echo "Matches of "${STRING}" in "${i}" are "${MATCHES}"!"
	fi
	shift 1
done
