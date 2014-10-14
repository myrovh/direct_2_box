#ifndef COLLISION_H
#define COLLISION_H

#include <d3dx9.h>

enum collision_type
{
	DOT,
	SPHERE
};

class Collision
{
protected:
	D3DXVECTOR3 position;
	collision_type type;

public:
	Collision();
	collision_type get_type() {
		return type;
	}
	D3DXVECTOR3 get_postion() {
		return position;
	}
	void update_position(D3DXVECTOR3 positon){
		this->position = positon;
	}
	static bool check_collision(Collision* target1, Collision* target2);
};

#endif