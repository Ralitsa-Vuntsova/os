#/bin/bash

[ $(id -u) -eq 0 ] || { echo "Script $0 not executed as root, nothing done!"; exit 1; }

for USER in $(ps -e -o user= | sort | uniq); do
        TOTAL_RSS=0
        PS_CNT=0

        while read PID RSS; do
                PS_CNT=$((${PS_CNT}+1))
                TOTAL_RSS=$((${TOTAL_RSS}+${RSS}))
                MAX_RSS=${RSS}
                MAX_RSS_PID=${PID}
        done < <(ps -u "${USER}" -o pid=,rss= | sort -n -k 2)

        if [ ${PS_CNT} -eq 0 ]; then
                continue
        fi

        AVG_RSS=$((${TOTAL_RSS}/${PS_CNT}))
        echo "${USER} ${PS_CNT} ${TOTAL_RSS}"

        if [ ${MAX_RSS} -gt $((${AVG_RSS}*2)) ]; then
                kill -SIGTERM ${MAX_RSS_PID}             
                sleep 2                                    
                kill -SIGKILL ${MAX_RSS_PID}
        fi
done
