#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H
#include <Windows.h>

#define LEFT_MOUSE 0
#define RIGHT_MOUSE 1
#define MIDDLE_MOUSE 2
#define NUMBER_OF_MOUSE_BUTTONS 3
#define NUMBER_OF_KEYS 256

class Input_Manager
{
private:
	HWND window_handle;

	bool current_keys[NUMBER_OF_KEYS];
	bool previous_keys[NUMBER_OF_KEYS];
	bool current_mouse[NUMBER_OF_MOUSE_BUTTONS];
	bool previous_mouse[NUMBER_OF_MOUSE_BUTTONS];
	int mouse_x_location;
	int mouse_y_location;
	UINT screen_width;
	UINT screen_height;

public:
	Input_Manager(HWND window_handle, UINT width, UINT height);

	void set_key_down(UINT key_code);
	void set_key_up(UINT key_code);

	bool get_key_down(UINT key_code);
	bool get_key_up(UINT key_code);

	void set_mouse_down(int mouse_button);
	void set_mouse_up(int mouse_button);

	bool get_mouse_down(int mouse_button);
	bool get_mouse_up(int mouse_button);
	int get_mouse_x();
	int get_mouse_y();
	int get_mouse_x_centered();
	int get_mouse_y_centered();

	void begin_update();
	void end_update();
};

#endif
