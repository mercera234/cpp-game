#include "ControlManager.h"
#include "Frame.h"
#include "GridMenu.h"
#include "LineItem.h"
#include "TextField.h"
#include "Palette.h"

void modalCallback(void* caller, void* ptr, int input)
{
	Frame* f = (Frame*)ptr;
	GridMenu* m = (GridMenu*)f->getControl();

	MenuItem* item = NULL;
	switch (input)
	{
	case KEY_LEFT: m->driver(REQ_LEFT_ITEM);  break;
	case KEY_RIGHT: m->driver(REQ_RIGHT_ITEM);  break;
		//case KEY_UP: m->driver(REQ_UP_ITEM);  break;
		//case KEY_DOWN: m->driver(REQ_DOWN_ITEM);  break;
	case '\r':
		item = m->getCurrentItem();
		break;
	}

	
	if (item != NULL)
	{
		ControlManager* cm = f->getControlManager();
		switch (item->index)
		{
		case 0: cm->setActive(false); break;
		case 1: cm->popControl(); break;
		}
	}

}








void newCallback(void* caller, void* ptr, int input)
{
	clear();
	mvaddstr(25, 25, "new callback called");
}

void quitCallback(void* caller, void* ptr, int input)
{
	ControlManager* cm = (ControlManager*)ptr;
	cm->setActive(false);
}








void callBackTest(void* caller, void* ptr, int input)
{
	GridMenu* m = (GridMenu*)ptr;
	MenuItem* item = NULL;

	switch (input)
	{
	case KEY_LEFT: m->driver(REQ_LEFT_ITEM);  break;
	case KEY_RIGHT: m->driver(REQ_RIGHT_ITEM);  break;
	case KEY_UP: m->driver(REQ_UP_ITEM);  break;
	case KEY_DOWN: m->driver(REQ_DOWN_ITEM);  break;
	case '\r':
		item = m->getCurrentItem();
		break;
	}


	if (item != NULL)
	{
		switch (item->index)
		{
		case 0:
			//create modal menu
		{
			WINDOW* win = newwin(4, 40, 6, 40);
			WINDOW* dWin = derwin(win, 1, 38, 2, 1);
			GridMenu* modal = new GridMenu(dWin, 1, 2);
			modal->setItem(new LineItem("Yes", 0, 0));
			modal->setItem(new LineItem("No", 1, 1));
			modal->post(true);
			Frame* f = new Frame(win, modal);
			f->setText("Are you sure you want to quit?", 1, 1);
			f->setModal(true);
			ControlManager* cm = m->getControlManager();
			cm->registerControl(f, KEY_LISTENER, modalCallback);
			cm->setFocus(f);
		}
		break;

		}
	}

}



void callBackTest2(void* caller, void* ptr, int input)
{
	/*Palette* p = (Palette*)ptr;

	MEVENT event;
	nc_getmouse(&event);

	int colorY = event.y;
	int colorX = event.x;
	bool retval = wmouse_trafo(p->getWindow(), &colorY, &colorX, false);
	
	p->pickItem(colorY, colorX);*/
}


void textCallback(void* caller, void* ptr, int input)
{
	TextField* field = (TextField*)ptr;

	switch (input)
	{
	case KEY_BTAB:
	case '\t': break;
	default: field->inputChar(input); break;
	}

}


void controlManagerTest()
{
	WINDOW* win = newwin(2, 20, 1, 1);
	GridMenu* m1 = new GridMenu(win, 2, 1);

	m1->setItem(new LineItem("Yes", 0, 0));
	m1->setItem(new LineItem("No", 1, 1));

	m1->post(true);

	int rows;
	int cols;
	rows = cols = 4;
	Palette* palette = new Palette(rows, cols, "CM Test", 7, 1);
	for (int i = 0; i < TOTAL_COLORS; i++)
	{
		chtype c = ' ' | (i << 28) & A_COLOR;
		int x = i % cols;
		int y = i / cols;
		palette->setItem(colorNames[i], c, i);
	}

	TextField* field1 = new TextField(15, 1, 30);


	ControlManager* cm = new ControlManager(NULL);
	cm->registerControl(m1, MOUSE_LISTENER | KEY_LISTENER, callBackTest);
	cm->registerControl(palette, MOUSE_LISTENER, callBackTest2);
	cm->registerControl(field1, KEY_LISTENER, textCallback);
	cm->registerShortcutKey(CTRL_N, newCallback);
	cm->registerShortcutKey(KEY_ESC, quitCallback);

	while (cm->isActive())
	{
		wnoutrefresh(stdscr);
		cm->draw();

		doupdate();
		int c = getch();

		clear();
		cm->handleInput(c);
	}
	cm->shutdown();

}
