#include "parser.h"
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

// mode 1 for interval and mode 0 for batch
void Parser::set_mode(string value){
	if(value.compare("interval")==0){
		mode = 1;
	}else{
		mode = 0;
	}
}

void Parser::set_period(string period_string){
	period = atoi(period_string.c_str());
}


void Parser::set_type(string value){
	file_type = value;
}


void Parser::set_files(string value){
	value.erase(std::remove(value.begin(), value.end(),'['), value.end());
	value.erase(std::remove(value.begin(), value.end(),']'), value.end());
	value.erase(std::remove(value.begin(), value.end(),'}'), value.end());
	value.erase(std::remove(value.begin(), value.end(),'{'), value.end());
	value.erase(std::remove(value.begin(), value.end(),'"'), value.end());
	string tmp;
	stringstream iss(value);
	while(iss){
		getline(iss,tmp,',');
		if(isdigit(*tmp.c_str())){
			break;
		}
		files.push_back(tmp);
		if(file_type.compare("pcap")==0){
			getline(iss,tmp,',');
			nbChannels.push_back(atoi(tmp.c_str()));
		}
	}

}


Parser::Parser(string file_name){
	// defaults
	period = 600;
	mode = 1;
	file_type = "pcap";

	ifstream file(file_name.c_str());
	string str;
	string label;
	string value;
	while(getline(file,str)){
		if(str.at(0)=='#'&&str.at(1)=='#'){
			continue;
		}
		stringstream iss(str);
		getline(iss,label,':');
		if(label.compare("mode")==0){
			iss >> value;
			set_mode(value);
		}else if(label.compare("period")==0){
			iss >> value;
			set_period(value);
		}else if(label.compare("file_type")==0){
			iss >> value;
			set_type(value);
		}else if(label.compare("files")==0){
			iss >> value;
			set_files(value);
		}
	}
}



