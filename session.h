#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <string>
#include <map>
#include <set>



class Session
{
public:
	Session(std::string ip,std::string channel,std::string start);
	void force_end();
	bool check_end(std::string end,double delta);


	std::string ip_;
	std::string channel_;
	std::string start_;
	std::string last_;
	std::string end_;	

	double duration;	


};



#endif