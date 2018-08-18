#pragma once
#include "MenuItem.h"


enum class Fielddraw
{
	DRAW_NAME,
	DRAW_ICON
};

class LineItem : public MenuItem
{
private:
	Fielddraw field;
	std::string description;
	chtype icon;
	int crossRef;
public:
	std::string name;
	LineItem(std::string name, int element, int crossRefNdx);
	
	void draw();
	void clear();

	//getters/setters
	void setField(Fielddraw field) { this->field = field; }
	void setIcon(chtype c) { icon = c; }
	chtype getIcon() { return icon; }
	int getCrossRef() { return crossRef; }
};