#include "game.h"

input_manager* input_manage = NULL;

LRESULT WINAPI WindowProcedure(HWND window_handler, UINT message_handle, WPARAM wParam, LPARAM lParam)
{
	switch(message_handle)
	{
		case WM_KEYDOWN:
			input_manage->set_key_down(wParam);
			return 0;
		case WM_KEYUP:
			input_manage->set_key_up(wParam);
			return 0;
		case WM_LBUTTONDOWN:
			input_manage->set_mouse_down(LEFT_MOUSE);
			return 0;
		case WM_LBUTTONUP:
			input_manage->set_mouse_up(LEFT_MOUSE);
			return 0;
		case WM_RBUTTONDOWN:
			input_manage->set_mouse_down(RIGHT_MOUSE);
			return 0;
		case WM_RBUTTONUP:
			input_manage->set_mouse_up(RIGHT_MOUSE);
			return 0;
		case WM_MBUTTONDOWN:
			input_manage->set_mouse_down(MIDDLE_MOUSE);
			return 0;
		case WM_MBUTTONUP:
			input_manage->set_mouse_up(MIDDLE_MOUSE);
			return 0;
		case WM_DESTROY:
		case WM_CLOSE:
			PostQuitMessage(0);
			break;

		default:
			break;
	}

	return DefWindowProc(window_handler, message_handle, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	game game_engine;
	MSG message_handle;

	// {{{ START Window Creation
	WNDCLASSEX window_class;
	char window_class_name[] = "Window Class";
	char window_class_title[] = "Test Engine";
	int window_x_location = 100;
	int window_y_location = 100;
	int window_width = 640;
	int window_height = 480;
	bool done = FALSE;
	memset(&window_class, 0, sizeof(window_class));

	window_class.cbSize = sizeof(WNDCLASSEX);
	window_class.style = CS_CLASSDC;
	window_class.lpfnWndProc = &WindowProcedure;
	window_class.hInstance = hInstance;
	window_class.lpszClassName = window_class_name;

	RegisterClassEx(&window_class);

	HWND window_handler = CreateWindow(
		window_class_name,
		window_class_title,
		WS_OVERLAPPEDWINDOW | WS_VISIBLE |
		WS_SYSMENU | WS_CLIPCHILDREN |
		WS_CLIPSIBLINGS,
		window_x_location, window_y_location,
		window_width, window_height,
		GetDesktopWindow(),
		NULL,
		window_class.hInstance,
		NULL
		);
	// }}} END Window Creation

	bool ok = TRUE;
	input_manage = new input_manager(window_handler);
	if(!game_engine.initialise(window_handler, FALSE, input_manage))
	{
		ok = FALSE;
	}
	else
	{
		if(!game_engine.initialise_content())
		{
			ok = FALSE;
		}
	}

	if(ok)
	{
		ShowWindow(window_handler, SW_SHOW);
		UpdateWindow(window_handler);

		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);
		LARGE_INTEGER previous_timer_count;
		QueryPerformanceCounter(&previous_timer_count);

		while(!done)
		{
			if(PeekMessage(&message_handle, NULL, NULL, NULL, PM_REMOVE))
			{
				// If a quit message is received then stop rendering and quit the app.
				if(message_handle.message == WM_QUIT)
				{
					done = TRUE;
				}

				TranslateMessage(&message_handle);
				DispatchMessage(&message_handle);
			}
			else
			{
				LARGE_INTEGER current_timer_count;
				QueryPerformanceCounter(&current_timer_count);

				float timestep = (current_timer_count.QuadPart - previous_timer_count.QuadPart) 
					/ (float)frequency.QuadPart;

				game_engine.update(timestep);
				game_engine.render();

				previous_timer_count = current_timer_count;
			}
		}
	}

	DestroyWindow(window_handler);
	UnregisterClass(window_class_name, hInstance);

	return 0;
}