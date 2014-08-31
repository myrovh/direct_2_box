#include "game.h"

game::game()
{
	direct3d* direct3d_manage = NULL;
	object_manager* object_manage = NULL;
	input_manager* input_manage = NULL;
}

game::~game()
{
	if(object_manage != NULL)
	{
		delete object_manage;
		object_manage = NULL;
	}

	if(direct3d_manage != NULL)
	{
		delete direct3d_manage;
		direct3d_manage = NULL;
	}
}

bool game::initialise(HWND window_handler, bool fullscreen, input_manager* input)
{
	input = new input_manager(window_handler);
	input_manage = input;

	direct3d_manage = new direct3d();
	if(!direct3d_manage->initialise(window_handler, fullscreen))
	{
		return FALSE;
	}

	object_manage = new object_manager();

	return TRUE;
}

bool game::initialise_content()
{
	return TRUE;
}

void game::update(float timestamp)
{

}

void game::render()
{
	Sleep(1000);
}

void game::trace(const char * fmt, ...)
{
	char buffer[8192];
	va_list args;

	va_start(args, fmt);
	vsprintf(buffer, fmt, args);

	OutputDebugString(buffer);
}