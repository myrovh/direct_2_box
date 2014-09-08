#include "game.h"

game::game()
{
	direct3d* direct3d_manage = NULL;
	texture_manager* texture_manage = NULL;
	mesh_manager* mesh_manage = NULL;
	input_manager* input_manage = NULL;
	yahtzee_game game_variables; //Not quite sure on how to be instantiating structs
	std::random_device rng_device;
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
	game_variables.ones_score = -1;
	game_variables.twos_score = -1;
	game_variables.threes_score = -1;
	game_variables.fours_score = -1;
	game_variables.fives_score = -1;
	game_variables.sixes_score = -1;
	game_variables.bonus_score = -1;
	game_variables.three_of_a_kind_score = -1;
	game_variables.four_of_a_kind_score = -1;
	game_variables.full_house_score = -1;
	game_variables.small_straight_score = -1;
	game_variables.large_straight_score = -1;
	game_variables.yahtzee_score = -1;
	game_variables.chance_score = -1;
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

	// START Font Rectangle for game over display
	// TODO build a struct to insert values into constructor
	RECT game_over_position;
	game_over_position.bottom = 200;
	game_over_position.top = 0;
	game_over_position.left = 300;
	game_over_position.right = 100;
	font_queue.push_back(new font_rectangle(game_over_position, DT_NOCLIP,
		D3DCOLOR_ARGB(255, 255, 255, 255), TRUE));
	font_queue[3]->set_invisible();
	// END Font Rectangle for dice values display

	return TRUE;
}

