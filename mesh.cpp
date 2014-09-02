#include "mesh.h"


mesh::mesh()
{
	reference_count = 0;
	material_buffer = NULL;
	mesh_storage = NULL;
	number_of_materials = NULL;
	materials = NULL;
	mesh_textures = NULL;
	filename = NULL;
}

bool mesh::load(LPDIRECT3DDEVICE9 device, const char* filename, texture_manager* texture_manage)
{
	if(FAILED(D3DXLoadMeshFromX(filename, D3DXMESH_SYSTEMMEM, device, NULL, &material_buffer, NULL, &number_of_materials, &mesh_storage)))
	{
		return FALSE;
	}

	this->filename = filename;
	materials = new D3DMATERIAL9[number_of_materials];
	mesh_textures = new texture*[number_of_materials];
	D3DXMATERIAL* loaded_materials = (D3DXMATERIAL*)material_buffer->GetBufferPointer();

	for(DWORD i = 0; i < number_of_materials; i++)
	{
		materials[i] = loaded_materials[i].MatD3D;
		materials[i].Ambient = materials[i].Diffuse;

		char* texture_filename = loaded_materials[i].pTextureFilename;
		if(texture_manage->load(device, texture_filename))
		{
			mesh_textures[i] = texture_manage->get_texture(texture_filename);
		}
		else
		{
			mesh_textures[i] = NULL;
		}
	}
	return TRUE;
}

void mesh::release(texture_manager* texture_manage)
{
	for(DWORD i = 0; i < number_of_materials; i++)
	{
		if(mesh_textures[i] != NULL)
		{
			texture* texture = mesh_textures[i];
			texture_manage->release_texture(texture);
			mesh_textures[i] = NULL;
		}
	}

	if(materials != NULL)
	{
		delete[] materials;
		materials = NULL;
		number_of_materials = 0;
	}

	if(mesh_textures != NULL)
	{
		delete[] mesh_textures;
		mesh_textures = NULL;
	}

	if(material_buffer != NULL)
	{
		material_buffer->Release();
		material_buffer = NULL;
	}

	if(mesh_storage != NULL)
	{
		mesh_storage->Release();
		mesh_storage = NULL;
	}
}

void mesh::render(LPDIRECT3DDEVICE9 device)
{
	for(DWORD i = 0; i < number_of_materials; i++)
	{
		device->SetMaterial(&materials[i]);
		device->SetTexture(0, mesh_textures[i]->get_texture());
		mesh_storage->DrawSubset(i);
	}
}