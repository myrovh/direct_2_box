#ifndef MESH_MANAGER_H
#define MESH_MANAGER_H
#include <map>
#include <string>
#include <d3dx9.h>
#include "Mesh.h"

class Mesh_Manager
{
private:
	Texture_Manager* texture_manage;
	typedef std::map<std::string, Mesh*> mesh_map;
	mesh_map mesh_queue;

public:
	Mesh_Manager(Texture_Manager* texture_manage);
	~Mesh_Manager();
	bool load(LPDIRECT3DDEVICE9 device, std::string filename);
	Mesh* get_mesh(std::string filename);
	void release_mesh(Mesh* mesh);
	void release();
};

#endif
