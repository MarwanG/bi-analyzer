#ifndef NODE_H_   /* Include guard */
#define NODE_H_

#include <vector>
#include <string>
#include <map>
#include <set>
#include <utility>
#include <unordered_map>






class Node
{
public:

	

 	bool operator< (const Node& n) const {
    	return index < n.index;
  	}


	Node(std::string name);
	bool addneighbours(Node* son);
	double get_degree();
	void add_cc(float new_cc);
	void add_cc_min(float new_cc);
	void add_cc_max(float new_cc);
	float get_cc ();
	float get_cc_min ();
	float get_cc_max ();
	void set_red(float r);
	float get_red();
	// float get_red_norm();
	void calculate_disp();
	void set_cc(float cc_new);
	void set_cc_min(float cc_new);
	void set_cc_max(float cc_new);
	// void add_ping(Node *son,std::string s);
	void add_pack_up(Node*son,int size_pack);
	void add_pack_down(Node*son,int size_pack);
	void calculate_ref();
	void calculate_redundancy();
	float get_disp();
	void set_index(int i);
	int get_index();
	int get_total_up();
	int get_total_down();
	
	std::string get_title();

	int nb_top_neighbours;
	int total_packs;

	std::set <int>   neighbours_indexs ;
	std::unordered_map<int,Node*> neighbours;

	std::map<int,std::vector<double> > freq_ping;
	
	
	std::map<int,std::string> freq_last_time;
	
	std::map<int,int> size_pack_list_total;
	std::map<int,std::vector<double> > size_pack_list;
	
	std::map<int,int> size_pack_list_total_down;
	std::map<int,std::vector<double> > size_pack_list_down;

	std::map<int,std::vector<int> > size_pack_list_total_detail;

	std::set<std::pair<int,int> > pairs;


	std::string first_appear; 
	std::string last_appear;
	unsigned long dur;
	unsigned long max_interval;
	int max_packet;
	long long nb_video_packs;
	long long nb_signalling_packs;

	long long video_packs;
	long long signal_packs;




	private:
		double degree;
		float cc;
		float cc_max;
		float cc_min;
		double red;
		float disp;
		int index;
		std::string title;
		
};



#endif
