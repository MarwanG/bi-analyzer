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
		/usr/sbin/tcpdump -tttt -n -q -r $i | awk '{print $1,$2,$4,$6,$7,$9,$8}' | sed  's/\(.*\):/\1/' |  sed 's/length//g' >>  $d.txt
	done
	echo "exiting $d"
	cd ..
done

cd -
