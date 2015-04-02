#include <iostream>
#include <sstream>
#include <vector>
#include <dirent.h>
#include <errno.h>

#include "cal.h"

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

// int main(int argc, char* argv[]){
// 	if(argc == 1){
// 		cout << "please enter options or add -help for more details \n";	
// 		return 0;
// 	}else{
// 		int i = 1;
// 		while(i < argc){
// 			string cmd = argv[i];
// 			if(cmd.compare("-help")==0){
// 				help();
// 				i++;
// 			}else if(cmd.compare("-f")==0){
// 				i++;
// 				folder = argv[i];
// 				i++;
// 			}else if(cmd.compare("-mode")==0){
// 				i++;
// 				string cmd2 = argv[i];
// 				if(cmd2.compare("b")==0){
// 					mode = 0;
// 				}else{
// 					mode = 1;
// 				}
// 				i++;
// 			}else if(cmd.compare("-t")==0){
// 				i++;
// 				string tmp = argv[i];
// 				interval = atoi(tmp.c_str());
// 				i++;
// 			}
// 		}
// 	}
// 	print_config();
// 	getdir(folder);
// 	return 0;
// }

int main(int argc, char* argv[]){




	if(argc > 1){
		cout << argv[1] << "\n";
		get_stat(argv[1]);
	}else{
		vector<int> nbChannels;
		vector<string> list;



	list.push_back("/data2/ghanem/tmp/short_a.txt");
    // list.push_back("/data2/ghanem/tmp/test_a.txt"); 
    // list.push_back("/data2/ghanem/tmp/test_b.txt");
    // list.push_back("/data2/ghanem/tmp/test_c.txt");
    // list.push_back("/data2/ghanem/tmp/test_d.txt");
    // list.push_back("/data2/ghanem/tmp/test_e.txt");
    // list.push_back("/data2/ghanem/tmp/test_f.txt");


	// list.push_back("/data2/ghanem/video_trace_by_size/video_size_a.txt");
	// list.push_back("/data2/ghanem/video_trace_by_size/video_size_b.txt");
	// list.push_back("/data2/ghanem/video_trace_by_size/video_size_c.txt");
	// list.push_back("/data2/ghanem/video_trace_by_size/video_size_d.txt");
	// list.push_back("/data2/ghanem/video_trace_by_size/video_size_e.txt");
	// list.push_back("/data2/ghanem/video_trace_by_size/video_size_f.txt");


	nbChannels.push_back(3);
	// nbChannels.push_back(1);
	// nbChannels.push_back(3);
	// nbChannels.push_back(1);
	// nbChannels.push_back(1);
	// nbChannels.push_back(3);

	get_stat_pcap_interval(list,nbChannels,30);
	// get_stat_pcap_batch(list,nbChannels);
	}	
}
