#include "game.h"

game::game()
{
	direct3d* direct3d_manage = NULL;
	texture_manager* texture_manage = NULL;
	mesh_manager* mesh_manage = NULL;
	input_manager* input_manage = NULL;
	yahtzee_game game_variables; //Not quite sure on how to be instantiating structs
	std::mt19937 rng_engine(rng_device());
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
	//camera = new camera_fixed(D3DXVECTOR3(18, 10, 10), D3DXVECTOR3(0, 0, 10), D3DXVECTOR3(0, 1, 0),
	//						  D3DX_PI / 2, 640 / (float)480, 0.1f, 60.0f);
	camera = new camera_fixed(D3DXVECTOR3(20, 50, 10), D3DXVECTOR3(0, 0, 10), D3DXVECTOR3(0, 1, 0),
							  D3DX_PI / 2, 640 / (float)480, 0.1f, 200.0f);
	// END Camera Initialise

	// START Init five dice in starting positions
	if(!mesh_manage->load(direct3d_manage->get_device(), "Die.x"))
	{
		return FALSE;
	}

	// TODO worth changing object constructor to use a struct instead of multiple values
	object_queue.push_back(new die(mesh_manage->get_mesh("Die.x"), D3DXVECTOR3(-12, 0, 0),
		0, 0, 0, 1.0f, 1.0f));
	object_queue.push_back(new die(mesh_manage->get_mesh("Die.x"), D3DXVECTOR3(-6, 0, 0),
		0, 0, 0, 1.0f, 1.0f));
	object_queue.push_back(new die(mesh_manage->get_mesh("Die.x"), D3DXVECTOR3(0, 0, 0),
		0, 0, 0, 1.0f, 1.0f));
	object_queue.push_back(new die(mesh_manage->get_mesh("Die.x"), D3DXVECTOR3(6, 0, 0),
		0, 0, 0, 1.0f, 1.0f));
	object_queue.push_back(new die(mesh_manage->get_mesh("Die.x"), D3DXVECTOR3(12, 0, 0),
		0, 0, 0, 1.0f, 1.0f));
	// END Init five dice in starting positions

	//START Physics Settings
	game_variables.throw_variance_floor = -1;
	game_variables.throw_variance_ceiling = 1;
	game_variables.throw_force = 50.0f;
	game_variables.throw_entropy = 30.0f;
	game_variables.gravity_force = 20.0f;
	game_variables.throw_direction.x = 0;
	game_variables.throw_direction.y = 1;
	game_variables.throw_direction.z = 0;
	//END Physics Settings

	// START Game settings
	// TODO these really should be const
	game_variables.max_rolls = 3;
	game_variables.bonus_threshold = 63;
	game_variables.bonus_value = 35;
	game_variables.full_house_value = 25;
	game_variables.small_straight_value = 30;
	game_variables.large_straight_value = 40;
	game_variables.yahtzee_value = 50;
	// END Game settings

	// START Zeoring Game Scores
	game_variables.twos_score = -1;
	game_variables.threes_score = -1;
	game_variables.fours_score = -1;
	game_variables.fives_score = -1;
	game_variables.sixes_score = -1;
	game_variables.three_of_a_kind_score = -1;
	game_variables.four_of_a_kind_score = -1;
	game_variables.full_house_score = -1;
	game_variables.small_straight_score = -1;
	game_variables.large_straight_score = -1;
	game_variables.yahtzee_score = -1;
	// END Zeoring Game Scores

	// START Zeroing Game State
	game_variables.is_game_over = FALSE;
	game_variables.is_score_for_round_set = FALSE;
	game_variables.round_count = 1;
	game_variables.rolls_remaining = game_variables.max_rolls;
	// END Zeroing Game State

	// START Font Rectangle for Title Display
	// TODO build a struct to insert values into constructor
	RECT title_position;
	title_position.bottom = 50;
	title_position.top = 0;
	title_position.left = 0;
	title_position.right = 80;
	font_queue.push_back(new font_rectangle(title_position, DT_LEFT | DT_NOCLIP | DT_VCENTER, 
		D3DCOLOR_ARGB(255, 255, 255, 255), TRUE));
	// END Font Rectangle for Title Display

	return TRUE;
}

void game::update(float timestamp)
{
	// font_queue[0] = Game Title
	std::stringstream game_title;
	game_title << " Yahtzee Round: " << game_variables.round_count
		<< "\n Rolls Left: " << game_variables.rolls_remaining;
	font_queue[0]->update(game_title.str());

	input_manage->begin_update();

	if(input_manage->get_key_down(VK_ESCAPE))
	{
		PostQuitMessage(0);
	}

	if(input_manage->get_key_down('R'))
	{
		if(game_variables.rolls_remaining > 0)
		{
			roll_dice();
		}
	}

	// TODO this is a bad solution
	// might be worth implementing a map for storage or some way 
	// to retrieve the object I want by name not queue location
	if(game_variables.rolls_remaining < game_variables.max_rolls)
	{
		if(input_manage->get_key_down('1'))
		{
			die* temp_pointer = (die*)object_queue[0];
			temp_pointer->set_locked();
		}
		if(input_manage->get_key_down('2'))
		{
			die* temp_pointer = (die*)object_queue[1];
			temp_pointer->set_locked();
		}
		if(input_manage->get_key_down('3'))
		{
			die* temp_pointer = (die*)object_queue[2];
			temp_pointer->set_locked();
		}
		if(input_manage->get_key_down('4'))
		{
			die* temp_pointer = (die*)object_queue[3];
			temp_pointer->set_locked();
		}
		if(input_manage->get_key_down('5'))
		{
			die* temp_pointer = (die*)object_queue[4];
			temp_pointer->set_locked();
		}
	}

	//TODO lock all dice when thrown has finished and there are no more rolls
	/*
	if(game_variables.rolls_remaining <= 0)
	{
		for(unsigned int i = 0; i < object_queue.size(); i++)
		{
			if(object_queue[i]->get_object_type() == DIE)
			{
				die* temp_pointer = (die*)object_queue[i];
				if(temp_pointer->get_locked() == FALSE)
				{
					temp_pointer->set_locked();
				}
			}
		}
	}
	*/

	if(input_manage->get_key_down('H'))
	{
		font_queue[0]->toggle_visibility();
	}

	for(unsigned int i = 0; i < object_queue.size(); i++)
	{
		object_queue[i]->update(timestamp);
	}


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
			// QUESTION using temp pointer to access custom fucntions of sub class correct method?
			die* temp_pointer = (die*)object_queue[i];
			if(temp_pointer->get_locked() == FALSE)
			{
				// START assign die face values
				int face_value = rand() % 6 + 1;
				temp_pointer->set_face_value(face_value);
				// END assign die face values

				//START create animation
				std::uniform_real_distribution<> variance(game_variables.throw_variance_floor, 
														  game_variables.throw_variance_ceiling);
				game_variables.throw_direction.x = variance(rng_engine);
				game_variables.throw_direction.z = variance(rng_engine);
				//TODO add check to run through all dice to ensure that they all get
				temp_pointer->set_throw(game_variables.throw_direction, game_variables.gravity_force,
										game_variables.throw_force, game_variables.throw_entropy);
				//END create animation

			}
		}
	}
	//game_variables.rolls_remaining--;
}