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
               
        }else{
             myfile << str << "\n";
        }
    }
    myfile.close();
    file.close();
}

// GETTING LIST OF CHANNELS FROM FILE.
map<string,set<string> > get_channels(string name){
	map<string,set<string> > list;
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
    	n = std::count(b.begin(), b.end(), '.');
        size_t n_t = std::count(t.begin(), t.end(), '.');
    	if(n == 3 && n_t == 3){
    		map<string,set<string> >::iterator it = list.find(b);
    		if(it != list.end()){
					list[b].insert(t);
		    }else{
                    std::set<string> tmp;
                    tmp.insert(t);
					list[b]=tmp;
			}
            it = list.find(t);
            if(it != list.end()){
                    list[t].insert(b);
            }else{
                    set<string> tmp;
                    tmp.insert(b);
                    list.insert(make_pair(t, tmp));
            }
		}
    }
    return list;
}

int main(int argc,char* argv[]){
	if(argc > 1){
		cout << argv[1] << "\n";
		map<string,set<string> > list = get_channels(argv[1]);
        set<string> list_tmp;
        map<string,set<string> >::iterator it;
        for(it = list.begin() ; it != list.end() ; it++){
            if(it->second.size() == 1){
                list_tmp.insert(it->first);
            }else{
                // list_tmp.insert(it->first);
                // cout << it->first << "   " << it->second.size() << "\n";
            }
        }
        // cout << "ici \n";
        // std::set<string>::iterator it1;
        // for (it1 = list_tmp.begin(); it1 != list_tmp.end(); ++it1)
        // {
        //    cout << *it1 << "  " << list[*it1].size() << "\n  ";
        //    set<string>::iterator it2;
        //    set<string> tmp = list[*it1];
        //    for (it2 = tmp.begin(); it2 != tmp.end(); ++it2)
        //   {
        //     cout << " == " << *it2;
        //   }
        //   cout << "\n";
        // }
		read_write(list_tmp,argv[1],argv[2]);
	}
}
