#include "Font_Block.h"

Font_Block::Font_Block()
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

Font_Block::Font_Block(RECT position, DWORD font_format, D3DCOLOR colour, bool show)
{
	this->positon = position;
	this->font_format = font_format;
	this->colour = colour;
	this->show = show;
	text = "";
}

void Font_Block::update(std::string text)
{
	this->text = text;
}

void Font_Block::render(LPD3DXFONT font)
{
	if(show)
	{
		font->DrawText(NULL, text.c_str(), text.length(), &positon, font_format, colour);
	}
}

void Font_Block::toggle_visibility()
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