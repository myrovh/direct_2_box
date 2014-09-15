#include "Game.h"

Game::Game()
{
	renderer = NULL;
	texture_manage = NULL;
	mesh_manage = NULL;
	input_manage = NULL;
}

Game::~Game()
{
	for(unsigned int i = 0; i < object_queue.size(); i++)
	{
		object_queue[i]->release(mesh_manage);
	}

	if(renderer != NULL)
	{
		delete renderer;
		renderer = NULL;
	}

	if(mesh_manage != NULL)
	{
		delete mesh_manage;
		mesh_manage = NULL;
	}

	if(texture_manage != NULL)
	{
		delete texture_manage;
		texture_manage = NULL;
	}

	if(input_manage != NULL)
	{
		delete input_manage;
		input_manage = NULL;
	}
}

bool Game::initialise(HWND window_handler, bool fullscreen, Input_Manager* input)
{
	input_manage = input;

	renderer = new Renderer();
	if(!renderer->initialise(window_handler, fullscreen))
	{
		return FALSE;
	}

	texture_manage = new Texture_Manager();
	mesh_manage = new Mesh_Manager(texture_manage);

	return TRUE;
}

bool Game::initialise_content()
{
	camera = new Camera();

	return TRUE;
}

void Game::update(float timestamp)
{
	input_manage->begin_update();

	if(input_manage->get_key_down(VK_ESCAPE))
	{
		PostQuitMessage(0);
	}

	for(unsigned int i = 0; i < object_queue.size(); i++)
	{
		object_queue[i]->update(timestamp);
	}

	input_manage->end_update();
}

void Game::render()
{
	renderer->render(object_queue, font_queue, camera);
}

void Game::trace(const char * fmt, ...)
{
	char buffer[8192];
	va_list args;

	va_start(args, fmt);
	vsprintf(buffer, fmt, args);

	OutputDebugString(buffer);
}