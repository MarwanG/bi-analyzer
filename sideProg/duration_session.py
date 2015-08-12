#!/usr/bin/python


import sys
import datetime

file=sys.argv[1]


with open(file) as f:
	for line in f:
		list = line.split()
		pair = list[0]
		channel = list[1]
		start = list[2]+" "+list[3]
		end = list[4]+" "+list[5]

		t1 = datetime.datetime.strptime(end, "%Y-%m-%d %H:%M:%S.%f")
		t2 = datetime.datetime.strptime(start, "%Y-%m-%d %H:%M:%S.%f")
		diff = (t1-t2).total_seconds()
		print pair + " " + channel + " " + str(diff)	
