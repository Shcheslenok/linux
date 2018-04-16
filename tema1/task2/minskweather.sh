#!/bin/bash

flag=false
for util in wget grep sed head sleep
do
	if [ "$(which "$util")" == "" ]
	then
		echo "You need to install $util"
		flag=true
	fi
done

if $flag
then
	exit 1
fi

N=$(sed 's/N=//' config.ini)

while true
do
	wget -O /tmp/pogoda_minsk -o /tmp/wget_log https://pogoda.tut.by

	echo "Now: $(grep 'class="temp-i"' "/tmp/pogoda_minsk" | head -n 1 | sed 's/<span class="temp-i">//; s!&deg;</span></div>!!')"

	sleep $N
done
