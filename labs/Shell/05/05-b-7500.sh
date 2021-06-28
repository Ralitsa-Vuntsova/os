#/bin/bash

NUM=$(( (RANDOM%200) -100 ))
GUESS_CNT=0

read -p "Guess? " GUESS

function validate_num(){
	grep -qE "^[+-]?[0-9]+$" <(echo $"{1}")
}

while true; do
	GUESS_CNT=$((GUESS_CNT+1))
	if [[ ! $(validate_num "${GUESS}") -eq 0 ]]; then
		echo "Not a valid guess!"
	else
		if [ "${GUESS}" -gt "${NUM}" ]; then
			echo "...smaller"
		elif [ "${GUESS}" -lt "${NUM}" ]; then
			echo "...bigger"
		else
			echo "RIGHT! Guessed at ${GUESS_CNT} tries!"
			exit 0
		fi
	fi
	read -p "Guess? " GUESS
done
