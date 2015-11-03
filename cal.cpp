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
	string hours;
	struct tm tm;
	time_t t1 = 4;
	int size_pack = 0;
	int z =  0;
	while (getline(file, str))
    {
    	z++;
    	istringstream iss(str);
    	iss >> time_str;
    	iss >> hours;
    	time_str.append(" " + hours);
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
    	if(n==4 && find(channels.begin(), channels.end(), b) == channels.end()){
	    		unsigned found = b.find_last_of(".");
		    	b = b.substr(0,found);
    	}
    	n = std::count(t.begin(), t.end(), '.');
		if(n==4 && find(channels.begin(), channels.end(), t) == channels.end()){
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
	    		addlink(g,b,t,&time_str,-size_pack);
	    }else{		
		    if(find(channels.begin(), channels.end(), t)!=channels.end() && find(channels.begin(), channels.end(), b)==channels.end()){
		   		addlink(g,t,b,&time_str,size_pack);
		    }
		}
    }
    cout << "nb of links : " << g->links  << " \n";
    cout << "size*size   : " << (float)(g->tops.size()*g->bots.size()) << "\n";
    file.close();
}



void get_stat_pcap_batch(vector<string> names,vector<int> nbChannels){
	map<string,int> * list = new map<string,int>();

    Graph * g = new Graph();
	for(int i = 0 ; i < names.size() ; i++){
		vector<string> v =  get_channels(names[i],nbChannels[i],list);
		for(int z = 0 ; z < v.size() ; z++){
			cout << v[z] << "\n";
		}
        cout << "====================== \n";
		file2data_PCAP_batch(names[i],v,g);
	}

	calculate_stat_graph(g);

    string current_time_ = current_time();
    stat_to_file(g,"global_stats_"+current_time_+".stat");
    g->free_data();
    delete(g);
}

//Reads the file till interval is reached and returns the graph g with the new data that
//was read.

long long file2dataPCAP_interval(ifstream * file,vector<string> channels,int interval,Graph *g){
	string str; 
	string t;
	string b;
	string time_str;
	string tmp;
	string tmp2;
	struct tm tm;
	time_t t1 = 4;
	time_t t2 = 4;
	long long size_pack_total = 0;
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
  		if(size_pack == 0){						// WHEN ITS A TEXT SIZE_PACK IS = 0.
			continue;
		}
    	if(count(b.begin(), b.end(), '.') > 2 &&  count(t.begin(), t.end(), '.') > 2){
	    	if(t1 == 4){
	    		t1 = timestamp_to_ctime(time_str.c_str());
	    		g->set_time(time_str);
	    	}else{    		
	    		t2 = timestamp_to_ctime(time_str.c_str());
	    		double diff = difftime(t2,t1);
	    		if(diff > interval){
	    			break;
	    		}
	    	}
	    	size_t n = count(b.begin(), b.end(), '.');
	    	if(n==4 && find(channels.begin(), channels.end(), b) == channels.end()){
	    		unsigned found = b.find_last_of(".");
		    	b = b.substr(0,found);
	    	}
	    	n = std::count(t.begin(), t.end(), '.');
			if(n==4 && find(channels.begin(), channels.end(), t) == channels.end()){
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
		size_pack_total = size_pack + size_pack_total;
	    }
	}
	return size_pack_total;
}


