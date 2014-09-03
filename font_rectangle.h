#ifndef FONT_H
#define FONT_H
#include <d3dx9.h>
#include <string>

class font_rectangle
{
private:
	RECT positon;
	std::string text;
	DWORD font_format;
	D3DCOLOR colour;
	bool show;

public:
	font_rectangle();
	font_rectangle(RECT positiont, DWORD font_format, D3DCOLOR colour, bool show);
	~font_rectangle() {}


	void toggle_visibility();
	void update(std::string);
	void render(LPD3DXFONT font);
};

#endif
