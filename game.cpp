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
	camera = new camera_fixed(D3DXVECTOR3(20, 40, 20), D3DXVECTOR3(0, 0, 10), D3DXVECTOR3(0, 1, 0),
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
	game_variables.throw_variance_floor = -0.3;
	game_variables.throw_variance_ceiling = 0.3;
	game_variables.throw_force = 60.0f;
	game_variables.throw_entropy = 2.0f;
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

	// START Font Rectangle for dice values display
	// TODO build a struct to insert values into constructor
	RECT dice_position;
	dice_position.bottom = 480;
	dice_position.top = 380;
	dice_position.left = 0;
	dice_position.right = 640;
	font_queue.push_back(new font_rectangle(dice_position, DT_CENTER | DT_NOCLIP | DT_VCENTER, 
		D3DCOLOR_ARGB(255, 255, 255, 255), TRUE));
	// END Font Rectangle for dice values display

	// START Font Rectangle for dice values display
	// TODO build a struct to insert values into constructor
	RECT score_position;
	score_position.bottom = 480;
	score_position.top = 0;
	score_position.left = 460;
	score_position.right = 640;
	font_queue.push_back(new font_rectangle(score_position, DT_NOCLIP,
		D3DCOLOR_ARGB(255, 255, 255, 255), TRUE));
	// END Font Rectangle for dice values display

	return TRUE;
}

