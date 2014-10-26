#ifndef ENTITY_H
#define ENTITY_H

#include <d3dx9.h>
#include "Mesh_Manager.h"
#include "Super_Dumb_Script.h"
#include "Collision_Sphere.h"

class Object
{
protected:
	D3DXVECTOR3 vector_position;
	D3DXQUATERNION rotation;
	float axis_rotation;
	Mesh* entity_mesh;
	float scale_factor;
	std::string entity_type;
	bool visible;
	Collision* hit_box;
	variable_map* variable_settings;

public:
	Object();
	Object(Mesh* mesh, variable_map* constructor_settings, variable_map* variable_settings);
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
	virtual std::string get_object_type() {
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
	virtual void reload_variables() = 0;
	virtual void render(LPDIRECT3DDEVICE9 device);
};

#endif
