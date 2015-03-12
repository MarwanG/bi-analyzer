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

#include "node.h"
#include "print_tools.h"
#include "graph.h"
#include "parser.h"

using namespace std;


int limit = 70;
// STATS MIGHT BE GOOD IDEA TO PASS THEM NOT SO SURE YET.


map<float,int> redundancys_top;
map<float,int> redundancys_bot;
map<float,int> ccs_top;
map<float,int> ccs_bot;
map<int,float> degree_cc_top; 
map<int,float> degree_cc_bot; 








// FUNCTION TO UPDATE REDUNDANCY TOP AND BOT

void update_redundancy_top(Node * n){
	map<float,int>::iterator it = redundancys_top.find(n->get_red());
	if(it != redundancys_top.end()){
		redundancys_top[n->get_red()]=redundancys_top[n->get_red()] +1;
	}else{
		redundancys_top[n->get_red()]=1;
	}
}

void update_redundancy_bot(Node * n){
	map<float,int>::iterator it = redundancys_bot.find(n->get_red());
	if(it != redundancys_bot.end()){
		redundancys_bot[n->get_red()]=redundancys_bot[n->get_red()] +1;
	}else{
		redundancys_bot[n->get_red()]=1;
	}
}

// FUNCTION TO UPDATE CCS TOP AND BOT

void update_ccs_top(Node *n){
	map<float,int>::iterator it = ccs_top.find(n->get_cc());
	if(it != ccs_top.end()){
		ccs_top[n->get_cc()]=ccs_top[n->get_cc()] +1;
	}else{
		ccs_top[n->get_cc()]=1;
	}
}

void update_ccs_bot(Node *n){
	map<float,int>::iterator it = ccs_bot.find(n->get_cc());
	if(it != ccs_bot.end()){
		ccs_bot[n->get_cc()]=ccs_bot[n->get_cc()] +1;
	}else{
		ccs_bot[n->get_cc()]=1;
	}
}

// FUNCTION TO UPDATE CCS/DEGREE TOP AND BOT

void update_degree_cc(Node *n){
	map<int,float>::iterator it = degree_cc_top.find(n->get_degree());
	if(it != degree_cc_top.end()){
		degree_cc_top[n->get_degree()] = degree_cc_top[n->get_degree()] + n->get_cc();
	}else{
		degree_cc_top[n->get_degree()] = n->get_cc();
	}
}

// FUNCTION CALCULATES CC/REDUNDANCY BETWEEN TWO NODES.

void calculate_cc(Node* n1,Node* n2){
	int max_size = max(n1->get_degree(),n2->get_degree());
	int min_size = min(n1->get_degree(),n2->get_degree());
	int total_size = n1->get_degree()+n2->get_degree();
	float common = 0;
	vector<int> tmp;
    std::set_intersection(n1->neighbours_indexs.begin(), n1->neighbours_indexs.end(),
                          n2->neighbours_indexs.begin(), n2->neighbours_indexs.end(),
                          std::back_inserter(tmp));
    common = tmp.size();
	if(common > 0){
	 	for(int i = 0 ; i < tmp.size() ; i++){
	 		for(int j = i+1 ; j < tmp.size() ; j++){
	 			n1->pairs.insert(make_pair(tmp[i],tmp[j]));
	 			n2->pairs.insert(make_pair(tmp[i],tmp[j]));
	 		}
	 	}	
	 	total_size-=common;
		float cc_node = common/total_size;
		float cc_min_node = common/min_size;
		float cc_max_node = common/max_size;
		n1->add_cc(cc_node);
		n1->add_cc_min(cc_min_node);
		n1->add_cc_max(cc_max_node);
		n2->add_cc(cc_node);
		n2->add_cc_min(cc_min_node);
		n2->add_cc_max(cc_max_node);
		n1->nb_top_neighbours = n1->nb_top_neighbours + 1;
		n2->nb_top_neighbours = n2->nb_top_neighbours + 1;
	}
}

