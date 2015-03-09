#ifndef Parser_H_
#define Parser_H_

#include <vector>
#include <string>
#include <sstream>
#include <iostream>


class Parser
{
public:
	Parser(std::string file_name);

	void set_mode(std::string mode);
	void set_period(std::string period);
	void set_type(std::string value);
	void set_files(std::string value);

	int mode;
	int period;
	std::string file_type;

	std::vector<int> nbChannels;
	std::vector<std::string> files;

};





#endif