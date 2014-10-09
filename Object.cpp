#include "Object.h"

Object::Object()
{
	entity_mesh = NULL;
	scale_factor = 1.0f;
	entity_type = NONE;
	axis_rotation = cos(0/2);
}
Object::Object(Mesh* model, D3DXVECTOR3 position, float scale_factor)
{
	entity_mesh = model;
	this->vector_position = position;
	this->scale_factor = scale_factor;
	D3DXQuaternionIdentity(&rotation);
	axis_rotation = cos(0/2);
}

Object::~Object()
{
}

void Object::release(Mesh_Manager* mesh_manage)
{
	if(visible)
	{
		if(entity_mesh)
		{
			mesh_manage->release_mesh(entity_mesh);
			entity_mesh = NULL;
		}
	}
}

void Object::render(LPDIRECT3DDEVICE9 device)
{
	//This method is in charge of creating the matrix needed to draw the model
	//in the correct position and with to correct rotations and scale.
	//It also sets this matrix as the world transform and renders the mesh
	if(visible)
	{
		if(entity_mesh)
		{
			D3DXMATRIX world;
			D3DXMATRIX scale;
			D3DXMATRIX rotation_matrix;
			D3DXMATRIX translation;

			D3DXMatrixScaling(&scale, scale_factor, scale_factor, scale_factor);
			D3DXMatrixRotationQuaternion(&rotation_matrix, &rotation);
			D3DXMatrixTranslation(&translation, vector_position.x, vector_position.y, vector_position.z);

			world = scale * rotation_matrix * translation;

			device->SetTransform(D3DTS_WORLD, &world);

			entity_mesh->render(device);
		}
	}
}