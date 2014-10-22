#include "Texture_Manager.h"

Texture_Manager::Texture_Manager()
{
}

Texture_Manager::~Texture_Manager()
{
	release();
}

bool Texture_Manager::load(LPDIRECT3DDEVICE9 device, std::string filename)
{
	Texture* temp_texture = NULL;

	if(filename.size() == 0)
	{
		return FALSE;
	}

	if(get_texture(filename) != NULL)
	{
		return TRUE;
	}

	temp_texture = new Texture();

	if(temp_texture->load(device, filename))
	{
		texture_queue.insert(std::pair<std::string, Texture*>(filename, temp_texture));
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

Texture* Texture_Manager::get_texture(std::string filename)
{
	texture_map::const_iterator search_result = texture_queue.find(filename);

	if(search_result != texture_queue.end())
	{
		search_result->second->add_reference();
		return search_result->second;
	}
	return NULL;
}

void Texture_Manager::release_texture(Texture* texture)
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

void Texture_Manager::release()
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