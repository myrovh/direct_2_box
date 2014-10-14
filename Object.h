#ifndef ENTITY_H
#define ENTITY_H

#include <d3dx9.h>
#include "Mesh_Manager.h"
#include "Collision.h"

enum object_type
{
	NONE
};

class Object
{
protected:
	D3DXVECTOR3 vector_position;
	D3DXQUATERNION rotation;
	float axis_rotation;
	Mesh* entity_mesh;
	float scale_factor;
	object_type entity_type;
	bool visible;
	Collision* hit_box;

public:
	Object();
	Object(Mesh* model, D3DXVECTOR3 position, float scale_factor);
	virtual ~Object();
	void release(Mesh_Manager* mesh_manage);

	Collision* get_hit_box() {
		return hit_box;
	}
	D3DXVECTOR3 get_object_location(){
		//TODO privacy leak
		return vector_position;
	}
	D3DXQUATERNION get_object_rotation(){
		//TODO privacy leak
		return rotation;
	}
	virtual object_type get_object_type() {
		return entity_type;
	}
	bool is_visible(){
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

#endif
