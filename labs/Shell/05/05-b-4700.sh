#!/bin/bash

if [ $# -eq 1 ]; then
	DELIMITER=" "
elif [ $# -eq 2 ]; then
	DELIMITER="${2}"
else 
	echo "Invalid number of arguments!"
	exit 1
fi

NUM=$(echo "${1}")
REV_NUM=$(echo "${NUM}" | sed 's/./&\n/g' | tac)
CNT=1

for i in ${REV_NUM}; do
	NEW_NUM+="${i}"
	if [ $(($CNT%3)) -eq 0 ]; then
		NEW_NUM+="${DELIMITER}"
	fi
	CNT=$((CNT+1))
done

if [ $(( (${CNT}-1)%3 )) -eq 0 ]; then
	DEL_FIRST_DELI=1
fi  

REST_NUM=$(echo -e "${NEW_NUM}" | grep -o . | tac | tr -d "\n" ; echo)
if [ ${DEL_FIRST_DELI} -eq 1 ]; then
	REST_NUM=$(echo "${REST_NUM}" | cut -c2-)
fi

echo "${REST_NUM}"
