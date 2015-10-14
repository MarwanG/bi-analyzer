#include <iostream>
#include <sstream>
#include <vector>
#include <dirent.h>
#include <errno.h>

#include "cal.h"
#include "session_detect.h"

using namespace std;

string folder = "";
int mode = 0;	//0 for batch 1 for interval
int interval = 300;

vector<string> getdir (string dir){
    DIR *dp;
    vector<string> files;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return files;
    }

    while ((dirp = readdir(dp)) != NULL) {
        files.push_back(string(dirp->d_name));
        cout << dirp->d_name << "\n";
    }
    closedir(dp);
    return files;
}

void help(){
	cout << "help \n";
}

void print_config(){
	cout << folder << "\n";
	cout << mode << "\n";
	cout << interval << "\n";
}

int main(int argc, char* argv[]){




	if(argc > 1){
		cout << argv[1] << "\n";
		get_stat(argv[1]);
	}else{
		vector<int> nbChannels;
		vector<string> list;

	// COMPUTER FILE TO TEST

	// list.push_back("../Data/a_5000000.txt");
	// list.push_back("../Data/proper_a.txt");

	// 	JAPAN FILES (COMPLETE)
	list.push_back("/data2/ghanem/tmp/proper_a.txt"); 
	list.push_back("/data2/ghanem/tmp/proper_b.txt");
	list.push_back("/data2/ghanem/tmp/proper_c.txt");
	list.push_back("/data2/ghanem/tmp/proper_d.txt");
	list.push_back("/data2/ghanem/tmp/proper_e.txt");
	list.push_back("/data2/ghanem/tmp/test_f.txt");

	// JAPAN FILES (VIDEO)
	//list.push_back("/data2/ghanem/tmp/video_a.txt"); 
	// list.push_back("/data2/ghanem/tmp/video_b.txt");
	// list.push_back("/data2/ghanem/tmp/video_c.txt");
	// list.push_back("/data2/ghanem/tmp/video_d.txt");
	// list.push_back("/data2/ghanem/tmp/video_e.txt");
	// list.push_back("/data2/ghanem/tmp/video_f.txt");
	

	// TEST LONG (COMPLETE DAY 1)
	// list.push_back("/data2/ghanem/long/PC_A.txt");
	// list.push_back("/data2/ghanem/long/PC_B.txt");
	// list.push_back("/data2/ghanem/long/PC_C.txt");
	// list.push_back("/data2/ghanem/long/PC_D.txt");
	// list.push_back("/data2/ghanem/long/PC_E.txt");


	
	nbChannels.push_back(3);
	nbChannels.push_back(1);
	nbChannels.push_back(3);
	nbChannels.push_back(1);
	nbChannels.push_back(1);
	nbChannels.push_back(3);

	// vector<string> filter = list_to_check("ip_filters.stat");
	// get_stat_pcap_interval_filter(list,nbChannels,filter,60);
	// get_stat_pcap_interval(list,nbChannels,60);
	get_stat_pcap_batch(list,nbChannels);

	// detect_(list,nbChannels,10);
	// detect_(list,nbChannels,120);
	// detect_(list,nbChannels,90);
	//detect_(list,nbChannels,180);
	// detect_(list,nbChannels,150);
	}	
}
