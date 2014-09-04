#include "die.h"

die::die(mesh* model, D3DXVECTOR3 position, float x_rotation, float y_rotation,
		 float z_rotation, float scale, float speed)
		 : object(model, position, x_rotation, y_rotation, z_rotation, scale)
{
	spin_speed = speed;
	locked = FALSE;
	thrown = FALSE;
	face_value = 2;
	entity_type = DIE;
	starting_location = position;
	gravity.x = 0;
	gravity.y = -1;
	gravity.z = 0;
	gravity_force = 0;
	throw_force = 0;
	facing_array[0] = new D3DXVECTOR3(D3DX_PI, 0, 0);
	facing_array[1] = new D3DXVECTOR3(0, 0, 0);
	facing_array[2] = new D3DXVECTOR3(0, 0, D3DX_PI / 2);
	facing_array[3] = new D3DXVECTOR3(0, 0, -(D3DX_PI / 2));
	facing_array[4] = new D3DXVECTOR3(D3DX_PI / 2, 0, 0);
	facing_array[5] = new D3DXVECTOR3(-(D3DX_PI / 2), 0, 0);
}

void die::update(float timesetp)
{
	if(locked == FALSE)
	{
		// TODO this will most likely need to change to account for dice have a different face pointing up
		//y_rotation += spin_speed * timesetp;
		if(thrown)
		{
			D3DXVECTOR3 movement;
			D3DXVECTOR3 gravity_temp;

			throw_direction = throw_direction * (throw_force * timesetp);
			gravity_temp = gravity * (gravity_force * timesetp);

			D3DXVec3Add(&movement, &throw_direction, &gravity_temp);
			D3DXVec3Add(&positon, &movement, &positon);

			if(throw_force > 0)
			{
				throw_force -= throw_entropy * timesetp;
			}

			if(positon.y <= 0)
			{
				thrown = FALSE;
			}
		}
		else
		{
			y_rotation += spin_speed * timesetp;
		}
	}

	// START Move dice to face_value
	float x_move_to = facing_array[face_value - 1]->x;
	if(x_rotation != x_move_to)
	{
		x_rotation = x_move_to;
	}

	float z_move_to = facing_array[face_value - 1]->z;
	if(z_rotation != z_move_to)
	{
		z_rotation = z_move_to;
	}
	// END Move dice to face_value
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

void die::set_throw(D3DXVECTOR3 throw_direction, float gravity_force, float throw_force, float throw_entropy)
{
	if(!thrown)
	{
		this->throw_direction = throw_direction;
		this->gravity_force = gravity_force;
		this->throw_force = throw_force;
		this->throw_entropy = throw_entropy;
		thrown = TRUE;
	}
}