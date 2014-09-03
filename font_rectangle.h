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

public:
	font_rectangle();
	font_rectangle(RECT positiont, DWORD font_format, D3DCOLOR colour);
	~font_rectangle() {}


	void update(std::string);
	void render(LPD3DXFONT font);
};

#endif
