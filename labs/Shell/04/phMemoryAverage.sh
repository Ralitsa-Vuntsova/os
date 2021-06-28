#!/bin/bash

COUNTER=$(ps --no-headers --group root | wc -l)
S=$(ps --no-headers -o 'pid group drs' --group root | awk '{SUM+=$3} END {print SUM}')

if [ "${COUNTER}" -eq 0 ]; then
	exit 1
fi

echo "$(( S / COUNTER ))"