void get_stat_pcap_interval(vector<string> names,vector<int> nbChannels,int interval){
	vector<string> channels;			//list of channels
	map<string,int> *list = new map<string,int>(); 				// map each channel's occurance
	vector<int> nb_bot_graph;			//nb of total peers
	vector<string> times;				//list of times	
	vector<string> dist_degree_by_top;	//nb of degree for top/bot ??
	vector<string> dist_degree_by_bot;

	map<string,vector<int> > variance_degree_each_bot;

	map<string,int> biggest_pack_per_ip;
	map<string,long long> video_packs_per_ip;
	map<string,long long> signalling_packs_per_ip;
	map<string,long long> nb_packets_per_ip;



	map<string,vector<int> > total_packets_per_peer;

	map<string,set<string> > channels_for_each_peer;

	vector<float> total_packet_exchange;

	vector<long long> video_packs;
	vector<long long> signalling_packs;

	vector<int> nb_video_peers;
	vector<int> nb_signalling_peers;

	vector<long long> packets_exchange_top;

	

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
    long long total_size = 0;
	while(keep){
		nb_interval++;
		Graph * g = new Graph();
		for(int i = 0 ; i < files.size() ; i++){
			long long tmp = file2dataPCAP_interval(files[i],channels,interval,g);
			if(files[i]->eof() != 0){
				keep = false;
				break;
			}
			total_size = total_size + tmp;
		}
		cout << g->time_ << "\n";
		cout << total_size << "\n";
		calculate_stat_graph(g);
		
		times.push_back(g->time_);
		nb_bot_graph.push_back(g->bots.size());
		
		dist_degree_by_top.push_back(g->degrees_to_string());
	 

		// Getting the degree of each Node and placing is in a list.
       	int nb_video_peers_count = 0;
       	int nb_signalling_peers_count = 0;
		for(int i = 0 ; i < g->bots.size() ; i++){
			int degree = g->bots[i]->get_degree();	
			if(g->bots[i]->max_packet < 1000){
				nb_signalling_peers_count++;
			}else{
				nb_video_peers_count++;
			}
			variance_degree_each_bot[g->bots[i]->get_title()].push_back(degree);
			if(biggest_pack_per_ip[g->bots[i]->get_title()]<g->bots[i]->max_packet){
				biggest_pack_per_ip[g->bots[i]->get_title()]=g->bots[i]->max_packet;
			}
						
			
			std::set<int>::iterator it;
			for (it = g->bots[i]->neighbours_indexs.begin(); it != g->bots[i]->neighbours_indexs.end(); ++it){
				channels_for_each_peer[g->bots[i]->get_title()].insert(g->tops[*it]->get_title());

				total_packets_per_peer[g->bots[i]->get_title()].insert(total_packets_per_peer[g->bots[i]->get_title()].begin(),
					g->tops[*it]->size_pack_list_total_detail[g->bots[i]->get_index()].begin(),
					g->tops[*it]->size_pack_list_total_detail[g->bots[i]->get_index()].end());
			}
		
		}
/*		long long sum_packets = 0;
		for(int i = 0; i < g->tops.size() ; i++){
			sum_packets = sum_packets + g->tops[i]->video_packs + g->tops[i]->signal_packs;
		}
		cout << sum_packets << "\n";*/
		packets_exchange_top.push_back(total_size);
		g->free_data();
		delete(g);
	}

    ofstream myfile;
    myfile.open("nb_window_totaldegree.stat");
    
    std::map<string,vector<int> >::iterator it;
    for(it = variance_degree_each_bot.begin(); it != variance_degree_each_bot.end() ; it++){
        vector<int> list = it->second;
        int length = list.size();
        int sum = 0;
        for(int i = 0 ; i < length ; i++){
            sum = sum + list[i];
        }
        myfile << it->first << "  " << length << "  " << sum << "\n";
    }
    myfile.close();


	myfile.open("packets_testing.stat");   
	for(int i = 0; i < packets_exchange_top.size() ; i++){
		myfile << times[i] << " " << packets_exchange_top[i] << "\n";
	} 
	myfile.close();

	//current time/interval to add to file names.
	stringstream stream1;
	stream1 << interval;
	string interval_string = stream1.str();
	string current_time_ = current_time();


	// nb of peers per time video and signalling.
	//create_graph_int(nb_video_peers,times,"nb_video_peers_"+current_time_+"_"+interval_string+".stat");
	//create_graph_int(nb_signalling_peers,times,"nb_signalling_peers_"+current_time_+"_"+interval_string+".stat");
	
	// avg-sd degree and largest packet for each ip.
	map<string,pair<float,float> > avg_sd_degree =  avg_for_each(variance_degree_each_bot);
	create_graph_map_pairs(avg_sd_degree,biggest_pack_per_ip,"avg_sd_degree_max_pack_"+current_time_+"_"+interval_string+".stat");
	
	// nb of active peers per channel per time.
	create_graph_nb_bot(nb_bot_graph,times,dist_degree_by_top,"nb_bot_interval_"+current_time_+"_"+interval_string+".stat");	
	
	
	//grouping servers
	create_graph_map_string_set(channels_for_each_peer,"channels_for_each_peer_"+current_time_+"_"+interval_string+".stat");

	
}	



