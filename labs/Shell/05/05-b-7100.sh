#!/bin/bash

if [ $# -ne 2 ]; then
	echo "There must be 2 arguments!"
	exit 1
fi

DIR="${1}"
NUM="${2}"

function validate_num {
	egrep -q '^[+-]?[0-9]+$' <(echo "${1}")
}

if [ ! -d  "${DIR}" ]; then
	echo "First argument must be directory!"
	exit 2
elif [ ! -r "${DIR}" ]; then
	echo "Directory is not readable!"
	exit 3
fi

if validate_num "${NUM}"; then
	find "${DIR}" -type f -size +"${NUM}"c -printf "%f\n"
fi
