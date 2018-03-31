#!/bin/bash

dir=$1
param=$2
if ! [ -d $dir ]
then
    echo "No exist directory";
    exit
fi

if [ $# -lt 3 ]
then
	rm_script=rm_dups.sh
else
	rm_script=$3
fi

echo "#!/bin/bash" > $rm_script

case $param in
	-name)
		#echo "Parameter name"
        find $dir -type f | rev | sort -t '/' -k 1 | rev > f_path
        
        while read line
        do
            printf '%q\n' "$line" >> f_path.m
        done < f_path

        cat f_path.m | rev | cut -d '/' -f 1 | rev | uniq -d > f_name
        
        while read line
        do
            echo "####-------|$line|-------" >> $rm_script
            grep -F "$(printf '%q' "$line")" f_path.m |
            sed 's/^/#rm /' >> $rm_script
        done < f_name ;;
	-cont)
		echo "Parameter cont"
		while read lineI
		do
			echo ""
			echo "$lineI"
			while read lineJ
			do
				echo "$lineJ"
				if diff $lineI $lineJ
				then
					echo "diff"
					echo "#rm $lineJ" >> $rm_script
					#попробовать sed	
				else
					echo "nodiff"
				fi
			done < files
			
		done < files;;
	*)
		echo "ERROR"
		exit 1;;
esac

rm -f f_path* f_name
