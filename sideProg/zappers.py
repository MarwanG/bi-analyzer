#!/usr/bin/python

import sys
import datetime
import math


file=sys.argv[1]
option=sys.argv[2]


zapping_sessions_nb = {}
zapping_lines_nb = {}

current_ip = ""
current_end = ""
v = (0,0)

channels = set()
channels_list = []
durations = {}

lines = []


def number_changement(l):
	res = 0
	prev = l[0]
	for i in range(1,len(l)):
		if(l[i] != prev):
			res+=1
		prev = l[i]
	return res


def standard_dev(ip,l):
	if(len(l) > 0):
		mean = 0.0
		for x in l:
			mean+=x
		mean = mean/len(l)
		sd = 0.0
		for x in l:
			tmp = pow(x - mean,2)
			sd+=tmp
		return (mean,math.sqrt(sd/len(l)))
	else:
		return (0,0)

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

if(option == "normal") :
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
					v=(v[0],v[1],v[2],v[3],(tmp[0],tmp[1],channels_list.index(tmp[2])),number_changement(channels_list))
					zapping_sessions_nb[current_ip].append(v)
					zapping_lines_nb[current_ip][v]=lines
					lines = []
			
				zapping_sessions_nb[ip]=[]
				zapping_lines_nb[ip] = {}
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
						v=(v[0],v[1],v[2],v[3],(tmp[0],tmp[1],channels_list.index(tmp[2])),number_changement(channels_list))
						zapping_sessions_nb[ip].append(v)
						zapping_lines_nb[ip][v]=lines
						lines = []

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
			lines.append(line)

	tmp =max(durations) 
	v=(v[0],v[1],v[2],v[3],(tmp[0],tmp[1],channels_list.index(tmp[2])),number_changement(channels_list))
	zapping_sessions_nb[ip].append(v)
	zapping_lines_nb[ip][v]=lines

	for x in zapping_sessions_nb:
		res = filter(lambda x: (x[1]!=1 and x[0] != 1),zapping_sessions_nb[x])
		for zap in res:
			t1 = datetime.datetime.strptime(zap[2], "%Y-%m-%d %H:%M:%S.%f")
			t2 = datetime.datetime.strptime(zap[3], "%Y-%m-%d %H:%M:%S.%f")
			diff = (t2-t1).total_seconds()
			print x + "  " + str(zap[0]) + " " + str(zap[1]) + " " + str(float(zap[0])/float(zap[1])) + "  " + str(zap[2]) + " " + str(zap[3]) + " " + str(diff) + " " + str(zap[4]) + " " + str(zap[5]) 
			# res2 = zapping_lines_nb[x][zap]
			# for y in res2:
			# 	print y,



else:
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
					v=(v[0],v[1],v[2],v[3],v[4])
					zapping_sessions_nb[current_ip].append(v)
				zapping_sessions_nb[ip]=[]
				current_ip = ip
				current_end = end
				v = (1,1,start,"",0)
				channels = set()
				channels.add(channel);
			else:
				t1 = datetime.datetime.strptime(start, "%Y-%m-%d %H:%M:%S.%f")
				t2 = datetime.datetime.strptime(current_end, "%Y-%m-%d %H:%M:%S.%f")
				diff = (t1-t2).total_seconds()
				if(diff > 0):
					if(v[0] != 0 and v[1] != 0):
						v=(v[0],v[1],v[2],v[3],v[4])	
						zapping_sessions_nb[ip].append(v)
					v = (1,1,start,end,0)	
					channels = set()
					channels.add(channel);
				else:
					# print ip + "  " + str(diff)
					channels.add(channel);
					t1 = datetime.datetime.strptime(end, "%Y-%m-%d %H:%M:%S.%f")
					t2 = datetime.datetime.strptime(current_end, "%Y-%m-%d %H:%M:%S.%f")	
					diff2 = (t1-t2).total_seconds()
	 				if(diff2 > 0):
						current_end = end
						# print "kaka \n"
					v=(len(channels),v[1]+1,v[2],current_end,abs(diff))					

	v=(v[0],v[1],v[2],v[3],v[4])	
	zapping_sessions_nb[ip].append(v)

	for x in zapping_sessions_nb:
	# 	# print x 
		res = filter(lambda x: (x[1] ==2),zapping_sessions_nb[x])
		for zap in res:
			print x + " " + str(zap[0]) + " " + str(zap[1]) + " " + str(zap[2]) + " " + str(zap[3]) + " " + str(zap[4])
