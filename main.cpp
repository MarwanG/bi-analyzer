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

#include "node.h"
#include "print_tools.h"


using namespace std;

//global defintion variables
int limit = 50;

//DATA
unordered_map<string,int> topsIndex;
unordered_map<string,int> botsIndex;
vector<Node*> tops;
vector<Node*> bots;
// STATS MIGHT BE GOOD IDEA TO PASS THEM NOT SO SURE YET.
map<int,int> degrees_top;
map<int,int> degrees_bot;
map<float,int> redundancys_top;
map<float,int> redundancys_bot;
map<float,int> ccs_top;
map<float,int> ccs_bot;
map<int,float> degree_cc_top; 
map<int,float> degree_cc_bot; 
// global variables
float average_degree_top_v;
float average_degree_bot_v;
int max_top;
int max_bot;
float cc;
float cc_min;
float cc_max;
float red;
float links;
float density;
float disp;


// Functions used in calculation


//FUNCTIONS TO UPDATE DEGREE BOT AND TOP

void update_degree_bot(Node * n){
	map<int,int>::iterator it = degrees_bot.find(n->get_degree());
	if(it != degrees_bot.end()){
		degrees_bot[n->get_degree()]=degrees_bot[n->get_degree()] +1;
	}else{
		degrees_bot[n->get_degree()]=1;
	}
}


void update_degree_top(Node * n){
	map<int,int>::iterator it = degrees_top.find(n->get_degree());
	if(it != degrees_top.end()){
		degrees_top[n->get_degree()]=degrees_top[n->get_degree()] +1;
	}else{
		degrees_top[n->get_degree()]=1;
	}
}

//FUNCTIONS TO CALCULATE AVERAGE AND MAXIMUM DEGREE BOT AND TOP


void average_top(){
	float total = 0;
	float total_disp=0;
	float nb_count = 0;
	float nb_count_red = 0;
	for(int i = 0 ; i < tops.size() ; i++){
		int d = tops[i]->get_degree();
		total += d;
		if(max_top <  d){
			max_top = d;
		}
		if(tops[i]->get_disp() != -1){
			total_disp += tops[i]->get_disp();
			nb_count++;
		}
		if(tops[i]->get_red() != -1){
			red += tops[i]->get_red();
			nb_count_red++;
		}else{
		}
	}
	red = red / nb_count_red;
	total = total / tops.size();
    average_degree_top_v = total;
    disp = total_disp/nb_count;
}


void average_max_degree_bot(){
	float total;
	for(int i = 0 ; i < bots.size() ; i++){
		update_degree_bot(bots[i]);
		int d = bots[i]->get_degree();
		total += d;
		if(max_bot <  d){
			max_bot = d;
		}
	}
	average_degree_bot_v =  total / bots.size();
}


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

void update_degree_cc(Node *n){
	map<int,float>::iterator it = degree_cc_top.find(n->get_degree());
	if(it != degree_cc_top.end()){
		degree_cc_top[n->get_degree()] = degree_cc_top[n->get_degree()] + n->get_cc();
	}else{
		degree_cc_top[n->get_degree()] = n->get_cc();
	}
}

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


void calculate_cc_total_bot(){
	for(int i = 0 ; i < bots.size() ; i++){
		Node *n = bots[i];
		vector<int> treated;
		for(int j = 0 ; j < n->get_degree() ; j++){
			Node *bo = n->neighbours[j];
			for(int z = 0 ; z < bo->get_degree() ; z++){
				Node *tmp = bo->neighbours[z];
				int index_x = topsIndex.find(tmp->get_title())->second;
				if(i < index_x && find(treated.begin(), treated.end(), index_x)==treated.end() && n!= tmp){
					calculate_cc(n,tmp);
					treated.push_back(index_x);
				}
			}
		}
		if(n->nb_top_neighbours > 0){
			n->set_cc(n->get_cc()/n->nb_top_neighbours);			
			cc += n->get_cc();
			cc_min+= (n->get_cc_min()/n->nb_top_neighbours);
			cc_max+= (n->get_cc_max()/n->nb_top_neighbours);
		}
		n->calculate_disp();
		n->calculate_redundancy();
		update_degree_bot(n);
		update_redundancy_bot(n);
		update_ccs_bot(n);
		// update_degree_cc(n); NEEDS TO BE DONE
	}
}


