#ifndef GAME_H
#define GAME_H
#define _CRT_SECURE_NO_DEPRECATE

#include <windows.h>
#include <stdio.h> //vsprintf
#include <vector> //object storage
#include <sstream> //font insertion

#include "Input_Manager.h"
#include "Texture_Manager.h"
#include "Mesh_Manager.h"
#include "Object.h"
#include "Renderer.h"
#include "Camera.h"
#include "Font_Block.h"

class Game
{
private:
	Renderer* renderer;
	Input_Manager* input_manage;
	Texture_Manager* texture_manage;
	Mesh_Manager* mesh_manage;
	Camera* camera;
	std::vector<Object*> object_queue;
	std::vector<Font_Block*> font_queue;

public:
	Game();
	~Game();

	bool initialise(HWND window_handler, bool fullscreen, Input_Manager* input);
	bool initialise_content();
	void update(float timestep);
	void render();
	void trace(const char * fmt, ...);
};

#endif
