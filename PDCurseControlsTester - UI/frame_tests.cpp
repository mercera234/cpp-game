#include "Frame.h"
#include "GridMenu.h"

void frameTest()
{
	WINDOW* w = newwin(4, 40, 1, 1);
	WINDOW* dw = derwin(w, 1, 38, 2, 1);
	GridMenu m(dw, 1, 2);
	//m->setItem("Yes", "", 0, 0);
	//m->setItem("No", "", 1, 1);

	Frame f(w, &m);
	mvwaddstr(w, 1, 1, "Are you sure you want to quit?");

	f.draw();
	doupdate();
	getch();

	//	f->setBorder(BORDER_NONE);
	f.draw();
	doupdate();
	getch();

	delwin(dw);
	delwin(w);
}

void drawEmptyFrameTest()
{
	WINDOW* win = newwin(4, 5, 1, 1);
	Frame f(win, nullptr);

	f.draw();
	doupdate();
	getch();

	delwin(win);
}