#!/bin/bash

ps -e --sort=start_time -o cmd --no-header | head -n 1
