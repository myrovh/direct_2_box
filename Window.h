#ifndef WINDOW_H
#define WINDOW_H
#include <Windows.h>

class Window
{
private:
	HINSTANCE instance;
	HWND handle;
	UINT width;
	UINT height;
	const char* title;
	const char* name;

public:
	Window(HINSTANCE instance, WNDPROC window_procdure, UINT width, UINT height,
		   const char* title, const char* name);
	~Window();

	//Accessors
	HWND get_handle() {
		return handle;
	}
	UINT get_width() {
		return width;
	}
	UINT get_height() {
		return height;
	}

	//Functions
	void show();
};

#endif