#ifndef FONT_H
#define FONT_H
#include <d3dx9.h>

class font_rectangle
{
private:
	RECT positon;
	char* text;
	int text_length;
	DWORD font_format;
	D3DCOLOR colour;

public:
	font_rectangle();
	font_rectangle(RECT positiont, DWORD font_format, D3DCOLOR colour);
	~font_rectangle() {}


	void update(char* text, int text_length);
	void render(LPD3DXFONT font);
};

#endif
