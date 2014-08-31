#include "input_manager.h"

input_manager::input_manager(HWND windowHandle)
{
	window_handle = windowHandle;

	memset(previous_keys, 0, sizeof(previous_keys));
	memset(current_keys, 0, sizeof(current_keys));
	memset(previous_mouse, 0, sizeof(previous_mouse));
	memset(current_mouse, 0, sizeof(current_mouse));

	mouse_x_location = 0;
	mouse_y_location = 0;
}

void input_manager::set_key_down(UINT key_code)
{
	current_keys[key_code] = true;
}

void input_manager::set_key_up(UINT key_code)
{
	current_keys[key_code] = false;
}

void input_manager::set_mouse_up(int mouse_button)
{
	current_mouse[mouse_button] = false;
}

void input_manager::set_mouse_down(int mouse_button)
{
	current_mouse[mouse_button] = true;
}

bool input_manager::get_key_down(UINT key_code)
{
	return previous_keys[key_code] && !current_keys[key_code];
}

bool input_manager::get_key_up(UINT key_code)
{
	return previous_keys[key_code] && !current_keys[key_code];
}

bool input_manager::get_mouse_down(int mouse_button)
{
	return current_mouse[mouse_button];
}

bool input_manager::get_mouse_up(int mouse_button)
{
	return previous_mouse[mouse_button] && !current_mouse[mouse_button];
}

int input_manager::get_mouse_x()
{
	return mouse_x_location;
}

int input_manager::get_mouse_y()
{
	return mouse_y_location;
}

void input_manager::begin_update()
{
	POINT temp_point;
	GetCursorPos(&temp_point);
	ScreenToClient(window_handle, &temp_point);
	mouse_x_location = temp_point.x;
	mouse_y_location = temp_point.y;
}

void input_manager::end_update()
{
	memcpy(previous_keys, current_keys, sizeof(current_keys));
	memcpy(previous_mouse, current_mouse, sizeof(current_mouse));
}