void calculate_cc_total_top(){
	for(int i = 0 ; i < tops.size() ; i++){
		Node *n = tops[i];
		set<int> treated;
		set<int>::iterator it;
		for(it =  n->neighbours_indexs.begin() ; it != n->neighbours_indexs.end() ; it++){
			Node *bo = n->neighbours[*it];
			set<int>::iterator it2;
			for(it2 = bo->neighbours_indexs.begin() ; it2 != bo->neighbours_indexs.end() ; it2++){
				Node *tmp = bo->neighbours[*it2];
				int index_x = topsIndex.find(tmp->get_title())->second;
				if(i < index_x && find(treated.begin(), treated.end(), index_x)==treated.end() && n!= tmp){
					calculate_cc(n,tmp);
					treated.insert(index_x);
				}
			}
		}
		if(n->nb_top_neighbours > 0){
			n->set_cc(n->get_cc()/n->nb_top_neighbours);			
			cc += n->get_cc();
			cc_min+= (n->get_cc_min()/n->nb_top_neighbours);
			cc_max+= (n->get_cc_max()/n->nb_top_neighbours);
		}
		n->calculate_disp();
	 	n->calculate_redundancy();
		update_degree_top(n);
		update_redundancy_top(n);
		update_ccs_top(n);
		update_degree_cc(n);
	}
}

// functions used for getting data from basic bi-partie graphs

void addlink(string t , string b){
	unordered_map<string, int>::const_iterator p;
	p = topsIndex.find(t);
	Node *top;
	if(p == topsIndex.end()){
		top = new Node(t);
		tops.push_back(top);
		top->set_index(tops.size()-1);
		topsIndex[t] = tops.size()-1;
	}else{
		top =tops[p->second];
	}
	unordered_map<string, int>::const_iterator p2;
	p2 = botsIndex.find(b);
	Node * bot;
	if(p2 == botsIndex.end()){
		bot = new Node(b);
		bots.push_back(bot);
		bot->set_index(bots.size() -1);
		botsIndex[b] = bots.size()-1;
	}else{
		bot = bots[p2->second];
	}
	top->addneighbours(bot);
 	bot->addneighbours(top);
    top = NULL;
    bot = NULL;
    delete top;
    delete bot;
}

float file2data(string name){
	ifstream file(name.c_str());
	string str; 
	string t;
	string b;
	float links = 0;

	while (getline(file, str))
    {
    	istringstream iss(str);
    	iss >> b;
    	iss >> t;
    	addlink(t,b);
    	links++;
    }
    file.close();
    return links;
}


//functions for treating pcap files.

