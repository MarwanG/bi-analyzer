#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <set>
#include <map>
#include <time.h>
#include "node.h"
using namespace std;


time_t timestamp_to_ctime(const char* time_stamp )
{
   time_t _return;
   struct tm tm_struct ; 
   strptime(time_stamp ,"%Y-%m-%d %H:%M:%S",&tm_struct);
   _return  = mktime(&tm_struct);   return
   _return;
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




















