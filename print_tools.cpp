#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <set>
#include <map>
#include <vector>
#include <time.h>

#include "graph.h"

using namespace std;


void split(const string& s, char c,
           vector<string>& v) {
   string::size_type i = 0;
   string::size_type j = s.find(c);

   while (j != string::npos) {
      v.push_back(s.substr(i, j-i));
      i = ++j;
      j = s.find(c, j);

      if (j == string::npos)
         v.push_back(s.substr(i, s.length()));
   }
}

bool my_own_regex(string s){
	vector<string> v;
	split(s,'.',v);
	if(v.size() > 2){
		if((v[0].compare("192") == 0) && (v[1].compare("168")==0))
			return true;
	}
	return false;
}

time_t timestamp_to_ctime(const char* time_stamp ){
   time_t _return;
   struct tm tm_struct ; 
   strptime(time_stamp ,"%Y-%m-%d %H:%M:%S",&tm_struct);
   _return  = mktime(&tm_struct);   
   return _return;
}

void stat_each_node(vector<Node*> data,string name){
	ofstream myfile;
	myfile.open(name);
	myfile.precision(6);
	for(int i = 0 ; i < data.size() ; i++){
		myfile << data[i]->get_title() << " ";
		myfile << data[i]->get_degree() << "  ";
		myfile << fixed << data[i]->get_cc() << " ";
		myfile << fixed << data[i]->get_red() << " ";
		myfile << fixed << data[i]->get_disp() << " \n";
	}
	myfile.close();
}

void graph_degree(map<int,int> data,int max,int size,string name){
	ofstream myfile;
	myfile.open (name);
	int found = 0 ;
	for(int i = 0 ; i <= max ; i++){
		myfile << i << " ";
		map<int,int>::iterator it = data.find(i-1);
		int v = 0;
		if(it != data.end()){
			v = data[i-1];
			found = found + v;
		}
		float s = ((float)(size - found) / (float)size) * 100;
		myfile << s << "\n";
	}
	myfile.close();
}

void graph_redundancy(map<float,int> data,int size,string name){
	ofstream myfile;
	myfile.open(name);
	map<float,int>::iterator it;
	int found = 0;
	int found_prev = 0;
	vector<float> keys;
	vector<int>values;
	for(it = data.begin(); it != data.end(); it++){
		keys.push_back(it->first);
		values.push_back(it->second);	
	}
	for(int i = 1 ; i < keys.size() ; i++){
		myfile << keys[i] << " ";
		found = found + values[i-1];
		float s = ((float)(size - found) / (float)size) * 100;
		myfile << s << "\n";
	}
	myfile.close();
}



void graph_cc(map<float,int> data,int size,string name){
	ofstream myfile;
	myfile.open ("graph_cc.dat");
	map<float,int>::iterator it;
	int found = 0;
	int found_prev = 0;
	vector<float> keys;
	vector<int>values;
	for(it = data.begin(); it != data.end(); it++){
		keys.push_back(it->first);
		values.push_back(it->second);	
	}

	for(int i = 1 ; i < keys.size() ; i++){
		myfile << keys[i] << " ";
		found = found + values[i-1];
		float s = ((float)(size - found) / (float)size) * 100;
		myfile << s << "\n";
	}
	myfile.close();
}


void stat_to_file(Graph *g,int nb_top){
	ofstream myfile;
	myfile.open ("global_test.txt");
	myfile.precision(6);
	if(g->time_ != NULL){
		myfile << "Start Time : " << g->time_ << "\n";
	}
	myfile << "Number of tops : " << g->tops.size() << "\n";
	myfile << "Number of bots : " << g->bots.size() << "\n";
	myfile << "Number of edges : " << fixed << g->links << "\n";
	myfile << "Density : " << fixed << g->density << "\n";
	myfile << "average_degree_top_v :" << g->average_degree_top_v << "\n";
	myfile << "Max degree (top) : " << g->max_top << " (" << nb_top << ")\n";
	myfile << "Clustering coefficient cc_top : " << g->cc << "\n";
	myfile << "Clustering coefficient cc_min_top : " << g->cc_min << "\n";
	myfile << "Clustering coefficient cc_max_top : " << g->cc_max << "\n";
	myfile << "Redundancy coefficient (top) : " << g->red << "\n";
	myfile << "Dispersion coefficient (top) : " << g->disp << "\n";
	myfile.close();
}

void stat_to_stdout(Graph *g){
	cout << "Number of tops : " << g->tops.size() << "\n";
	cout << "Number of bots : " << g->bots.size() << "\n";
	cout << "Number of edges : " << g->links << "\n";
	cout << "Density : " << fixed << g->density << "\n";
}


void create_graph_int(vector<int> v,string name){
	ofstream myfile;
	myfile.open (name);
	for(int i = 0 ; i < v.size() ; i++){
		myfile << i << " " << v[i] << "\n";
	}
	myfile.close();
}

void create_graph_float(vector<float> v,string name){
	ofstream myfile;
	myfile.open (name);
	for(int i = 0 ; i < v.size() ; i++){
		myfile << i << " " << v[i] << "\n";
	}
	myfile.close();
}


// NEEDS TO BE MOVED TO BE FIXED
// void graph_degree_cc(){
// 	ofstream myfile;
// 	myfile.open("graph_degree_cc.dat");
// 	map<int,float>::iterator it;
// 	for(it = degree_cc_top.begin(); it != degree_cc_top.end() ; it++){
// 		int deg = it->first;
// 		float v = it->second;
// 		v = v / (float)degrees_top[deg];
// 		myfile << deg << " ";
// 		myfile << v << "\n";
// 	}
// 	myfile.close();
// }
















