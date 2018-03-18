#!/bin/bash

N=$(sed 's/N=//' config.ini)

while true
do
	wget -O pogoda_minsk -o wget_log https://pogoda.tut.by

	temp_now=$(grep 'class="temp-i"' pogoda_minsk |
	head -n 1 |
	sed 's/<span class="temp-i">//; s!&deg;</span></div>!!')

	echo "Now: $temp_now"
	sleep $N
done
