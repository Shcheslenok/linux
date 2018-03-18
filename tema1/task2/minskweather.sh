#!/bin/bash

#rm -f index.html

while true
do
	wget -b -o pogoda_minsk https://pogoda.tut.by

	temp_now=$(grep 'class="temp-i"' pogoda_minsk |
	head -n 1 |
	sed 's/<span class="temp-i">//; s!&deg;</span></div>!!')

#	rm index.html

	echo "Now: $temp_now"
	sleep 3
done
