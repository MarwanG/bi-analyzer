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
        // cout << t << "  " << b << "\n";
        // container.find(element) != container.end();
        if(list.find(b) != list.end() || list.find(t) != list.end()){
             myfile << str << "\n";   
        }
    }
    myfile.close();
    file.close();
}

// GETTING LIST OF CHANNELS FROM FILE.
void get_channels(string name,map<string,set<string> > *list){
	ifstream file(name.c_str());
	string str;
	string b;
	string t;
	string time_str;
	int i = 0;

	while (getline(file, str))
    {
    	i++;
    	if(i%3000000 == 0){
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
    	n = std::count(b.begin(), b.end(), '.');
        size_t n_t = std::count(t.begin(), t.end(), '.');
    	if(n == 3 && n_t == 3){
    		map<string,set<string> >::iterator it = list->find(b);
    		if(it != list->end()){
					list->operator[](b).insert(t);
		    }else{
                    std::set<string> tmp;
                    tmp.insert(t);
					list->operator[](b)=tmp;
			}
            it = list->find(t);
            if(it != list->end()){
                    list->operator[](t).insert(b);
            }else{
                    set<string> tmp;
                    tmp.insert(b);
                    list->insert(make_pair(t, tmp));
            }
		}
    }
}

int main(int argc,char* argv[]){
	// if(argc > 1){
		// cout << argv[1] << "\n";
    string f1 = "/data2/ghanem/same_time/PC_E_edit.txt";
    string f2 = "/data2/ghanem/same_time/PC_B_edit.txt";
    string f3 = "/data2/ghanem/orginal/PC_F.txt";
    string f4 = "/data2/ghanem/same_time/PC_C_edit.txt";
    string f5 = "/data2/ghanem/same_time/PC_A_edit.txt";

	map<string,set<string> > *list = new map<string,set<string> >;
    
    cout << f1 << "\n";
    get_channels(f1,list);
    cout << f2 << "\n";
    get_channels(f2,list);
    cout << f3 << "\n";
    get_channels(f3,list);
    cout << f4 << "\n";
    get_channels(f4,list);
    cout << f5 << "\n";
    get_channels(f5,list);
   
    set<string> list_tmp;
    map<string,set<string> >::iterator it;
    ofstream myfile;
    myfile.open("to_remove.txt");
    for(it = list->begin() ; it != list->end() ; it++){
        if(it->second.size() == 1){
            cout << it->first << "\n";
            myfile << it->first << "\n";
            list_tmp.insert(it->first);
        }
    }
    myfile.close();
}