// FUNCTION TO CALCULATES STATS FOR WHOLE GRAPH.
void calculate_stat_graph(Graph * g){
	for(int i = 0 ; i < g->tops.size() ; i++){
		Node *n = g->tops[i];
		set<int> treated;
		set<int>::iterator it;
		for(it =  n->neighbours_indexs.begin() ; it != n->neighbours_indexs.end() ; it++){
			Node *bo = n->neighbours[*it];
			set<int>::iterator it2;
			for(it2 = bo->neighbours_indexs.begin() ; it2 != bo->neighbours_indexs.end() ; it2++){
				if(bo->get_degree() == 1){
                    cout << bo->neighbours[*it2]->get_title() << "\n";
                }
                Node *tmp = bo->neighbours[*it2];
				int index_x = g->topsIndex.find(tmp->get_title())->second;
				if(i < index_x && find(treated.begin(), treated.end(), index_x)==treated.end() && n!= tmp){
					calculate_cc(n,tmp);
					treated.insert(index_x);
				}
			}
		}
		
		if(n->nb_top_neighbours > 0){
			n->set_cc(n->get_cc()/n->nb_top_neighbours);			
			g->cc += n->get_cc();
			g->cc_min+= (n->get_cc_min()/n->nb_top_neighbours);
			g->cc_max+= (n->get_cc_max()/n->nb_top_neighbours);
		}

		n->calculate_disp();
	 	n->calculate_redundancy();
		g->update_degree_top(n);
		update_redundancy_top(n);
		update_ccs_top(n);
		update_degree_cc(n);

		g->average_degree_top_v = g->average_degree_top_v + n->get_degree();
		if(n->get_degree() > g->max_top){
			g->max_top=n->get_degree();
		}
	 	if(n->get_disp() != -1){
	 		g->disp = g->disp + n->get_disp();
	 		g->nb_disp = g->nb_disp + 1;
		}
		if(n->get_red() != -1){
			g->red = g->red + n->get_red();
			g->nb_red = g->nb_red + 1;
		}
	}
	g->final_calculation();
}

// FUNCTION ADD CONNECTION
void addlink(Graph *g,string t , string b){
	unordered_map<string, int>::const_iterator p;
	p = g->topsIndex.find(t);
	Node *top;
	if(p == g->topsIndex.end()){
		top = new Node(t);
		g->tops.push_back(top);
		top->set_index(g->tops.size()-1);
		g->topsIndex[t] = g->tops.size()-1;
	}else{
		top =g->tops[p->second];
	}
	unordered_map<string, int>::const_iterator p2;
	p2 = g->botsIndex.find(b);
	Node * bot;
	if(p2 == g->botsIndex.end()){
		bot = new Node(b);
		g->bots.push_back(bot);
		bot->set_index(g->bots.size() -1);
		g->botsIndex[b] = g->bots.size()-1;
	}else{
		bot = g->bots[p2->second];
	}
	bool bTop = top->addneighbours(bot);
 	bool bBot = bot->addneighbours(top);
    top = NULL;
    bot = NULL;
    delete top;
    delete bot;
    if(bTop && bBot){
   	 	g->links = g->links + 1;
   	}
}

// FILE TO DATA
void file2data(string name,Graph *g){
	ifstream file(name.c_str());
	string str; 
	string t;
	string b;

	while (getline(file, str))
    {
    	istringstream iss(str);
    	iss >> b;
    	iss >> t;
    	addlink(g,t,b);
    }
    g->density = g->links / (float)(g->tops.size()*g->bots.size());
    file.close();
}

// GET STAT
void get_stat(string name){
	Graph * g = new Graph();
	file2data(name,g);
	calculate_stat_graph(g);
	stat_to_stdout(g);
 	stat_to_file(g);
 	g->free_data();
 	delete(g);
}


// GETTING LIST OF CHANNELS FROM FILE.
vector<string> get_channels(string name,int nbChannels,map<string,int> list){
	ifstream file(name.c_str());
	string str;
	string b;
	vector<string> tmp;
	string time_str;
	while (getline(file, str) && tmp.size() < nbChannels)
    {
        istringstream iss(str);
    	iss >> time_str;
    	iss >> time_str;
    	iss >> b;  
    	size_t n = count(b.begin(), b.end(), '.');
    	if(n==4){
    		unsigned found = b.find_last_of(".");
	    	b = b.substr(0,found);
    	}
    	if (my_own_regex(b) && (find(tmp.begin(), tmp.end(), b)==tmp.end())){
    		size_t n = std::count(b.begin(), b.end(), '.');
    		if(n == 3){
    			map<string,int>::iterator it = list.find(b);
    			if(it != list.end()){
					list[b]=list[b] +1;
				}else{
					list[b]=1;
				}
				if (list[b] > limit){
					tmp.push_back(b);
				}
    		}
    	}	
    }
 	return tmp;
}


