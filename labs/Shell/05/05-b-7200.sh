#/bin/bash

if [ $# -eq 0 ]; then
	echo "There must be at least one argument!"
	exit 1
fi

for i; do
	if [ -f "${i}" ]; then
		if [ -r "${i}" ]; then
			echo "$(basename "${i}") is readable"
		else
			echo "$(basename "${i}") is not readable"
		fi
	elif [ -d "${i}" ]; then
		COUNT_FILES=$(find "${i}" -maxdepth 1 -type f | wc -l)
		echo "$(find "${i}" -maxdepth 1 -type f -size -"${COUNT_FILES}"c | xargs basename -a)"
	else
		echo "$(basename "${i}") is not a file/directory name!"
	fi
done
