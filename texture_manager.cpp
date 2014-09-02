#include "texture_manager.h"


texture_manager::texture_manager()
{
}


texture_manager::~texture_manager()
{
	release();
}

bool texture_manager::load(LPDIRECT3DDEVICE9 device, const char* filename)
{
	texture* temp_texture = NULL;

	if(filename == 0)
	{
		return FALSE;
	}

	if(get_texture(filename) != NULL)
	{
		return TRUE;
	}

	temp_texture = new texture();

	if(temp_texture->load(device, filename))
	{
		std::string filename_string = filename;
		texture_queue[filename_string] = temp_texture;
		temp_texture = NULL;
		return TRUE;
	}
	else
	{
		delete temp_texture;
		temp_texture = NULL;
		return FALSE;
	}
}

texture* texture_manager::get_texture(const char* filename)
{
	std::string filename_string = filename;

	texture_map::const_iterator search_result = texture_queue.find(filename_string);

	if(search_result != texture_queue.end())
	{
		search_result->second->add_reference();
		return search_result->second;
	}
	return NULL;
}

void texture_manager::release_texture(texture* texture)
{
	if(texture)
	{
		texture->remove_reference();
		if(texture->get_reference_count() <= 0)
		{
			std::string filename_string = texture->get_filename();
			texture_map::const_iterator search_result = texture_queue.find(filename_string);
			if(search_result != texture_queue.end())
			{
				texture_queue.erase(search_result);
			}
			delete texture;
			texture = NULL;
		}
	}
}

void texture_manager::release()
{
	if(!texture_queue.empty())
	{
		texture_map::iterator map_iterator;
		for(map_iterator = texture_queue.begin(); map_iterator != texture_queue.end(); map_iterator++)
		{
			map_iterator->second->release();
			map_iterator->second = NULL;
		}
		texture_queue.clear();
	}
}