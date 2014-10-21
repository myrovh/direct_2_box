#include "Super_Dumb_Script.h"

Super_Dumb_Script::Super_Dumb_Script()
{
	file_directory = "./script/";
	file_constructor_pattern = "*.sdsc";
	file_variable_pattern = "*.sdsv";
	line_comment = "//";
	line_delineation = "=";
	search_path = file_directory + file_constructor_pattern;
}

Super_Dumb_Script::~Super_Dumb_Script()
{

}

bool Super_Dumb_Script::load_files()
{
	find_handle = FindFirstFile(search_path.c_str(), &find_data);

	if(find_handle == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	do 
	{
		std::string file_path = file_directory + find_data.cFileName;
		std::ifstream in(file_path.c_str());
		if(in)
		{
			//going to need checks to account for already loaded files so that the variable map is updated rather than being skipped
			variable_map temp_variable = load_variables(file_path);
			if(temp_variable.size() > 0)
			{
				storage.insert(std::pair<std::string, variable_map*>(find_data.cFileName, new variable_map(temp_variable)));
			}
		}
	} while (FindNextFile(find_handle, &find_data) > 0);

	if(GetLastError() != ERROR_NO_MORE_FILES)
	{
		return FALSE;
	}

	return TRUE;
}

variable_map Super_Dumb_Script::load_variables(std::string file_path)
{
	std::ifstream file_in;
	std::string raw_string;
	std::string variable_name;
	std::string variable_type;
	std::string variable_data;
	variable_map new_variables;
	variable_store resolved_data;
	
	//Open the file make sure it is not broken
	file_in.open(file_path.c_str());
	if(file_in.good())
	{
		//Scan and load each line
		do
		{
			char temp_char[LINE_SIZE];
			file_in.getline(temp_char, LINE_SIZE);
			raw_string = temp_char;

			//check to see if '//' exists, skip line if it does
			std::size_t found = raw_string.find(line_comment);
			if(found == std::string::npos)
			{
				std::size_t equals_1 = raw_string.find(line_delineation);
				std::size_t equals_2 = raw_string.rfind(line_delineation);
				//ensure two unique equals signs exist on the line
				if(equals_1 != std::string::npos && equals_2 != std::string::npos && equals_1 != equals_2)
				{
					//split line into three variables
					variable_name = raw_string.substr(0, equals_1);
					variable_type = raw_string.substr(equals_1 + 1, (equals_2 - equals_1) - 1);
					variable_data = raw_string.substr(equals_2 + 1);

					//use second value to determine what data should be inserted into third value
					if(variable_type == "INT")
					{
						resolved_data = boost::lexical_cast<int>(variable_data);
					}
					else if(variable_type == "FLOAT")
					{
						resolved_data = boost::lexical_cast<float>(variable_data);
					}
					else if(variable_type == "STRING")
					{
						resolved_data = variable_data;
					}
					else
					{
						resolved_data = "error: invalid type definition";
					}
					//take the first string and the value inserted into a Property and insert into variable_map
					new_variables.insert(std::pair<std::string, variable_store>(variable_name, resolved_data));
				}
			}
		} while(file_in);
	}

	return new_variables;
}

variable_map* Super_Dumb_Script::get_file(std::string file_name)
{
	file_map::const_iterator search_result = storage.find(file_name);

	if(search_result != storage.end())
	{
		return search_result->second;
	}

	return NULL;
}