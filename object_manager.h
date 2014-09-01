#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H

#include <map>
#include <string>
#include <d3dx9.h>
#include "object.h"
#include "mesh.h"

enum object_type {MESH, TEXTURE, CAMERA, TEXT, LIGHT};
struct object_settings
{
	//GENERAL
	LPDIRECT3DDEVICE9 device;
	const char* filename;

	//ENTITY
	D3DXVECTOR3 location;
	float x_rotation;
	float y_rotation;
	float z_rotation;
	mesh* mesh_object;

	//MESH

	//TEXTURE

	//CAMERA
	float field_of_view;
	float aspect_ratio;
	float near_plane;
	float far_plane;

	D3DXVECTOR3 eye;
	D3DXVECTOR3 loot_at;
	D3DXVECTOR3 up_direction;
};

class object_manager
{
private:
	typedef std::map<std::string, object*> object_map;
	object_map object_stack;

public:
	object_manager();
	~object_manager();
	bool insert_object(object_type type, object_settings settings);
	object* get_object(const char* filename);
	void release_object(object* object);
	void release();

	void render(LPDIRECT3DDEVICE9 device); // at one point design was to have enity remember device to call it to draw at any time
	void update();
};

#endif