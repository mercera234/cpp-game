#include "ControlManageApp.h"
#include "ControlManager.h"
#include "Frame.h"
#include "GridMenu.h"
#include "LineItem.h"
#include "TextField.h"
#include "Palette.h"
#include "SimpleCommand.h"


ControlManageApp::ControlManageApp()
{
}


ControlManageApp::~ControlManageApp()
{
}


void ControlManageApp::modalCallback()
{
	Frame* f = (Frame*)cm->getFocusedControl();
	GridMenu* m = (GridMenu*)f->getControl();

	MenuItem* item = NULL;
	switch (cm->getInput())
	{
		//broken
	/*case KEY_LEFT: m->driver(REQ_LEFT_ITEM);  break;
	case KEY_RIGHT: m->driver(REQ_RIGHT_ITEM);  break;*/
		//case KEY_UP: m->driver(REQ_UP_ITEM);  break;
		//case KEY_DOWN: m->driver(REQ_DOWN_ITEM);  break;
	case '\r':
		item = m->getCurrentItem();
		break;
	}

	if (item != NULL)
	{
		switch (item->index)
		{
		case 0: cm->setExitCode(ExitCode::TERMINATE); break;
		case 1: cm->popControl();
			cm->setExitCode(ExitCode::HANDLED);
			break;
		}
	}
}


void ControlManageApp::newCallback()
{
	clear();
	mvaddstr(20, 25, "new callback called");
	refresh();
	cm->setExitCode(ExitCode::HANDLED);
}

void ControlManageApp::quitCallback()
{
	cm->setExitCode(ExitCode::TERMINATE);
}


void ControlManageApp::callBackTest()
{
	GridMenu* m = (GridMenu*)cm->getFocusedControl();
	MenuItem* item = NULL;

	switch (cm->getInput())
	{
	/*case KEY_LEFT: m->driver(REQ_LEFT_ITEM);  break;
	case KEY_RIGHT: m->driver(REQ_RIGHT_ITEM);  break;
	case KEY_UP: m->driver(REQ_UP_ITEM);  break;
	case KEY_DOWN: m->driver(REQ_DOWN_ITEM);  break;*/
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

			SimpleCommand<ControlManageApp>* cmd = new SimpleCommand<ControlManageApp>();
			cmd->setReceiver(this);
			cmd->setAction(&ControlManageApp::modalCallback);

			cm->registerControl(f, KEY_LISTENER, cmd);
			cm->setFocusedControl(f);
		}
		break;

		}
	}
	cm->setExitCode(ExitCode::HANDLED);
}



void ControlManageApp::callBackTest2()
{
	Palette* p = (Palette*)cm->getFocusedControl();
	//broken
	//p->driver(cm->getInput());

	cm->setExitCode(ExitCode::HANDLED);
}


void ControlManageApp::textCallback()
{
	TextField* field = (TextField*)cm->getFocusedControl();

	int input = cm->getInput();
	switch (input)
	{
	case KEY_BTAB:
	case '\t': break;
	default: field->inputChar(input); break;
	}

	cm->setExitCode(ExitCode::HANDLED);
}
