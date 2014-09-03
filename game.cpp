#include "game.h"

game::game()
{
	direct3d* direct3d_manage = NULL;
	texture_manager* texture_manage = NULL;
	mesh_manager* mesh_manage = NULL;
	input_manager* input_manage = NULL;

	srand(time(NULL));
}

game::~game()
{
	for(unsigned int i = 0; i < object_queue.size(); i++)
	{
		object_queue[i]->release(mesh_manage);
	}

	if(direct3d_manage != NULL)
	{
		delete direct3d_manage;
		direct3d_manage = NULL;
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
	// START Camera Initialise
	camera = new camera_fixed(D3DXVECTOR3(18, 10, 10), D3DXVECTOR3(0, 0, 10), D3DXVECTOR3(0, 1, 0),
							  D3DX_PI / 2, 640 / (float)480, 0.1f, 60.0f);
	// END Camera Initialise

	// START Init five dice in starting positions
	if(!mesh_manage->load(direct3d_manage->get_device(), "Die.x"))
	{
		return FALSE;
	}

	// TODO worth changing object constructor to use a struct instead of multiple values
	object_queue.push_back(new die(mesh_manage->get_mesh("Die.x"), D3DXVECTOR3(-12, 0, 0),
		0, 0, 0, 1.0f, 7.0f));
	object_queue.push_back(new die(mesh_manage->get_mesh("Die.x"), D3DXVECTOR3(-6, 0, 0),
		0, 0, 0, 1.0f, 7.0f));
	object_queue.push_back(new die(mesh_manage->get_mesh("Die.x"), D3DXVECTOR3(0, 0, 0),
		0, 0, 0, 1.0f, 7.0f));
	object_queue.push_back(new die(mesh_manage->get_mesh("Die.x"), D3DXVECTOR3(6, 0, 0),
		0, 0, 0, 1.0f, 7.0f));
	object_queue.push_back(new die(mesh_manage->get_mesh("Die.x"), D3DXVECTOR3(12, 0, 0),
		0, 0, 0, 1.0f, 7.0f));
	// END Init five dice in starting positions


	RECT test_position;
	test_position.bottom = 50;
	test_position.top = 0;
	test_position.left = 0;
	test_position.right = 50;
	font_queue.push_back(new font_rectangle(test_position, DT_CENTER, D3DCOLOR_ARGB(255, 255, 255, 255)));

	return TRUE;
}

void game::update(float timestamp)
{
	input_manage->begin_update();

	if(input_manage->get_key_down(VK_ESCAPE))
	{
		PostQuitMessage(0);
	}

	if(input_manage->get_key_down('R'))
	{
		roll_dice();
	}

	for(unsigned int i = 0; i < object_queue.size(); i++)
	{
		object_queue[i]->update(timestamp);
	}

	font_queue[0]->update("TEST", 4);

	input_manage->end_update();
}

void game::render()
{
	direct3d_manage->render(object_queue, font_queue, camera);
}

void game::trace(const char * fmt, ...)
{
	char buffer[8192];
	va_list args;

	va_start(args, fmt);
	vsprintf(buffer, fmt, args);

	OutputDebugString(buffer);
}

void game::roll_dice()
{
	for(unsigned int i = 0; i < object_queue.size(); i++)
	{
		if(object_queue[i]->get_object_type() == DIE)
		{
			die* temp_pointer = (die*)object_queue[i];
			if(temp_pointer->get_locked() == FALSE)
			{
				int face_value = rand() % 6 + 1;
				temp_pointer->set_face_value(face_value);
			}
		}
	}
}