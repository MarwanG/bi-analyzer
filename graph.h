#ifndef GRAPH_H_
#define GRAPH_H_

#include <unordered_map>
#include <vector>
#include <string>
#include <utility>
#include "node.h"


class Graph
{
public:
	Graph();


	void update_distr(Node * n);
	void final_calculation();
	void free_data();
	void set_time(std::string s);
	void update_degree_bot(Node * n);
	void update_degree_top(Node * n);
	void update_ccs_top(Node *n);
	void update_ccs_bot(Node *n);
	void update_degree_cc(Node *n);
	void update_redundancy_bot(Node * n);
	void update_redundancy_top(Node * n);
	void add_communication(std::string ip,std::string time_,int size);
	std::map<std::string,std::vector< std::pair<std::string,int> > > calcute_window_peaks(int delta);
	std::string packs_to_string_bot();
	std::string degrees_to_string();
	std::string degrees_to_string_bot();

	// data
	std::unordered_map<std::string,int> topsIndex;
	std::unordered_map<std::string,int> botsIndex;
	std::vector<Node*> tops;
	std::vector<Node*> bots;


	std::unordered_map<int,std::set<std::string> > distr_by_degree;


	// STATS MIGHT BE GOOD IDEA TO PASS THEM NOT SO SURE YET.
	std::map<int,int> degrees_top;
	std::map<int,int> degrees_bot;


	std::map<float,int> redundancys_top;
	std::map<float,int> redundancys_bot;
	std::map<float,int> ccs_top;
	std::map<float,int> ccs_bot;
	std::map<int,float> degree_cc_top; 
	std::map<int,float> degree_cc_bot;

	std::map<std::string,std::vector<std::string> > time_communications_per_ip;
	std::map<std::string,std::vector<int> > pack_size_per_ip;
	
	// std::map<std::string,std::pair<string,int>> results_time_value_per_ip;


	// variables for stat
	float average_degree_top_v;
	float average_degree_bot_v;
	
	int max_top;
	int max_bot;

	int min_bot;

	
	float cc;
	float cc_min;
	float cc_max;
	
	float red;
	int nb_red;
	
	float links;
	float density;
	
	float disp;
	int nb_disp;

	std::string time_;

};




#endif
