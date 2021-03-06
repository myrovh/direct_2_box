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
	//Calculate values
	D3DXMATRIX view;
	D3DXQUATERNION rotation = look_at_target->get_object_rotation();
	D3DXVECTOR3 look_at_position = look_at_target->get_object_location();
	D3DXQUATERNION rotation_inverse;
	D3DXQuaternionConjugate(&rotation_inverse, &rotation);
	D3DXQUATERNION offset_quaternion(position.x, position.y, position.z, 0.0f);
	D3DXQUATERNION rotated_offset = rotation_inverse * offset_quaternion * rotation;

	//Set camera position to target location minus offset rotated with the same rotation as target
	D3DXVECTOR3 offset_vector(rotated_offset.x, rotated_offset.y, rotated_offset.z);
	offset_vector = -offset_vector;
	D3DXVECTOR3 camera_location = offset_vector + look_at_position;

	//Set up direction to target up instead of world up
	D3DXVECTOR3 camera_up = up_direction;
	D3DXMATRIX transform_up;
	D3DXMatrixRotationQuaternion(&transform_up, &rotation);
	D3DXVec3TransformCoord(&camera_up, &camera_up, &transform_up);

	D3DXMatrixLookAtLH(&view, &camera_location, &look_at_position, &camera_up);
	return view;
}

D3DXMATRIX Camera_Third::get_projection()
{
	D3DXMATRIX projection;
	D3DXMatrixPerspectiveFovLH(&projection, field_of_view, aspect_ratio, near_plane, far_plane);
	return projection;
}