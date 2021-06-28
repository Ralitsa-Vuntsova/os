#!/bin/bash

ps -e --no-headers -o "tty pid comm" | egrep "^\?" | tr -s " " | cut -d " " -f 3 | sort | uniq
