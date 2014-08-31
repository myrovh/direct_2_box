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