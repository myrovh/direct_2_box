#ifndef BUTTON_H
#define BUTTON_H

#include <d3dx9.h>
#include <string>
#include "Texture.h"

class Button
{
private:
	int width;
	int height;
	D3DXVECTOR3 position;
	D3DXVECTOR3 center;
	Texture* texture;
	LPD3DXSPRITE sprite;
	LPD3DXFONT font;
	std::string text;
	RECT border;
	RECT texture_window;
	void(*on_click_action)();

public:
	Button(Texture* texture, LPD3DXFONT font, std::string text, int width, int height, 
		   D3DXVECTOR3 positon, void(*action)());
	~Button();
	bool initialise_button(LPDIRECT3DDEVICE9 device);
	void text_update(std::string text);
	void update(int mouse_x, int mouse_y, bool mouse_buttton);
	void render();
};

#endif