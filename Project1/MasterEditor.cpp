#include "MasterEditor.h"
#include "dirent.h"
#include "LineItem.h"

MasterEditor::MasterEditor()
{
	//setup filename (this should eventually be moved to program that sets up master editor
	fileName = DEF_FILENAME;
	WINDOW* textWin = newwin(1, 15, 0, 1);
	fileNameLbl = new TextLabel(textWin, fileName);

	//setup default file path for opening/saving files
	char buf[256];
	GetFullPathName(".", 256, buf, NULL);
	dialogDefPath = buf;

	modified = false;

	//resize window
	resize_term(EDITOR_HEIGHT, EDITOR_WIDTH);

	//populate menu
	masterMenu = new GridMenu(newwin(EDITOR_HEIGHT - 4, 20, 2, 2), 3, 1);
	masterMenu->setItem(new LineItem("Map", 0, -1));
	masterMenu->setItem(new LineItem("Actor", 1, -1));
	masterMenu->setItem(new LineItem("Quit", 2, -1));

	masterMenu->post(true);
	
	cm = new ControlManager(this);

	cm->registerShortcutKey(KEY_ESC, globalCallback);

	cm->registerControl(fileNameLbl, NULL, NULL);
	
	cm->registerControl(masterMenu, KEY_LISTENER, menuCallback);
	cm->setFocus(masterMenu);

	//curEditor = (Editor*)this; //set to this by default
	curEditor = NULL; //by default no editor is loaded

	editors.push_back((Editor*)new MapEditor());
	editors.push_back((Editor*)new ActorEditor());
}

void MasterEditor::draw()
{
	clear();
	wnoutrefresh(stdscr);
	fileNameLbl->draw();
	masterMenu->draw();

	if (curEditor != NULL)
		curEditor->draw();
}

bool MasterEditor::processInput(int input)
{
	if(curEditor == NULL)
		return cm->handleInput(input);

	bool usingEditor = curEditor->processInput(input);

	if (usingEditor == false)
	{
		curEditor = NULL;
	}

	return true;
}

void MasterEditor::processMenuInput(int input)
{
	MenuItem* item = NULL;
	switch (input)
	{
	case KEY_DOWN: masterMenu->driver(REQ_DOWN_ITEM); break;
	case KEY_UP: masterMenu->driver(REQ_UP_ITEM); break;

	case '\r':
	case '\n':
	case KEY_ENTER:
		item = masterMenu->getCurrentItem(); 
		break;
	}

	if (item != NULL)
	{
		switch (item->index)
		{ //this logic is really bad, but we are just testing for now
		case 0: curEditor = editors.front(); break;
		case 1: curEditor = editors.back(); break;
		case 2: cm->setActive(false); break;
		}
		
		if (curEditor != NULL)
		{			
			ControlManager* cm = curEditor->getControlManager();
			cm->setActive(true);			
		}
	
	}



}

void MasterEditor::processGlobalInput(int input)
{
	//handle preliminary input
	switch (input)
	{
	case KEY_ESC: //quit
		if (modified)
		{
			/*	Frame* f = createConfirmDialog();

			cm->registerControl(f, KEY_LISTENER, confirmQuitCallback);
			cm->setFocus(f);*/
		}
		else
		{
			//cm->prepareForShutdown();
			cm->setActive(false);
		}
		break;
	case CTRL_N:
		if (modified)
		{
			/*	Frame* f = createConfirmDialog();

			cm->registerControl(f, KEY_LISTENER, confirmNewCallback);
			cm->setFocus(f);*/
		}
		else
		{
		//	createNew();
		}
		break;
	case CTRL_O:
		/*if (modified)
		{
		Frame* f = createConfirmDialog();

		cm->registerControl(f, KEY_LISTENER, confirmOpenCallback);
		cm->setFocus(f);
		}
		else
		{
		setupFileDialog(OPEN_DIALOG);
		}*/

		break;
	case CTRL_S:
		//if (modified) //save only if there are changes
		//	setupFileDialog(SAVE_DIALOG);

		//temporary only!!!!
		/*saveFields();

		ofstream* os = new ofstream("test.atr", ios::binary | ios::trunc);
		actor->def->save(os);
		os->close();*/
		break;
	}
}

void MasterEditor::globalCallback(void* caller, void* ptr, int input) //static
{
	MasterEditor* ae = (MasterEditor*)caller;
	ae->processGlobalInput(input);
}

void MasterEditor::menuCallback(void* caller, void* ptr, int input) //static
{
	MasterEditor* me = (MasterEditor*)caller;
	me->processMenuInput(input);
}