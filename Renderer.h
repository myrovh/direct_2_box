#ifndef DIRECT3D_H
#define DIRECT3D_H

#include <d3dx9.h>
#include <vector>
#include "Object.h"
#include "Text.h"
#include "Camera.h"
#include "Button.h"

class Renderer
{
private:
	LPDIRECT3D9 direct3d_object;
	LPDIRECT3DDEVICE9 direct3d_device;
	LPD3DXFONT direct3d_font;
	UINT width;
	UINT height;

public:
	Renderer();
	~Renderer();

	bool initialise(HWND window_instance, UINT width, UINT height, bool fullscreen);
	void render(std::vector<Object*> object_queue, std::vector<Button*> button_queue, 
				std::vector<Text*> font_queue, Camera* camera);
	void release();

	LPDIRECT3DDEVICE9 get_device() {
		return direct3d_device;
	}
	LPD3DXFONT get_font() {
		return direct3d_font;
	}

	UINT get_height() {
		return height;
	}
	UINT get_width() {
		return width;
	}
};

#endif