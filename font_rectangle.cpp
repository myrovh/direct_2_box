#include "font_rectangle.h"

font_rectangle::font_rectangle()
{
	positon.bottom = 0;
	positon.top = 0;
	positon.right = 0;
	positon.left = 0;
	text = "broken";
	colour = NULL;
	font_format = NULL;
	show = FALSE;
}

font_rectangle::font_rectangle(RECT position, DWORD font_format, D3DCOLOR colour, bool show)
{
	this->positon = position;
	this->font_format = font_format;
	this->colour = colour;
	this->show = show;
	text = "";
}

void font_rectangle::update(std::string text)
{
	this->text = text;
}

void font_rectangle::render(LPD3DXFONT font)
{
	if(show)
	{
		font->DrawText(NULL, text.c_str(), text.length(), &positon, font_format, colour);
	}
}

void font_rectangle::toggle_visibility()
{
	if(show == TRUE)
	{
		show = FALSE;
	}
	else
	{
		show = TRUE;
	}
}
