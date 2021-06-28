#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Invalid number of arguemtns"
	exit 1
fi

USER="${1}"

ps -u "${USER}" -o "rss vsz" --no-header | sort -nr -k 2 | awk '{print $1/$2}'

