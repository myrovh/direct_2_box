#include "Button.h"

Button::Button(Game* game, Texture* texture, LPD3DXFONT font, 
			   std::string text, int width, int height, 
			   D3DXVECTOR3 positon, game_function_pointer action)
{
	//Assign given parameters
	this->texture = texture;
	this->font = font;
	this->width = width;
	this->height = height;
	this->position = positon;
	this->on_click_action = action;
	this->m_gameObject = game;

	//Sets texture window to show the top half of the texture
	texture_window.top = 0;
	texture_window.left = 0;
	texture_window.bottom = height;
	texture_window.right = width;

	//Sets border of button based of positon vector
	border.top = positon.y - height / 2;
	border.bottom = positon.y - height / 2;
	border.left = positon.x - width / 2;
	border.right = positon.x - width / 2;

	//Sets center of button at half the width and height
	center.x = width / 2;
	center.y = height / 2;
	center.z = 0;
}

Button::~Button()
{
	if(sprite)
	{
		sprite->Release();
		sprite = NULL;
	}
}

bool Button::initialise_button(LPDIRECT3DDEVICE9 device)
{
	//This sprite will be used to draw the button
	if(FAILED(D3DXCreateSprite(device, &sprite)))
	{
		return FALSE;
	}
	return TRUE;
}

void Button::update(int mouse_x, int mouse_y, bool mouse_button)
{
	if ((mouse_x > border.left && mouse_x < border.right) &&
		(mouse_y > border.top && mouse_y < border.bottom))
	{
		texture_window.top = height;
		texture_window.bottom = height * 2;
		if(mouse_button)
		{
			(m_gameObject->*on_click_action)(40);
		}
	} 
	else
	{
		texture_window.top = 0;
		texture_window.bottom = height;
	}
}

void Button::render()
{
	sprite->Begin(D3DXSPRITE_ALPHABLEND);

	sprite->Draw(texture->get_texture(), &texture_window, &center, &position,
				 D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f));

	sprite->End();

	font->DrawText(NULL, text.c_str(), text.length(), &border, DT_CENTER | DT_VCENTER,
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

void Button::text_update(std::string text)
{
	this->text = text;
}