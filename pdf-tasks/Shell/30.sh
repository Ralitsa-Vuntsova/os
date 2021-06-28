#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Invalid number of arguments. Usage $0 <dir>"
	exit 1
fi

DIR="${1}"

if [ ! -d "${DIR}" || ! -r "${DIR}" ]; then
	echo "First argument must be a directory and it must be readable"
	exit 2
fi

FILES=$(mktemp)
find "${DIR}" -type f -name "[^_]+_report_[0-9]+.tgz$" > "${FILES}"
mkdir -p "${DIR}/extracted"

while read line; do
	tar -tf "${line}" | grep -q "meow.txt"
	if [ $? -eq 0 ]; then
		NAME=$(basename "${line}" | cut -d "_" -f 1)
		TIMESTAMP=$(basename "${line}" | cut -d "_" -f 3 | cut -d "." -f 1)
		tar -xzf "${line}" "meow.txt" > "${DIR}/extracted/${NAME}_${TIMESTAMP}"
	else
		continue
	fi
done < "${FILES}"

rm -- "${FILES}"
