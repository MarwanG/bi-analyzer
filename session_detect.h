#ifndef SESSION_DETECT_H_
#define SESSION_DETECT_H_




#include <set>
#include <map>
#include <vector>
#include <string>

void detect_session(std::string name,std::vector<std::string> channels,double delta);

void detect_(std::vector<std::string> names,std::vector<int> nbChannels,double delta);

#endif