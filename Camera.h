#ifndef CAMERA_FIXED_H
#define CAMERA_FIXED_H
#include <d3dx9.h>

class Camera
{
private:
	D3DXVECTOR3 position;
	D3DXVECTOR3 look_at_target;
	D3DXVECTOR3 up_direction;
	float field_of_view;
	float aspect_ratio;
	float near_plane;
	float far_plane;

public:
	Camera();
	Camera(D3DXVECTOR3 position, D3DXVECTOR3 look_at_target, D3DXVECTOR3 up_direction,
		   float field_of_fiew, float aspect_ratio, float near_plane, float far_plane);
	virtual ~Camera() {};

	D3DXMATRIX get_view();
	D3DXMATRIX get_projection();
	virtual void update(float timestep) {};
};

#endif