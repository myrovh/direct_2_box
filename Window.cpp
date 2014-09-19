#include "Window.h"

Window::Window(HINSTANCE instance, WNDPROC window_procdure, UINT width, UINT height, const char* title, const char* name)
{
	this->instance = instance;
	this->width = width;
	this->height = height;
	this->title = title;
	this->name = name;

	//Create window class
	WNDCLASSEX window_class;
	window_class.cbSize = sizeof(WNDCLASSEX);
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpfnWndProc = window_procdure;
	window_class.cbClsExtra = 0;
	window_class.cbWndExtra = 0;
	window_class.hInstance = instance;
	window_class.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
	window_class.hbrBackground = NULL;
	window_class.lpszMenuName = NULL;
	window_class.lpszClassName = name;
	window_class.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	//Register window_class with windows
	RegisterClassEx(&window_class);

	//Create window and store it in the 'handle'
	handle = CreateWindowEx(NULL,
							name,
							title,
							WS_OVERLAPPEDWINDOW | WS_VISIBLE |
							WS_SYSMENU | WS_CLIPCHILDREN |
							WS_CLIPSIBLINGS,
							100, 100, // Starting coordinates of window corner
							width, height,
							NULL,
							NULL,
							instance,
							NULL);
}

Window::~Window() 
{
	UnregisterClass(name, instance);
}

void Window::show()
{
	ShowWindow(handle, SW_SHOW);
}