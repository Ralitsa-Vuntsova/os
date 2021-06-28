#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Invalid number of arguments. Usage: $0 <logfile>"
	exit 1
fi

FILE="${1}"

if [ ! -f "${FILE}" ]; then
	echo "First argument must be a file"
	exit 2
fi

FIRST_SITE=$(cat "${FILE}" | cut -d " " -f 2 | sort | uniq -c | sort -rn | head -n1)
SECOND_SITE=$(cat "${FILE}" | cut -d " " -f 2 | sort | uniq -c | sort -rn | head -n2 | tail -n 1)
THIRD_SITE=$(cat "${FILE}" | cut -d " " -f 2 | sort | uniq -c | sort -rn | head -n3 | tail -n 1)

FS_HTTP2_CNT=$(cat "${FILE}" | grep "${FIRST_SITE}" | grep "HTTP/2.0" | wc -l)
SS_HTTP2_CNT=$(cat "${FILE}" | grep "${SECOND_SITE}" | grep "HTTP/2.0" | wc -l)
TS_HTTP2_CNT=$(cat "${FILE}" | grep "${THIRD_SITE}" | grep "HTTP/2.0" | wc -l)

FS_nonHTTP2_CNT=$(cat "${FILE}" | grep "${FIRST_SITE}" | grep -v "HTTP/2.0" | wc -l)
SS_nonHTTP2_CNT=$(cat "${FILE}" | grep "${SECOND_SITE}" | grep -v "HTTP/2.0" | wc -l)
TS_nonHTTP2_CNT=$(cat "${FILE}" | grep "${THIRD_SITE}" | grep -v "HTTP/2.0" | wc -l)

FS_CODES=$(mktemp)
cat "${FILE}" | grep "${FIRST_SITE}" | sed -E 's/(.*)+HTTP//' | cut -d " " -f 2 | awk '$1>302 {print $1}' > "${FS_CODES}"

FS_WITH_CODES=$(mktemp)
while read line; do
	cat "${FILE}" | cat "${FIRST_SITE}" | grep "${line}" >> "${FS_WITH_CODES}"
done < "${FS_CODES}"

FS_TOP5=$(mktemp)
cat "${FS_WITH_CODES}" | cut -d " " -f 1 | sort | uniq -c | sort -rn | head -n 5 > "${FS_TOP5}"

SS_CODES=$(mktemp)
cat "${FILE}" | grep "${SECOND_SITE}" | sed -E 's/(.*)+HTTP//' | cut -d " " -f 2 | awk '$1>302 {print $1}' > "${SS_CODES}"

SS_WITH_CODES=$(mktemp)
while read line; do
    cat "${FILE}" | cat "${SECOND_SITE}" | grep "${line}" >> "${SS_WITH_CODES}"
done < "${SS_CODES}"

SS_TOP5=$(mktemp)
cat "${SS_WITH_CODES}" | cut -d " " -f 1 | sort | uniq -c | sort -rn | head -n 5 > "${SS_TOP5}"

TS_CODES=$(mktemp)
cat "${FILE}" | grep "${THIRD_SITE}" | sed -E 's/(.*)+HTTP//' | cut -d " " -f 2 | awk '$1>302 {print $1}' > "${TS_CODES}"

TS_WITH_CODES=$(mktemp)
while read line; do
    cat "${FILE}" | cat "${THIRD_SITE}" | grep "${line}" >> "${TS_WITH_CODES}"
done < "${TS_CODES}"

TS_TOP5=$(mktemp)
cat "${TS_WITH_CODES}" | cut -d " " -f 1 | sort | uniq -c | sort -rn | head -n 5 > "${TS_TOP5}"

echo "${FIRST_SITE} HTTP/2.0: ${FS_HTTP2_CNT} non-HTTP/2.0: ${FS_nonHTTP2_CNT}"
cat ${FS_TOP5}
echo "${SECOND_SITE} HTTP/2.0: ${SS_HTTP2_CNT} non-HTTP/2.0: ${SS_nonHTTP2_CNT}"
cat ${SS_TOP5}
echo "${THIRD_SITE} HTTP/2.0: ${TS_HTTP2_CNT} non-HTTP/2.0: ${TS_nonHTTP2_CNT}"
cat ${TS_TOP5}

rm -- "${FS_CODES}"
rm -- "${FS_WITH_CODES}"
rm -- "${FS_TOP5}"
rm -- "${SS_CODES}"
rm -- "${SS_WITH_CODES}"
rm -- "${SS_TOP5}"
rm -- "${TS_CODES}"
rm -- "${TS_WITH_CODES}"
rm -- "${TS_TOP5}"
