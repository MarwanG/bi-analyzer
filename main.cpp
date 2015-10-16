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


	// 	JAPAN 2013 FILES (COMPLETE)
	//list.push_back("/data2/ghanem/tmp/proper_a.txt"); 
	//list.push_back("/data2/ghanem/tmp/proper_b.txt");
	//list.push_back("/data2/ghanem/tmp/proper_c.txt");
	//list.push_back("/data2/ghanem/tmp/proper_d.txt");
	//list.push_back("/data2/ghanem/tmp/proper_e.txt");
	//list.push_back("/data2/ghanem/tmp/test_f.txt");

	// JAPAN FILES 2013 (VIDEO)
	//list.push_back("/data2/ghanem/tmp/video_a.txt"); 
	// list.push_back("/data2/ghanem/tmp/video_b.txt");
	// list.push_back("/data2/ghanem/tmp/video_c.txt");
	// list.push_back("/data2/ghanem/tmp/video_d.txt");
	// list.push_back("/data2/ghanem/tmp/video_e.txt");
	// list.push_back("/data2/ghanem/tmp/video_f.txt");
	

    // JAPAN FILES 2015
    list.push_back("/data2/ghanem/Japan2015/AB.txt");
    list.push_back("/data2/ghanem/Japan2015/CD.txt");
    list.push_back("/data2/ghanem/Japan2015/EFG.txt");
    list.push_back("/data2/ghanem/Japan2015/HIJ.txt");


	
	nbChannels.push_back(2);
	nbChannels.push_back(2);
	nbChannels.push_back(3);
	nbChannels.push_back(3);
	//nbChannels.push_back(1);
	//nbChannels.push_back(3);

	// vector<string> filter = list_to_check("ip_filters.stat");
	// get_stat_pcap_interval_filter(list,nbChannels,filter,60);
	get_stat_pcap_interval(list,nbChannels,60);
	//get_stat_pcap_batch(list,nbChannels);

	// detect_(list,nbChannels,10);
	// detect_(list,nbChannels,120);
	// detect_(list,nbChannels,90);
	//detect_(list,nbChannels,180);
	// detect_(list,nbChannels,150);	


	}	
}
