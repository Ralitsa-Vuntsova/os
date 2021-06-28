#!/bin/bash

read -p "Type name of directory: " DIRECTORY

if [ ! -d "${DIRECTORY}" ]; then
	echo "You must give directory"
fi

echo "Files: $(find "${DIRECTORY}" -type f 2>/dev/null | wc -l)"
echo "Dirs: $(find "${DIRECTORY}" -type d 2>/dev/null | wc -l)"
