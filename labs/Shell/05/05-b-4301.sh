#!/bin/bash

if [ $# -ne 3 ]
then
	echo "Wrong number of arguments!"
	exit 1
fi

if [ ! -f "${1}" ]; then
	echo "Argument must be a file!"
	exit 2
fi

if [ ! -w "${1}" ]; then
	echo "Addressbook file not writtable!"
	exit 3
fi

FILE="${1}"
NAME="${2}"
NICK="${3}"
UNAME=$(cat /etc/passwd | grep "${NAME}" | cut -d ":" -f 1)

if [ -z "${UNAME}" ]; then
	echo "No user found!"
	exit 4
fi

echo "${NICK} ${UNAME}" >> "${FILE}"
