#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Invalid number of arguments. Usage: $0 SRC DST"
	exit 1
fi

SRC="${1}"
DST="${2}"

if [ ! -d "${SRC}" || ! -r "${SRC}" ]; then
	echo "First argument must be a directory and it must be readable "
	exit 2
fi

mkdir -p "${DST}/images"
mkdir -p "${DST}/by-date"
mkdir -p "${DST}/by-album"
mkdir -p "${DST}/by-title"

JPG=$(mktemp)
find "${SRC}" -type f -name "*.jpg" > "${JPG}"

while read line; do
	TITLE=$(basename "${line}" | cut -d "." -f 1 | sed -E 's/[(][^)]*[)]//g' | awk '$1=$1')
	A=$(basename "${line}" | cut -d "." -f 1 | awk -F "[()]" '{ for (i=2; i<NF; i+=2) print $i }' | tail -n 1 | awk '$1=$1')
	if [ -z "${A}" ]; then
		ALBUM="misc"
	else
		ALBUM="${A}"
	fi
	DATE=$(stat -c "%y" "${line}" | cut -d " " -f 1)
	HASH=$(sha256 "${line}" | cut -c 1-16)

	cp "${line}" "${DST}/images/${HASH}.jpg"
	ln -s "${DST}/images/${HASH}.jpg" "${DST}/by-date/${DATE}/by-album/${ALBUM}/by-title/${TITLE}.jpg"
	ln -s "${DST}/images/${HASH}.jpg" "${DST}/by-date/${DATE}/by-title/${TITLE}.jpg"
	ln -s "${DST}/images/${HASH}.jpg" "${DST}/by-album/${ALBUM}/by-date/${DATE}/by-title/${TITLE}.jpg"
	ln -s "${DST}/images/${HASH}.jpg" "${DST}/by-album/${ALBUM}/by-title/${TITLE}.jpg"
	ln -s "${DST}/images/${HASH}.jpg" "${DST}/by-title/${TITLE}.jpg"
done < "${JPG}"

rm -- "${JPG}"
