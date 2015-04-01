
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
		if(size_pack == 0){
			continue;
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

long double get_ecart_list(vector<double> list,long double avg){
	long double sd = 0;
	for(int i = 0 ; i < list.size() ; i++){
		long double sd_tmp = pow(list[i]-avg,2);
		sd = sd + sd_tmp;
	}
	sd = sqrt(sd/(long double)list.size());
	return sd;
}

long double get_avg_list(vector<double> list){
	unsigned long long avg = 0;
	long double res = 0;
	if(list.size() <= 1){
		return -1;
	}
	for(int i = 0 ; i < list.size() ; i++){
		avg = avg + list[i];
	}
	cout << avg << "\n";
	res = avg/(long double)list.size();
	cout << res << "\n";
	return res;
}

void get_stat_pcap_batch(vector<string> names,vector<int> nbChannels){
	map<string,int> list;
	map<string,long double> ecart_type_time;
	map<string,long double> ecart_type_pack;
	map<string,long double> avg_pack;
	map<string,long double> avg_time;

	Graph * g = new Graph();
	for(int i = 0 ; i < names.size() ; i++){
		vector<string> v =  get_channels(names[i],nbChannels[i],list);
		for(int z = 0 ; z < v.size() ; z++){
			cout << v[z] << "\n";
		}
		file2data_PCAP_batch(names[i],v,g);
	}


	//PACK/TIME TREATEMENT.
	for(int i = 0 ; i < g->tops.size() ; i++){
		Node * n = g->tops[i];
		std::map<int,std::vector<double> >::iterator it;
		for(it = n->freq_ping.begin() ; it != n->freq_ping.end() ; it++){
			std::vector<double> v = it->second;		
			
		
			string tmp_string =  n->neighbours[it->first]->get_title();
			string title = n->get_title() + " " + tmp_string;
			
			long double avg_time_res = get_avg_list(v);	
			long double ecart_time_res = get_ecart_list(v,avg_time_res);

			long double avg_pack_res = get_avg_list(n->size_pack_list[it->first]);
			long double ecart_pack_res = get_ecart_list(n->size_pack_list[it->first],avg_pack_res);

			if(avg_time_res != -1){
				avg_time[title] = avg_time_res;
				ecart_type_time[title]=ecart_time_res;
				avg_pack[title] = avg_pack_res;
				ecart_type_pack[title] = ecart_pack_res;
			}
		}
	}	
	stat_to_stdout(g);
	calculate_stat_graph(g);
	stat_to_file(g);
    


	string current_time_ = current_time();

    create_graph_2_map(avg_pack,ecart_type_pack,"avg_ecart_pack_"+current_time_+".stat");
    create_graph_2_map(avg_time,ecart_type_time,"avg_ecart_time_"+current_time_+".stat");
    create_graph_2_map(ecart_type_time,avg_pack,"ecart_type_time_avg_pack_"+current_time_+".stat");
	
	g->free_data();
	delete(g);
}

//Reads the file till interval is reached and returns the graph g with the new data that
//was read.

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
  		if(size_pack == 0){
			continue;
		}
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
		    		addlink(g,b,t,&time_str,-size_pack);
		    }else{		
			    if(find(channels.begin(), channels.end(), t)!=channels.end() && find(channels.begin(), channels.end(), b)==channels.end()){
			   		addlink(g,t,b,&time_str,size_pack);
			    }
			}
	    }
	}
	g->density = g->links / (float)(g->tops.size()*g->bots.size());
}


