#!/bin/bash

if [ $# -eq 0 -o $# -gt 2 ]; then
	echo "There must be 1 or 2 arguments!"
	exit 1
fi

if [ ! -d $1 ]; then
	echo "Argument must be a directory!"
	exit 2
elif [ ! -r $1 ]; then
	echo "Directory must be readable!"
	exit 3
fi

DIR="${1}"
FLAG="${2}"

if [ "${FLAG}" = -a ]; then
	while read line; do
		if [ -f "${line}" ]; then
			echo "$(basename "${line}") ($(wc -c "${line}" | cut -d " " -f 1) bytes)"
		elif [ -d "${line}" ]; then
			echo "$(basename "${line}") ($(find "${line}" | wc -l) entries)"
		fi
	done < <(find "${DIR}" 2> /dev/null)
else
	while read line; do
		if [ -f "${line}" ]; then
			echo "$(basename "${line}") ($(wc -c "${line}" | cut -d " " -f 1) bytes)"
		elif [ -d "${line}" ]; then
			echo "$(basename "${line}") ($(find "${line}" | wc -l) entries)"	
		fi
	done < <(find "${DIR}" -not -name ".*"  2> /dev/null)
fi
