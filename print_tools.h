#ifndef Print_Tools_H_  /* Include guard */
#define	Print_Tools_H_


void stat_each_node(vector<Node*> data,string name);
void graph_degree(map<int,int> data,int max,int size,string name);
void graph_redundancy(map<float,int> data,int size,string name);
void graph_cc(map<float,int> data,int size,string name);

#endif