#!/bin/bash

read -p "Please, enter username: " USER;

who | grep "${USER}" | wc -l
