#!/bin/bash

[ $(id -u) -eq 0 ] || { echo "Script ${0} is not executed as root."; exit 2; }

TOTAL_ROOT_RSS="$(ps -u "root" -o rss= | awk '{s+=$1} END {print s}')"

while read USER_UID HOME; do
	if [ "${USER_UID}" -eq 0 ]; then
		continue
	fi
	
	[ ! -d "${HOME}" ] || [ "$(stat -c "%u" "${HOME}")" != "${USER_UID}" ] || [ "$(stat -c "%A" "${HOME}"| cut -c3)" != "w" ] || continue

	TOTAL_USER_RSS="$(ps -u "${USER_UID}" -o rss= | awk '{s+=$1} END {print s}')"
	
	if [ "${TOTAL_ROOT_RSS}" -lt "${TOTAL_USER_RSS}" ]; then
		killall -u "${USER_UID}" -SIGTERM
		sleep 2
		killall -u "${USER_UID}" -SIGKILL
	fi
done < <(cut -d':' -f3,6 /etc/passwd)
