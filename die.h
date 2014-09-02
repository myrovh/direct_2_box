#ifndef DIE_H
#define DIE_H
#include "object.h"

class die :
	public object
{
private:
	float spin_speed;

public:
	die(mesh* model, D3DXVECTOR3 position, float x_rotation, float y_rotation,
		float z_rotation, float scale, float speed);
	void update(float timestep);
};

#endif