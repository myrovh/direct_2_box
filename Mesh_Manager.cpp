#include "Mesh_Manager.h"

Mesh_Manager::Mesh_Manager(Texture_Manager* texture_manage)
{
	this->texture_manage = texture_manage;
}


Mesh_Manager::~Mesh_Manager()
{
	release();
}

bool Mesh_Manager::load(LPDIRECT3DDEVICE9 device, const char* filename)
{
	Mesh* temp_mesh = NULL;

	if(filename == 0)
	{
		return FALSE;
	}

	if(get_mesh(filename) != NULL)
	{
		return TRUE;
	}

	temp_mesh = new Mesh();
	if(temp_mesh->load(device, filename, texture_manage))
	{
		std::string filename_string = filename;
		mesh_queue[filename_string] = temp_mesh;
		temp_mesh = NULL;
		return TRUE;
	}
	else
	{
		delete temp_mesh;
		temp_mesh = NULL;
		return FALSE;
	}
}

Mesh* Mesh_Manager::get_mesh(const char* filename)
{
	std::string filename_string = filename;
	mesh_map::const_iterator search_result = mesh_queue.find(filename_string);

	if(search_result != mesh_queue.end())
	{
		search_result->second->add_reference();
		return search_result->second;
	}

	return NULL;
}

void Mesh_Manager::release_mesh(Mesh* mesh)
{
	if(mesh)
	{
		mesh->remove_reference();
		if(mesh->get_reference_count() <= 0)
		{
			std::string filename_string = mesh->get_filename();
			mesh_map::const_iterator search_result = mesh_queue.find(filename_string);
			if(search_result != mesh_queue.end())
			{
				mesh_queue.erase(search_result);
			}
			delete mesh;
			mesh = NULL;
		}
	}
}

void Mesh_Manager::release()
{
	if(!mesh_queue.empty())
	{
		mesh_map::iterator map_iterator;
		for(map_iterator = mesh_queue.begin(); map_iterator != mesh_queue.end(); map_iterator++)
		{
			map_iterator->second->release(texture_manage);
			map_iterator->second = NULL;
		}
		mesh_queue.clear();
	}
}
