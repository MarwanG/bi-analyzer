#! /bin/bash

echo "Converting files pcap to txt \n"
echo "in directory $1"

cd $1

# for i in *.pcap
# do 
# 	tcpdump -tttt -n -q -r $i ip | awk '{print $4,$6}' | sed 's/://g' > $i.txt
# done 
#'{print $1$2,$4,$6,$7}'

find . -name '*.pcap' -print0 | while IFS= read -r -d '' file
do 
    tcpdump -tttt -n -q -r $file ip | awk '{print $4,$6}' | sed 's/://g' > $file.txt
done

cd -
