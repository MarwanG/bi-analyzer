#include "session.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "print_tools.h"
#include <math.h>

using namespace std;


Session::Session(string ip,string channel,string start){
	ip_ = ip;
	channel_ = channel;
	start_ = start;
	last_ = start;
	end_ = "";
	duration = 0;
}


void Session::force_end(){
	end_ = last_;
}


bool Session::check_end(string end,double delta){
	time_t t1 = timestamp_to_ctime(last_.c_str());
	time_t t2 =	timestamp_to_ctime(end.c_str());
	time_t t3 = timestamp_to_ctime(start_.c_str());

	double diff = fmod(difftime(t2,t1),3600);
	
	if(diff < delta){
		last_ = end;
		return false;
	}else{
		duration = fmod(difftime(t2,t3),3600);
		end_ = last_;
		return true;
	}
}	
