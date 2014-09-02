#include "camera_fixed.h"

camera_fixed::camera_fixed()
{
	position = D3DXVECTOR3(0, 0, 0);
	look_at_target = D3DXVECTOR3(0, 0, 0);
	up_direction = D3DXVECTOR3(0, 0, 0);
	field_of_view = D3DX_PI / 2;
	aspect_ratio = 0;
	near_plane = 0.1f;
	far_plane = 12.0f;
}

camera_fixed::camera_fixed(D3DXVECTOR3 position, D3DXVECTOR3 look_at_target, D3DXVECTOR3 up_direction,
			 float field_of_fiew, float aspect_ratio, float near_plane, float far_plane)
{
	this->position = position;
	this->look_at_target = look_at_target;
	this->up_direction = up_direction;
	this->field_of_view = field_of_fiew;
	this->aspect_ratio = aspect_ratio;
	this->near_plane = near_plane;
	this->far_plane = far_plane;
}

D3DXMATRIX camera_fixed::get_view()
{
	D3DXMATRIX view;
	D3DXMatrixLookAtLH(&view, &position, &look_at_target, &up_direction);
	return view;
}

D3DXMATRIX camera_fixed::get_projection()
{
	D3DXMATRIX projection;
	D3DXMatrixPerspectiveFovLH(&projection, field_of_view, aspect_ratio, near_plane, far_plane);
	return projection;
}