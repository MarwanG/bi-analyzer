#!/usr/bin/env python2.7

import time
import datetime
import sys

from dateutil import parser

print 'time : ' + sys.argv[1]+" "+sys.argv[2]
print 'file : ' + sys.argv[3]

# start = time.mktime(datetime.datetime.strptime(sys.argv[1], "%h:%M:%s").timetuple())

start = parser.parse(sys.argv[1]+" "+sys.argv[2]) 

print start

fo = open(sys.argv[3], "rw+")


line = fo.readline()
i = 0;

while line:
	i = i + 1
   	try:
		line = fo.readline()
		if "truncated-ip" in line:
			print line
		else:
			split = line.split(" ")
			time = split[0] + " " + split[1]
			end = parser.parse(time)
			# print end
			seconds = (start - end).total_seconds()
			if(seconds <= 0):
				print seconds
				print end
				break
			# print seconds
	except:
		continue
fo.close()
print i 