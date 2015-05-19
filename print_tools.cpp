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



time_t timestamp_to_ctime(const char* time_stamp ){
   time_t _return;
   struct tm tm_struct ; 
   strptime(time_stamp ,"%Y-%m-%d %H:%M:%S",&tm_struct);
   _return  = mktime(&tm_struct);   
   return _return;
}

double time_diff(string prev_time,string s){
	string t1_new = prev_time.substr(0,prev_time.find('.'));
	string t2_new = s.substr(0,s.find('.'));
	cout << prev_time << "  " << s << "\n";

	time_t t1 = timestamp_to_ctime(prev_time.c_str());
	time_t t2 = timestamp_to_ctime(s.c_str());
	double t = difftime(t2,t1);

	string t1_str_milli = s.substr(s.find('.')+1,s.length());
	if(t1_str_milli[0]=='0'){
	  t1_str_milli = s.substr(s.find('.')+2,s.length());
	}
	string t2_str_milli = prev_time.substr(prev_time.find('.')+1,prev_time.length());
	if(t2_str_milli[0]=='0'){
	  t2_str_milli = prev_time.substr(prev_time.find('.')+2,prev_time.length());
	}

	int t1_milli = atoi(t1_str_milli.c_str());
	int t2_milli = atoi(t2_str_milli.c_str());
	int diff_milli_tmp = abs(t1_milli-t2_milli);
 	cout << t << "  " << diff_milli_tmp << "\n";
	t = t * 1000000;
	t = t + diff_milli_tmp;
	cout << "final t : " << t << "\n";
	return t;
}

string current_time(){
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time (&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer,80,"%d-%m-%Y %I:%M:%S",timeinfo);
	string str(buffer);
	return str;
}

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


void stat_to_file(Graph *g,string name){
	int nb_top = g->degrees_top[g->max_top];
	ofstream myfile;
	myfile.open (name);
	myfile.precision(6);
	if(!g->time_.empty()){
		myfile << "Start Time : " << g->time_ << "\n";
	}
	myfile << "Number of tops : " << g->tops.size() << "\n";
	myfile << "Number of bots : " << g->bots.size() << "\n";
	myfile << "Number of edges : " << fixed << g->links << "\n";
	myfile << "Density : " << fixed << g->density << "\n";
	myfile << "average_degree_top_v :" << g->average_degree_top_v << "\n";
	myfile << "average_degree_bot_v :" << g->average_degree_bot_v << "\n";
	myfile << "Max degree (top) : " << g->max_top << " (" << nb_top << ")\n";
	myfile << "Min degree (bot) : " << g->min_bot << "\n";
	myfile << "Clustering coefficient cc_top : " << g->cc << "\n";
	myfile << "Clustering coefficient cc_min_top : " << g->cc_min << "\n";
	myfile << "Clustering coefficient cc_max_top : " << g->cc_max << "\n";
	myfile << "Redundancy coefficient (top) : " << g->red << "\n";
	myfile << "Dispersion coefficient (top) : " << g->disp << "\n";
	for(int i = 0 ; i < g->tops.size() ; i++){
		myfile << "Degree : " << i << "  " << g->distr_by_degree[i].size() << "\n";
	}
	for(int i = 0 ; i < g->tops.size() ; i++){
		myfile << "Number of multi-peers : " << g->tops[i]->get_title() << " " << g->nb_multi_peers_per_channel[g->tops[i]->get_title()] << "\n";
	}
	for(int i = 0 ; i < g->tops.size() ; i++){
		myfile << "Number of peers : " << g->tops[i]->get_title() << " " << g->tops[i]->get_degree() << "\n";
	}
	myfile.close();
}

void stat_to_stdout(Graph *g){
	if(!g->time_.empty()){
		cout << "Start Time : " << g->time_ << "\n";
	}
	cout << "Number of tops : " << g->tops.size() << "\n";
	cout << "Number of bots : " << g->bots.size() << "\n";
	cout << "Number of edges : " << g->links << "\n";
	cout << "Density : " << fixed << g->density << "\n";
}


void create_graph_int(vector<int> v,vector<string> s,string name){
	ofstream myfile;
	myfile.open (name);
	for(int i = 0 ; i < v.size() ; i++){
		myfile << s[i] << " " << v[i] << "\n";
	}
	myfile.close();
}

void create_graph_float(vector<float> v,vector<string> s,string name){
	ofstream myfile;
	myfile.open (name);
	for(int i = 0 ; i < v.size() ; i++){
		myfile << s[i] << " " << v[i] << "\n";
	}
	myfile.close();
}


void create_graph_long_long(vector<long long> v,vector<string> s,string name){
	ofstream myfile;
	myfile.open (name);
	for(int i = 0 ; i < v.size() ; i++){
		myfile << s[i] << " " << v[i] << "\n";
	}
	myfile.close();
}


void create_graph_string(vector<string> v,vector<string> s,string name){
	ofstream myfile;
	myfile.open (name);
	for(int i = 0 ; i < v.size() ; i++){
		myfile << s[i] << " " << v[i] << "\n";
	}
	myfile.close();
}

void create_graph_int_map(map<int,int>degrees_bot,string name){
	ofstream myfile;
	myfile.open(name);
    map<int,int>::iterator it;
    for(it=degrees_bot.begin();it!=degrees_bot.end();it++){
       cout << it->first << "  " << it->second << "\n";
       myfile << it->first << "  " << it->second << "\n";
    }
    myfile.close();
}

