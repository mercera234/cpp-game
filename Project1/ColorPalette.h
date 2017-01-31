#pragma once
#include "curses.h"
#include "Controllable.h"

#define TOTAL_COLORS 16
class ColorPalette : public Controllable
{
private:

	WINDOW* win;
	char mark;
	short selectedColor;
	chtype* colors;

public:
	ColorPalette(int y, int x);

	void draw();
	~ColorPalette();
	chtype pickColor(int y, int x);
	WINDOW* getWindow();
};
