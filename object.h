#ifndef ENTITY_H
#define ENTITY_H

#include <d3dx9.h>
#include "mesh_manager.h"

class object
{
protected:
	D3DXVECTOR3 positon;
	mesh* entity_mesh;
	float x_rotation;
	float y_rotation;
	float z_rotation;
	float scale_factor;

public:
	object();
	object(mesh* model, D3DXVECTOR3 position, float x_rotation, float y_rotation,
		   float z_rotation, float scale_factor);
	virtual ~object();
	void release(mesh_manager* mesh_manage);

	virtual void update(float timestep) = 0;
	virtual void render(LPDIRECT3DDEVICE9 device);
};

// Create the bounding box inside here
// using two points dynamically generated or a bounding sphere using centre point and a radius
// x, y, z min and max for bounding box
#endif