void game::update(float timestamp)
{
	std::stringstream font_output;
	// font_queue[0] = Game Title
	if(font_queue[0]->is_visible())
	{
		font_output << " Yahtzee Round: " << game_variables.round_count
			<< "\n Rolls Left: " << game_variables.rolls_remaining;
		font_queue[0]->update(font_output.str());
		font_output.str("");
	}
	// font_queue[1] = Dice Values
	if(font_queue[1]->is_visible())
	{
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
	}
	// font_queue[2] = Score List
	if(font_queue[2]->is_visible())
	{
		// START generate score inserts
		if(game_variables.ones_score == -1) { 
			font_output << "Ones: " << calculate_yahtzee_values(ONES) << " *\n"; }
		else { font_output << "Ones: " << game_variables.ones_score << "\n"; }
		if(game_variables.twos_score == -1) { 
			font_output << "Twos: " << calculate_yahtzee_values(TWOS) << " *\n"; }
		else { font_output << "Twos: " << game_variables.twos_score << "\n"; }
		if(game_variables.threes_score == -1) { 
			font_output << "Threes: " << calculate_yahtzee_values(THREES) << " *\n"; }
		else { font_output << "Threes: " << game_variables.threes_score << "\n"; }
		if(game_variables.fours_score == -1) { 
			font_output << "Fours: " << calculate_yahtzee_values(FOURS) << " *\n"; }
		else { font_output << "Fours: " << game_variables.fours_score << "\n"; }
		if(game_variables.fives_score == -1) { 
			font_output << "Fives: " << calculate_yahtzee_values(FIVES) << " *\n"; }
		else { font_output << "Fives: " << game_variables.fives_score << "\n"; }
		if(game_variables.sixes_score == -1) { 
			font_output << "Sixes: " << calculate_yahtzee_values(SIXES) << " *\n"; }
		else { font_output << "Sixes: " << game_variables.sixes_score << "\n"; }
		if(game_variables.bonus_score == -1) { 
			font_output << "Bonus: " << calculate_yahtzee_values(BONUS) << " *\n"; }
		else { font_output << "Bonus: " << game_variables.bonus_score << "\n"; }
		if(game_variables.three_of_a_kind_score == -1) { 
			font_output << "3 of a Kind : " << calculate_yahtzee_values(KIND_3) << " *\n"; }
		else { font_output << "3 of a Kind: " << game_variables.three_of_a_kind_score << "\n"; }
		if(game_variables.four_of_a_kind_score == -1) { 
			font_output << "4 of a Kind : " << calculate_yahtzee_values(KIND_4) << " *\n"; }
		else { font_output << "4 of a Kind: " << game_variables.four_of_a_kind_score << "\n"; }
		if(game_variables.full_house_score == -1) { 
			font_output << "Full House: " << calculate_yahtzee_values(HOUSE) << " *\n"; }
		else { font_output << "Full House: " << game_variables.full_house_score << "\n"; }
		if(game_variables.small_straight_score == -1) { 
			font_output << "Small Straight: " << calculate_yahtzee_values(S_STRAIGHT) << " *\n"; }
		else { font_output << "Small Straight: " << game_variables.small_straight_score << "\n"; }
		if(game_variables.large_straight_score == -1) { 
			font_output << "Large Straight: " << calculate_yahtzee_values(L_STRAIGHT) << " *\n"; }
		else { font_output << "Large Straight: " << game_variables.large_straight_score << "\n"; }
		if(game_variables.yahtzee_score == -1) { 
			font_output << "Yahtzee: " << calculate_yahtzee_values(YAHTZEE) << " *\n"; }
		else { font_output << "Yahtzee: " << game_variables.yahtzee_score << "\n"; }
		if(game_variables.chance_score == -1) { 
			font_output << "Chance: " << calculate_yahtzee_values(CHANCE) << " *\n"; }
		else { font_output << "Chance: " << game_variables.chance_score << "\n"; }
		// END generate score inserts
		// START print text and score inserts
		font_queue[2]->update(font_output.str());
		// END print text and score inserts
		font_output.str("");
	}
	// font_queue[3] = Game Over Score
	if(font_queue[3]->is_visible())
	{
		font_output << "Game is Over. Final Score = " << calculate_final_score() << "\n";
		font_output.str("");
	}

	input_manage->begin_update();

	if(input_manage->get_key_down(VK_ESCAPE))
	{
		PostQuitMessage(0);
	}

	if(!game_variables.is_game_over)
	{
		if(input_manage->get_key_down('R'))
		{
			if(game_variables.rolls_remaining > 0)
			{
				// TODO currently rolls will count down even if a die does not accept a roll
				roll_dice();
			}
		}
	}

	// START listen for command to lock dice
	if(!game_variables.is_game_over)
	{
		// TODO this is a bad solution
		// might be worth implementing a map for storage or some way 
		// to retrieve the object I want by name not queue location
		// TODO CHANGE THIS TO A TOGGLE
		if(TRUE)//game_variables.rolls_remaining < game_variables.max_rolls)
		{
			if(input_manage->get_key_down('1'))
			{
				die* temp_pointer = (die*)object_queue[0];
				temp_pointer->toggle_locked();
			}
			if(input_manage->get_key_down('2'))
			{
				die* temp_pointer = (die*)object_queue[1];
				temp_pointer->toggle_locked();
			}
			if(input_manage->get_key_down('3'))
			{
				die* temp_pointer = (die*)object_queue[2];
				temp_pointer->toggle_locked();
			}
			if(input_manage->get_key_down('4'))
			{
				die* temp_pointer = (die*)object_queue[3];
				temp_pointer->toggle_locked();
			}
			if(input_manage->get_key_down('5'))
			{
				die* temp_pointer = (die*)object_queue[4];
				temp_pointer->toggle_locked();
			}
		}
	}
	// END listen for command to lock dice

	//START set scores
	//TODO can implement a notification that value is already assigned
	if(!game_variables.is_game_over)
	{
		if(!game_variables.is_score_for_round_set)
		{
			if(input_manage->get_key_down('Z'))
			{ game_variables.is_score_for_round_set = assign_yahtzee_values(ONES); }
			if(input_manage->get_key_down('X'))
			{ game_variables.is_score_for_round_set = assign_yahtzee_values(TWOS); }
			if(input_manage->get_key_down('C'))
			{ game_variables.is_score_for_round_set = assign_yahtzee_values(THREES); }
			if(input_manage->get_key_down('V'))
			{ game_variables.is_score_for_round_set = assign_yahtzee_values(FOURS); }
			if(input_manage->get_key_down('B'))
			{ game_variables.is_score_for_round_set = assign_yahtzee_values(FIVES); }
			if(input_manage->get_key_down('N'))
			{ game_variables.is_score_for_round_set = assign_yahtzee_values(SIXES); }
			if(input_manage->get_key_down('M'))
			{ game_variables.is_score_for_round_set = assign_yahtzee_values(KIND_3); }
			if(input_manage->get_key_down('A'))
			{ game_variables.is_score_for_round_set = assign_yahtzee_values(KIND_4); }
			if(input_manage->get_key_down('S'))
			{ game_variables.is_score_for_round_set = assign_yahtzee_values(HOUSE); }
			if(input_manage->get_key_down('D'))
			{ game_variables.is_score_for_round_set = assign_yahtzee_values(S_STRAIGHT); }
			if(input_manage->get_key_down('F'))
			{ game_variables.is_score_for_round_set = assign_yahtzee_values(L_STRAIGHT); }
			if(input_manage->get_key_down('G'))
			{ game_variables.is_score_for_round_set = assign_yahtzee_values(YAHTZEE); }
			if(input_manage->get_key_down('H'))
			{ game_variables.is_score_for_round_set = assign_yahtzee_values(CHANCE); }
		}
	}
	//END set scores

	// START Check for game over
	game_variables.is_game_over = check_game_over();
	if(game_variables.is_game_over)
	{
		font_queue[3]->set_visible();
		assign_yahtzee_values(BONUS);
	}
	// END Check for game over

	if(game_variables.is_score_for_round_set && !game_variables.is_game_over)
	{
		new_round();
	}

	if(input_manage->get_key_down('U'))
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
				std::uniform_int_distribution<> die_range(1, 6);
				int face_value = die_range(rng_engine);
				temp_pointer->set_face_value(face_value);
				// END assign die face values

				//START create animation
				std::uniform_real_distribution<> variance(game_variables.throw_variance_floor, 
														  game_variables.throw_variance_ceiling);
				game_variables.throw_direction.x = variance(rng_engine);
				game_variables.throw_direction.z = variance(rng_engine);
				temp_pointer->set_throw(game_variables.throw_direction, game_variables.gravity_force,
										game_variables.throw_force, game_variables.throw_entropy);
				//END create animation

			}
		}
	}
	game_variables.rolls_remaining--;
}

