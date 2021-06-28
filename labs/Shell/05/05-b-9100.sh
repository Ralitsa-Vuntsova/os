#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Invalid number of arguments!"
	exit 1
fi

SOURCE="${1}"
DESTINATION="${2}"

while read line; do
	mkdir -p "${DESTINATION}"/"${line}"
	find "${SOURCE}" -type f -name "*.${line}" 2>/dev/null -print0 | xargs -0 -I {} mv {} "${DESTINATON}/${line}"
done < <(find "${SOURCE}" -type f -regex ".*" | cut -d "." -f 2 | sort | uniq)
