#ifndef DIE_H
#define DIE_H
#include "object.h"

class die :
	public object
{
private:
	float spin_speed;
	bool locked;
	int face_value;

public:
	die(mesh* model, D3DXVECTOR3 position, float x_rotation, float y_rotation,
		float z_rotation, float scale, float speed);
	void update(float timestep);

	void set_face_value(int face_value);
	void toggle_locked();

	bool get_locked() {return locked;}
	int get_face_value() {return face_value;}
};

#endif