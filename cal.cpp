
#include "cal.h"

#include <iostream>
#include <sstream>
#include <set>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <regex>
#include <unordered_map>
#include <math.h> 


using namespace std;


void file2data_PCAP_batch(string name,vector<string> channels,Graph * g){
	ifstream file(name.c_str());
	string str; 
	string t;
	string b;
	string time_str;
	string tmp;
	struct tm tm;
	time_t t1 = 4;
	int size_pack = 0;
	int z =  0;
	while (getline(file, str))
    {
    	istringstream iss(str);
    	iss >> time_str;
    	iss >> tmp;
    	time_str.append(" " + tmp);
    	iss >> b;
    	iss >> t;
        iss >> tmp;
  		iss >> tmp;
  		if(tmp.compare("ip-proto-17")==0){
  			continue;
  		}
  		size_pack = atoi(tmp.c_str());
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
	    		addlink(g,b,t,&time_str,size_pack);
	    }else{		
		    if(find(channels.begin(), channels.end(), t)!=channels.end() && find(channels.begin(), channels.end(), b)==channels.end()){
		   			addlink(g,t,b,&time_str,size_pack);
		    }
		}
    }
    cout << "nb of links : " << g->links  << " \n";
    cout << "size*size   : " << (float)(g->tops.size()*g->bots.size()) << "\n";
    g->density = g->links / (float)(g->tops.size()*g->bots.size());
    file.close();
}

float get_ecart_list(vector<double> list){
	float avg = 0;
	if(list.size() <= 1){
		return -1;
	}
	for(int i = 0 ; i < list.size() ; i++){
		avg = avg + list[i];
	}
	avg = avg/(float)list.size();
	float sd = 0;
	for(int i = 0 ; i < list.size() ; i++){
		float sd_tmp = pow(list[i]-avg,2);
		sd = sd + sd_tmp;
	}
	sd = sqrt(sd/(float)list.size());
	return sd;
}

void get_stat_pcap_batch(vector<string> names,vector<int> nbChannels){
	map<string,int> list;
	map<string,float> ecart_type;
	map<string,float> avg_pack;
	Graph * g = new Graph();
	for(int i = 0 ; i < names.size() ; i++){
		vector<string> v =  get_channels(names[i],nbChannels[i],list);
		for(int z = 0 ; z < v.size() ; z++){
			cout << v[z] << "\n";
		}
		file2data_PCAP_batch(names[i],v,g);
	}
	for(int i = 0 ; i < g->tops.size() ; i++){
		Node * n = g->tops[i];
		std::map<int,std::vector<double> >::iterator it;
		for(it = n->freq_ping.begin() ; it != n->freq_ping.end() ; it++){
			std::vector<double> v = it->second;		
			stringstream stream1;
			stream1 << it->first;
			string tmp_string = stream1.str();
			string title = n->get_title() + " " + tmp_string;	
			float res = get_ecart_list(v);
			float res2 = n->size_pack_list[it->first] / (float)v.size();
			if(res != -1){
				ecart_type[title]=res;
				avg_pack[title] = res2;
			}
		}
	}	
	stat_to_stdout(g);
	calculate_stat_graph(g);
	stat_to_file(g);
    create_graph_int_map(g->degrees_bot,"dist_degree_bot.stat");
    create_graph_2_map(ecart_type,avg_pack,"ecart_type_time_avg_pack.stat");
	g->free_data();
	delete(g);
}


