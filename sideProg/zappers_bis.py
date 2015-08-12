#!/usr/bin/python

import sys
import datetime
import math


file=sys.argv[1]
option=sys.argv[2]


def max(l):
	t = -1
	ind = ""
	total=0
	for k,x in l.iteritems():
		total+=x
		if(x > t):
			t = x
			ind = k
	return (total,t,ind)


if(option == "filter"):
	lasttime = ""
	keptlast = False
	lastip = ""
	lastline = ""
	lastch = ""
	with open(file) as f:
			for line in f:
				split = line.split()
				ip = split[0]
				channel = split[1]
				start = split[2]+" "+split[3]
				end = split[4]+" "+split[5]
				if (lastip == ""):
					lasttime = end
					lastip = ip
				else:
					if(lastip == ip):
						t1 = datetime.datetime.strptime(lasttime, "%Y-%m-%d %H:%M:%S.%f")
						t2 = datetime.datetime.strptime(start, "%Y-%m-%d %H:%M:%S.%f")		
						duration = (t2-t1).total_seconds()
						if(duration < 0):
							if(keptlast):
								print line,
								x =0
							else:
								print lastline,
								print line,
								keptlast = True
						else:
							lastline = line
							keptlast = False
					else:
						lastip = ip
						lastline = line
						keptlast = False
						lasttime = end
				lasttime = end
				lastch = channel
else:
	zapping_sessions_nb = {}
	current_ip = ""
	current_end = ""
	v = (0,0)
	channels = set()
	channels_list = []
	durations = {}
	with open(file) as f:
		for line in f:
			split = line.split()
			ip = split[0]
			channel = split[1]
			start = split[2]+" "+split[3]
			end = split[4]+" "+split[5]

			t1 = datetime.datetime.strptime(start, "%Y-%m-%d %H:%M:%S.%f")
			t2 = datetime.datetime.strptime(end, "%Y-%m-%d %H:%M:%S.%f")		
			duration = (t2-t1).total_seconds()

			if ip not in zapping_sessions_nb:
				if(v[0] != 0 and v[1] != 0):
					tmp =max(durations) 
					v=(v[0],v[1],v[2],v[3],(tmp[0],tmp[1],channels_list.index(tmp[2])))
					zapping_sessions_nb[current_ip].append(v)
				zapping_sessions_nb[ip]=[]
				current_ip = ip
				current_end = end
				v = (1,1,start,"",0)
				channels = set()
				channels_list = []
				durations = {}
				durations[channel] = duration
				channels.add(channel);
				channels_list.append(channel);
			else:
				t1 = datetime.datetime.strptime(start, "%Y-%m-%d %H:%M:%S.%f")
				t2 = datetime.datetime.strptime(current_end, "%Y-%m-%d %H:%M:%S.%f")
				diff = (t1-t2).total_seconds()
				if(diff > 360):
					if(v[0] != 0 and v[1] != 0):
						tmp =max(durations) 
						v=(v[0],v[1],v[2],v[3],(tmp[0],tmp[1],channels_list.index(tmp[2])))	
						zapping_sessions_nb[ip].append(v)
					v = (1,1,start,end,0)
					channels = set()
					channels_list = []
					durations = {}
					if channel in durations:
						durations[channel]+=duration
					else:
						durations[channel]=duration
					channels.add(channel);
					channels_list.append(channel);	
					current_end = end
				else:
					channels.add(channel);
					channels_list.append(channel);
					if channel in durations:
						durations[channel]+=duration
					else:
						durations[channel]=duration
					t1 = datetime.datetime.strptime(end, "%Y-%m-%d %H:%M:%S.%f")
					t2 = datetime.datetime.strptime(current_end, "%Y-%m-%d %H:%M:%S.%f")	
					diff = (t1-t2).total_seconds()
	 				if(diff > 0):
						current_end = end
					v=(len(channels),v[1]+1,v[2],current_end,0)
	tmp =max(durations) 
	v=(v[0],v[1],v[2],v[3],(tmp[0],tmp[1],channels_list.index(tmp[2])))	
	zapping_sessions_nb[ip].append(v)


	for x in zapping_sessions_nb:
		res = filter(lambda x: (x[1]!=1 and x[0] != 1),zapping_sessions_nb[x])
		for zap in res:
			t1 = datetime.datetime.strptime(zap[2], "%Y-%m-%d %H:%M:%S.%f")
			t2 = datetime.datetime.strptime(zap[3], "%Y-%m-%d %H:%M:%S.%f")
			diff = (t2-t1).total_seconds()
			print x + "  " + str(zap[0]) + " " + str(zap[1]) + " " + str(float(zap[0])/float(zap[1])) + "  " + str(zap[2]) + " " + str(zap[3]) + " " + str(diff) + " " + str(zap[4]) 