bool game::check_game_over()
{
	bool any_scores_unset = FALSE;

	//Check all vars
	if(game_variables.ones_score == -1) { any_scores_unset = TRUE; }
	if(game_variables.twos_score == -1) { any_scores_unset = TRUE; }
	if(game_variables.threes_score == -1) { any_scores_unset = TRUE; }
	if(game_variables.fours_score == -1) { any_scores_unset = TRUE; }
	if(game_variables.fives_score == -1) { any_scores_unset = TRUE; }
	if(game_variables.sixes_score == -1) { any_scores_unset = TRUE; }
	if(game_variables.full_house_score == -1) { any_scores_unset = TRUE; }
	if(game_variables.small_straight_score == -1) { any_scores_unset = TRUE; }
	if(game_variables.large_straight_score == -1) { any_scores_unset = TRUE; }
	if(game_variables.yahtzee_score == -1) { any_scores_unset = TRUE; }
	if(game_variables.chance_score == -1) { any_scores_unset = TRUE; }

	if(any_scores_unset)
	{
		return FALSE;
	}

	return TRUE;
}

int game::calculate_final_score()
{
	int final_score = 0;

	// TODO recalculates each draw of game over should only do this once
	final_score += game_variables.ones_score;
	final_score += game_variables.twos_score;
	final_score += game_variables.threes_score;
	final_score += game_variables.fours_score;
	final_score += game_variables.fives_score;
	final_score += game_variables.sixes_score;
	final_score += game_variables.bonus_score;
	final_score += game_variables.full_house_score;
	final_score += game_variables.small_straight_score;
	final_score += game_variables.large_straight_score;
	final_score += game_variables.yahtzee_score;
	final_score += game_variables.chance_score;

	return final_score;
}

void game::new_round()
{
	game_variables.round_count++;
	game_variables.is_score_for_round_set = FALSE;
	game_variables.rolls_remaining = game_variables.max_rolls;
	for(unsigned int i = 0; i < object_queue.size(); i++)
	{
		if(object_queue[i]->get_object_type() == DIE)
		{
			die* temp_pointer = (die*)object_queue[i];
			temp_pointer->set_unlocked();
		}
	}
}

