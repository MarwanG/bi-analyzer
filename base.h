#ifndef BASE_H_
#define BASE_H_


#include <map>
#include <vector>
#include "node.h"
#include "print_tools.h"
#include "graph.h"



void calculate_cc(Node* n1,Node* n2);
void calculate_stat_graph(Graph * g);
void addlink(Graph *g,std::string t ,std::string b,std::string *t1,int size_pack);
void file2data(std::string name,Graph *g);
void get_stat(std::string name);
std::vector<std::string> get_channels(std::string name,int nbChannels,std::map<std::string,int> list);
std::map<std::string,std::pair<float,float> > avg_for_each (std::map<std::string,std::vector<int> > list);
std::vector<std::pair<float,float> > avg_nb_for_each (std::map<std::string,std::vector<int> > list);
#endif