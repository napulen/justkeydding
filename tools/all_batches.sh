#!/bin/bash
declare -a kps=("krumhansl_kessler"
		"aarden_essen"
		"sapp"
		"bellman_budge"
		"temperley")

declare -a kts=("exponential2" "exponential10")

for transition in "${kts[@]}"
do
	for major in "${kps[@]}"
	do
		for minor in "${kps[@]}"
		do
			echo "$transition, $major, $minor"
			./batch_processing.sh $1 "-t $transition -M $major -m $minor -e" > "results/${transition}_${major}_${minor}.csv"
		done
	done
done
 
