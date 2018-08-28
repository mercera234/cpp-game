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
		case 1: cm->popControl(); 
			
			break;
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
			WINDOW* win = newwin(4, 40, 6, 4);
			WINDOW* dWin = derwin(win, 1, 38, 2, 1);
			GridMenu* modal = new GridMenu(dWin, 1, 2);
			modal->setItem(new LineItem("Yes", 0, 0));
			modal->setItem(new LineItem("No", 1, 1));
			modal->setCurrentItem(0);
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
	Palette* p = (Palette*)ptr;
	p->driver(input);
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
	GridMenu m1(win, 2, 1);

	m1.setItem(new LineItem("Yes", 0, 0));
	m1.setItem(new LineItem("No", 1, 1));
	m1.setCurrentItem(0);

	m1.post(true);

	Palette palette(4, 4, "CM Test", 7, 1);
	
	for (int i = 0; i < 7; i++)
	{
		chtype c = ' ' | i << BKGDCOLOR_OFFSET;
		palette.setItem(colorNames[i], c, i);
	}
	palette.setFocusable(false);
	palette.setCurrentItem(0);
	palette.post(true);

	TextField field1(15, 1, 30);


	ControlManager cm;
	cm.registerControl(&palette, MOUSE_LISTENER, callBackTest2);
	cm.registerControl(&field1, MOUSE_LISTENER | KEY_LISTENER, textCallback);
	cm.registerControl(&m1, MOUSE_LISTENER | KEY_LISTENER, callBackTest);
	cm.registerShortcutKey(CTRL_N, newCallback);
	cm.registerShortcutKey(KEY_ESC, quitCallback);

	cm.setFocus(&field1);

	while (cm.isActive())
	{
		wnoutrefresh(stdscr);
		cm.draw();

		doupdate();
		int c = getch();

		clear();
		cm.handleInput(c);
	}
	cm.shutdown();

}
