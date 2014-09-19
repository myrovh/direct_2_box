#include "Game.h"
#include "Window.h"

Input_Manager* input_manage = NULL;

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
	Window window(hInstance, &WindowProcedure, 1024, 768, "Test Engine", "Test Engine");
	input_manage = new Input_Manager(window.get_handle());
	Game game_engine;
	MSG message_handle;
	bool done = FALSE;

	bool ok = TRUE;
	if(!game_engine.initialise(window.get_handle(), FALSE, input_manage))
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
		window.show();
		UpdateWindow(window.get_handle());

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
	return 0;
}