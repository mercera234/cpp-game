#include "Palette.h"

Palette::Palette(int rows, int cols, int y, int x)
{
	this->rows = rows;
	this->cols = cols;
	win = newwin(rows + 1, cols * 3, y, x); //an extra row is added for the display text of the selected tool
	mark = '>';
	selectedItem = 0;
	itemCount = rows * cols;
	//items = new chtype[itemCount];
	items = new PaletteItem[itemCount];
}

void Palette::setItem(string name, chtype icon, int row, int col)
{
	int element = row * cols + col;
	items[element].index = element;
	items[element].name = name;
	items[element].icon = icon;
}

void Palette::draw()
{
	int scale = 3;
	wclear(win);

	string displayName;
	//cycle through each tool
	for (int row = 0; row < rows; row++) 
	{
		for (int col = 0; col < cols; col++)
		{
			int element = row * cols + col;
			chtype c = items[element].icon;

			if (element == selectedItem)
			{
				mvwaddch(win, row, col * scale, mark);
				displayName = items[element].name;
			}
			else
			{
				mvwaddch(win, row, col * scale, ' ');
			}

			mvwaddch(win, row, (col * scale) + 1, c);
		}
	}
	
	mvwaddstr(win, rows, 0, displayName.c_str());
	wnoutrefresh(win);
}



Palette::~Palette()
{
	delwin(win);
}


PaletteItem* Palette::pickItem(int y, int x) //returns the raw color and sets that to the selected color
{
	int row = y;
	int col = x / 3;
	selectedItem = row * cols + col;
	return &items[selectedItem];
}

WINDOW* Palette::getWindow()
{
	return win;
}



