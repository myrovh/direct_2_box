#include "object.h"

object::object()
{
	entity_mesh = NULL;
	x_rotation = 0.0f;
	y_rotation = 0.0f;
	z_rotation = 0.0f;
	scale_factor = 1.0f;
}
object::object(mesh* model, D3DXVECTOR3 position, float x_rotation, float y_rotation,
			   float z_rotation, float scale_factor)
{
	entity_mesh = model;
	this->positon = position;
	this->x_rotation = x_rotation;
	this->y_rotation = y_rotation;
	this->z_rotation = z_rotation;
	this->scale_factor = scale_factor;
}

object::~object()
{
}

void object::release(mesh_manager* mesh_manage)
{
	if(entity_mesh)
	{
		mesh_manage->release_mesh(entity_mesh);
		entity_mesh = NULL;
	}
}

void object::render(LPDIRECT3DDEVICE9 device)
{
	//This method is in charge of creating the matrix needed to draw the model
	//in the correct position and with to correct rotations and scale.
	//It also sets this matrix as the world transform and renders the mesh
	if(entity_mesh)
	{
		D3DXMATRIX world;
		D3DXMATRIX scale;
		D3DXMATRIX rotation;
		D3DXMATRIX translation;

		D3DXMatrixScaling(&scale, scale_factor, scale_factor, scale_factor);
		D3DXMatrixRotationYawPitchRoll(&rotation, y_rotation, x_rotation, z_rotation);
		D3DXMatrixTranslation(&translation, positon.x, positon.y, positon.z);

		world = scale * rotation * translation;

		device->SetTransform(D3DTS_WORLD, &world);

		entity_mesh->render(device);
	}	
}
