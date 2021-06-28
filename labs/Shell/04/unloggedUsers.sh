#!/bin/bash

comm -2 -3 <(cat /etc/passwd | cut -d ':' -f 1 | sort | uniq) \
	<(who | cut -d ' ' -f 1 | sort | uniq)
