#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <map>
#include <regex>

using namespace std;


map< string,set<string> > list;

int main(int argc, char* argv[]){
	if(argc > 1){
		ifstream file(argv[1]);
		string b;
		string str;
		string time_str;
		string t;
		while (getline(file, str)){
			istringstream iss(str);
			iss >> time_str;
    		iss >> time_str;
    		iss >> b;
    		iss >> t;
			if (regex_match (b, regex("(192.168)(.*)")) && (!regex_match (t, regex("(192.168)(.*)")))){
				// map<string,set<string>>::iterator it = list.find(t);
				list[t].insert(b);	
			}
			if(regex_match (t, regex("(192.168)(.*)")) && (!regex_match (b, regex("(192.168)(.*)")))){
				list[b].insert(t);
			}
		}
		map< string,set<string> >::iterator it;
		int nb = 0;
		for(it = list.begin() ; it != list.end() ; it++){
			if(it->second.size() == 1){
				nb++;
			}
		}
		cout << nb << "\n";
		cout << list.size() << "\n";
		file.close();
	}
}

