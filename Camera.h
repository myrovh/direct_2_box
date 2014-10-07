#ifndef CAMERA_H
#define CAMERA_H

#include <d3dx9.h>
#include "Object.h"

class Camera
{
protected:
	D3DXVECTOR3 position;
	Object* look_at_target;
	D3DXVECTOR3 up_direction;
	float field_of_view;
	float aspect_ratio;
	float near_plane;
	float far_plane;

public:
	Camera();
	Camera(D3DXVECTOR3 position, Object* look_at_target, D3DXVECTOR3 up_direction,
		   float field_of_fiew, float aspect_ratio, float near_plane, float far_plane);
	virtual ~Camera() {};

	virtual D3DXMATRIX get_view();
	virtual D3DXMATRIX get_projection();
	virtual void update(float timestep) {};
};

#endif