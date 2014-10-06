#include "Camera_Third.h"

Camera_Third::Camera_Third()
{
	position = D3DXVECTOR3(0, 0, 0);
	look_at_target = NULL;
	up_direction = D3DXVECTOR3(0, 0, 0);
	field_of_view = D3DX_PI / 2;
	aspect_ratio = 0;
	near_plane = 0.1f;
	far_plane = 12.0f;
}

Camera_Third::Camera_Third(D3DXVECTOR3 position, Object* look_at_target, D3DXVECTOR3 up_direction,
			   float field_of_fiew, float aspect_ratio, float near_plane, float far_plane)
			   : Camera(position, look_at_target, up_direction, field_of_fiew, aspect_ratio, near_plane, far_plane)
{
}

D3DXMATRIX Camera_Third::get_view()
{
	D3DXMATRIX view;
	D3DXVECTOR3 look_at_vector = look_at_target->get_object_location();
	D3DXVECTOR3 camera_location = look_at_vector + position;
	D3DXMatrixLookAtLH(&view, &camera_location, &look_at_vector, &up_direction);
	return view;
}

D3DXMATRIX Camera_Third::get_projection()
{
	D3DXMATRIX projection;
	D3DXMatrixPerspectiveFovLH(&projection, field_of_view, aspect_ratio, near_plane, far_plane);
	return projection;
}