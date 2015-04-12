#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include "print_tools.h"

using namespace std;

Graph::Graph(){
	disp = 0;
	nb_disp = 0;
	red = 0;
	nb_red = 0;
	average_degree_top_v = 0;
	min_bot = 10000;
	max_top = 0;
	max_bot = 0;
	links = 0;
	time_.clear();
}

void Graph::set_time(string s){
	if(time_.empty()){
		time_ = s;
	}
}


void Graph::update_distr(Node * n){
	unordered_map<int, set<string> >::const_iterator p;
	p = distr_by_degree.find(n->get_degree());
	if(p == distr_by_degree.end()){
		set<string> list;
		list.insert(n->get_title());
		distr_by_degree[n->get_degree()] = list;
	}else{
		distr_by_degree[n->get_degree()].insert(n->get_title());
	}
}

void Graph::final_calculation(){
	cc = cc/tops.size();
	cc_min = cc_min/tops.size();
 	cc_max = cc_max/tops.size();
 	average_degree_top_v = average_degree_top_v/tops.size();
 	disp = disp / nb_disp;
 	red = red / nb_red;
 	for(int i = 0 ; i < bots.size() ; i++){
 		update_degree_bot(bots[i]);
 		int degree_tmp = bots[i]->get_degree();
 		if(min_bot > degree_tmp){
 			min_bot = degree_tmp;
 		}
 		if(max_bot < degree_tmp){
 			max_bot = degree_tmp;
 		}
 		update_distr(bots[i]);
 		average_degree_bot_v = average_degree_bot_v + degree_tmp;
 	}
 	average_degree_bot_v = average_degree_bot_v/bots.size();
}


string Graph::degrees_to_string(){
	string res = "";
    string s;
    for(int i = 0 ; i < tops.size() ; i++){
	    ostringstream oss;
        oss << tops[i]->get_degree();
        res = res + oss.str() + " ";
	}
	return res;
}

string Graph::degrees_to_string_bot(){
	string res = "";
	string s;
	for(int i = 1 ; i <= tops.size() ; i++){
		if (degrees_bot.find(i) == degrees_bot.end()){
			res = res + "0 ";
		}else{
			ostringstream oss;
			oss << degrees_bot[i];
			res = res + oss.str() + " ";
		}
	}
	return res;
}

string Graph::packs_to_string_bot(){
	string res = "";
	string s;
	for(int i = 0 ; i < tops.size() ;i++){
		ostringstream oss;
        oss << tops[i]->total_packs;
        res = res + oss.str() + " ";
	}
	return res;
}

void Graph::free_data(){
	for(int i = 0 ; i < tops.size() ; i++){
		delete tops[i];
	}
	for(int i = 0 ; i < bots.size() ; i++){
        delete bots[i];
	}
}

//FUNCTIONS TO UPDATE DEGREE BOT AND TOP
void Graph::update_degree_top(Node * n){
	map<int,int>::iterator it = degrees_top.find(n->get_degree());
	if(it != degrees_top.end()){
		degrees_top[n->get_degree()]=degrees_top[n->get_degree()] +1;
	}else{
		degrees_top[n->get_degree()]=1;
	}
}

void Graph::update_degree_bot(Node * n){
	map<int,int>::iterator it = degrees_bot.find(n->get_degree());
	if(it != degrees_bot.end()){
		degrees_bot[n->get_degree()]=degrees_bot[n->get_degree()] +1;
	}else{
		degrees_bot[n->get_degree()]=1;
	}
}

// FUNCTION TO UPDATE REDUNDANCY TOP AND BOT

void Graph::update_redundancy_top(Node * n){
	map<float,int>::iterator it = redundancys_top.find(n->get_red());
	if(it != redundancys_top.end()){
		redundancys_top[n->get_red()]=redundancys_top[n->get_red()] +1;
	}else{
		redundancys_top[n->get_red()]=1;
	}
}

void Graph::update_redundancy_bot(Node * n){
	map<float,int>::iterator it = redundancys_bot.find(n->get_red());
	if(it != redundancys_bot.end()){
		redundancys_bot[n->get_red()]=redundancys_bot[n->get_red()] +1;
	}else{
		redundancys_bot[n->get_red()]=1;
	}
}


// FUNCTION TO UPDATE CCS TOP AND BOT

void Graph::update_ccs_top(Node *n){
	map<float,int>::iterator it = ccs_top.find(n->get_cc());
	if(it != ccs_top.end()){
		ccs_top[n->get_cc()]=ccs_top[n->get_cc()] +1;
	}else{
		ccs_top[n->get_cc()]=1;
	}
}

void Graph::update_ccs_bot(Node *n){
	map<float,int>::iterator it = ccs_bot.find(n->get_cc());
	if(it != ccs_bot.end()){
		ccs_bot[n->get_cc()]=ccs_bot[n->get_cc()] +1;
	}else{
		ccs_bot[n->get_cc()]=1;
	}
}

// FUNCTION TO UPDATE CCS/DEGREE TOP AND BOT

void Graph::update_degree_cc(Node *n){
	map<int,float>::iterator it = degree_cc_top.find(n->get_degree());
	if(it != degree_cc_top.end()){
		degree_cc_top[n->get_degree()] = degree_cc_top[n->get_degree()] + n->get_cc();
	}else{
		degree_cc_top[n->get_degree()] = n->get_cc();
	}
}

void Graph::add_communication(string ip,string time_,int size){
	time_communications_per_ip[ip].push_back(time_);
	pack_size_per_ip[ip].push_back(size);	
}


// double time_diff(string prev_time,string s){
// 	time_t t1 = timestamp_to_ctime(prev_time.c_str());
// 	time_t t2 = timestamp_to_ctime(s.c_str());
// 	double t = difftime(t1,t2);

// 	string t1_str_milli = s.substr(s.find('.')+1,s.length());
// 	if(t1_str_milli[0]=='0'){
// 	  t1_str_milli = s.substr(s.find('.')+2,s.length());
// 	}
// 	string t2_str_milli = prev_time.substr(prev_time.find('.')+1,prev_time.length());
// 	if(t2_str_milli[0]=='0'){
// 	  t2_str_milli = prev_time.substr(prev_time.find('.')+2,prev_time.length());
// 	}
// 	int t1_milli = atoi(t1_str_milli.c_str());
// 	int t2_milli = atoi(t2_str_milli.c_str());
// 	int diff_milli_tmp = abs(t1_milli-t2_milli);
// 	t = t * 1000;
// 	t = t + diff_milli_tmp;
// 	return t;
// }


// map<string,vector<pair<string,int> > > Graph::calcute_window_peaks(int delta){
// 	map<string,vector< pair<string,int> > > res;
// 	map<string,vector<string> >::iterator it;
// 	for(it = time_communications_per_ip.begin() ; it != time_communications_per_ip.end() ; it++){
// 		string ip = it->first;
// 		vector<string> times = it->second;
// 		vector<int> packs = pack_size_per_ip[ip];
	
// 		int index = 0;

// 		string start_time = times[index];
// 		string end_time ;
// 		int total_size = packs[index];
// 		while(index < packs.size()){
// 			index++;
// 			string current_time = times[index];
// 			if(time_diff(start_time,current_time) <= delta){
// 				total_size = total_size + packs[index];
// 				end_time = current_time;
// 			}else{
// 				res[ip].push_back(make_pair(end_time,total_size));
// 				start_time = current_time;
// 				total_size = packs[index];
// 			}
// 		}
// 	}
// 	return res;
// }




