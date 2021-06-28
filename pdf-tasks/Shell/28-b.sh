#!/bin/bash

temp=$(mktemp)
data=$(mktemp)

cat | egrep "^[-+]?[0-9]+$" | sort -n > "${temp}"

[ "$(cat "$temp" | wc -l )" -ne 0 ] || exit 1

while read NUM; do
	SUM=0
    NUM_COPY="$NUM"
    NUM="$(echo $NUM | sed -E "s/-//" )"
    while [ "$NUM" -ne 0 ]; do
        REM=$(("${NUM}" % 10))
        SUM=$(("${SUM}" + "${REM}"))
        NUM=$(("${NUM}" / 10))
    done	

	echo "${NUM_COPY} ${SUM}" >> "${data}"
done < <(cat "${temp}")

BIGGEST_SUM=$(cat "${data}" | sort -n -t " " -k 2 | cut -d " " -f 2 | tail -n 1)
RESULT=$(cat "${data}" | egrep "${BIGGEST_SUM}$" | sort -n -t " " -k 1 | head -n 1 | cut -d " " -f 1)

echo "${RESULT}"

rm -- "${temp}"
rm -- "${data}"
