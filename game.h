#ifndef GAME_H
#define GAME_H
#define _CRT_SECURE_NO_DEPRECATE

#include "input_manager.h"
#include "texture_manager.h"
#include "mesh_manager.h"
#include "object.h"
#include "direct3d.h"
#include "die.h"
#include "camera_fixed.h"
#include "font_rectangle.h"
#include <windows.h>
#include <stdio.h> //vsprintf
#include <vector> //object_queue
#include <list>
#include <sstream> //font insertion
#include <random>

enum yahtzee_types {
	ONES, 
	TWOS, 
	THREES, 
	FOURS, 
	FIVES, 
	SIXES, 
	BONUS,
	KIND_3,
	KIND_4,
	HOUSE,
	S_STRAIGHT,
	L_STRAIGHT,
	YAHTZEE,
	CHANCE
};

struct yahtzee_game
{
	// START State Settings
	bool is_game_over;
	bool is_score_for_round_set;
	int round_count;
	int rolls_remaining;
	int max_rolls;
	// END State Settings

	//START Physics Settings
	double throw_variance_floor;
	double throw_variance_ceiling;
	float throw_entropy;
	float throw_force;
	float gravity_force;
	D3DXVECTOR3 throw_direction;
	//END Physics Settings

	// START upper section
	int bonus_threshold;
	int bonus_value;
	int bonus_score;

	int ones_score;
	int twos_score;
	int threes_score;
	int fours_score;
	int fives_score;
	int sixes_score;
	// END upper section

	// START lower section
	int full_house_value;
	int small_straight_value;
	int large_straight_value;
	int yahtzee_value;

	int three_of_a_kind_score;
	int four_of_a_kind_score;
	int full_house_score;
	int small_straight_score;
	int large_straight_score;
	int yahtzee_score;
	int chance_score;
	// END lower section
};

class game
{
private:
	direct3d* direct3d_manage;
	input_manager* input_manage;
	texture_manager* texture_manage;
	mesh_manager* mesh_manage;
	camera_fixed* camera;
	std::vector<object*> object_queue;
	std::vector<font_rectangle*> font_queue;
	std::random_device rng_device;
	std::mt19937 rng_engine;
	yahtzee_game game_variables;

public:
	game();
	~game();

	bool initialise(HWND window_handler, bool fullscreen, input_manager* input);
	bool initialise_content();
	void update(float timestep);
	void render();
	void trace(const char * fmt, ...);

	void roll_dice();
	void new_round();
	int calculate_yahtzee_values(yahtzee_types value_to_return);
	bool assign_yahtzee_values(yahtzee_types value_to_set);
};

#endif