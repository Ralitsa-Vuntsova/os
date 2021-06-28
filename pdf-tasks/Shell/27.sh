#!/bin/bash

[ $# -eq 2 ] || { echo "Usage: ${0} <a.csv> <b.csv>"; exit 1; }

a="${1}"
b="${2}"

if [ ! -f "${a}" ] ;then
    echo "Invalid file"
    exit 2
elif [ ! -r "${a}" ]; then
	echo "File not readable"
	exit 3
fi

[ -e "${b}" ] || touch "${b}"
if [ ! -w "${b}" ]; then
	 exit 4
fi

while read LINE; do
    CUTLINE="$(echo "$LINE" | cut -d',' -f2-)"
    if egrep -q ",${CUTLINE}$" "${b}"; then
        continue;
    else
        echo "${LINE}" >> "${b}"
    fi
done < <( cat "${a}" | sort -t',' -n -k 1)
