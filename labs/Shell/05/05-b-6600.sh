#/bin/bash

if [ $# -ne 1 ]; then
	echo "Invalid number of arguments!"
	exit 1
fi

if [ ! -d "${1}" ]; then
	echo "Argument must be a directory!"
	exit 2
fi
if [ ! -r "${1}" ]; then
	echo "Directory must be readable!"
	exit3
fi

HASHES=$(mktemp)

find "${1}" -maxdepth 1 -type f -exec md5sum {} \; | sort > "${HASHES}"

DUPLICATES=$(mktemp)
cat "${HASHES}" | awk '{print $1}' | uniq -d > "${DUPLICATES}"

while read DUPLICATED_HASH; do
	grep "${DUPLICATED_HASH}" "${HASHES}" | awk '{print $2}' | sort | head -n +1 | xargs rm
done < "${DUPLICATES}"

rm "${HASHES}"
rm "${DUPLICATES}"
