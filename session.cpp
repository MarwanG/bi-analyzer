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
	nbofpackets=1;
}


void Session::force_end(){
	// duration = fmod(difftime(t2,t1),3600);
	end_ = last_;
}


bool Session::check_end(string end,double delta){
	time_t t1 = timestamp_to_ctime(last_.c_str());
	time_t t2 =	timestamp_to_ctime(end.c_str());
	time_t t3 = timestamp_to_ctime(start_.c_str());

// -       cout << ip_ << " " << start_ << "  " << last_ << "  " << end << "\n";
 
// -       double diff = fmod(difftime(t2,t1),3600);
// -	
	    // cout << ip_ << "  " << diff << "\n";
     double diff = difftime(t2,t1);
        
// -       if(diff >= 0 && diff < delta){

	// cout << ip_ << " " << start_ << "  " << last_ << "  " << end << "\n";

	// double diff = fmod(difftime(t2,t1),3600);

	// cout << ip_ << "  " << diff << "\n";
	
	// if(diff >= 0 && diff < delta){
   	if(diff < delta){
		nbofpackets++;
		last_ = end;
		return false;
	}else{
		force_end();
		return true;
	}
}	
