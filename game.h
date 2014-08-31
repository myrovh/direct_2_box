#ifndef GAME_H
#define GAME_H
#define _CRT_SECURE_NO_DEPRECATE

#include "input_manager.h"
#include "object_manager.h"
#include "direct3d.h"
#include <windows.h>
#include <stdio.h> //vsprintf

class game
{
private:
	direct3d* direct3d_manage;
	object_manager* object_manage;
	input_manager* input_manage;

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