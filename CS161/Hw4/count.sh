#!/bin/bash

max=0
for (( num=1; num<=50; num++ ))
do
	tmp=($(grep -w "$num" graph2.txt | wc -l))

	if [ "$max" lt "$tmp" ]
	then
		max="$tmp"
	fi
done
echo "$max"