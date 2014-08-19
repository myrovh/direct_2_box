#include "d2b.h"


d2b::d2b(d2b_setup* d2b_settings)
{
	directx_loaded = false;

	default_settings = new d2b_setup;
	if(default_settings != NULL)
	{
		memcpy(default_settings, d2b_settings, sizeof(d2b_settings));
	}

	engine_pointer = this;

	// START Creating window
	WNDCLASSEX window_class;
	window_class.cbSize = sizeof(WNDCLASSEX);
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpfnWndProc = WindowProc;
	window_class.cbClsExtra = 0;
	window_class.cbWndExtra = 0;
	window_class.hInstance = default_settings->window_instance;
	window_class.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
	window_class.hbrBackground = NULL;
	window_class.lpszMenuName = NULL;
	window_class.lpszClassName = "window_class";
	window_class.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&window_class);

	window_handel = CreateWindowEx(
		NULL,
		"window_class",
		default_settings->window_name,
		WS_OVERLAPPEDWINDOW | WS_VISIBLE |
		WS_SYSMENU | WS_CLIPCHILDREN |
		WS_CLIPSIBLINGS,
		100, 100,
		640, 480,
		NULL,
		NULL,
		default_settings->window_instance,
		NULL);
	// END Creating window

	directx_loaded = true;
}


d2b::~d2b()
{
	if(directx_loaded)
	{

	}

	UnregisterClass("window_class", default_settings->window_instance);
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		default:
			return DefWindowProc(hWnd, msg, wparam, lparam);
	}
}

void d2b::d2b_run()
{
	if(directx_loaded == true)
	{
		ShowWindow(window_handel, SW_SHOW);
		UpdateWindow(window_handel);

		MSG msg;
		ZeroMemory(&msg, sizeof(MSG));
		while(msg.message != WM_QUIT)
		{
			if(PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				d2b_render();
			}
		}
	}
}

void d2b::d2b_render()
{
	Sleep(1000);
}