#!/bin/bash

if [ $# -eq 0 || $# -gt 2 ]; then
	echo "Invalid number of arguments"
	exit 1
fi

NUM=""

if [ $# -eq 2 ]; then
	NUM="${2}"
fi

DIR="${1}"

if [ ! -d "${DIR}" ]; then
	echo "Not a valid directory name!"
	exit 2
elif [ ! -r "${DIR}" ]; then
	echo "Directory is not readable!"
	exit 3
fi

function count_hardlinks {
	find "${1}" -samefile "${2}" 2>/dev/null | wc -l
}

function is_broken_symlink {
	file "${1}" | grep -q 'broken'
}

if [ -n "${NUM}" ]; then
	while read f; do
		if [ $(count_hardlinks "${DIR}" "${f}") -ge "${NUM}" ]; then
			echo "${f}"
		fi
	done < <(find "${DIR}" -type f 2>/dev/null)
else
	while read l; do
		if is_broken_symlink "${l}"; then
			echo "${l}"
		fi
	done < <(find "${DIR}" -type l 2>/dev/null)
fi
