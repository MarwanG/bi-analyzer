#ifndef CAL_H_
#define CAL_H_


#include <vector>
#include <iostream>
#include <sstream>
#include <map>



#include "node.h"
#include "print_tools.h"
#include "graph.h"
#include "base.h"


void file2data_PCAP_batch(std::string name,std::vector<std::string> channels,Graph * g);
void get_stat_pcap_batch(std::vector<std::string> names,std::vector<int> nbChannels);
void file2dataPCAP_interval(std::ifstream * file,std::vector<std::string> channels,int interval,Graph *g);
void get_stat_pcap_interval(std::vector<std::string> names,std::vector<int> nbChannels,int interval);
std::map<std::string,float> get_ecart_type(std::map<std::string,std::vector<int> > list);


#endif