vector<string> get_channels(string name,int nbChannels,map<string,int> list){
	ifstream file(name.c_str());
	string str;
	string b;
	vector<string> tmp;
	while (getline(file, str) && tmp.size() < nbChannels)
    {
    	istringstream iss(str);
    	iss >> b;
    	if (regex_match (b, regex("(192.168)(.*)")) && (find(tmp.begin(), tmp.end(), b)==tmp.end())){
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


float file2dataPCAP(string name,vector<string> channels){
	ifstream file(name.c_str());
	string str; 
	string t;
	string b;
	float links = 0;


	while (getline(file, str))
    {
    	istringstream iss(str);
    	iss >> b;
    	iss >> t;
    	size_t n = std::count(b.begin(), b.end(), '.');
    	if(n==4){
    		unsigned found = b.find_last_of(".");
	    	b = b.substr(0,found);
    	}
	    if(find(channels.begin(), channels.end(), b)!=channels.end()){
	    		addlink(b,t);
	    		continue;
	    		links++;
	    }
		n = std::count(b.begin(), b.end(), '.');
		if(n==4){
			unsigned found = t.find_last_of(".");
	    	t = t.substr(0,found);
		}
	    if(find(channels.begin(), channels.end(), t)!=channels.end()){
	   			addlink(t,b);
	    		links++;
	    		continue;
	    }
    }
    file.close();
    return links;
}

// functions to generate stat files and graphs


void get_stat_pcap(vector<string> names,vector<int> nbChannels){
	max_top = 0;
	max_bot = 0;
	cc=0;
	cc_min =0;
	cc_max = 0;
	red = 0;
	links = 0;

	map<string,int> list;
	cout << names.size() << "\n";
	for(int i = 0 ; i < names.size() ; i++){
		cout << "Treating : " << names[i] << "\n";
		vector<string> v =  get_channels(names[i],nbChannels[i],list);
		cout << "FOUND CHANNEL : " << "\n";
		for(int j = 0 ; j < v.size() ; j++){
			cout << v[j] << "\n";
		}
		links = links + file2dataPCAP(names[i],v);
		cout << "Number of links found till now : " << links << "\n";
	}
	cout << "Number of tops : " << tops.size() << "\n";
	cout << "Number of bots : " << bots.size() << "\n";
	cout << fixed << "Number of edges : " << links << "\n";
	density = links / (float)(tops.size()*bots.size());
	cout << "Density : " << fixed << density << "\n";
	cout << "Calculation " << "\n";
	calculate_cc_total_top();
	average_top();
	average_max_degree_bot();	
	cc = cc/tops.size();
	cc_min = cc_min/tops.size();
    cc_max = cc_max/tops.size();
	
}

void get_stat(string name){
	max_top = 0;
	max_bot = 0;
	cc=0;
	cc_min =0;
	cc_max = 0;
	red = 0;

	links = file2data(name);
	cout << "Number of tops : " << tops.size() << "\n";
	cout << "Number of bots : " << bots.size() << "\n";
	cout << "Number of edges : " << links << "\n";
	density = links / (float)(tops.size()*bots.size());
	cout << "Density : " << fixed << density << "\n";
	cout << "Calculation " << "\n";
	calculate_cc_total_top();
	average_top();
	average_max_degree_bot();	
	cc = cc/tops.size();
	cc_min = cc_min/tops.size();
    cc_max = cc_max/tops.size();
	
}



void stat_to_file(){
	ofstream myfile;
	myfile.open ("global.txt");
	myfile.precision(6);
	myfile << "Number of tops : " << tops.size() << "\n";
	myfile << "Number of bots : " << bots.size() << "\n";
	myfile << "Number of edges : " << fixed << links << "\n";
	myfile << "Density : " << fixed << density << "\n";
	myfile << "average_degree_top_v :" << average_degree_top_v << "\n";
	myfile << "average_degree_bot_v :" << average_degree_bot_v << "\n";
	myfile << "Max degree (top) : " << max_top << " (" << degrees_top[max_top] << ")\n";
	myfile << "Max degree (bot) : " << max_bot << " (" << degrees_bot[max_bot] << ")\n";
	myfile << "Clustering coefficient cc_top : " << cc << "\n";
	myfile << "Clustering coefficient cc_min_top : " << cc_min << "\n";
	myfile << "Clustering coefficient cc_max_top : " << cc_max << "\n";
	myfile << "Redundancy coefficient (top) : " << red << "\n";
	myfile << "Dispersion coefficient (top) : " << disp << "\n";
	myfile.close();
}

void graph_degree_cc(){
	ofstream myfile;
	myfile.open("graph_degree_cc.dat");
	map<int,float>::iterator it;
	for(it = degree_cc_top.begin(); it != degree_cc_top.end() ; it++){
		int deg = it->first;
		float v = it->second;
		v = v / (float)degrees_top[deg];
		myfile << deg << " ";
		myfile << v << "\n";
	}
	myfile.close();
}


//free data

void free_data(){
	for(int i = 0 ; i < tops.size() ; i++){
		delete tops[i];
	}
	for(int i = 0 ; i < bots.size() ; i++){
		delete bots[i];
	}
}

int main(int argc, char* argv[]){
	if(argc > 1 ){
		cout << argv[1] << "\n";
		get_stat(argv[1]);
		stat_to_file();
		stat_each_node(tops,"detail");
   	 	free_data();
   	 	return 0;
	}else{
	vector<int> nbChannels;
	vector<string> list;
	list.push_back("../Data/Japon2013/SIT-exp131219/PC_A/A_G_H_00001_20131218115553.pcap.txt");
	list.push_back("../Data/Japon2013/SIT-exp131219/PC_A/A_G_H_00002_20131218132135.pcap.txt");
	// list.push_back("../Data/Japon2013/SIT-exp131219/PC_A/A_G_H_00003_20131218143817.pcap.txt");
	// list.push_back(" ../Data/Japon2013/SIT-exp131219/PC_A/A_G_H_00004_20131218160722.pcap.txt");
	// list.push_back("../Data/Japon2013/SIT-exp131219/PC_A/A_G_H_00005_20131218173528.pcap.txt");
	// list.push_back("../Data/Japon2013/SIT-exp131219/PC_A/A_G_H_00006_20131218191707.pcap.txt");
	// list.push_back("../Data/Japon2013/SIT-exp131219/PC_A/A_G_H_00007_20131218204422.pcap.txt");
	// list.push_back("../Data/Japon2013/SIT-exp131219/PC_A/A_G_H_00008_20131218215110.pcap.txt");
	// list.push_back("../Data/Japon2013/SIT-exp131219/PC_A/A_G_H_00009_20131218231353.pcap.txt");
	// list.push_back("../Data/Japon2013/SIT-exp131219/PC_A/A_G_H_00010_20131219003426.pcap.txt");

	// list.push_back("../Data/Japon2013/SIT-exp131219/PC_B/B_00001_20131218120014.pcap.txt");
	// list.push_back("../Data/Japon2013/SIT-exp131219/PC_B/B_00002_20131218141733.pcap.txt");
	// list.push_back("../Data/Japon2013/SIT-exp131219/PC_B/B_00003_20131218172202.pcap.txt");
	// list.push_back("../Data/Japon2013/SIT-exp131219/PC_B/B_00004_20131218194842.pcap.txt");
	// list.push_back("../Data/Japon2013/SIT-exp131219/PC_B/B_00005_20131218211907.pcap.txt");
	// list.push_back("../Data/Japon2013/SIT-exp131219/PC_B/B_00006_20131218220346.pcap.txt");
	// list.push_back("../Data/Japon2013/SIT-exp131219/PC_B/B_00007_20131218224840.pcap.txt");
	// list.push_back("../Data/Japon2013/SIT-exp131219/PC_B/B_00008_20131219002334.pcap.txt");

	// list.push_back("../Data/Japon2013/SIT-exp131219/PC_C/C_I_J_00001_20131218120218.pcap.txt");
	// list.push_back("../Data/Japon2013/SIT-exp131219/PC_C/C_I_J_00002_20131218131749.pcap.txt");
	// list.push_back("../Data/Japon2013/SIT-exp131219/PC_C/C_I_J_00003_20131218143928.pcap.txt");
	// list.push_back("../Data/Japon2013/SIT-exp131219/PC_C/C_I_J_00004_20131218154832.pcap.txt");
	// list.push_back("../Data/Japon2013/SIT-exp131219/PC_C/C_I_J_00005_20131218170101.pcap.txt");
	// list.push_back("../Data/Japon2013/SIT-exp131219/PC_C/C_I_J_00006_20131218181937.pcap.txt");
	// list.push_back("../Data/Japon2013/SIT-exp131219/PC_C/C_I_J_00007_20131218193727.pcap.txt");
	// list.push_back("../Data/Japon2013/SIT-exp131219/PC_C/C_I_J_00008_20131218205810.pcap.txt");
	// list.push_back("../Data/Japon2013/SIT-exp131219/PC_C/C_I_J_00009_20131218221915.pcap.txt");
	// list.push_back("../Data/Japon2013/SIT-exp131219/PC_C/C_I_J_00010_20131218233604.pcap.txt");
	// list.push_back("../Data/Japon2013/SIT-exp131219/PC_C/C_I_J_00011_20131219010324.pcap.txt");


	// list.push_back("../Data/Japon2013/SIT-exp131219/PC_D/D_00001_20131218120321.pcap.txt");
	// list.push_back("../Data/Japon2013/SIT-exp131219/PC_D/D_00002_20131218192029.pcap.txt");
	// list.push_back("../Data/Japon2013/SIT-exp131219/PC_D/D_00003_20131219000742.pcap.txt");


	// list.push_back("../Data/Japon2013/SIT-exp131219/PC_E/E_00001_20131218120435.pcap.txt");
	// list.push_back("../Data/Japon2013/SIT-exp131219/PC_E/E_00002_20131218182235.pcap.txt");


	// list.push_back("../Data/Japon2013/SIT-exp131219/PC_F/F_K_L_00001_20131218120615.pcap.txt");
	// list.push_back("../Data/Japon2013/SIT-exp131219/PC_F/F_K_L_00002_20131218134102.pcap.txt");
	// list.push_back("../Data/Japon2013/SIT-exp131219/PC_F/F_K_L_00003_20131218150055.pcap.txt");
	// list.push_back("../Data/Japon2013/SIT-exp131219/PC_F/F_K_L_00004_20131218165206.pcap.txt");
	// list.push_back("../Data/Japon2013/SIT-exp131219/PC_F/F_K_L_00005_20131218184403.pcap.txt");
	// list.push_back("../Data/Japon2013/SIT-exp131219/PC_F/F_K_L_00006_20131218200349.pcap.txt");
	// list.push_back("../Data/Japon2013/SIT-exp131219/PC_F/F_K_L_00007_20131218211442.pcap.txt");
	// list.push_back("../Data/Japon2013/SIT-exp131219/PC_F/F_K_L_00008_20131218221643.pcap.txt");
	// list.push_back("../Data/Japon2013/SIT-exp131219/PC_F/F_K_L_00009_20131218231258.pcap.txt");
	// list.push_back("../Data/Japon2013/SIT-exp131219/PC_F/F_K_L_00010_20131219002102.pcap.txt");
	// list.push_back("../Data/Japon2013/SIT-exp131219/PC_F/F_K_L_00011_20131219014213.pcap.txt");
	


	nbChannels.push_back(3);
	nbChannels.push_back(3);
	// nbChannels.push_back(3);
	// nbChannels.push_back(3);
	// nbChannels.push_back(3);
	// nbChannels.push_back(3);
	// nbChannels.push_back(3);
	// nbChannels.push_back(3);
	// nbChannels.push_back(3);
	// nbChannels.push_back(3);
		
	// nbChannels.push_back(1);
	// nbChannels.push_back(1);
	// nbChannels.push_back(1);
	// nbChannels.push_back(1);
	// nbChannels.push_back(1);
	// nbChannels.push_back(1);
	// nbChannels.push_back(1);
	// nbChannels.push_back(1);

	// nbChannels.push_back(3);
	// nbChannels.push_back(3);
	// nbChannels.push_back(3);
	// nbChannels.push_back(3);
	// nbChannels.push_back(3);
	// nbChannels.push_back(3);
	// nbChannels.push_back(3);
	// nbChannels.push_back(3);
	// nbChannels.push_back(3);
	// nbChannels.push_back(3);
	// nbChannels.push_back(3);

	// nbChannels.push_back(1);
	// nbChannels.push_back(1);
	// nbChannels.push_back(1);

	// nbChannels.push_back(1);
	// nbChannels.push_back(1);

	// nbChannels.push_back(3);
	// nbChannels.push_back(3);
	// nbChannels.push_back(3);
	// nbChannels.push_back(3);
	// nbChannels.push_back(3);
	// nbChannels.push_back(3);
	// nbChannels.push_back(3);
	// nbChannels.push_back(3);
	// nbChannels.push_back(3);
	// nbChannels.push_back(3);
	// nbChannels.push_back(3);
	


	get_stat_pcap(list,nbChannels);
	stat_to_file();
	stat_each_node(tops,"detail");
	graph_degree(degrees_top,max_top,tops.size(),"graph_degree_top.dat");
	graph_degree(degrees_bot,max_bot,bots.size(),"graph_degree_bot.dat");
	system("top -l 1 -o mem  > top-output.txt");
	free_data();

	return 0;
	}
}
