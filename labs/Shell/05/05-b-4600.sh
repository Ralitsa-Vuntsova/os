#!/bin/bash

if [ $# -ne 3 ]; then
	echo "Wrong number of arguments!"
	exit 10
fi

for i; do
	if ! [[ "$i" =~ ^[+-]?[0-9]+$ ]]; then
		exit 3
	fi
done

if [ $2 -gt $3 ]; then
	exit 2
fi

if [ $1 -lt $2 -o $1 -gt $3 ]; then
	exit 1
fi

if [ $1 -gt $2 -a $1 -lt $3 ]; then
	exit 0
fi
