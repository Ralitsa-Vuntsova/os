#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Invalid number of arguments. Usage: $0 <path/to/repo /path/to/package>"
	exit 1
fi

REPO="${1}"
PACKAGE="${2}"

if [ ! -d "${REPO}" || ! -w "${REPO}" ]; then
	echo "First argument must be directory and it must be writtable"
	exit 2
fi

if [ ! -d "${PACKAGE}" || ! -r "${PACKAGE}" ]; then
	echo "Second argument must be directory and it must be readable"
	exit 3
fi

VERSION=$(find "${PACKAGE}" -name "version" | xargs cat)
tar -cvf "${REPO}/packages/package.tar" $(find "${PACKAGE}")
xz "${REPO}"/packages/package.tar 
HASH=$(sha256 "${REPO}"/packages/package.tar.xz | cut -d " " -f 1)
mv ${REPO}/packages/package.tar.xz ${REPO}/packages/"${HASH}".tar.xz

echo "${VERSION} ${HASH}" >> "${REPO}"/db

HASH=$(sha256 package)