void create_graph_map_int_float(map<int,float>degrees_bot,string name){
	ofstream myfile;
	myfile.open(name);
    map<int,float>::iterator it;
    for(it=degrees_bot.begin();it!=degrees_bot.end();it++){
       myfile << it->first << "  " << it->second << "\n";
    }
    myfile.close();
}


void create_graph_nb_bot(vector<int> v,vector<string> time,vector<string> s,string name){
	ofstream myfile;
	myfile.open(name);
    for(int i = 0 ; i < v.size() ; i++){
    	myfile << time[i] << " " << v[i] << " " << s[i] << "\n";
    }
    myfile.close();
}

void create_graph_map(map<string,float> list,string name){
    ofstream myfile;
    myfile.open(name);
    map<string,float>::iterator it;
    for(it=list.begin();it!=list.end();it++){
        myfile << it->first << "  " << fixed << it->second << "\n";
    }
    myfile.close();
}

void create_graph_2_map(map<string,long double> list,map<string,long double> list2,string name){
    ofstream myfile;
    myfile.open(name);
    map<string,long double>::iterator it;
    for(it=list.begin();it!=list.end();it++){
        myfile << it->first << "  " << fixed << it->second << "  " << fixed << list2[it->first] << "\n";
    }
    myfile.close();
}

void create_graph_2_map_int(map<string,long long> list,map<string,long long> list2,string name){
    ofstream myfile;
    myfile.open(name);
    map<string,long long>::iterator it;
    for(it=list.begin();it!=list.end();it++){
        myfile << it->first << "  " << fixed << it->second << "  " << fixed << list2[it->first] << "\n";
    }
    myfile.close();
}


void create_graph_pairs(vector<pair<float,float> > list,string name){
	ofstream myfile;
    myfile.open(name);
    for(int i = 0 ; i < list.size() ; i++){
    	myfile << list[i].first << "  " << list[i].second << "\n";
    }
    myfile.close();

}


void create_graph_map_pairs(map<string,pair<float,float> > list,map<string,int> list2,string name){
	ofstream myfile;
    myfile.open(name);
    map<string,pair<float,float> >::iterator it;
    for(it=list.begin();it!=list.end();it++){
    	myfile << it->first << "  " << it->second.first << " " << it->second.second << " " << list2[it->first] << "\n";
	}
	myfile.close();
}


void create_graph_map_pairs_pairs(map<string,pair<float,float> > list,map<string,pair<float,float> >list2,string name){
	ofstream myfile;
    myfile.open(name);
    map<string,pair<float,float> >::iterator it;
    for(it=list.begin();it!=list.end();it++){
    	myfile << it->first << "  " << it->second.first << " " << it->second.second << " " << list2[it->first].first << "  " << list2[it->first].second << "\n";
	}
	myfile.close();
}

void create_graph_vector_vector_int(vector<vector<int> > list,vector<string> times,string name){
	ofstream myfile;
	myfile.open(name);
	for(int i = 0 ; i < times.size() ; i++){
		myfile << times[i] << " ";
		for(int j = 0 ; j < list[i].size() ; j++){
			myfile << list[i][j] << " ";
		}
		myfile << "\n";
	}
	myfile.close();
}

void create_graph_degree_change(map<int,vector<int> > nb_each_degree,map<int,vector<int> > diff_nb_each_degree,vector<string> times,string name){
	ofstream myfile;
	myfile.open(name);
	for(int i = 0 ; i < times.size() ; i++){
		myfile << times[i] << " ";
		map<int,vector<int> >::iterator it;
		map<int,vector<int> >::iterator it2;
		it2 = diff_nb_each_degree.begin();
		for(it=nb_each_degree.begin() ; it != nb_each_degree.end() ; it++){
			myfile << it->second[i] << " " << it2->second[i] << " ";
			it2++;
		}
		myfile << "\n";
	}
	myfile.close();
}


void stats_to_file_interval(std::vector<Graph*> list,std::string name){
	ofstream myfile;
	myfile.open(name);
	for(int i = 0 ; i < list.size() ; i++){
		myfile << "Graph : " << i << "\n";
		myfile.precision(6);
		if(!list[i]->time_.empty()){
			myfile << "Start Time : " << list[i]->time_ << "\n";
		}
		myfile << "Number of tops : " << list[i]->tops.size() << "\n";
		myfile << "Number of bots : " << list[i]->bots.size() << "\n";
		myfile << "Number of edges : " << fixed << list[i]->links << "\n";
		myfile << "Density : " << fixed << list[i]->density << "\n";
		myfile << "average_degree_top_v :" << list[i]->average_degree_top_v << "\n";
		myfile << "Max degree (top) : " << list[i]->max_top << "\n";
		myfile << "Clustering coefficient cc_top : " << list[i]->cc << "\n";
		myfile << "Clustering coefficient cc_min_top : " << list[i]->cc_min << "\n";
		myfile << "Clustering coefficient cc_max_top : " << list[i]->cc_max << "\n";
		myfile << "Redundancy coefficient (top) : " << list[i]->red << "\n";
		myfile << "Dispersion coefficient (top) : " << list[i]->disp << "\n";
	}
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
















