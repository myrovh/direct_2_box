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
	bool show;

public:
	Text();
	Text(RECT positiont, DWORD font_format, D3DCOLOR colour, bool show);
	~Text() {}

	void toggle_visibility();
	void set_visible() {
		show = TRUE;
	}
	void set_invisible() {
		show = FALSE;
	}
	bool is_visible() {
		return show;
	}

	void update(std::string);
	void render(LPD3DXFONT font);
};

#endif
