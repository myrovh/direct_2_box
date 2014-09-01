#ifndef ENTITY_H
#define ENTITY_H
#include "object.h"
#include "object_manager.h"

class entity :
	public object
{
public:
	entity();
	entity(object_settings settings);
	virtual ~entity();

	virtual void update(float timestep) = 0;
	virtual void render(LPDIRECT3DDEVICE9 device);
};

// Create the bounding box inside here
// using two points dynamically generated or a bounding sphere using centre point and a radius
// x, y, z min and max for bounding box
#endif