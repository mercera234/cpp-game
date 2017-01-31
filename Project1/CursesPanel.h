#pragma once

#include "curses.h"

#include "panel.h"
#include <string>
using namespace std;

class CursesPanel 
{
private:
	PANEL* mainPanel;
	WINDOW* win;
	WINDOW* subWin;
	string label;
	int colorPair;
	bool showing;
public:
	CursesPanel(int x, int y, int width, int height, string label);
	CursesPanel(PANEL* panel, WINDOW* mainWin, WINDOW* subWin, string label);
	void setColor(int colorPair);
	void toggle();
	void draw();
	void show();
	void hide();
	void top();
	void bottom();

	~CursesPanel();
};