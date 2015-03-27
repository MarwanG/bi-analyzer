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


void read_write(set<string> list,string name,string new_name){
	ifstream file(name.c_str());
	ofstream myfile(new_name);
	string str,b,t,time_str;
	while (getline(file, str))
    {
    	istringstream iss(str);
    	iss >> time_str;
    	iss >> time_str;
    	iss >> b;  
    	iss >> t;
        size_t n = count(b.begin(), b.end(), '.');
        if(n==4){
            unsigned found = b.find_last_of(".");
            b = b.substr(0,found);
        }
        n = std::count(t.begin(), t.end(), '.');
        if(n==4){
            unsigned found = t.find_last_of(".");
            t = t.substr(0,found);
        }
        if(list.find(b) != list.end() || list.find(t) != list.end()){
             myfile << str << "\n";   
        }
    }
    myfile.close();
    file.close();
}


set<string> get_filter(string filter){
    ifstream file(filter.c_str());
    string str;
    string b;
    string t;
    set<string> res;
    while (getline(file, str))
    {
        istringstream iss(str);
        iss >> b;  
        iss >> t;
        res.insert(t);
    }
    file.close();
    return res;
}

int main(int argc,char* argv[]){
    string filter = argv[1];
    string file  = argv[2];
    string new_file = argv[3];
    set<string> filters = get_filter(filter);
    read_write(filters,file,new_file);
}

