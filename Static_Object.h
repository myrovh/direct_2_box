#ifndef STATIC_OBJECT_H
#define STATIC_OBJECT_H
#include "../engine/Object.h"
#include "../engine/Collision_Sphere.h"

class Static_Object : public Object
{
private:
	float rotate_speed;

public:
	Static_Object(Mesh* model, variable_map* constructor_settings, variable_map* variable_settings);

	void set_rotate_speed(float rotate_speed) {
		this->rotate_speed = rotate_speed;
	}
	void update(float timestep);
};

#endif