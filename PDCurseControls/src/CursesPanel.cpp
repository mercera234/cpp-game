#include "CursesPanel.h"


CursesPanel::CursesPanel(int x, int y, int width, int height, string label)
{
	win = newwin(height, width, x, y);
	mainPanel = new_panel(win);
	showing = false;
	this->label = label;
	box(win, 0, 0);
	
	int totalMarginSpace = width - label.length();
	int startNdx = totalMarginSpace / 2;
	mvwaddstr(win, 0, startNdx, label.c_str());
	
	wbkgd(win, COLOR_PAIR(5));
}

CursesPanel::CursesPanel(PANEL* panel, WINDOW* mainWin, WINDOW* subWin, string label)
{
	mainPanel = panel;
	win = mainWin;
	this->subWin = subWin;
	mainPanel = new_panel(win);
	showing = false;
	this->label = label;
}

void CursesPanel::setColor(int colorPair)
{
	this->colorPair = colorPair;
}


void CursesPanel::toggle()
{
	
}

void CursesPanel::draw()
{
	wattron(win, COLOR_PAIR(colorPair));
	update_panels();
}

void CursesPanel::show()
{
	showing = true;
	
}

void CursesPanel::hide()
{

}

void CursesPanel::top()
{
	top_panel(mainPanel);
	update_panels();
}

void CursesPanel::bottom()
{
	bottom_panel(mainPanel);
	update_panels();
}

CursesPanel::~CursesPanel()
{
	delwin(win);
	delwin(subWin);
	del_panel(mainPanel);
}