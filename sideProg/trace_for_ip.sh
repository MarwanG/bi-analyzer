#! /bin/bash

echo "Filtering the ip \n"
echo "in directory $1 \n"

cd $1

for d in *;
do
	echo "entering $d" 
	cd "$d" && ls
	touch $d_$2.txt
	for i in *.pcap
	do
		tshark -r $i -T fields ip.src eq $2 or ip.dst eq $2 -e frame.time -e ip.src -e ip.dst -e ip.id |  head | awk '{print $4,$5,$6,strtonum($7)}' >>  $d_$2.txt
	done
	echo "exiting $d"
	cd ..
done

cd -
