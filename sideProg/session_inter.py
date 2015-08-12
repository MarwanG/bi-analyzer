#!/usr/bin/python

import sys
import time
import datetime



def sessions_inter(ip,session1,session2):
	if(session1[2]==session2[2]):
		return (False,-1)
	else:
		time_1_start = time.mktime(datetime.datetime.strptime(session1[0], "%Y-%m-%d %H:%M:%S.%f").timetuple())
		time_1_end = time.mktime(datetime.datetime.strptime(session1[1], "%Y-%m-%d %H:%M:%S.%f").timetuple())
		
		time_2_start = time.mktime(datetime.datetime.strptime(session2[0], "%Y-%m-%d %H:%M:%S.%f").timetuple())
		time_2_end = time.mktime(datetime.datetime.strptime(session2[1], "%Y-%m-%d %H:%M:%S.%f").timetuple())
		
		if(time_1_start < time_2_start):
			if(time_1_end < time_2_start):
				return (False,-1)
			else:
				duration = time_1_end - time_2_start
				print "Sessions : " + session1[2] + " " + session2[2]
				return (True,duration)
		else:
			if(time_2_end < time_1_start):
				return (False,-1)
			else:
				duration = time_2_end - time_1_start
				print "Sessions : " + session2[2] + " " + session1[2]
				return (True,duration)


def calculate_pair(ip,sessions):
	nb_para_sessions = 0
	duration_session_per_pair = 0
	for i in range(len(sessions)):
		for j in range(i+1,len(sessions)):
			res = sessions_inter(ip,sessions[i],sessions[j])
			if(res[0]):
				nb_para_sessions+=1
				duration_session_per_pair+=res[1]
	if(nb_para_sessions > 0 and duration_session_per_pair > 0):
		print ip + "  " + str(nb_para_sessions) + "  " + str(duration_session_per_pair) + "  " + str(duration_session_per_pair/nb_para_sessions)



file=sys.argv[1]
sessions = {}
channels = {}
currentpair = ""

with open(file) as f:
	for line in f:
		split = line.split()
		channel = split[1]
		split_tmp = channel.split('.')
		if(len(split_tmp) > 4):
			channel = channel.split('.'+split_tmp[len(split_tmp)-1])[0]
		pair = split[0]
		start = split[2]+" "+split[3]
		end = split[4]+" "+split[5]
		if(currentpair==""):
			channels[pair] = set()
			currentpair = pair
		elif(currentpair != pair):
			if (len(channels[currentpair]) > 1):
				calculate_pair(currentpair,sessions[currentpair])
			currentpair = pair
			channels[pair] = set()
		if(pair in sessions):
			sessions[pair].append((start,end,channel))
		else:
			sessions[pair] = [(start,end,channel)]
		channels[pair].add(channel)                                    





