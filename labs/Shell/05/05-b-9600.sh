#!/bin/bash

if [ $# -eq 0 ]; then
	echo "Invalid number of arguments!"
fi

FLAG=0

if [ "${1}" = -r ]; then
	FLAG=1
	shift 1
fi

function timestamp {
	date +"%Y-%m-%d-%H-%M-%S"
}

function archive_and_save {
	SRC=$1
	POSTFIX=$2
	TIMESTAMP=$(timestamp)
	tar -czf "${BACKUP_DIR}/${SRC}_${POSTFIX}_${TIMESTAMP}.tgz" "${SRC}"
}

function compress_and_save {
	SRC=$1
	TIMESTAMP=$(timestamp)
	gzip -c "${SRC}" > "${BACKUP_DIR}/${SRC}_${TIMESTAMP}.gz"
}

for i; do
	if [ -d "${i}" ]; then
		DIR_CONTENT=$(find "${i}" -mindepth 1 -maxdepth 1 2>/dev/null | wc -l)
		if [ "${DIR_CONTENT}" -eq 0 ]; then
			archive_and_save "${i}" "empty_dir"
			rmdir "${i}"
		elif [ "${FLAG}" -eq 1 ];then
			archive_and_save "${i}" "full_dir"
			rm -R "${i}"
		else
			echo "error ${i} is not empty, will not delete"
		fi
	elif [ -f "${i}" ]; then
		compress_and_save "${i}"
		rm "${i}"
	fi
done
