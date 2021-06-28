#!/bin/bash

CNT=0

while read line; do
	if [ -x "${line}" ]; then
		CNT=$((CNT+1))
	fi
done < <(find $(echo "${PATH}" | tr ":" "\n"))

echo "Number of executable files is: ${CNT}"
