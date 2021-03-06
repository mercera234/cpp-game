#pragma once
#include <iostream>
#include "curses.h"
#include "Controllable.h"
#include "GridMenu.h"
#include "TextLabel.h"
#include "LineItem.h"
#include "InputProcessor.h"


/*
A palette is a special menu that allows the user to select particular icons usually as tools
*/
class Palette : public Controllable, public InputProcessor
{
private:
	TextLabel title;
	GridMenu menu;
	TextLabel selection;
public:
	Palette() {}
	Palette(unsigned short y, unsigned short x, const std::string& titleText, unsigned short menuRows, unsigned short menuCols);
	

	/*Set the placement of the palette, and the amount of rows, cols that the menu will have. 
	The title and selection will automatically get 1 row and be the same width as the menu*/
	void setWindows(unsigned short y, unsigned short x, unsigned short menuRows, unsigned short menuCols);

	void setItem(const std::string& name, chtype icon, unsigned short index);

	void draw(); //override
	void processInput();
	
	bool post(bool post);
	void setFocus(bool focusIn);
	
	LineItem* getItem(int menuRow, int menuCol);
	void setCurrentItem(int index);
	LineItem* getCurrentItem();

	//getters/setters
	void setTitle(const std::string& text);
	std::string getTitle() { return title.getText(); }
};