#/bin/bash

if [ $# -ne 1 ]; then
	echo "Invalid number of arguments"
	exit 1
fi

FOO="${1}"

[ $(id -u "${FOO}") -eq 0 ] || exit 2
[ $(id -u) -eq 0 ] || exit 3

PS=$(mktemp)
UNIQUE_USERS=$(mktemp)

ps -e -o user=,pid=,etimes= | sort -t' ' -k1 > "${PS}"

FOO_PS_CNT=$(awk '{ if ($1 == "${FOO}") ++cnt } END {print cnt}' "${PS}")
cat "${PS}" | cut -d ' ' -f 1 | uniq > "${UNIQUE_USERS}"

echo "Users that have more processes than ${FOO}:"
while read USER; do
    CUR_CNT=$(grep -c "${USER}" "${PS}")

    if [ "${CUR_CNT}" -gt "${FOO_PS_CNT}" ]; then
        echo "${USER}"
    fi
done < "${UNIQUE_USERS}"

echo "Average etime: "
AVG=$(awk '{total+=$3} END {print int(total/NR)}' "${PS}")

while read PID ETIMES; do
    if [ "${ETIMES}" -gt $(( "${AVG}" * 2 )) ]; then
        kill -15 ${PID}
        sleep 2
        kill -9 ${PID}
    fi
done < <(egrep "^${FOO}" "${PS}" | awk '{print $2, $3}')

rm -- "${PS}"
rm -- "${UNIQUE_USERS}"
