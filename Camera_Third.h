#ifndef CAMERA_THIRD_H
#define CAMERA_THIRD_H

#include <d3dx9.h>
#include "Camera.h"

class Camera_Third : public Camera
{
public:
	Camera_Third();
	Camera_Third(D3DXVECTOR3 position, Object* look_at_target, D3DXVECTOR3 up_direction,
		   float field_of_fiew, float aspect_ratio, float near_plane, float far_plane);
	virtual ~Camera_Third() {};

	D3DXMATRIX get_view();
	D3DXMATRIX get_projection();
	virtual void update(float timestep) {};
};

#endif