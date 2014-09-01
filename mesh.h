#ifndef MESH_H
#define MESH_H
#include <d3dx9.h>
#include "texture_manager.h"

class mesh
{
private:
	int reference_count;
	LPD3DXBUFFER material_buffer;
	LPD3DXMESH mesh_storage;
	DWORD number_of_materials;
	D3DMATERIAL9* materials;
	texture** mesh_textures;
	const char* filename;

	void release(texture_manager* texture_manage);

protected:
	mesh();
	~mesh() {};

	void add_reference()
	{
		reference_count++;
	}
	void remove_reference()
	{
		reference_count--;
	}
	int get_reference_count()
	{
		return reference_count;
	}

public:
	bool load(LPDIRECT3DDEVICE9 device, const char* filename, texture_manager* texture_manage);

	const char* get_filename()
	{
		return filename;
	}
	LPD3DXMESH get_d3d_mesh()
	{
		return mesh_storage;
	}
	D3DMATERIAL9* get_mesh_material(int index)
	{
		return &materials[index];
	}
	texture* get_mesh_texture(int index)
	{
		return mesh_textures[index];
	}

	void render(LPDIRECT3DDEVICE9 device);

	friend class mesh_manager;
};

#endif