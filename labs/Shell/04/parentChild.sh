#!/bin/bash

function count_children() {
ps --ppid "${1}" --no-header | wc -l
}

PID=$(ps --no-header -e -o "pid")

while read pid; do
	[[ $(count_children "${pid}") -ge $(count-children $(ps --pid ${pid} --no-header -o "ppid")) ]] && echo "${pid}"
done < "${PID}"
