#ifndef ENTITY_H
#define ENTITY_H

#include <d3dx9.h>
#include "Mesh_Manager.h"

enum object_type
{
	NONE
};

class Object
{
protected:
	D3DXVECTOR3 vector_position;
	D3DXVECTOR3 rotation;
	Mesh* entity_mesh;
	float scale_factor;
	object_type entity_type;
	bool visible;

public:
	Object();
	Object(Mesh* model, D3DXVECTOR3 position, D3DXVECTOR3 rotation,
		   float scale_factor);
	virtual ~Object();
	void release(Mesh_Manager* mesh_manage);

	virtual object_type get_object_type() {
		return entity_type;
	}
	bool get_is_visible(){
		return visible;
	}
	void set_visible(){
		visible = TRUE;
	}
	void set_invisible(){
		visible = FALSE;
	}

	virtual void update(float timestep) = 0;
	virtual void render(LPDIRECT3DDEVICE9 device);
};

// Create the bounding box inside here
// using two points dynamically generated or a bounding sphere using center point and a radius
// x, y, z min and max for bounding box
#endif
