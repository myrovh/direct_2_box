#include "font_rectangle.h"

font_rectangle::font_rectangle()
{
	positon.bottom = 0;
	positon.top = 0;
	positon.right = 0;
	positon.left = 0;
	text = "broken";
	text_length = 0;
	colour = NULL;
}

font_rectangle::font_rectangle(RECT position, DWORD font_format, D3DCOLOR colour)
{
	this->positon = position;
	this->font_format = font_format;
	this->colour = colour;
	text = "";
	text_length = 0;
}

void font_rectangle::update(char* text, int text_length)
{
	this->text = text;
	this->text_length = text_length;
}

void font_rectangle::render(LPD3DXFONT font)
{
	//font_rectangle::font.DrawText(NULL, text, text_length, positon, font_format, colour);
	font->DrawText(NULL, text, text_length, &positon, font_format, colour);
}
