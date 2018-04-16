#!/bin/bash

dir=$1
param=$2

#which grep rev sed sort cut uniq find
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

        rev f_path.m | cut -d '/' -f 1 | rev | uniq -d > f_name
        
        while read line
        do
            echo "####-------|$line|-------" >> $rm_script
            grep -F "$(printf '%q' "$line")" f_path.m | sed 's/^/#rm /' >> $rm_script
        done < f_name ;;
	-cont)
		echo "Parameter cont"
		#find $dir -type f > f_path
        find $dir -type f | rev | sort -t '/' -k 1 | rev > f_path
        
        while read line
        do
            printf '%q\n' "$line" >> f_path.m
        done < f_path

        #cat f_path.m | rev | cut -d '/' -f 1 | rev | sort | uniq > f_name
		sort f_path -o f_path
		cat f_path.m

        while [ "$(head -n 1 f_path)" != "" ]
        do
            file=$(head -n 1 f_path)
            
			#echo "$file"
			echo "$file" > f_rm
			#printf '%q' "$file" | md5sum
			#md5sum "$file"
			#basename "$file"
			md5_file=$(md5sum "$file" | cut -d ' ' -f1)
			file_name=$(basename "$file")
            
            flag=true
			while read line
            do
				#echo "$line"
                #printf '%q' "$line" | md5sum
				#md5sum "$line"
				#basename "$line"
				md5_line=$(md5sum "$line" | cut -d ' ' -f1)
				name_line=$(basename "$line")

		#line=$(printf '%q' "$(line)")
                #if [ "$(diff $file $line)" = "" ] 
			#"$(diff $file $line)" = "" ]]
				#if [ "$(printf '%q' "$line" | md5sum | cut -d ' ' -f 1)" == "$(printf '%q' "$file" | md5sum | cut -d ' ' -f1)" ] && [ "$(basename "$(printf '%q' "$line")")" != "$(basename "$(printf '%q' "$file")")" ]
				if [ "$md5_line" == "$md5_file" ] && [ "$(basename "$line")" != "$(basename "$file")" ]
				then
					echo "EQUAL"
					if $flag
					then
						echo "####------------------------------" >> $rm_script
						printf '%q\n' "$file" | sed 's/^/#rm /' >> $rm_script
						flag=false
					fi

					printf '%q\n' "$line" | sed 's/^/#rm /' >> $rm_script
					echo "$line" >> f_rm
			#printf '%q\n' "$line" >> f_rm
				fi
			done < f_path

				#while read line
				#do
				#	line=`printf '%q' "$line"`
				#	sed -e "/$line/d" f_path.m
				#done < f_rm
			sort f_rm -o f_rm
			comm -23 f_path f_rm > f_path.m
			cp f_path.m f_path
			#awk 'NR==FNR{a[$0];next} !($0 in a)' f_rm f_path.m
        done
        #while read lineI
		#do
			#echo ""
			#echo "$lineI"
			#while read lineJ
			#do
				#echo "$lineJ"
                #basename $lineI
				#if [ ($lineI != $lineJ) && ("$(diff $lineI $lineJ)" = "") ]
				#then
					#echo "diff"
					#echo "#rm $lineJ" >> $rm_script
					#попробовать sed	
				#else
					#echo "nodiff"
				#fi
			#done < f_path
			
		#done < f_path
        ;;
	*)
		echo "ERROR"
		exit 1;;
esac

rm -f f_*
