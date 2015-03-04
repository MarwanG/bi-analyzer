#! /bin/bash

echo "Converting files pcap to txt \n"
echo "in directory $1"

cd $1

for d in *;
do
	echo "entering $d" 
	cd "$d" && ls
	touch $d.txt
	for i in *.pcap
	do
		tcpdump -tttt -n -q -r $i ip | awk '{print $1,$2,$4,$6,$7}'| sed 's/\(.*\):/\1/' >> $d.txt
	done
	echo "exiting $d"
	cd ..
done

cd -
