#ifndef GAME_H
#define GAME_H
#define _CRT_SECURE_NO_DEPRECATE

#include "input_manager.h"
#include "texture_manager.h"
#include "mesh_manager.h"
#include "object.h"
#include "direct3d.h"
#include "die.h"
#include <windows.h>
#include <stdio.h> //vsprintf
#include <vector>

class game
{
private:
	direct3d* direct3d_manage;
	input_manager* input_manage;
	texture_manager* texture_manage;
	mesh_manager* mesh_manage;
	std::vector<object*> object_queue;

public:
	game();
	~game();

	bool initialise(HWND window_handler, bool fullscreen, input_manager* input);
	bool initialise_content();
	void update(float timestep);
	void render();
	void trace(const char * fmt, ...);
};

#endif