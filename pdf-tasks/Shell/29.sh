#!/bin/bash

[ $# -ne 0 ] || { echo "Usage: ${0} [-n N] FILE1..."; exit 1; }

N=10
CUR_ARG=1

if [ "${1}" = "-n" ] && [ $# -gt 2 ]; then
    N="${2}"
    CUR_ARG=3
fi

ALL_ARG="$#"

buf=$(mktemp)

while [ $CUR_ARG -le $ALL_ARG ];do
   CUR_FILE="${CUR_ARG}"
   if [ -f ${CUR_FILE} ] && [ -r ${CUR_FILE} ]; then
	id=$(echo "${CUR_FILE}" | cut -d "." -f 1)
	cat "$CUR_FILE" | tail -n "${N}" | sed -E "s/([^ ]+ [^ ]+ )(.*)/\1${id} \2/" >> "$buf"
   fi
   CUR_ARG=$(($CUR_ARG + 1))
done

cat "$buf" | sort

rm -- "${buf}"
