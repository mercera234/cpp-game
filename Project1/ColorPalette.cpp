#include "ColorPalette.h"

ColorPalette::ColorPalette(int y, int x)
{
	win = newwin(4, 4 * 3, y, x);
	mark = '>';
	selectedColor = COLOR_BLACK;
	colors = new chtype[TOTAL_COLORS]; //16 total colors

	for (int i = 0; i < TOTAL_COLORS; i++)
	{
		colors[i] = (i << 28) & A_COLOR; //elements are background colors (what is rendered on screen)
	}
}

void ColorPalette::draw()
{
	int scale = 3;

	//cycle through each color
	for (int row = 0; row < 4; row++) //draw a 4x4 palette
	{
		for (int col = 0; col < 4; col++)
		{
			int element = row * 4 + col;
			chtype c = ' ' | colors[element];
			
			if (element == selectedColor)
			{
				mvwaddch(win, row, col * scale, mark);
			}
			else
			{
				mvwaddch(win, row, col * scale, ' ');
			}

			mvwaddch(win, row, (col * scale) + 1, c);
		}
	}

	wnoutrefresh(win);
}



ColorPalette::~ColorPalette()
{
	delwin(win);
}


chtype ColorPalette::pickColor(int y, int x) //returns the raw color and sets that to the selected color
{
	int row = y;
	int col = x / 3;
	int color = row * 4 + col;
	selectedColor = color;
	return selectedColor;
}

WINDOW* ColorPalette::getWindow()
{
	return win;
}