bool game::assign_yahtzee_values(yahtzee_types value_to_set)
{
	if(value_to_set == BONUS)
	{
		//TODO little problem here with unfilled columns subtracting from score
		int upper_score = game_variables.ones_score;
		upper_score += game_variables.twos_score;
		upper_score += game_variables.fours_score;
		upper_score += game_variables.fives_score;
		upper_score += game_variables.sixes_score;
		if(game_variables.bonus_score == -1)
		{
			if(upper_score >= game_variables.bonus_threshold)
			{
				game_variables.bonus_score = game_variables.bonus_value;
				return TRUE;
			}
		}
	}
	if(game_variables.rolls_remaining <= (game_variables.max_rolls - 1))
	{
		if(value_to_set == ONES)
		{
			if(game_variables.ones_score == -1)
			{
				game_variables.ones_score = calculate_yahtzee_values(ONES);
				return TRUE;
			}
		}
		if(value_to_set == TWOS)
		{
			if(game_variables.twos_score == -1)
			{
				game_variables.twos_score = calculate_yahtzee_values(TWOS);
				return TRUE;
			}
		}
		if(value_to_set == THREES)
		{
			if(game_variables.threes_score == -1)
			{
				game_variables.threes_score = calculate_yahtzee_values(THREES);
				return TRUE;
			}
		}
		if(value_to_set == FOURS)
		{
			if(game_variables.fours_score == -1)
			{
				game_variables.fours_score = calculate_yahtzee_values(FOURS);
				return TRUE;
			}
		}
		if(value_to_set == FIVES)
		{
			if(game_variables.fives_score == -1)
			{
				game_variables.fives_score = calculate_yahtzee_values(FIVES);
				return TRUE;
			}
		}
		if(value_to_set == SIXES)
		{
			if(game_variables.sixes_score == -1)
			{
				game_variables.sixes_score = calculate_yahtzee_values(SIXES);
				return TRUE;
			}
		}
		if(value_to_set == KIND_3)
		{
			if(game_variables.three_of_a_kind_score == -1)
			{
				game_variables.three_of_a_kind_score = calculate_yahtzee_values(KIND_3);
				return TRUE;
			}
		}
		if(value_to_set == KIND_4)
		{
			if(game_variables.four_of_a_kind_score == -1)
			{
				game_variables.four_of_a_kind_score = calculate_yahtzee_values(KIND_4);
				return TRUE;
			}
		}
		if(value_to_set == HOUSE)
		{
			if(game_variables.full_house_score == -1)
			{
				game_variables.full_house_score = calculate_yahtzee_values(HOUSE);
				return TRUE;
			}
		}
		if(value_to_set == S_STRAIGHT)
		{
			if(game_variables.small_straight_score == -1)
			{
				game_variables.small_straight_score = calculate_yahtzee_values(S_STRAIGHT);
				return TRUE;
			}
		}
		if(value_to_set == L_STRAIGHT)
		{
			if(game_variables.large_straight_score == -1)
			{
				game_variables.large_straight_score = calculate_yahtzee_values(L_STRAIGHT);
				return TRUE;
			}
		}
		if(value_to_set == YAHTZEE)
		{
			if(game_variables.yahtzee_score == -1)
			{
				game_variables.yahtzee_score = calculate_yahtzee_values(YAHTZEE);
				return TRUE;
			}
		}
		if(value_to_set == CHANCE)
		{
			if(game_variables.chance_score == -1)
			{
				game_variables.chance_score = calculate_yahtzee_values(CHANCE);
				return TRUE;
			}
		}
	}

	return FALSE;
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
		return return_score;
	}

	if(value_to_return == L_STRAIGHT)
	{
		bool is_large_straight = FALSE;
		int valid_count = 0;
		std::list<int>::iterator i = dice_value_queue.begin();

		for(int count = 1; count < 5; count++)
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
		return return_score;
	}
	// END test small and large straight

	// START yahtzee test
	if(value_to_return == YAHTZEE)
	{
		bool is_yahtzee = TRUE;
		std::list<int>::iterator i = dice_value_queue.begin();
		int previous_value = 0;

		for(; i != dice_value_queue.end(); i++)
		{
			if(previous_value != 0)
			{
				if(previous_value != *i)
				{
					is_yahtzee = FALSE;
				}
			}
			previous_value = *i;
		}

		if(is_yahtzee)
		{
			return_score = game_variables.yahtzee_value;
		}
		return return_score;
	}
	// END yahtzee test

	// START Chance test
	if(value_to_return == CHANCE)
	{
		return_score = total_dice_values;
		return return_score;
	}
	// END Chance test
}