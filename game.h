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
#include <stdlib.h> //srand + rand
#include <time.h> //random seed
#include <sstream> //font insertion

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
	int throw_variance_floor;
	int throw_variance_ceiling;
	float throw_entropy;
	float throw_force;
	float gravity_force;
	//END Physics Settings

	// START upper section
	int bonus_threshold;
	int bonus_value;

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
};

#endif