#include "die.h"

die::die(mesh* model, D3DXVECTOR3 position, float x_rotation, float y_rotation,
		 float z_rotation, float scale, float speed)
		 : object(model, position, x_rotation, y_rotation, z_rotation, scale)
{
	spin_speed = speed;
}

void die::update(float timesetp)
{
	y_rotation += spin_speed * timesetp;
}