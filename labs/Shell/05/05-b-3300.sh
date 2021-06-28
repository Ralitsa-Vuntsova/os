#!/bin/bash

read -p "Input 3 files: " FIRST SECOND THIRD

if [ ! -f "${FIRST}" ] || [ ! -f "${SECOND}" ]; then
	echo "First and second arguments must be files."
	exit 1
fi

if [ ! -r "${FIRST}" ] || [ ! -r "${SECOND}" ]; then
	echo "First and second arguments must be readable files."
	exit 2
fi

paste "${FIRST}" "${SECOND}" | sort -o "${THIRD}"

