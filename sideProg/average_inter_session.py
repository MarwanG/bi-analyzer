#!/usr/bin/python

import sys
import datetime
from time import sleep

file=sys.argv[1]
option=sys.argv[2]


# NEED TO SORT WITH THIS  sort -k 1,1 -k 2,2 -k 4,4


currentPair = ""
currentEnd = ""
currentStart = "" 
currentChannel = ""


differences = []



dict = {}
dict2 = {}
dict3 = {}
channels_per_user = {}


largest_diff = {}





with open(file) as f:
	for line in f:
		list = line.split()

		pair = list[0]
		channel = list[1]
		start = list[2]+" "+list[3]
		end = list[4]+" "+list[5]
		
		if(option == "avg" or option == "diff"):
			if(currentPair=="" or currentPair!=pair or currentChannel!=channel):
				currentPair = pair
				currentEnd = end
				currentChannel = channel
				if currentPair not in dict:
					dict[currentPair] = []
			else:
				t1 = datetime.datetime.strptime(start, "%Y-%m-%d %H:%M:%S.%f")
				t2 = datetime.datetime.strptime(currentEnd, "%Y-%m-%d %H:%M:%S.%f")
				diff = (t1-t2).total_seconds()
				currentEnd = end
				dict[currentPair].append(diff)
				differences.append(diff)
		elif(option == "chng"):
			if(currentPair=="" or currentPair!=pair):
				currentPair = pair
				currentEnd = end
				currentChannel = channel
				if currentPair not in dict:
					dict[currentPair] = []
					channels_per_user[currentPair] = set()
					dict2[currentPair] = 0
					dict3[currentPair] = 0
					largest_diff[currentPair] = []
				channels_per_user[currentPair].add(currentChannel)
			elif(currentChannel != channel):
				t1 = datetime.datetime.strptime(start, "%Y-%m-%d %H:%M:%S.%f")
				t2 = datetime.datetime.strptime(currentEnd, "%Y-%m-%d %H:%M:%S.%f")
				diff = (t1-t2).total_seconds()
				currentEnd = end
				dict[currentPair].append(diff)
				# if(diff > largest_diff[currentPair] or diff < 0):
				# 	largest_diff[currentPair] = diff
				largest_diff[currentPair].append(diff)
				differences.append(diff)
				currentChannel = channel
				currentEnd = end
				dict3[currentPair]+=1
				channels_per_user[currentPair].add(currentChannel)
			else:
				t1 = datetime.datetime.strptime(start, "%Y-%m-%d %H:%M:%S.%f")
				t2 = datetime.datetime.strptime(currentEnd, "%Y-%m-%d %H:%M:%S.%f")
				diff = (t1-t2).total_seconds()
				# if(diff > largest_diff[currentPair] or diff < 0):
				# 	largest_diff[currentPair] = 
				dict2[currentPair]+=1
				currentEnd = end
				channels_per_user[currentPair].add(currentChannel)


if(option == "avg"):		
	for k in dict:
		if(len(dict[k]) > 1):
			print k + "  " +  str(sum(dict[k])/ len(dict[k]))
elif (option == "diff"):
	for diff in differences:
		print diff
elif (option == "chng"):
	# for x in largest_diff:
	# 	if(len(largest_diff[x]) > 0):			
	# 		res = filter(lambda x: (x <= 360),largest_diff[x])
	# 		if(len(res) > 0):
	# 			print x + "  " + str(res)
	# for diff in differences:
	# 	print diff
	for x in dict2 :
		if(dict3[x]+dict2[x] > 0):
			print x + " " + str(len(channels_per_user[x])) + " " + str(dict3[x]) + " " + str(float(dict3[x])/float(dict3[x]+dict2[x])) 




