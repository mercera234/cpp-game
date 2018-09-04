#include "Palette.h"

Palette::Palette(unsigned short y, unsigned short x, const std::string& titleText, unsigned short menuRows, unsigned short menuCols)
{
	setWindows(y, x, menuRows, menuCols);
	setTitle(titleText);
}

void Palette::setTitle(const std::string& text)
{
	title.setText(text);
}

void Palette::setWindows(unsigned short y, unsigned short x, unsigned short menuRows, unsigned short menuCols)
{
	int paletteWidth = menuCols * 3;
	setWindow(newwin(menuRows + 2, paletteWidth, y, x));//not sure if I need this or not

	title.setWindow(newwin(1, paletteWidth, y, x));

	menu.setCursor(shortCursor);

	int menuY = y + 1;
	menu.setWindow(newwin(menuRows, paletteWidth, menuY, x));
	menu.setItemWidth(1);
	menu.setFocusable(false);
	menu.setAcceptsMouseInput(true);
	menu.resetItems(menuRows, menuCols);

	selection.setWindow(newwin(1, paletteWidth, menuY + menuRows, x));
}


void Palette::setItem(const std::string& name, chtype icon, unsigned short index)
{
	LineItem* dotItem = new LineItem(name, index, -1);
	dotItem->setField(Fielddraw::DRAW_ICON);
	dotItem->setIcon(icon);

	menu.setItem(dotItem);
}

void Palette::setFocus(bool focusIn)
{
	Controllable::setFocus(focusIn);
	menu.setFocus(focusIn);
}


void Palette::draw()
{
	title.draw();
	menu.draw();
	selection.draw();
}


LineItem* Palette::getItem(int menuRow, int menuCol)
{
	return (LineItem*)menu.getItem(menuRow, menuCol);
}

void Palette::setCurrentItem(int index)
{
	menu.setCurrentItem(index);
}

LineItem* Palette::getCurrentItem()
{
	return (LineItem*)menu.getCurrentItem();
}

void Palette::driver(int input)
{
	menu.driver(input);
	LineItem* item = (LineItem*)menu.getCurrentItem();
	selection.setText(item->name);
}

bool Palette::post(bool post)
{
	bool retval = menu.post(post);
	LineItem* item = (LineItem*)menu.getCurrentItem();
	selection.setText(item->name);
	return retval;
}



