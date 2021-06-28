#!/bin/bash

temp=$(mktemp)

cat | egrep "^[-+]?[0-9]+$" | sort -n > "${temp}"

[ "$(cat "$temp" | wc -l )" -ne 0 ] || exit 1

MAX="$(cat "$temp" | tail -n 1)"
MIN="$(cat "$temp" | head -n 1)"

if [ $MAX -eq $MIN ];then
    echo ${MAX}
else
    ABS_MIN="$(echo $MIN | sed -E "s/-//")"
    ABS_MAX="$(echo $MAX | sed -E "s/-//")"
    if [ $ABS_MIN -eq $ABS_MAX ];then
        echo "$MIN"
        echo "$MAX"
    elif [ $ABS_MIN -lt $ABS_MAX ]; then
        echo "$MAX"
    else
        echo "$MIN"
    fi
fi

rm -- "${temp}"
