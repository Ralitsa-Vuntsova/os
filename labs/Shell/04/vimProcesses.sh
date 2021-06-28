#!/bin/bash

ps -e -o 'user cmd' | grep 'vim' | awk '{print $1}' | sort | uniq -c | awk '$1>1 {print $2}'
