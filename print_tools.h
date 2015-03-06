#ifndef Print_Tools_H_  /* Include guard */
#define	Print_Tools_H_


#include "graph.h"
#include "node.h"

time_t timestamp_to_ctime(const char* time_stamp );

void stat_each_node(std::vector<Node*> data,std::string name);
void graph_degree(std::map<int,int> data,int max,int size,std::string name);
void graph_redundancy(std::map<float,int> data,int size,std::string name);
void graph_cc(std::map<float,int> data,int size,std::string name);
void stat_to_file(Graph *g,int nb_top);
void stat_to_stdout(Graph *g);
void create_graph_float(std::vector<float> v,std::string name);
void create_graph_int(std::vector<int> v,std::string name);

#endif