void file2data_PCAP_batch(string name,vector<string> channels,Graph * g){
	ifstream file(name.c_str());
	string str; 
	string t;
	string b;
	string time_str;

	int z =  0;
	while (getline(file, str))
    {
    	istringstream iss(str);
    	iss >> time_str;
    	iss >> time_str;
    	iss >> b;
    	iss >> t;
    	// cout << b << "   " << t << "\n";
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
		if(my_own_regex(b)&&find(channels.begin(), channels.end(), b) == channels.end()){
			continue;
		}
		if(my_own_regex(t)&&find(channels.begin(), channels.end(), t) == channels.end()){
			continue;
		}
	    if(find(channels.begin(), channels.end(), b)!=channels.end() && find(channels.begin(), channels.end(), t)==channels.end()){
	    		addlink(g,b,t);
	    }else{		
		    if(find(channels.begin(), channels.end(), t)!=channels.end() && find(channels.begin(), channels.end(), b)==channels.end()){
		   			addlink(g,t,b);
		    }
		}
    }
    cout << "nb of links : " << g->links  << " \n";
    cout << "size*size   : " << (float)(g->tops.size()*g->bots.size()) << "\n";
    g->density = g->links / (float)(g->tops.size()*g->bots.size());
    file.close();
}

void get_stat_pcap_batch(vector<string> names,vector<int> nbChannels){
	map<string,int> list;
	Graph * g = new Graph();
	for(int i = 0 ; i < names.size() ; i++){
		vector<string> v =  get_channels(names[i],nbChannels[i],list);
		for(int z = 0 ; z < v.size() ; z++){
			cout << v[z] << "\n";
		}
		file2data_PCAP_batch(names[i],v,g);
	}
	for(int i = 0 ; i < g->tops.size() ; i++){
		cout << g->tops[i]->get_title() << "\n";
	}	
	stat_to_stdout(g);
	calculate_stat_graph(g);
	stat_to_file(g);
	create_graph_int_map(g->degree_bot,"dist_degree_bot.stat");
	g->free_data();
	delete(g);
}


void file2dataPCAP_interval(ifstream * file,vector<string> channels,int interval,Graph *g){
	string str; 
	string t;
	string b;
	string time_str;
	string tmp;
	struct tm tm;
	time_t t1 = 4;
	time_t t2 = 4;
	while (getline(*file, str))
    {
    	istringstream iss(str);
    	iss >> time_str;
    	iss >> tmp;
    	time_str.append(" " + tmp);
    	iss >> b;
    	iss >> t;
    	if(count(b.begin(), b.end(), '.') > 2 &&  count(t.begin(), t.end(), '.') > 2){
	    	if(t1 == 4){
	    		t1 = timestamp_to_ctime(time_str.c_str());
	    		g->set_time(tmp);
	    		// cout << time_str << "\n";
	    	}else{    		
	    		t2 = timestamp_to_ctime(time_str.c_str());
	    		double diff = difftime(t2,t1);
	    		if(diff > interval){
	    			break;
	    		}
	    	}
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
		    if(find(channels.begin(), channels.end(), b)!=channels.end()  && find(channels.begin(), channels.end(), t)==channels.end()){
		    		addlink(g,b,t);
		    }else{		
			    if(find(channels.begin(), channels.end(), t)!=channels.end() && find(channels.begin(), channels.end(), b)==channels.end()){
			   		addlink(g,t,b);
			    }
			}
	    }
	}
	g->density = g->links / (float)(g->tops.size()*g->bots.size());
}



