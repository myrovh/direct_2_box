#include "Collision_Sphere.h"

Collision_Sphere::Collision_Sphere(D3DXVECTOR3 offset, float radius) : Collision()
{
	this->offset = offset;
	this->radius = radius;
	this->type = SPHERE;
}