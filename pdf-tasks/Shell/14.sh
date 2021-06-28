#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Invalid number of arguments!"
	exit 1
fi

[ $(id -u) -eq 0 ] || exit 0

egrep -q "^[+-]?[0-9]+$" <(echo "${1}")
if [ $? -ne 0 ]; then
	echo "Invalid argument!"
	exit 2
fi

USERS=$(mktemp)

ps -e -o "user=" | sort | uniq > USERS

while read USER; do
	USER_TOTAL_RSS=0
	while read PID RSS; do
		USER_TOTAL_RSS=$(($USER_TOTAL_RSS+$RSS))
		LAST_PID=${PID}
	done <(ps -u "${USER}" -o "pid= rss=" | sort -n -k 2)

	echo "${USER_TOTAL_RSS} total rss for user ${USER}"

	if [ $USER_TOTAL_RSS -gt "${1}" ]; then
		kill -15 $LAST_PID
		sleep 1
		kill -9 $LAST_PID
	fi
done < USERS

rm -- USERS
