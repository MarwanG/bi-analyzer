#ifndef Print_Tools_H_  /* Include guard */
#define	Print_Tools_H_


#include "graph.h"
#include "node.h"

time_t timestamp_to_ctime(const char* time_stamp );
void split(const std::string& s, char c,std::vector<std::string>& v);
bool my_own_regex(std::string s);
std::string current_time();


void stat_each_node(std::vector<Node*> data,std::string name);
void graph_degree(std::map<int,int> data,int max,int size,std::string name);
void graph_redundancy(std::map<float,int> data,int size,std::string name);
void graph_cc(std::map<float,int> data,int size,std::string name);
void stat_to_file(Graph *g);
void stat_to_stdout(Graph *g);
void create_graph_float(std::vector<float> v,std::vector<std::string> s,std::string name);
void create_graph_int(std::vector<int> v,std::vector<std::string> s,std::string name);
void stats_to_file_interval(std::vector<Graph*> list,std::string name);
void create_graph_int_map(std::map<int,int> degrees_bot,std::string name);
void create_graph_nb_bot(std::vector<int> v,std::vector<std::string> time,std::vector<std::string> s,std::string name);
void create_graph_string(std::vector<std::string> v,std::vector<std::string> s,std::string name);
void create_graph_map(std::map<std::string,float> list,std::string name);
void create_graph_2_map(std::map<std::string,long double> list,std::map<std::string,long double> list2,std::string name);
void create_graph_map_int_float(std::map<int,float>degrees_bot,std::string name);
void create_graph_pairs(std::vector<std::pair<float,float> > list,std::string name);
void create_graph_degree_change(std::map<int,std::vector<int> > nb_each_degree,std::map<int,std::vector<int> > diff_nb_each_degree,std::vector<std::string> times,std::string name);
void create_graph_vector_vector_int(std::vector<std::vector<int> > list,std::vector<std::string> times,std::string name);


#endif
