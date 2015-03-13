#! /bin/bash

echo "removing ips with degree 1"
echo "in directory $1"
echo "using file $2"


cp $2 $1
cd $1

FILE=$2
while read line; do
	echo "removing $line"
    sed -i '/\<$line\>/d' PC_A_edit.txt    
done < $FILE

rm $2
cd -
