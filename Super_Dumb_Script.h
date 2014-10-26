#ifndef SUPER_DUMB_SCRIPT_H
#define SUPER_DUMB_SCRIPT_H

#include <windows.h>
#include <iostream>
#include <fstream>
#include <map>
#include <boost/any.hpp>
#include <boost/lexical_cast.hpp>

typedef std::map<std::string, boost::any> variable_map;
typedef std::map<std::string, variable_map*> file_map;
#define LINE_SIZE 100

class Super_Dumb_Script
{
private:
	std::string file_directory;
	std::string file_constructor_pattern;
	std::string file_variable_pattern;
	std::string search_path;
	std::string line_comment;
	std::string line_delineation;
	WIN32_FIND_DATA find_data;
	HANDLE find_handle;
	file_map storage;

	variable_map load_variables(std::string file_path);

public:
	Super_Dumb_Script();
	~Super_Dumb_Script();
	bool load_constructor_files(); //input a string that defines what extension gets loaded or reloaded
	bool load_variable_files(); //input a string that defines what extension gets loaded or reloaded
	variable_map* get_file(std::string file_name);
};

#endif