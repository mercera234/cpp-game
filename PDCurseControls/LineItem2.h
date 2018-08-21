#pragma once
#include "ObjectLinkedItem.h"

//this is a primitive system for drawing specific fields
enum class Fielddraw
{
	DRAW_NAME,
	DRAW_ICON
};

template<typename T>
class LineItem2 : public ObjectLinkedItem<T>
{
private:
	Fielddraw field;
	std::string description;
	chtype icon;
public:
	std::string name;
	LineItem2(std::string name, int element, T crossRefNdx);
	
	void draw();
	void clear();

	//getters/setters
	void setField(Fielddraw field) { this->field = field; }
	void setIcon(chtype c) { icon = c; }
	chtype getIcon() { return icon; }
};

template<typename T>
LineItem2<T>::LineItem2(std::string name, int element, T crossRefNdx) : ObjectLinkedItem<T>(crossRefNdx)
{
	index = element;
	//crossRef = crossRefNdx;
	this->name = name;
	selectable = true;
	selected = false;
}


template<typename T>
void LineItem2<T>::draw()
{
	MenuItem::draw();

	//draw item
	switch (field)
	{
	case Fielddraw::DRAW_ICON:
		mvwaddch(win, topRow, itemX, icon);
		break;

	case Fielddraw::DRAW_NAME:
		mvwaddstr(win, topRow, itemX, name.c_str());
		break;
	}
}

template<typename T>
void LineItem2<T>::clear()
{
	name = "";
	MenuItem::clear();
}

//template<typename T>
//class LineItem2<T*> : public ObjectLinkedItem<T*>
//{
//private:
//	Fielddraw field;
//	std::string description;
//	chtype icon;
//public:
//	std::string name;
//	LineItem2(std::string name, int element, T* crossRefNdx);
//
////	void draw();
//	void clear();
//
//	//getters/setters
//	void setField(Fielddraw field) { this->field = field; }
//	void setIcon(chtype c) { icon = c; }
//	chtype getIcon() { return icon; }
//};
//
//template<typename T>
//LineItem2<T*>::LineItem2(std::string name, int element, T* crossRefNdx)
//{
//	index = element;
//	crossRef = crossRefNdx;
//	this->name = name;
//	selectable = true;
//	selected = false;
//}


//template<typename T>
//void LineItem2<T*>::draw()
//{
//	int offY = menu->getUlY();
//	int offX = menu->getUlX();
//
//	int markPosX = 0;
//	int itemPosX = 0;
//
//	markPosX = posX - offX;
//	itemPosX = markPosX + menu->getCursor().length();
//
//	WINDOW* win = menu->getWindow();
//	if (selected)
//		wattron(win, A_REVERSE);
//	else
//		wattroff(win, A_REVERSE);
//
//	if (this == menu->getCurrentItem()) //draw mark
//	{
//		mvwaddstr(win, posY - offY, markPosX, menu->getCursor().c_str());
//	}
//
//	//draw item
//	switch (field)
//	{
//	case Fielddraw::DRAW_ICON:
//		mvwaddch(win, posY - offY, itemPosX, icon);
//		break;
//
//	case Fielddraw::DRAW_NAME:
//		mvwaddstr(win, posY - offY, itemPosX, name.c_str());
//		break;
//	}
//
//
//}

//template<typename T*>
//void LineItem2<T*>::clear()
//{
//	name = "";
//	MenuItem::clear();
//}