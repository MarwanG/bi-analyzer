
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

	int z =  0;
	while (getline(file, str))
    {
    	istringstream iss(str);
    	iss >> time_str;
    	iss >> time_str;
    	iss >> b;
    	iss >> t;
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
    create_graph_int_map(g->degrees_bot,"dist_degree_bot.stat");
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



map<string,float> get_ecart_type(map<string,vector<int> > list){
	map<string,float> res;
	map<string,vector<int> >::iterator it;
	for(it=list.begin();it!=list.end();it++){
		vector<int> tmp = it->second;
		float avg = 0;
		for(int i = 0 ; i < tmp.size() ; i++){
			avg = avg + tmp[i]; 
		}
		avg = avg/(float)tmp.size();
		float sd = 0;
		for(int i = 0 ; i < tmp.size() ; i++){
			float sd_tmp = pow(sd-avg,2);
			sd = sd + sd_tmp;
		}
		sd = sqrt(sd/(float)tmp.size());
		res[it->first] = sd;
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
	vector<float> change_degree_top;

	map<string,vector<int> > distr_tops;

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
    set<string> prev;

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
		dist_degree_by_top.push_back(g->degrees_to_string());
		dist_degree_by_bot.push_back(g->degrees_to_string_bot());
		nb_super_pere.push_back(g->degrees_bot[g->max_bot]);

		if(distr_tops.empty()){
			for(int i = g->max_bot - 4 ; i <= g->max_bot ; i++){
				set<string> list_tmp = g->distr_by_degree[i];
				set<string>::iterator it;
				for (it = list_tmp.begin(); it != list_tmp.end(); ++it){
					vector<int> tmp_;
					tmp_.push_back(i); 
					distr_tops[*it] = tmp_;
				}		 
			}
		}else{
			for(int i = g->max_bot - 4 ; i <= g->max_bot ; i++){
				set<string> list_tmp = g->distr_by_degree[i];
				set<string>::iterator it;
				for (it = list_tmp.begin(); it != list_tmp.end(); ++it){
					if(distr_tops.find(*it) == distr_tops.end()){
						vector<int> tmp_;
						tmp_.push_back(i); 
						distr_tops[*it] = tmp_;
					}else{
						distr_tops[*it].push_back(i);
					}
				}
			}
		}

		set<string> id_super_pere = g->distr_by_degree[g->max_bot]; 
        set<string>::iterator it;
        if(prev.empty()){
            cout << "i am here \n";
            prev = id_super_pere;
        }else{
            vector<string> tmp;
            std::set_intersection(id_super_pere.begin(), id_super_pere.end(),
                                  prev.begin(),prev.end(),std::back_inserter(tmp));
            float val = (float)tmp.size()*2/(float)id_super_pere.size()+prev.size();
            cout << val << "\n";
            change_degree_top.push_back(val);
            prev = id_super_pere;
        }
		
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
	string current_time_ = current_time();

	map<string,float> ecart_distr =  get_ecart_type(distr_tops);
	map<string,float>::iterator it;
	for(it=ecart_distr.begin();it!=ecart_distr.end();it++){
		cout << it->first << "  " << it->second << "\n";
	}

	//graphs using basic functions
	create_graph_float(cc_graph,times,"cc_interval_"+current_time_+".stat");
	create_graph_float(degree_graph,times,"density_interval_"+current_time_+".stat");
	create_graph_float(change_degree_top,times,"changement_"+current_time_+".stat");
	create_graph_int(nb_super_pere,times,"nb_super_pere_"+current_time_+".stat");
	create_graph_string(dist_degree_by_bot,times,"dist_degree_bot_"+current_time_+".stat");

	create_graph_nb_bot(nb_bot_graph,times,dist_degree_by_top,"nb_bot_interval_"+current_time_+".stat");	
}




