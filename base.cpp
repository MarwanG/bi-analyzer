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
#include <math.h> 



using namespace std;




long double get_ecart_list(vector<double> list,long double avg){
	long double sd = 0;
	for(int i = 0 ; i < list.size() ; i++){
		long double sd_tmp = pow(list[i]-avg,2);
		sd = sd + sd_tmp;
	}
	sd = sqrt(sd/(long double)list.size());
	return sd;
}

long double get_avg_list(vector<double> list){
	unsigned long long avg = 0;
	long double res = 0;
	if(list.size() <= 1){
		return -1;
	}
	for(int i = 0 ; i < list.size() ; i++){
		avg = avg + list[i];
	}
	res = avg/(long double)list.size();
	return res;
}


// Returns list of pairs containing the number of elements as well avg.
vector<pair<float,float> > avg_nb_for_each (map<string,vector<int> > list){
	map<string,vector<int> >::iterator it;
	vector<pair<float,float> > res;
	for(it = list.begin() ; it != list.end() ; it++){
		vector<int> values = it->second;
		float avg = 0;
		for(int i = 0 ; i < values.size() ; i++){
			avg = avg + values[i];
		}
		avg = avg / (float)values.size();
		res.push_back(make_pair(values.size(),avg));
	}
	return res;
}

// Returns list of pairs containing the avg of elements as well as the sd.
map<string,pair<float,float> > avg_for_each (map<string,vector<int> > list){	
	map<string,vector<int> >::iterator it;
	map<string,pair<float,float> > res;
	for(it = list.begin() ; it != list.end() ; it++){
		vector<int> values = it->second;
		float avg = 0;
		float sd = 0;
		for(int i = 0 ; i < values.size() ; i++){
			avg = avg + values[i];
		}
		avg = avg / (float)values.size();
		

		for(int i = 0 ; i < values.size() ; i++){
			float tmp = pow((values[i]-avg),2);
			sd = sd + tmp;
		}
		sd = sqrt(sd/(float)values.size());
		res[it->first]=(make_pair(avg,sd));
	}
	return res;
}




vector<string> list_to_check(string filename){
	vector<string> filter;
	string str;
	string ip; 
	ifstream * file = new ifstream(filename.c_str());

	while (getline(*file, str))
    {
    	istringstream iss(str);
    	iss >> ip;
    	filter.push_back(ip);
    }
    file->close();
    return filter;
}



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
//loop to each of the top
//loop for each bot of the current top 
//(can easily add the size/freq treatement here if needed should be faster)
// calculates complex graph metrics not called in simplified version
void calculate_stat_graph(Graph * g){
	for(int i = 0 ; i < g->tops.size() ; i++){
		Node *n = g->tops[i];
		set<int> treated;
		set<int>::iterator it;
		int nb_multi_peers =0;;
		for(it =  n->neighbours_indexs.begin() ; it != n->neighbours_indexs.end() ; it++){
			Node *bo = n->neighbours[*it];
			if(bo->get_degree() > 1){
				nb_multi_peers++;
			}
			set<int>::iterator it2;
			for(it2 = bo->neighbours_indexs.begin() ; it2 != bo->neighbours_indexs.end() ; it2++){
                Node *tmp = bo->neighbours[*it2];
				int index_x = g->topsIndex.find(tmp->get_title())->second;
				if(i < index_x && find(treated.begin(), treated.end(), index_x)==treated.end() && n!= tmp){
					calculate_cc(n,tmp);
					treated.insert(index_x);
				}
			}
		}
		g->nb_multi_peers_per_channel[n->get_title()] = nb_multi_peers ;
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
	if(b.compare("239.255.255.250")==0){
		return;
	}
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
	string new_t1 = t1->substr(0,19);
	if(p2 == g->botsIndex.end()){
		bot = new Node(b);
		bot->first_appear = new_t1;
		bot->last_appear = new_t1;
		g->bots.push_back(bot);
		bot->set_index(g->bots.size() -1);
		g->botsIndex[b] = g->bots.size()-1;
	}else{
		bot = g->bots[p2->second];
		time_t time_1 = timestamp_to_ctime(bot->last_appear.c_str());
		time_t t2 = timestamp_to_ctime(new_t1.c_str());
		double new_interval = difftime(t2,time_1);
		if(new_interval > bot->max_interval){
			bot->max_interval = new_interval-3600;
		}
		bot->last_appear = new_t1;
	}
	bool bTop = top->addneighbours(bot);
 	bool bBot = bot->addneighbours(top);
	if(t1 != NULL){
    		if(size_pack < 0 ){
    			top->add_pack_up(bot,-size_pack);
    		}else{
    			top->add_pack_down(bot,size_pack);
    		}
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
 	stat_to_file(g,"testing_stat");
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
    	if (my_own_regex(b) && (find(tmp.begin(), tmp.end(), b)==tmp.end())){
    		size_t n = std::count(b.begin(), b.end(), '.');
    		if(n > 2 && n < 4){ // add the n<4 for port need to look more into tht
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