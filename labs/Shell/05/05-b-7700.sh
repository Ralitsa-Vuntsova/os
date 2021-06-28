#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Invalid number of arguments!"
	exit 1
fi

DIR="${1}"
NUM="${2}"

if [ ! -d "${DIR}" ]; then
	echo "Argument must be directory!"
	exit 2
elif [ ! -r "${DIR}" ]; then
	echo "Directory is not readable!"
	exit 3
fi

function validate_num {
	egrep -q "^[+-]?[0-9]+$" <(echo "${1}")
}

if validate_num "${NUM}"; then
	find "${DIR}" -type f -size +"${NUM}"c -printf "%s\n" | awk '{SUM+=$1} END {print SUM}'
fi
