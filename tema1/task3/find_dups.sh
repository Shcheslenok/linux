#!/bin/bash

dir=$1
param=$2

flag=false
for util in head grep rev sed sort cut uniq find comm
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

find $dir -type f | rev | sort -t '/' -k 1 | rev > f_path
echo "#!/bin/bash" > $rm_script

case $param in
	-name)
        while read line
        do
            printf '%q\n' "$line" >> f_path.m
        done < f_path

        rev f_path.m | cut -d '/' -f 1 | rev | uniq -d > f_name
        
        while read line
        do
            echo "####-------|$line|-------" >> $rm_script
            grep -F "$(printf '%q' "$line")" f_path.m | sed 's/^/#rm /' >> $rm_script
        done < f_name ;;
	-cont)
		sort f_path -o f_path

        while [ "$(head -n 1 f_path)" != "" ]
        do
            file=$(head -n 1 f_path)
			echo "$file" > f_rm
			md5_file=$(md5sum "$file" | cut -d ' ' -f1)
			name_file=$(basename "$file")
            
            flag=true
			while read line
            do
				md5_line=$(md5sum "$line" | cut -d ' ' -f1)
				name_line=$(basename "$line")

				if [ "$md5_line" == "$md5_file" ] && [ "$name_line" != "$name_file" ]
				then
					if $flag
					then
						echo "####------------------------------" >> $rm_script
						printf '%q\n' "$file" | sed 's/^/#rm /' >> $rm_script
						flag=false
					fi

					printf '%q\n' "$line" | sed 's/^/#rm /' >> $rm_script
					echo "$line" >> f_rm
				fi
			done < f_path

			sort f_rm -o f_rm
			comm -23 f_path f_rm > f_path.m
			cp f_path.m f_path
        done;;
	*)
		echo "ERROR"
esac

rm -f f_*