void get_stat_pcap_interval(vector<string> names,vector<int> nbChannels,int interval){
	vector<string> channels;			//list of channels
	map<string,int> list; 				// map each channel's occurance
	vector<float> cc_graph;				//cc per time graph
	vector<float> density_graph;		//density per time graph
	vector<int> nb_bot_graph;			//nb of total peers
	vector<string> times;				//list of times	
	vector<string> dist_degree_by_top;	//nb of degree for top/bot ??
	vector<string> dist_degree_by_bot;
	//variance of degree for each peer.
	map<string,vector<int> > variance_degree_each_bot;
	//for variance each degree >= 6
	map<int,vector<int> > nb_each_degree;
	map<int,vector<int> > diff_nb_each_degree;	
	map<int,set<string> > prev_set_by_degree;
	map<int,set<string> > current_set_by_degree;


	//nb of peers
	map<string,set<string> > peer_per_channel;
	vector<vector<int > > nb_peer_per_channel;

	// size of packets UP
	vector<vector<int> >size_up_per_channel;


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
		density_graph.push_back(g->density);
		nb_bot_graph.push_back(g->bots.size());
		dist_degree_by_top.push_back(g->degrees_to_string());
		dist_degree_by_bot.push_back(g->degrees_to_string_bot());
       

		vector<int> size_pack_up;
       	for(int i = 0 ; i < g->tops.size() ; i++){
       		size_pack_up.push_back(g->tops[i]->get_total_up());
       	}
       	size_up_per_channel.push_back(size_pack_up);

		// Getting the degree of each Node and placing is in a list.
		for(int i = 0 ; i < g->bots.size() ; i++){
			int degree = g->bots[i]->get_degree();	
			if(degree >= 6){
				current_set_by_degree[degree].insert(g->bots[i]->get_title());
			}
			variance_degree_each_bot[g->bots[i]->get_title()].push_back(degree);
			std::set<int>::iterator it;
			for (it = g->bots[i]->neighbours_indexs.begin(); it != g->bots[i]->neighbours_indexs.end(); ++it){
				peer_per_channel[g->tops[*it]->get_title()].insert(g->bots[i]->get_title());
			}
		}
		
		map<string,set<string> >::iterator it;
		vector<int> size_now_per_peer;
		for(it = peer_per_channel.begin() ; it != peer_per_channel.end() ; it++){
			size_now_per_peer.push_back(it->second.size());
		}
		nb_peer_per_channel.push_back(size_now_per_peer);


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
			prev_set_by_degree = current_set_by_degree;
			current_set_by_degree.clear();
		}

		g->free_data();
		delete(g);
	}

	//vectors for peer-degree variance
	vector<pair<float,float> > avg_sd_degree =  avg_for_each(variance_degree_each_bot);
	vector<pair<float,float> > avg_nb_degree = avg_nb_for_each(variance_degree_each_bot);


	//current time/interval to add to file names.
	stringstream stream1;
	stream1 << interval;
	string interval_string = stream1.str();
	string current_time_ = current_time();
	//tmp list of times without the last time
	vector<string> times_tmp = times;
	times_tmp.erase (times_tmp.begin());
	//creating graph
	create_graph_vector_vector_int(size_up_per_channel,times,"up_stream_per_channel_"+current_time_+"_"+interval_string+".stat");
	create_graph_vector_vector_int(nb_peer_per_channel,times,"nb_peers_per_channel_"+current_time_+"_"+interval_string+".stat");
	create_graph_pairs(avg_nb_degree,"avg_nb_degree_"+current_time_+"_"+interval_string+".stat");
	create_graph_degree_change(nb_each_degree,diff_nb_each_degree,times_tmp,"degree_change_"+current_time_+"_"+interval_string+".stat");
	create_graph_pairs(avg_sd_degree,"avg_sd_degree_"+current_time_+"_"+interval_string+".stat");
	create_graph_float(cc_graph,times,"cc_interval_"+current_time_+"_"+interval_string+".stat");
	create_graph_float(density_graph,times,"density_interval_"+current_time_+"_"+interval_string+".stat");
	create_graph_string(dist_degree_by_bot,times,"dist_degree_bot_"+current_time_+"_"+interval_string+".stat");
	create_graph_nb_bot(nb_bot_graph,times,dist_degree_by_top,"nb_bot_interval_"+current_time_+"_"+interval_string+".stat");	
}




