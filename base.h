#ifndef BASE_H_
#define BASE_H_


#include <map>
#include <vector>
#include "node.h"
#include "print_tools.h"
#include "graph.h"



void calculate_cc(Node* n1,Node* n2);
void calculate_stat_graph(Graph * g);
void addlink(Graph *g,std::string t ,std::string b,std::string *t1);
void file2data(std::string name,Graph *g);
void get_stat(std::string name);
std::vector<std::string> get_channels(std::string name,int nbChannels,std::map<std::string,int> list);



#endif