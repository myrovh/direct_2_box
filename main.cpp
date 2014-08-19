#include "d2b.h"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev, LPSTR cmdLine, int cmdShow)
{
	d2b_setup settings;
	settings.window_instance = instance;
	settings.window_name = "Test App";

	new d2b(&settings);

	engine_pointer->d2b_run();

	return true;
}