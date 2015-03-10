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


void read_write(map<string,int> list,string name,string new_name){
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
        // cout << t << "  " << b << "\n";
    	if(list[b] != 1 && list[t] != 1){
    		myfile << str << "\n";
    	}
	}
    myfile.close();
    file.close();
}

// GETTING LIST OF CHANNELS FROM FILE.
map<string,int> get_channels(string name){
	map<string,int> list;
	ifstream file(name.c_str());
	string str;
	string b;
	string t;
	string time_str;
	int i = 0;

	while (getline(file, str))
    {
    	i++;
    	if(i%1000000 == 0){
    		cout << str << "\n";
    		i=0;
    	}
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
    	// if (my_own_regex(b)){
    		n = std::count(b.begin(), b.end(), '.');
    		if(n == 3){
    			map<string,int>::iterator it = list.find(b);
    			if(it != list.end()){
					list[b]=list[b] +1;
				}else{
					list[b]=1;
				}
    		}
    	// }
    	// if (my_own_regex(t)){
    		n = std::count(t.begin(), t.end(), '.');
    		if(n == 3){
    			map<string,int>::iterator it = list.find(t);
    			if(it != list.end()){
					list[t]=list[t] +1;
				}else{
					list[t]=1;
				}
    		}
    	// }	
    }
    return list;
}

int main(int argc,char* argv[]){
	if(argc > 1){
		cout << argv[1] << "\n";
		map<string,int> list = get_channels(argv[1]);
        map<string,int>::iterator it;
        for(it = list.begin() ; it != list.end() ; it++){
            if(it->second == 1){
                cout << it->first << "  " << it->second << "\n";
            }
        }
		read_write(list,argv[1],argv[2]);
	}
}