void get_stat_pcap_interval(vector<string> names,vector<int> nbChannels,int interval){
	vector<string> channels;
	map<string,int> list;
	vector<float> cc_graph;
	vector<float> degree_graph;
	vector<int> nb_bot_graph;
	vector<string> times;

	// get all channels
	
	for(int i = 0 ; i < names.size() ; i++){
		vector<string> tmp = get_channels(names[i],nbChannels[i],list);
		channels.insert(channels.end(), tmp.begin(), tmp.end());
	}

    for(int i = 0 ; i < channels.size() ; i++){
        cout << channels[i] << "\n";
    }
	//opening all the files.

	vector<ifstream*> files;
	for(int i = 0 ; i < names.size() ; i++){
		ifstream * file = new ifstream(names[i].c_str());
		files.push_back(file);
	}

	//going through each file for interval seconds.
	bool keep = true;
	int index = 0;
	float lowest_density = 1.0;
	float highest_density = 0.0;
	Graph * g_lowest;
	Graph * g_highest;


	while(keep){
		Graph * g = new Graph();
		for(int i = 0 ; i < files.size() ; i++){
			file2dataPCAP_interval(files[i],channels,interval,g);
			if(files[i]->eof() != 0){
				keep = false;
				break;
			}
		}
		calculate_stat_graph(g);
		stat_to_stdout(g);
		times.push_back(g->time_);
		cc_graph.push_back(g->cc);
		degree_graph.push_back(g->density);
		nb_bot_graph.push_back(g->bots.size());
		if(g->density > highest_density){
			highest_density = g->density;
			g_highest = g;
			g = NULL;
		}else if(g->density < lowest_density){
			lowest_density = g->density;
			g_lowest = g;
			g = NULL;
		}else{
			g->free_data();
			delete(g);
		}
	}
	vector<Graph *> peaks;
	peaks.push_back(g_lowest);
	peaks.push_back(g_highest);
	stats_to_file_interval(peaks,"peaks.stat");
	g_lowest->free_data();
	delete(g_lowest);
	g_highest->free_data();
	delete(g_highest);


	stringstream stream1;
	stream1 << interval;
	string interval_string = stream1.str();

	create_graph_float(cc_graph,times,"cc_interval_"+current_time()+".stat");
	create_graph_float(degree_graph,times,"density_interval_"+current_time()+".stat");
	create_graph_int(nb_bot_graph,times,"nb_bot_interval_"+current_time()+".stat");
}


int main(int argc, char* argv[]){
	if(argc > 1){
		cout << argv[1] << "\n";
		get_stat(argv[1]);
	}else{
		vector<int> nbChannels;
		vector<string> list;

		//list.push_back("../Data/Japon2013/SIT-exp131219/PC_A/PC_A_short_new.txt");
		// list.push_back("../Data/Japon2013/SIT-exp131219/PC_E/PC_E.txt");
        
           list.push_back("/data2/ghanem/tmp/PC_A_new.txt");
           list.push_back("/data2/ghanem/tmp/PC_B_new.txt");
           list.push_back("/data2/ghanem/tmp/PC_C_new.txt");
           list.push_back("/data2/ghanem/tmp/PC_D_new.txt");
           list.push_back("/data2/ghanem/tmp/PC_E_new.txt");
       // list.push_back("/data2/ghanem/same_time/PC_A_edit.txt");
  //   /   list.push_back("/data2/ghanem/same_time/PC_B_edit.txt");
 //       list.push_back("/data2/ghanem/same_time/PC_C_edit.txt");
//        list.push_back("/data2/ghanem/same_time/PC_D_edit.txt");
  //      list.push_back("/data2/ghanem/same_time/PC_E_edit.txt");
//        list.push_back("/data2/ghanem/same_time/PC_F.txt");

//		list.push_back("/data2/ghanem/PC_A_new.txt");
//		list.push_back("/data2/ghanem/PC_B_edit_no_degree_1.txt");
//		list.push_back("/data2/ghanem/PC_C_edit_no_degree_1.txt");		
//		list.push_back("/data2/ghanem/PC_D_edit_no_degree_1.txt");
		//list.push_back("/data2/ghanem/PC_E_new.txt");
//		list.push_back("/data2/ghanem/PC_F_new.txt");

		nbChannels.push_back(3);
		nbChannels.push_back(1);
		nbChannels.push_back(3);
		nbChannels.push_back(1);
		nbChannels.push_back(1);
		nbChannels.push_back(3);

//        get_stat_pcap_interval(list,nbChannels,60);
		get_stat_pcap_batch(list,nbChannels);
	}
	// Parser p = Parser(argv[1]);

	// cout << "MODE :: " << p.mode << "\n";
	// cout << "File_type :: " << p.file_type << "\n";
	// cout << "Period :: " << p.period << "\n";
	// cout << "Files  :: \n"
	// for(int i = 0 ; i < p.files.size() ; i++){
	// 	cout << p.files[i] << "\n";
	// }

	// if(p.file_type.compare("simple")){
	// 	get_stat(files[0]);
	// }else{
	// 	if(p.mode == 1){
	// 		// interval
	// 		get_stat_pcap_interval(p.files,p.nbChannels,p.period);
	// 	}else{
	// 		get_stat_pcap_batch(p.files,p.nbChannels);
	// 	}
	// }


}
