#include "Frame.h"
#include "GridMenu.h"
#include "Image.h"

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
	WINDOW* win = TUI::winMgr.newWin(4, 5, 1, 1);
	Frame f(win, nullptr);

	f.draw();
	doupdate();
	getch();
}

void frameWithImageTest()
{
	resize_term(30, 100);

	Image img;
	img.setWindow(newwin(10, 10, 1, 1));
	img.setDimensions(5, 5);

	Frame frame;
	frame.setWindow(newwin(12, 12, 0, 0));
	frame.setControl(&img);

	int y = -1;
	int x = -1;
	img.setPosition(y, x);
	ITwoDStorage<chtype>& tileMap = img.getTileMap();
	tileMap.fill('!' | COLOR_BLUE << BKGDCOLOR_OFFSET);

	bool playing = true;
	while (playing)
	{
		frame.draw();
		doupdate();
		int c = getch();

		switch (c)
		{
		case KEY_DOWN: y--; break;
		case KEY_UP: y++; break;
		case KEY_LEFT: x++; break;
		case KEY_RIGHT: x--; break;
		case KEY_ESC:
			playing = false;
			continue;
			break;
		}
		img.setPosition(y, x);
	}
}