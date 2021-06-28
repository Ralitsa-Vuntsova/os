#!/bin/bash

read -p "Enter full path name of file: " FILE_NAME
read -p "Enter string to match: " EXP

grep -sF "${EXP}" "${FILE_NAME}"
echo $?
