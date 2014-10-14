#ifndef COLLISION_SPHERE_H
#define COLLISION_SPHERE_H

#include "Collision.h"

class Collision_Sphere : public Collision
{
private:
	D3DXVECTOR3 offset;
	float radius;

public:
	Collision_Sphere(D3DXVECTOR3 offset, float radius);
	D3DXVECTOR3 get_offset() {
		return offset;
	}
	float get_radius(){
		return radius;
	}
};

#endif