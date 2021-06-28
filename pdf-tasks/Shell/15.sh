#!/bin/bash

[ $(id -u) -eq 0 ] || exit 0

while read USER HOME; do
	if [ ! -d "${HOME}" || ! -w "${HOME}" ]; then
		echo "${USER}"
	fi
done < <(cat /etc/passwd | awk -F ":" '{print $1, $6}')
