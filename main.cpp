#include "directx.h"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev, LPSTR cmdLine, int cmdShow)
{
	// pass struct into directx class to define information for window instance and directx properties
	new directx(instance);

	engine_pointer->run();

	return true;
}