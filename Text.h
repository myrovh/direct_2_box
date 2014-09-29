#ifndef FONT_H
#define FONT_H
#include <d3dx9.h>
#include <string>

class Text
{
private:
	RECT positon;
	std::string text;
	DWORD font_format;
	D3DCOLOR colour;
	bool visible;

public:
	Text();
	Text(RECT positiont, DWORD font_format, D3DCOLOR colour);
	~Text() {}

	void set_visible() {
		visible = TRUE;
	}
	void set_invisible() {
		visible = FALSE;
	}
	bool is_visible() {
		return visible;
	}

	void update(std::string);
	void render(LPD3DXFONT font);
};

#endif
