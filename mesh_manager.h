#ifndef MESH_MANAGER_H
#define MESH_MANAGER_H
#include <map>
#include <string>
#include <d3dx9.h>
#include "mesh.h"

class mesh_manager
{
private:
	texture_manager* texture_manage;
	typedef std::map<std::string, mesh*> mesh_map;
	mesh_map mesh_queue;

public:
	mesh_manager(texture_manager* texture_manage);
	~mesh_manager();
	bool load(LPDIRECT3DDEVICE9 device, const char* filename);
	mesh* get_mesh(const char* filename);
	void release_mesh(mesh* mesh);
	void release();
};

#endif