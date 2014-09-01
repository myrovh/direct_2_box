#include "object_manager.h"

object_manager::object_manager()
{
}

object_manager::~object_manager()
{
	release();
}

bool object_manager::insert_object(object_type type, object_settings settings)
{
	return false;
}

object* object_manager::get_object(const char* filename)
{
	std::string filename_string = filename;
	object_map::const_iterator search_result = object_stack.find(filename_string);

	if(search_result != object_stack.end())
	{
		// Need to add reference counter once object has that function
		return search_result->second;
	}

	return NULL;
}

void object_manager::release_object(object* object)
{

}

void object_manager::release()
{
	if(!object_stack.empty())
	{
		object_map::iterator object_iterator;
		for(object_iterator = object_stack.begin(); object_iterator != object_stack.end(); object_iterator++)
		{
			//Add release object call here
			object_iterator->second = NULL;
		}
		object_stack.clear();
	}
}