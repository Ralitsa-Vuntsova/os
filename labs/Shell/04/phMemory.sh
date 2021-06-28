#!/bin/bash

ps -e --group root -o "drs" | awk '{sum += $1} END {print sum}'
