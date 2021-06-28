#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Invalid argument count"
	exit 1
fi

MAXCOUNT=0
CURRCOUNT=0

for i in $(echo "$(cat "$1" | grep -o "." | egrep "({|})" | tr "\n" " ")"); do
	if [ $i = "{" ]; then
		CURRCOUNT=$((CURRCOUNT+1))
		if [ $CURRCOUNT -gt $MAXCOUNT ]; then
			MAXCOUNT=$CURRCOUNT
		fi
	else
		CURRCOUNT=$((CURRCOUNT-1))
	fi
done

echo "The deepest nesting is ${MAXCOUNT} levels"
