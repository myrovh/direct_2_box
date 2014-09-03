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
}

font_rectangle::font_rectangle(RECT position, DWORD font_format, D3DCOLOR colour)
{
	this->positon = position;
	this->font_format = font_format;
	this->colour = colour;
	text = "";
}

void font_rectangle::update(std::string text)
{
	this->text = text;
}

void font_rectangle::render(LPD3DXFONT font)
{
	//font_rectangle::font.DrawText(NULL, text, text_length, positon, font_format, colour);
	font->DrawText(NULL, text.c_str(), text.length(), &positon, font_format, colour);
}
