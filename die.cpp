#include "die.h"

die::die(mesh* model, D3DXVECTOR3 position, float x_rotation, float y_rotation,
		 float z_rotation, float scale, float speed)
		 : object(model, position, x_rotation, y_rotation, z_rotation, scale)
{
	spin_speed = speed;
	locked = FALSE;
	face_value = 0;
	entity_type = DIE;
}

void die::update(float timesetp)
{
	y_rotation += spin_speed * timesetp;
}

void die::set_face_value(int face_value)
{
	if(face_value > 0 && face_value < 7)
	{
		this->face_value = face_value;
	}
	else
	{
		this->face_value = 0;
	}
}

void die::toggle_locked()
{
	if(locked == TRUE)
	{
		locked = FALSE;
	}
	else
	{
		locked = TRUE;
	}
}