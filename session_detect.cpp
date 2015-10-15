#include "session_detect.h"

#include <iostream>
#include <sstream>
#include <set>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <regex>
#include <unordered_map>
#include <math.h> 


#include "print_tools.h"
#include "session.h"
#include "base.h"


using namespace std;




void detect_session(string name,vector<string> channels,double delta){
	ifstream file(name.c_str());
	string str; 
	string t;
	string b;
	string time_str;
	string tmp;
	string hours;
	struct tm tm;
	time_t t1 = 4;
	int size_pack = 0;

	int z = 0;

	map<pair<string,string>,Session *> current_sessions;
	map<pair<string,string>,vector<Session *> > sessions;


	while (getline(file, str))
    {
     	z++;
     	if(z % 2500000 == 0){
     		cout << str << "\n";
     	}

    	istringstream iss(str);
    	iss >> time_str;
    	iss >> hours;
    	time_str.append(" " + hours);
    	iss >> b;
    	iss >> t;
        iss >> tmp;
  		iss >> tmp;
  		if(tmp.compare("ip-proto-17")==0){
  			continue;
  		}
		size_pack = atoi(tmp.c_str());
		if(size_pack == 0){
			continue;
		}
		size_t n = count(b.begin(), b.end(), '.');
    	if(n==4 && find(channels.begin(), channels.end(), b) == channels.end()){
	    		unsigned found = b.find_last_of(".");
		    	b = b.substr(0,found);
    	}
    	n = std::count(t.begin(), t.end(), '.');
		if(n==4 && find(channels.begin(), channels.end(), t) == channels.end()){
			unsigned found = t.find_last_of(".");
	    	t = t.substr(0,found);
		}
		if(my_own_regex(b)&&find(channels.begin(), channels.end(), b) == channels.end()){
            continue;
		}
		if(my_own_regex(t)&&find(channels.begin(), channels.end(), t) == channels.end()){
			continue;
		}
		// T IS THE CHANNEL
		// B IS THE IP
		if(find(channels.begin(), channels.end(), t)!=channels.end() && find(channels.begin(), channels.end(), b)==channels.end()){
		   	map<pair<string,string>,Session *>::iterator it;
		   	it = current_sessions.find(make_pair(t,b));
		   	if(it != current_sessions.end()){
		   		Session * s = it->second;
		   		bool is_end = s->check_end(time_str,delta);
		   		if(is_end){
		   			sessions[make_pair(t,b)].push_back(s);
		   			s = new Session(b,t,time_str);
		   			current_sessions.erase(it);
		   			current_sessions.insert(make_pair(make_pair(t,b),s));
		   		}
		   	}else{
		   		Session * s = new Session(b,t,time_str);
		   		current_sessions.insert(make_pair(make_pair(t,b),s));
		   	}
		}
	}
	file.close();

	map<pair<string,string>,Session *>::iterator it;
	for(it = current_sessions.begin() ; it != current_sessions.end() ; it++){
		Session * s = it->second;
		s->force_end();
		sessions[make_pair(s->ip_,s->channel_)].push_back(s);
	}


	string current_time_ = current_time();
	
	ofstream myfile;

	stringstream stream1;
	stream1 << delta;
	string delta_string = stream1.str();

	myfile.open("sessions_"+delta_string+"_"+current_time_+".stat");

	map<pair<string,string>,vector<Session *> >::iterator it_total;
	for(it_total = sessions.begin() ; it_total != sessions.end() ; it_total++){
		vector<Session*> tmp = it_total->second;
		for(int i = 0 ; i < tmp.size() ; i++){
			myfile << tmp[i]->ip_ << "  " << tmp[i]->channel_ << " " << tmp[i]->start_ << "  " << tmp[i]->end_ << "  " << tmp[i]->nbofpackets <<"\n";
		}
	}
	myfile.close();
}




void detect_(vector<string> names,vector<int> nbChannels,double delta){
	vector<string> channels;			//list of channels
	map<string,int> *list; 

	// for(int i = 0 ; i < names.size() ; i++){
		
	// 	channels.insert(channels.end(), tmp.begin(), tmp.end());
	// }

	for(int i = 0 ; i < names.size() ; i++){
		vector<string> tmp = get_channels(names[i],nbChannels[i],list);
		detect_session(names[i],tmp,delta);
	}
}