void file2dataPCAP_interval_filter(ifstream * file,vector<string> channels,int interval,vector<string> filter,Graph *g){
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
    	// cout << "LINE << " << str << "\n";
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
		    		if(find(filter.begin(),filter.end(),t)!=filter.end())
		    			addlink(g,b,t,&time_str,-size_pack);
		    }else{		
			    if(find(channels.begin(), channels.end(), t)!=channels.end() && find(channels.begin(), channels.end(), b)==channels.end()){
			   		if(find(filter.begin(),filter.end(),b)!=filter.end())
			   			addlink(g,t,b,&time_str,size_pack);
			    }
			}
	    }
	}
	g->density = g->links / (float)(g->tops.size()*g->bots.size());
}

void get_stat_pcap_interval_filter(vector<string> names,vector<int> nbChannels,vector<string> filter,int interval){
	vector<string> channels;			//list of channels
	map<string,int> * list; 				// map each channel's occurance
	vector<float> cc_graph;				//cc per time graph
	vector<float> density_graph;		//density per time graph
	vector<int> nb_bot_graph;			//nb of total peers
	vector<string> times;				//list of times	
	vector<string> dist_degree_by_top;	//nb of degree for top/bot ??
	vector<string> dist_degree_by_bot;

	map<string,vector<int> > variance_degree_each_bot;


	map<string,vector< pair<string,int> > > window_result_total;
	map<string,vector< pair<string,int> > > window_result_current;


	map<string,int> biggest_pack_per_ip;
	map<string,long long> video_packs_per_ip;
	map<string,long long> signalling_packs_per_ip;


	map<string,vector<int> > total_packets_per_peer;


	//nb of peers
	map<string,set<string> > peer_per_channel; // ???
	

	vector<float> total_packet_exchange;

	vector<long long> video_packs;
	vector<long long> signalling_packs;

	vector<int> nb_video_peers;
	vector<int> nb_signalling_peers;


	

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


	for(int i = 0 ; i < channels.size() ; i++){
		cout << channels[i] << "\n";
	}

	//going through each file for interval seconds.
	bool keep = true;
    set<string> prev;
    int nb_interval = 0;
	while(keep){
		nb_interval++;
		Graph * g = new Graph();
		for(int i = 0 ; i < files.size() ; i++){
			file2dataPCAP_interval_filter(files[i],channels,interval,filter,g);
			if(files[i]->eof() != 0){
				keep = false;
				break;
			}
		}
		cout << g->time_ << "\n";
		calculate_stat_graph(g);
		stat_to_stdout(g);
    
		times.push_back(g->time_);
		cc_graph.push_back(g->cc);
		density_graph.push_back(g->density);
		nb_bot_graph.push_back(g->bots.size());
		
		dist_degree_by_top.push_back(g->degrees_to_string());
		dist_degree_by_bot.push_back(g->degrees_to_string_bot());
       

		
		int total_packets = 0;
		int video_packets = 0;
		int signalling_packets = 0;
       	for(int i = 0 ; i < g->tops.size() ; i++){
       		total_packets+=g->tops[i]->get_total_up() + g->tops[i]->get_total_down();
       		video_packets+=g->tops[i]->video_packs;
       		signalling_packets+=g->tops[i]->nb_signalling_packs;
       	}
       	total_packet_exchange.push_back(total_packets);
       	video_packs.push_back(video_packets);
       	signalling_packs.push_back(signalling_packets);


		// Getting the degree of each Node and placing is in a list.
       	int nb_video_peers_count = 0;
       	int nb_signalling_peers_count = 0;
		for(int i = 0 ; i < g->bots.size() ; i++){
			int degree = g->bots[i]->get_degree();	

			
			variance_degree_each_bot[g->bots[i]->get_title()].push_back(degree);
			if(biggest_pack_per_ip[g->bots[i]->get_title()]<g->bots[i]->max_packet){
				biggest_pack_per_ip[g->bots[i]->get_title()]=g->bots[i]->max_packet;
			}
			if(biggest_pack_per_ip[g->bots[i]->get_title()] < 1000){
				nb_signalling_peers_count++;
			}else{
				nb_video_peers_count++;
			}			
			video_packs_per_ip[g->bots[i]->get_title()]=+g->bots[i]->nb_video_packs;
			signalling_packs_per_ip[g->bots[i]->get_title()]=+g->bots[i]->nb_signalling_packs;

			std::set<int>::iterator it;
			for (it = g->bots[i]->neighbours_indexs.begin(); it != g->bots[i]->neighbours_indexs.end(); ++it){
				peer_per_channel[g->tops[*it]->get_title()].insert(g->bots[i]->get_title());
				total_packets_per_peer[g->bots[i]->get_title()].insert(total_packets_per_peer[g->bots[i]->get_title()].begin(),
					g->tops[*it]->size_pack_list_total_detail[g->bots[i]->get_index()].begin(),
					g->tops[*it]->size_pack_list_total_detail[g->bots[i]->get_index()].end());
			}
		}
		nb_video_peers.push_back(nb_video_peers_count);
		nb_signalling_peers.push_back(nb_signalling_peers_count);
		g->free_data();
		delete(g);
	}

	map<string,pair<float,float> > avg_sd_degree =  avg_for_each(variance_degree_each_bot);

	map<string,pair<float,float> > avg_sd_packet = avg_for_each(total_packets_per_peer);


	//current time/interval to add to file names.
	stringstream stream1;
	stream1 << interval;
	string interval_string = stream1.str();
	string current_time_ = current_time();
	//tmp list of times without the last time
	vector<string> times_tmp = times;
	times_tmp.erase (times_tmp.begin());
	//creating graph

	create_graph_int(nb_video_peers,times,"nb_video_peers_"+current_time_+"_"+interval_string+".stat");
	create_graph_int(nb_signalling_peers,times,"nb_signalling_peers_"+current_time_+"_"+interval_string+".stat");
	create_graph_2_map_int(video_packs_per_ip,signalling_packs_per_ip,"nb_video_signalling"+current_time_+"_"+interval_string+".stat");
	create_graph_map_pairs(avg_sd_degree,biggest_pack_per_ip,"avg_sd_degree_max_pack_"+current_time_+"_"+interval_string+".stat");
	create_graph_float(total_packet_exchange,times,"total_packets_"+current_time_+"_"+interval_string+".stat");
	create_graph_long_long(video_packs,times,"video_packs_"+current_time_+"_"+interval_string+".stat");
	create_graph_long_long(signalling_packs,times,"signalling_packs_"+current_time_+"_"+interval_string+".stat");
	create_graph_nb_bot(nb_bot_graph,times,dist_degree_by_top,"nb_bot_interval_"+current_time_+"_"+interval_string+".stat");	
	create_graph_map_pairs_pairs(avg_sd_degree,avg_sd_packet,"avg_sd_degree_avg_sd_packet_"+current_time_+"_"+interval_string+".stat");
}




