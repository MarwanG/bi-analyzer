#ifndef GRAPH_H_
#define GRAPH_H_

#include <unordered_map>
#include <vector>
#include "node.h"

class Graph
{
public:
	Graph();

	void final_calculation();
	void free_data();

	// data
	std::unordered_map<std::string,int> topsIndex;
	std::unordered_map<std::string,int> botsIndex;
	std::vector<Node*> tops;
	std::vector<Node*> bots;

	// variables for stat
	float average_degree_top_v;
	float average_degree_bot_v;
	int max_top;

	
	float cc;
	float cc_min;
	float cc_max;
	
	float red;
	int nb_red;
	
	float links;
	float density;
	
	float disp;
	int nb_disp;

};




#endif