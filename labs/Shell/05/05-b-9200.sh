#!/bin/bash

if [ $# -eq 0 ]; then
	echo "Invalid number of arguments!"
	exit 1
fi

if [ "${1}" = -r ]; then
	FLAG=1
	shift 1
fi

export LOG_FILE=$(mktemp)

for i; do
	if [ -f "${i}" ]; then
		rm "${i}"
		echo "[$(date +"%Y-%m-%d %H:%M:%S")] File ${i} removed!" >> "${LOG_FILE}"
	elif [ -d "${i}" ]; then
		DIR_CONTENTS=$(find "${i}" -maxdepth 1 | wc -l)
		if [ "${DIR_CONTENTS}" -eq 0 ]; then
			rmdir "${i}"
			echo "[$(date +"%Y-%m-%d %H:%M:%S")] Directory ${i} removed!" >> "${LOG_FILE}"
		elif [ "${DIR_CONTENTS} -ne 0 " ]; then
			if [ "${FLAG}" -eq 1 ]; then
				rm -R "${i}"
				echo "[$(date +"%Y-%m-%d %H:%M:%S")] Directory ${i} removed recursively!" >> "${LOG_FILE}"
			else 
				echo "Directory not empty!"
			fi
		fi
	fi
done

echo "Log file info: "
cat "${LOG_FILE}"