void game::update(float timestamp)
{
	std::stringstream font_output;
	// font_queue[0] = Game Title
	font_output << " Yahtzee Round: " << game_variables.round_count
		<< "\n Rolls Left: " << game_variables.rolls_remaining;
	font_queue[0]->update(font_output.str());
	font_output.str("");
	// font_queue[1] = Dice Values
	for(unsigned int i = 0; i < object_queue.size(); i++)
	{
		if(object_queue[i]->get_object_type() == DIE)
		{
			die* temp_pointer = (die*)object_queue[i];
			font_output << "Die " << i + 1 << ": " << temp_pointer->get_face_value() << "    ";
		}
	}
	font_queue[1]->update(font_output.str());
	font_output.str("");
	// font_queue[2] = Score List
	font_output << "Ones: " << calculate_yahtzee_values(ONES) << "\n"
		<< "Twos: " << calculate_yahtzee_values(TWOS) << "\n"
		<< "Threes: " << calculate_yahtzee_values(THREES) << "\n"
		<< "Fours: " << calculate_yahtzee_values(FOURS) << "\n"
		<< "Fives: " << calculate_yahtzee_values(FIVES) << "\n"
		<< "Sixes: " << calculate_yahtzee_values(SIXES) << "\n"
		<< "Bonus: " << calculate_yahtzee_values(BONUS) << "\n"
		<< "3 of a Kind: " << calculate_yahtzee_values(KIND_3) << "\n"
		<< "4 of a Kind: " << calculate_yahtzee_values(KIND_4) << "\n"
		<< "Full House: " << calculate_yahtzee_values(HOUSE) << "\n"
		<< "Small Straight: " << calculate_yahtzee_values(S_STRAIGHT) << "\n"
		<< "Large Straight: " << calculate_yahtzee_values(L_STRAIGHT) << "\n";
	font_queue[2]->update(font_output.str());
	font_output.str("");

	input_manage->begin_update();

	if(input_manage->get_key_down(VK_ESCAPE))
	{
		PostQuitMessage(0);
	}

	if(input_manage->get_key_down('R'))
	{
		if(game_variables.rolls_remaining > 0)
		{
			// TODO currently rolls will count down even if a die does not accept a roll
			roll_dice();
		}
	}

	// TODO this is a bad solution
	// might be worth implementing a map for storage or some way 
	// to retrieve the object I want by name not queue location
	// TODO CHANGE THIS TO A TOGGLE
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

int game::calculate_yahtzee_values(yahtzee_types value_to_return)
{
	std::list<int> dice_value_queue;
	int return_score = 0;
	int total_dice_values = 0;

	// START put dice value into a queue
	for(unsigned int i = 0; i < object_queue.size(); i++)
	{
		if(object_queue[i]->get_object_type() == DIE)
		{
			die* temp_pointer = (die*)object_queue[i];
			dice_value_queue.push_back(temp_pointer->get_face_value());
		}
	}
	dice_value_queue.sort();
	// END put dice value into a queue

	// START calculate total value of rolled dice
	for(std::list<int>::iterator i = dice_value_queue.begin(); i != dice_value_queue.end(); i++)
	{
		total_dice_values += *i;
	}
	// END calculate total value of rolled dice

	// START Calculate ONES
	if(value_to_return == ONES)
	{
		for(std::list<int>::iterator i = dice_value_queue.begin(); i != dice_value_queue.end(); i++)
		{
			if(*i == 1)
			{
				return_score += *i;
			}
		}
		return return_score;
	}
	// END Calculate ONES

	// START Calculate TWOS
	if(value_to_return == TWOS)
	{
		for(std::list<int>::iterator i = dice_value_queue.begin(); i != dice_value_queue.end(); i++)
		{
			if(*i == 2)
			{
				return_score += *i;
			}
		}
		return return_score;
	}
	// END Calculate TWOS

	// START Calculate THREES
	if(value_to_return == THREES)
	{
		for(std::list<int>::iterator i = dice_value_queue.begin(); i != dice_value_queue.end(); i++)
		{
			if(*i == 3)
			{
				return_score += *i;
			}
		}
		return return_score;
	}
	// END Calculate THREES

	// START Calculate FOURS
	if(value_to_return == FOURS)
	{
		for(std::list<int>::iterator i = dice_value_queue.begin(); i != dice_value_queue.end(); i++)
		{
			if(*i == 4)
			{
				return_score += *i;
			}
		}
		return return_score;
	}
	// END Calculate FOURS

	// START Calculate FIVES
	if(value_to_return == FIVES)
	{
		for(std::list<int>::iterator i = dice_value_queue.begin(); i != dice_value_queue.end(); i++)
		{
			if(*i == 5)
			{
				return_score += *i;
			}
		}
		return return_score;
	}
	// END Calculate FIVES

	// START Calculate SIXES
	if(value_to_return == SIXES)
	{
		for(std::list<int>::iterator i = dice_value_queue.begin(); i != dice_value_queue.end(); i++)
		{
			if(*i == 6)
			{
				return_score += *i;
			}
		}
		return return_score;
	}
	// END Calculate SIXES

	// START Calculate BONUS
	if(value_to_return == BONUS)
	{
		// TODO worth throwing score values into a map or vector to make this test easier
		int upper_section_total = 0;
		if(game_variables.ones_score != -1)
		{
			upper_section_total += game_variables.ones_score;
		}
		if(game_variables.twos_score != -1)
		{
			upper_section_total += game_variables.twos_score;
		}
		if(game_variables.threes_score != -1)
		{
			upper_section_total += game_variables.threes_score;
		}
		if(game_variables.fours_score != -1)
		{
			upper_section_total += game_variables.fours_score;
		}
		if(game_variables.fives_score != -1)
		{
			upper_section_total += game_variables.fives_score;
		}
		if(game_variables.sixes_score != -1)
		{
			upper_section_total += game_variables.sixes_score;
		}
		if(upper_section_total >= game_variables.bonus_threshold)
		{
			return_score = game_variables.bonus_value;
		}
		return return_score;
	}
	// END Calculate BONUS

	// START Calculate KIND_3 and KIND_4 and YAHTZEE and HOUSE
	if(value_to_return == KIND_3 || value_to_return == KIND_4 || 
	   value_to_return == YAHTZEE || value_to_return == HOUSE)
	{
		bool is_two_pair = FALSE;
		bool is_three_pair = FALSE;
		bool is_four_pair = FALSE;
		bool is_five_pair = FALSE;
		int face_count[6] = {0, 0, 0, 0, 0, 0};

		// START Get number of each die value
		for(std::list<int>::iterator i = dice_value_queue.begin(); i != dice_value_queue.end(); i++)
		{
			if(*i == 1)
			{
				face_count[0]++;
			}
			if(*i == 2)
			{
				face_count[1]++;
			}
			if(*i == 3)
			{
				face_count[2]++;
			}
			if(*i == 4)
			{
				face_count[3]++;
			}
			if(*i == 5)
			{
				face_count[4]++;
			}
			if(*i == 6)
			{
				face_count[5]++;
			}
		}
		// END Get number of each die value

		//START Test for yahtzee
		for(unsigned int i = 0; i < 6; i++)
		{
			if(face_count[i] == 5)
			{
				is_five_pair = TRUE;
			}
		}
		//END Test for yahtzee

		//START Test for four of a kind
		for(unsigned int i = 0; i < 6; i++)
		{
			if(face_count[i] == 4)
			{
				is_four_pair = TRUE;
			}
		}
		//END Test for four of a kind

		//START Test state for dice sets
		for(unsigned int i = 0; i < 6; i++)
		{
			if(face_count[i] == 5)
			{
				is_five_pair = TRUE;
			}
			if(face_count[i] == 4)
			{
				is_four_pair = TRUE;
			}
			if(face_count[i] == 3)
			{
				is_three_pair = TRUE;
			}
			if(face_count[i] == 2)
			{
				is_two_pair = TRUE;
			}
		}
		//START Test state for dice sets

		//START assign scores
		if(value_to_return == KIND_3)
		{
			if(is_three_pair)
			{
				return_score = total_dice_values;
			}
		}
		else if(value_to_return == KIND_4)
		{
			if(is_four_pair)
			{
				return_score = total_dice_values;
			}
		}
		else if(value_to_return == HOUSE)
		{
			if(is_two_pair && is_three_pair)
			{
				return_score = game_variables.full_house_value;
			}
		}
		else if(value_to_return == YAHTZEE)
		{
			if(is_five_pair)
			{
				return_score = game_variables.yahtzee_value;
			}
		}
		//END assign scores
		return return_score;
	}
	// END Calculate KIND_3 and KIND_4 and YAHTZEE and HOUSE

	// START test small and large straight
	if(value_to_return == S_STRAIGHT)
	{
		bool is_small_straight = FALSE;
		std::list<int>::iterator start = dice_value_queue.begin();

		for(int iterator_count = 0; iterator_count <= 3 && !is_small_straight; iterator_count++)
		{
			int valid_count = 0;
			for(int count = 0; count <= 4; count++)
			{
				std::list<int>::iterator pass = start; 
				if(*(pass++) == (*pass + 1))
				{
					valid_count++;
					if(valid_count == 4)
					{
						is_small_straight = TRUE;
					}
				}
				pass++;
			}
			start++;
		}

		if(is_small_straight)
		{
			return_score = game_variables.small_straight_value;
		}
	}

	if(value_to_return == L_STRAIGHT)
	{
		bool is_large_straight = FALSE;
		int valid_count = 0;

		for(std::list<int>::iterator i = dice_value_queue.begin(); i != dice_value_queue.end(); i++)
		{
			if(*(i++) == (*i + 1))
			{
				valid_count++;
				if(valid_count == 5)
				{
					is_large_straight = TRUE;
				}
			}
		}

		if(is_large_straight)
		{
			return_score = game_variables.large_straight_score;
		}
	}
	// END test small and large straight
}