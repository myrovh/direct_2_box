#ifndef STATIC_OBJECT_H
#define STATIC_OBJECT_H
#include "../engine/Object.h"
#include "../engine/Collision_Sphere.h"

class Static_Object : public Object
{
private:
	float rotate_speed;

public:
	Static_Object(Mesh* model, D3DXVECTOR3 position, float scale);
	Static_Object(Mesh* model, D3DXVECTOR3 position, float scale, float rotate_speed);

	void set_rotate_speed(float rotate_speed) {
		this->rotate_speed = rotate_speed;
	}
	void update(float timestep);
};

#endif