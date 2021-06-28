#!/bin/bash

if [ $# -gt 3 -o $# -eq 0 ]; then
	echo "Invalid number of arguments"
	exit 1
fi

if [ $# -eq 1 ] && [ ! "${1}" = -r ] && [ ! "${1}" = -g ] && [ ! "${1}" = -b ]; then
	echo "${1}"
	exit 2
fi

FLAG="${1}"
STRING="${2}"

if [ "${FLAG}" = -r ]; then
	echo -e "\033[0;31m ${STRING}"
elif [ "${FLAG}" = -g ]; then
	echo -e "\033[0;32m ${STRING}"
elif [ "${FLAG}" = -b ]; then
	echo -e "\033[0;34m ${STRING}"
else
	echo "Unknown color!"
fi

echo -e '\033[0m'
