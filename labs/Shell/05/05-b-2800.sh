#!/bin/bash

if [ "$#" -ne 1 ]
then
	echo "There must be only one parameter!"
elif [[ "$1" =~ ^[a-zA-Z0-9]+$ ]]
then
	echo "There are only letters and digits!"
else
	echo "There must be only letters and digits!"
fi
