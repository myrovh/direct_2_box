#include "game.h"

input_manager* input_manage = NULL;

LRESULT WINAPI WindowProcedure(HWND window_handler, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_DESTROY:
		case WM_CLOSE:
			PostQuitMessage(0);
			break;

		default:
			break;
	}

	return DefWindowProc(window_handler, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	game game_engine;

	// {{{ START Window Creation
	WNDCLASSEX window_class;
	char window_class_name[] = "Window Class";
	char window_class_title[] = "Test Engine";
	int window_x_location = 100;
	int window_y_location = 100;
	int window_width = 800;
	int window_height = 600;
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
		WS_OVERLAPPEDWINDOW,
		window_x_location, window_y_location,
		window_height, window_width,
		GetDesktopWindow(),
		NULL,
		window_class.hInstance,
		NULL
		);
	// }}} END Window Creation

	bool ok = TRUE;
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

		// BOOL bRet;
		MSG msg;

		LARGE_INTEGER frequency;
		QueryPerformanceCounter(&frequency);
		LARGE_INTEGER previous_count;
		QueryPerformanceCounter(&previous_count);

		while(!done)
		{
			LARGE_INTEGER current_count;
			QueryPerformanceCounter(&current_count);

			float timestep = (current_count.QuadPart - previous_count.QuadPart) / (float)frequency.QuadPart;

			if(PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
			{
				// If a quit message is received then stop rendering and quit the app.
				if(msg.message == WM_QUIT)
				{
					done = true;
				}

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				game_engine.update(timestep);
				game_engine.render();
			}
		}
	}

	DestroyWindow(window_handler);
	UnregisterClass(window_class_name, hInstance);

	return 0;
}