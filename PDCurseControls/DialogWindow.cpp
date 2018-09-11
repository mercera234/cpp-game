#include "DialogWindow.h"



DialogWindow::DialogWindow()
{
	modal = true;
}


DialogWindow::~DialogWindow()
{
	if(control != nullptr)
		delete control;

	if(innerWin != nullptr)
		delwin(innerWin);
}

void DialogWindow::setFocus(bool focusIn)
{
	frame.setFocus(focusIn);
}

void DialogWindow::draw()
{
	frame.draw();
}

void DialogWindow::setControl(Controllable* control)
{
	this->control = control;
	if (innerWin != nullptr)
		control->setWindow(innerWin);

	frame.setControl(control);
}



void DialogWindow::setWindow(WINDOW* win)
{
	Controllable::setWindow(win);
	frame.setWindow(win);

	int controlCols = getmaxx(win) - 2;
	int controlRows = getmaxy(win) - 2;
	int controlX = getbegx(win) + 1;
	int controlY = getbegy(win) + 1;
	innerWin = newwin(controlRows, controlCols, controlY, controlX);

	if(control != nullptr)
		control->setWindow(innerWin);
}


