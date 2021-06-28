#!/bin/bash

ps -e -o 'pid command' --sort=vsz | tail -n 1
