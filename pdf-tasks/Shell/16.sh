#!/bin/bash

if [ $# -ne 3 ]; then
	echo "Invalid number of arguments. Usage ./script.sh  <file><string1><string2>"
	exit 1
fi

if [ ! -f "${1}" ]; then
	if [ ! -r "${1}" ]; then
		exit 2
	fi
fi

if [ ! -n "${2}" ]; then
	exit 3
fi

if [ ! -n "${3}" ]; then
	exit 4
fi

FILE="${1}"
KEY1="${2}"
KEY2="${3}"

VAL1=$(cat "${FILE}" | grep "${KEY1}=" | cut -d "=" -f 2)
VAL2=$(cat "${FILE}" | grep "${KEY2}=" | cut -d "=" -f 2)
echo "VAL1: ${VAL1}"
echo "VAL2: ${VAL2}"

if [ ! -n "${VAL1}" || ! -n "${VAL2}" ]; then
	exit 5
fi

while read line; do
	SAME+=$(grep -o "${line}" <(echo "$VAL1"))
done < <(echo "${VAL2}" | grep -o .)
echo "SAME: ${SAME}"

NEW_VAL=$(sed -E "s/[${SAME}]//g" <(echo "${VAL2}"))
echo $NEW_VAL

sed -i -E "s/${KEY2}=${VAL2}/${KEY2}=${NEW_VAL}/" ${FILE}
