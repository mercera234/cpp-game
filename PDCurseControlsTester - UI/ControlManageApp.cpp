#include "ControlManageApp.h"
#include "ControlManager.h"
#include "Frame.h"
#include "GridMenu.h"
#include "LineItem.h"
#include "TextField.h"
#include "Palette.h"
#include "SimpleControlCommand.h"


ControlManageApp::ControlManageApp()
{
}


ControlManageApp::~ControlManageApp()
{
}


int ControlManageApp::modalCallback(Controllable* ptr, int input)
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


	int handled = HANDLED;
	if (item != NULL)
	{
		ControlManager* cm = f->getControlManager();
		switch (item->index)
		{
		case 0: handled = 1; break;
		case 1: cm->popControl();

			break;
		}
	}

	return handled;
}


int ControlManageApp::newCallback(Controllable* ptr, int input)
{
	clear();
	mvaddstr(20, 25, "new callback called");
	refresh();
	return HANDLED;
}

int ControlManageApp::quitCallback(Controllable* ptr, int input)
{
	return 1;
}


int ControlManageApp::callBackTest(Controllable* ptr, int input)
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

			SimpleControlCommand<ControlManageApp>* cmd = new SimpleControlCommand<ControlManageApp>();
			cmd->setReceiver(this);
			cmd->setAction(&ControlManageApp::modalCallback);

			cm->registerControl(f, KEY_LISTENER, cmd);
			cm->setFocus(f);
		}
		break;

		}
	}
	return HANDLED;
}



int ControlManageApp::callBackTest2(Controllable* ptr, int input)
{
	Palette* p = (Palette*)ptr;
	p->driver(input);

	return HANDLED;
}


int ControlManageApp::textCallback(Controllable* ptr, int input)
{
	TextField* field = (TextField*)ptr;

	switch (input)
	{
	case KEY_BTAB:
	case '\t': break;
	default: field->inputChar(input); break;
	}

	return HANDLED;
}
