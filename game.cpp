#include "game.h"

game::game()
{
	direct3d* direct3d_manage = NULL;
	texture_manager* texture_manage = NULL;
	mesh_manager* mesh_manage = NULL;
	input_manager* input_manage = NULL;
}

game::~game()
{
	if(direct3d_manage != NULL)
	{
		delete direct3d_manage;
		direct3d_manage = NULL;
	}

	if(texture_manage != NULL)
	{
		delete texture_manage;
		texture_manage = NULL;
	}

	if(mesh_manage != NULL)
	{
		delete mesh_manage;
		mesh_manage = NULL;
	}

	if(input_manage != NULL)
	{
		delete input_manage;
		input_manage = NULL;
	}
}

bool game::initialise(HWND window_handler, bool fullscreen, input_manager* input)
{
	//input = new input_manager(window_handler);
	input_manage = input;

	direct3d_manage = new direct3d();
	if(!direct3d_manage->initialise(window_handler, fullscreen))
	{
		return FALSE;
	}

	texture_manage = new texture_manager();
	mesh_manage = new mesh_manager(texture_manage);

	return TRUE;
}

bool game::initialise_content()
{
	if(!mesh_manage->load(direct3d_manage->get_device(), "Die.x"))
	{
		return FALSE;
	}

	object_queue.push_back(new die(mesh_manage->get_mesh("Die.x"), D3DXVECTOR3(0, 0, 0),
		0, 0, 0.5f, 1.0f, 7.5f));

	return TRUE;
}

void game::update(float timestamp)
{
	input_manage->begin_update();

	if(input_manage->get_key_down('E'))
	{
		trace("Test Trace: E pressed \n");
	}

	for(int i = 0; i < object_queue.size(); i++)
	{
		object_queue[i]->update(timestamp);
	}

	input_manage->end_update();
}

void game::render()
{
	direct3d_manage->render(object_queue);
}

void game::trace(const char * fmt, ...)
{
	char buffer[8192];
	va_list args;

	va_start(args, fmt);
	vsprintf(buffer, fmt, args);

	OutputDebugString(buffer);
}