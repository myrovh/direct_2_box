#include "Text.h"

Text::Text()
{
	positon.bottom = 0;
	positon.top = 0;
	positon.right = 0;
	positon.left = 0;
	text = "broken";
	colour = NULL;
	font_format = NULL;
	visible = FALSE;
}

Text::Text(RECT position, DWORD font_format, D3DCOLOR colour)
{
	this->positon = position;
	this->font_format = font_format;
	this->colour = colour;
	text = "";
	visible = FALSE;
}

void Text::update(std::string text)
{
	this->text = text;
}

void Text::render(LPD3DXFONT font)
{
	if(visible)
	{
		font->DrawText(NULL, text.c_str(), text.length(), &positon, font_format, colour);
	}
}