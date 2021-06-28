#!/bin/bash

[ $# -eq 3 ] || { echo "Usage: $0 <dirname> <dirname> <string>"; exit 1; }

src="${1}"
dst="${2}"
str="${3}"

[ -d "${src}" ] || { echo "Source is not a directory!"; exit 2; }
[ -d "${dst}" ] || { echo "Destination is not a directory!"; exit 3; }
[ -r "${src}" -a -w "${src}" ] || { echo "Source does not have \"r\" or \"w\" perm!"; exit 4; }
[ -r "${dst}" -a -w "${dst}" ] || { echo "Destination does not have \"r\" or \"w\" perm!"; exit 5; }
[ -n "${str}" ] || { echo "String is empty!"; exit 6; }

[ $(id -u) -eq 0 ] || { echo "Script not run as root (do nothing)!"; exit 0; }

dst_content=$(find "${dst}" 2>/dev/null | wc -l)
[ "${dst_content}" -eq 0 ] || { echo "Destination dir must not have other files!"; exit 7; }

while read file; do
   real_dir_base_name="$(echo "${file}" | sed -E "s/${src}\///")"
   mkdir -p "${dst}/$(dirname "${real_dir_base_name}")"
   mv "${file}" "${dst}/$(dirname "${real_dir_base_name}")"
done < <(find "${src}" -type f -name "*${str}*" 2>/dev/null)