void file2dataPCAP_interval(ifstream * file,vector<string> channels,int interval,Graph *g){
	string str; 
	string t;
	string b;
	string time_str;
	string tmp;
	string tmp2;
	struct tm tm;
	time_t t1 = 4;
	time_t t2 = 4;
	int size_pack = 0;
	while (getline(*file, str))
    {
    	istringstream iss(str);
    	iss >> time_str;
    	iss >> tmp;
    	time_str.append(" " + tmp);
    	iss >> b;
    	iss >> t; 
    	iss >> tmp2;
  		iss >> tmp2;
  		if(tmp2.compare("ip-proto-17")==0){
  			continue;
  		}
  		size_pack = atoi(tmp2.c_str());
    	if(count(b.begin(), b.end(), '.') > 2 &&  count(t.begin(), t.end(), '.') > 2){
	    	if(t1 == 4){
	    		t1 = timestamp_to_ctime(time_str.c_str());
	    		g->set_time(tmp);
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
			if(my_own_regex(b)&&find(channels.begin(), channels.end(), b) == channels.end()){
            	continue;
			}
			if(my_own_regex(t)&&find(channels.begin(), channels.end(), t) == channels.end()){
				continue;
			}
		    if(find(channels.begin(), channels.end(), b)!=channels.end()  && find(channels.begin(), channels.end(), t)==channels.end()){
		    		addlink(g,b,t,NULL,size_pack);
		    }else{		
			    if(find(channels.begin(), channels.end(), t)!=channels.end() && find(channels.begin(), channels.end(), b)==channels.end()){
			   		addlink(g,t,b,NULL,size_pack);
			    }
			}
	    }
	}
	g->density = g->links / (float)(g->tops.size()*g->bots.size());
}



map<string,float> get_ecart_type(map<string,vector<float> > list){
	map<string,float> res;
	map<string,vector<float> >::iterator it;
	for(it=list.begin();it!=list.end();it++){
		vector<float> tmp = it->second;
		float avg = 0;
		for(int i = 0 ; i < tmp.size() ; i++){
			avg = avg + tmp[i]; 
		}
		avg = avg/(float)tmp.size();
		res[it->first+"_avg"] = avg;
		float sd = 0;
		for(int i = 0 ; i < tmp.size() ; i++){
			float sd_tmp = pow(tmp[i]-avg,2);
			sd = sd + sd_tmp;
		}
		sd = sqrt(sd/(float)tmp.size());
		res[it->first+"_sd"] = sd;
	}
	return res;
}


vector<pair<float,float> > avg_for_each (map<string,vector<int> > list){
	
	map<string,vector<int> >::iterator it;
	vector<pair<float,float> > res;
	for(it = list.begin() ; it != list.end() ; it++){
		vector<int> values = it->second;
		float avg = 0;
		float sd = 0;
		for(int i = 0 ; i < values.size() ; i++){
			avg = avg + values[i];
		}
		avg = avg / (float)values.size();
		

		for(int i = 0 ; i < values.size() ; i++){
			float tmp = pow((values[i]-avg),2);
			sd = sd + tmp;
		}
		sd = sqrt(sd/(float)values.size());
		res.push_back(make_pair(avg,sd));
	}
	return res;
}


void get_stat_pcap_interval(vector<string> names,vector<int> nbChannels,int interval){
	vector<string> channels;
	map<string,int> list;
	vector<float> cc_graph;
	vector<float> degree_graph;
	vector<int> nb_bot_graph;
	vector<string> times;
	vector<string> dist_degree_by_top;
	vector<int> nb_super_pere;
	vector<string> dist_degree_by_bot;
	map<string,vector<int> > variance_degree_each_bot;
	vector<string> size_pack_top;


	map<int,vector<int> > nb_each_degree;
	map<int,vector<int> > diff_nb_each_degree;
	
	map<int,set<string> > prev_set_by_degree;
	map<int,set<string> > current_set_by_degree;


	set<string> diff_bots;



	// get all channels	
	for(int i = 0 ; i < names.size() ; i++){
		vector<string> tmp = get_channels(names[i],nbChannels[i],list);
		channels.insert(channels.end(), tmp.begin(), tmp.end());
	}

	//opening all the files.
	vector<ifstream*> files;
	for(int i = 0 ; i < names.size() ; i++){
		ifstream * file = new ifstream(names[i].c_str());
		files.push_back(file);
	}

	//going through each file for interval seconds.
	bool keep = true;
    set<string> prev;
    int nb_interval = 0;
	while(keep){
		nb_interval++;
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
		dist_degree_by_top.push_back(g->degrees_to_string());
		dist_degree_by_bot.push_back(g->degrees_to_string_bot());
        size_pack_top.push_back(g->packs_to_string_bot());
		nb_super_pere.push_back(g->degrees_bot[g->max_bot]);

		// Getting the degree of each Node and placing is in a list.
		if(variance_degree_each_bot.empty()){
			for(int i = 0 ; i < g->bots.size() ; i++){
				diff_bots.insert(g->bots[i]->get_title());
				int degree = g->bots[i]->get_degree();	
				if(degree >= 6){
					current_set_by_degree[degree].insert(g->bots[i]->get_title());
				}
				vector<int> tmp_;
				tmp_.push_back(degree);
				variance_degree_each_bot[g->bots[i]->get_title()] = tmp_;
			}
		}else{
			for(int i = 0 ; i < g->bots.size() ; i++){
				diff_bots.insert(g->bots[i]->get_title());
				int degree = g->bots[i]->get_degree();	
				if(degree >= 6){
					current_set_by_degree[degree].insert(g->bots[i]->get_title());
				}
				variance_degree_each_bot[g->bots[i]->get_title()].push_back(degree);
			}
		}

		if(prev_set_by_degree.empty()){
			prev_set_by_degree = current_set_by_degree;
		}else{
			map<int,set<string> >::iterator it;
			for(it = prev_set_by_degree.begin() ; it != prev_set_by_degree.end() ;it++){
				int degree = it->first;
				set<string> set_1 = it->second;
				set<string> set_2 = current_set_by_degree[degree];
  				vector<string> tmp;
  	            std::set_intersection(set_1.begin(), set_1.end(),
  			                     set_2.begin(),set_2.end(),std::back_inserter(tmp));
  	            nb_each_degree[degree].push_back(set_2.size());
  	            diff_nb_each_degree[degree].push_back(tmp.size());
			}
		}

		g->free_data();
		delete(g);
	}




	

	stringstream stream1;
	stream1 << interval;
	string interval_string = stream1.str();
	string current_time_ = current_time();


	vector<pair<float,float> > avg_sd_degree =  avg_for_each(variance_degree_each_bot);


	cout << "NUMBER OF DIFFERENT BOTS  :: " << diff_bots.size() << "\n";

	vector<string> times_tmp = times;
	times_tmp.erase (times_tmp.begin());


	create_graph_degree_change(nb_each_degree,diff_nb_each_degree,times_tmp,"degree_change_"+current_time_+".stat");
	create_graph_pairs(avg_sd_degree,"avg_sd_degree_"+current_time_+".stat");
	create_graph_float(cc_graph,times,"cc_interval_"+current_time_+".stat");
	create_graph_float(degree_graph,times,"density_interval_"+current_time_+".stat");
	create_graph_int(nb_super_pere,times,"nb_super_pere_"+current_time_+".stat");
	create_graph_string(dist_degree_by_bot,times,"dist_degree_bot_"+current_time_+".stat");
	create_graph_string(size_pack_top,times,"size_pack_top_"+current_time_+".stat");
	create_graph_nb_bot(nb_bot_graph,times,dist_degree_by_top,"nb_bot_interval_"+current_time_+".stat");	
}




