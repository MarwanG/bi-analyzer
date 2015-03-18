#include "base.h"
#include <iostream>
#include <sstream>
#include <set>
#include <map>
#include <string>
#include <fstream>
#include <algorithm>
#include <regex>
#include <unordered_map>
#include <utility>
#include <ctime>
#include <time.h>



using namespace std;

// FUNCTION CALCULATES CC/REDUNDANCY BETWEEN TWO NODES.

void calculate_cc(Node* n1,Node* n2){
	int max_size = max(n1->get_degree(),n2->get_degree());
	int min_size = min(n1->get_degree(),n2->get_degree());
	int total_size = n1->get_degree()+n2->get_degree();
	float common = 0;
	vector<int> tmp;
    std::set_intersection(n1->neighbours_indexs.begin(), n1->neighbours_indexs.end(),
                          n2->neighbours_indexs.begin(), n2->neighbours_indexs.end(),
                          std::back_inserter(tmp));
    common = tmp.size();
	if(common > 0){
	 	for(int i = 0 ; i < tmp.size() ; i++){
	 		for(int j = i+1 ; j < tmp.size() ; j++){
	 			n1->pairs.insert(make_pair(tmp[i],tmp[j]));
	 			n2->pairs.insert(make_pair(tmp[i],tmp[j]));
	 		}
	 	}	
	 	total_size-=common;
		float cc_node = common/total_size;
		float cc_min_node = common/min_size;
		float cc_max_node = common/max_size;
		n1->add_cc(cc_node);
		n1->add_cc_min(cc_min_node);
		n1->add_cc_max(cc_max_node);
		n2->add_cc(cc_node);
		n2->add_cc_min(cc_min_node);
		n2->add_cc_max(cc_max_node);
		n1->nb_top_neighbours = n1->nb_top_neighbours + 1;
		n2->nb_top_neighbours = n2->nb_top_neighbours + 1;
	}
}

// FUNCTION TO CALCULATES STATS FOR WHOLE GRAPH.
void calculate_stat_graph(Graph * g){
	for(int i = 0 ; i < g->tops.size() ; i++){
		Node *n = g->tops[i];
		set<int> treated;
		set<int>::iterator it;
		for(it =  n->neighbours_indexs.begin() ; it != n->neighbours_indexs.end() ; it++){
			Node *bo = n->neighbours[*it];
			set<int>::iterator it2;
			for(it2 = bo->neighbours_indexs.begin() ; it2 != bo->neighbours_indexs.end() ; it2++){
			//	if(bo->get_degree() == 1){
              //      cout << bo->neighbours[*it2]->get_title() << "\n";
             //   }
                Node *tmp = bo->neighbours[*it2];
				int index_x = g->topsIndex.find(tmp->get_title())->second;
				if(i < index_x && find(treated.begin(), treated.end(), index_x)==treated.end() && n!= tmp){
					calculate_cc(n,tmp);
					treated.insert(index_x);
				}
			}
		}
		
		if(n->nb_top_neighbours > 0){
			n->set_cc(n->get_cc()/n->nb_top_neighbours);			
			g->cc += n->get_cc();
			g->cc_min+= (n->get_cc_min()/n->nb_top_neighbours);
			g->cc_max+= (n->get_cc_max()/n->nb_top_neighbours);
		}

		n->calculate_disp();
	 	n->calculate_redundancy();
		g->update_degree_top(n);
		g->update_redundancy_top(n);
		g->update_ccs_top(n);
		g->update_degree_cc(n);

		g->average_degree_top_v = g->average_degree_top_v + n->get_degree();
		if(n->get_degree() > g->max_top){
			g->max_top=n->get_degree();
		}
	 	if(n->get_disp() != -1){
	 		g->disp = g->disp + n->get_disp();
	 		g->nb_disp = g->nb_disp + 1;
		}
		if(n->get_red() != -1){
			g->red = g->red + n->get_red();
			g->nb_red = g->nb_red + 1;
		}
	}
	g->final_calculation();
}

// FUNCTION ADD CONNECTION
void addlink(Graph *g,string t , string b,string *t1,int size_pack){
	unordered_map<string, int>::const_iterator p;
	p = g->topsIndex.find(t);
	Node *top;
	if(p == g->topsIndex.end()){
		top = new Node(t);
		g->tops.push_back(top);
		top->set_index(g->tops.size()-1);
		g->topsIndex[t] = g->tops.size()-1;
	}else{
		top =g->tops[p->second];
	}
	unordered_map<string, int>::const_iterator p2;
	p2 = g->botsIndex.find(b);
	Node * bot;
	if(p2 == g->botsIndex.end()){
		bot = new Node(b);
		g->bots.push_back(bot);
		bot->set_index(g->bots.size() -1);
		g->botsIndex[b] = g->bots.size()-1;
	}else{
		bot = g->bots[p2->second];
	}
	bool bTop = top->addneighbours(bot);
 	bool bBot = bot->addneighbours(top);
	if(t1 != NULL){
    		top->add_ping(bot,*t1,size_pack);
	}
    if(bTop && bBot){
   	 	g->links = g->links + 1;
   	}
   	top = NULL;
    bot = NULL;
   	delete top;
    delete bot;
}

// FILE TO DATA
void file2data(string name,Graph *g){
	ifstream file(name.c_str());
	string str; 
	string t;
	string b;

	while (getline(file, str))
    {
    	istringstream iss(str);
    	iss >> b;
    	iss >> t;
    	addlink(g,t,b,NULL,0);
    }
    g->density = g->links / (float)(g->tops.size()*g->bots.size());
    file.close();
}

// GET STAT
void get_stat(string name){
	Graph * g = new Graph();
	file2data(name,g);
	calculate_stat_graph(g);
	stat_to_stdout(g);
 	stat_to_file(g);
 	g->free_data();
 	delete(g);
}

// GETTING LIST OF CHANNELS FROM FILE.
vector<string> get_channels(string name,int nbChannels,map<string,int> list){
	ifstream file(name.c_str());
	string str;
	string b;
	vector<string> tmp;
	string time_str;
	int limit = 70;
	while (getline(file, str) && tmp.size() < nbChannels)
    {
        istringstream iss(str);
    	iss >> time_str;
    	iss >> time_str;
    	iss >> b;  
    	size_t n = count(b.begin(), b.end(), '.');
    	if(n==4){
    		unsigned found = b.find_last_of(".");
	    	b = b.substr(0,found);
    	}
    	if (my_own_regex(b) && (find(tmp.begin(), tmp.end(), b)==tmp.end())){
    		size_t n = std::count(b.begin(), b.end(), '.');
    		if(n == 3){
    			map<string,int>::iterator it = list.find(b);
    			if(it != list.end()){
					list[b]=list[b] +1;
				}else{
					list[b]=1;
				}
				if (list[b] > limit){
					tmp.push_back(b);
				}
    		}
    	}	
    }
 	return tmp;
}