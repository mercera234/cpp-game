#pragma once
#include "curses.h"
#include "Controllable.h"
#include <iostream>
using namespace std;

/*
Class is obsolete. All this functionality can be done in the GridMenu class.
*/
struct PaletteItem
{
	unsigned short index;
	string name;
	chtype icon;
};

class Palette : public Controllable
{
private:
	char mark;
	short selectedItem;
	unsigned short rows, cols;
	unsigned short itemCount; // rows * cols
	
	PaletteItem* items;
public:
	Palette(int rows, int cols, int y, int x);
	void setItem(string name, chtype icon, int row, int col);

	void draw();
	~Palette();
	//chtype pickItem(int y, int x);
	PaletteItem* pickItem(int y, int x);
};