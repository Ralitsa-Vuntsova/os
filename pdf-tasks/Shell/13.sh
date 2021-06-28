#!/bin/bash

if [ $# -ne 1 ]; then
	echo "There must be at least and only one argument!"
	exit 1
fi

ARG="${1}"

if [ ! -d ${ARG} ]; then
	echo "Argument must be a directory!"
	exit 2
fi

if [ ! -r ${ARG} ]; then
	echo "Directory is not readable!"
	exit 3
fi

echo "Symlinks in directory ${ARG}:"
find "${ARG}" -maxdepth 1 -mindepth 1 -type l -exec file {} \; 2> /dev/null | grep -v broken | cut -d ":" -f 1

echo "Broken symlinks in directory ${ARG}:"
find "${ARG}" -maxdepth 2 -mindepth 1 -type l -exec file {} \; 2> /dev/null | grep broken | cut - d ":" -f 1
