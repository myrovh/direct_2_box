#include "Static_Object.h"

Static_Object::Static_Object(Mesh* model, D3DXVECTOR3 position, float scale)
		 : Object(model, position, scale)
{
	this->rotate_speed = 0;
	this->hit_box = new Collision_Sphere(D3DXVECTOR3(0, 0, 0), 3.0f);
}

Static_Object::Static_Object(Mesh* model, D3DXVECTOR3 position, float scale, float rotate_speed)
		 : Object(model, position, scale)
{
	this->rotate_speed = rotate_speed;
	this->hit_box = new Collision_Sphere(D3DXVECTOR3(0, 0, 0), 3.0f);
}

void Static_Object::update(float timestep)
{
	if(rotate_speed != 0)
	{
		D3DXQUATERNION rotation_test;
		D3DXQuaternionRotationYawPitchRoll(&rotation_test, timestep * rotate_speed, 0.0f, 0.0f);
		D3DXQuaternionNormalize(&rotation_test, &rotation_test);
		rotation *= rotation_test;
	}
}