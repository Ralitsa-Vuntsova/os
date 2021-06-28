#!/bin/bash

if [ $# -eq 0 ] || [ $# -gt 2 ]; then
	echo "Invalid number of arguments!"
	exit 1
fi

FLAG=0

if [ "${1}" = -l ]; then
	FLAG=1
fi

CURR_DIR=$(pwd)

if [ "${FLAG}" -eq 1 ]; then
	find "${BACKUP_DIR}" -printf "%f\n" | xargs basename -a | cut -d "_" -f 1,2 | tr "_" " "
elif [ "${FLAG}" -eq 0 ] && [ $# -eq 1 ]; then
	COUNT=$(find "${BACKUP_DIR}" | grep "${1}" | wc -l)
	if [ "${COUNT}" -gt 1 ]; then
		find "${BACKUP_DIR}" | grep "${1}" | sort | grep -n "${1}"
	   	read -p "Choose a number: " NUMBER
		find "${BACKUP_DIR}" | grep "${1}" | sort | grep -n "${1}" | egrep "^${NUMBER}" | cut -d ":" -f 2- | xargs gzip -d > "${CURR_DIR}/${1}"
		rm $(find "${BACKUP_DIR}" | grep "${1}" | sort | grep -n "${1}" | egrep "^${NUMBER}" | cut -d ":" -f 2- )
	else 
		find "${BACKUP_DIR}" | grep "${1}" | xargs gzip -d > "${CURR_DIR}/${1}"
		rm $(find "${BACKUP_DIR}" | grep "${1}")
	fi
elif [ "${FLAG}" -eq 0 ] && [ $# -eq 2 ]; then 
	if [ ! -d "${2}" ]; then
		echo "Second argument must be directory!"
		exit 2
	elif [ ! -r "${2}" ]; then
		echo "Directory is not readable!"
		exit 3
	fi

	COUNT=$(find "${BACKUP_DIR}" | grep "${1}" | wc -l)
    if [ "${COUNT}" -gt 1 ]; then         
		find "${BACKUP_DIR}" | grep "${1}" | sort | grep -n "${1}"
        read -p "Choose a number: " NUMBER
        find "${BACKUP_DIR}" | grep "${1}" | sort | grep -n "${1}" | egrep "^${NUMBER}" | cut -d ":" -f 2- | xargs gzip -d > "${2}/${1}"
		rm $(find "${BACKUP_DIR}" | grep "${1}" | sort | grep -n "${1}" | egrep "^${NUMBER}" | cut -d ":" -f 2-)
	else
		find "${BACKUP_DIR}" | grep "${1}" | xargs gzip -d > "${2}/${1}"
		rm $(find "${BACKUP_DIR}" | grep "${1}")
	fi
fi
