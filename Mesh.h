#ifndef MESH_H
#define MESH_H
#include <d3dx9.h>
#include "Texture_Manager.h"

class Mesh
{
private:
	int reference_count;
	LPD3DXBUFFER material_buffer;
	LPD3DXMESH mesh_storage;
	DWORD number_of_materials;
	D3DMATERIAL9* materials;
	Texture** mesh_textures;
	const char* filename;

	void release(Texture_Manager* texture_manage);

protected:
	Mesh();
	~Mesh() {};

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
	bool load(LPDIRECT3DDEVICE9 device, const char* filename, Texture_Manager* texture_manage);

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
	Texture* get_mesh_texture(int index)
	{
		return mesh_textures[index];
	}

	void render(LPDIRECT3DDEVICE9 device);

	friend class Mesh_Manager;
};

#endif
