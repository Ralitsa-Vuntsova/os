#!/bin/bash

if [ "$#" -lt 1 -o "$#" -gt 2 ]; then
	echo "Wrong number of arguments!"
	exit 1
fi

if [ ! -d "${1}" ]; then
	echo "First argument should be directory!"
	exit 2
fi
DIR1="${1}"

if [ "$#" -eq 2 ]; then
	DIR2="${2}"
fi

if [ "$#" -eq 1 ]; then
	DIR2="$(date | cut -d " " -f 2,3,4 | tr " " "-")"
	mkdir "${DIR2}"
fi

if [ ! -d "${DIR2}" ]; then
	echo "Argument not a directory!"
	exit 3
fi

echo "Source directory: ${DIR1}"
echo "Destination directory ${DIR2}"

echo -e "\nChanged files in the last 45 minutes in source directory: "
find "${DIR1}" -type f -mmin -45 -exec echo {} \;
find "${DIR1}" -type f -mmin -45 -exec cp {} ${DIR2} \;
tar -cvf ${DIR2}.tar ${DIR2}
