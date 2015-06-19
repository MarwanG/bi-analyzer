#! /bin/bash

# NOT SURE WHAT THIS FILE IS FOR NEED TO CHECK

DIR=$(dirname "$1")
cd $DIR


for var in "$@"
do
	file=$(basename "$var")
	if [ $file != "proper_*" ]
	then
		echo $file
		echo "Treating : $file"
		awk '{print $2,$6}' $file > tmp.txt
		cat tmp.txt | awk '{line="";for (i = 2; i <= NF; i++) line = line $i " "; table[$1]=table[$1] line;} END {for (key in table) print key,table[key];}' > tmp2.txt
		cat tmp2.txt | awk 'BEGIN {FS=OFS=" "}{sum=0;for(i=2;i<=NF;i++){sum+=$i}print $1,sum}' > proper_$file
		sort -k 1 -s proper_$file > tmp.txt
		mv tmp.txt  proper_$file
		rm tmp2.txt
	fi
done
cd -
