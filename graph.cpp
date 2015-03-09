#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

Graph::Graph(){
	disp = 0;
	nb_disp = 0;
	red = 0;
	nb_red = 0;
	average_degree_top_v = 0;
	max_top = 0;
	links = 0;
	time_.clear();
}

void Graph::set_time(string s){
	if(time_.empty()){
		time_ = s;
	}
}

void Graph::final_calculation(){
	cc = cc/tops.size();
	cc_min = cc_min/tops.size();
 	cc_max = cc_max/tops.size();
 	average_degree_top_v = average_degree_top_v/tops.size();
 	disp = disp / nb_disp;
 	red = red / nb_red;
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