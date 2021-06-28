#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Invalid number of arguments. Usage: $0 <file> <directory>"
	exit 1
fi

FILE="${1}"
DIR="${2}"

if [ ! "${FILE}" == "bar.csv" ]; then
	echo "The name of the file must be bar.csv"
	exit 2
fi

if [ ! -d ${DIR} || ! -r ${DIR} ]; then
	echo "Second argument must be a directory and it must be readable"
	exit 3
fi

echo "hostname,phy,vlans,hosts,failover,VPN-3DES_AES,peers,VLAN Trunk Ports,license,SN,key" > "${FILE}"

LOGFILES=$(mktemp)
find ${DIR} -type f -name "*.log" > "${LOGFILES}"

while read line; do
	HOSTNAME=$(basename "${line}" | cut -d "." -f 1)
	PHY=$(cat "${line}" | head -n3 | tail -n1 | cut -d ":" -f 2 | cut -c 2-)
	VLANS=$(cat "${line}" | head -n4 | tail -n1 | cut -d ":" -f 2 | cut -c 2-)
	HOSTS=$(cat "${line}" | head -n5 | tail -n1 | cut -d ":" -f 2 | cut -c 2-)
	FAILOVER=$(cat "${line}" | head -n6 | tail -n1 | cut -d ":" -f 2 | cut -c 2-)
	VPN=$(cat "${line}" | head -n7 | tail -n1 | cut -d ":" -f 2 | cut -c 2-)
	PEERS=$(cat "${line}" | head -n8 | tail -n1 | cut -d ":" -f 2 | cut -c 2-)
	VLANT=$(cat "${line}" | head -n9 | tail -n1 | cut -d ":" -f 2 | cut -c 2-)
	LICENSE=$(cat "${line}" | head -n11 | tail -n 1 | cut -d " " -f 6)
	SN=$(cat "${line}" | head -n13 | tail -n1 | cut -d ":" -f 2 | cut -c 2-)
	KEY=$(cat "${line}" | head -n15 | tail -n1 | cut -d ":" -f 2 | cut -c 2-)
	echo "${HOSTNAME},${PHY},${VLANS},${HOSTS},${FAILOVER},${VPN},${PEERS},${VLANT},${LICENSE},${SN},${KEY}" >> "${FILE}" 
done < "${LOGFILES}"

rm -- "${LOGFILES}"
