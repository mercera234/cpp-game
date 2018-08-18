#include "Editor.h"
#include "TUI.h"
#include "LineItem.h"

void Editor::setModified(bool mod)
{
	if (modified == mod && mod)//exit if no change in modified state and modified is set to true
		return;

	modified = mod;
	
	updateFileNameLabel();

	/*string lblTxt;
	lblTxt = modified ? fileName.substr(0, 14) + "*" : fileName;

	fileNameLbl->setText(lblTxt);*/
}

void Editor::processGlobalInput(int input)
{
	//handle preliminary input
	switch (input)
	{
	case KEY_ESC: //quit
		if (modified)
		{
			setupConfirmDialog(confirmQuitCallback);
		}
		else
		{
			cm->setActive(false);
		}
		break;
	case CTRL_N:
		if (modified)
		{
			setupConfirmDialog(confirmNewCallback);
		}
		else
		{
			createNew();
		}
		break;
	case CTRL_O:
		if (modified)
		{
			setupConfirmDialog(confirmOpenCallback);
		}
		else
		{
			setupFileDialog(FileDialogType::OPEN_DIALOG);
		}

		break;
	case CTRL_S:
		if (modified) //save only if there are changes
		{
			if(fileName.compare(DEF_FILENAME) == 0) //bring up filechooser if default name is still used 
				setupFileDialog(FileDialogType::SAVE_DIALOG);
			else
			{
				save(dialogDefPath + '\\' + fileName);
				setModified(false);
			}
				
		}
		break;
	case CTRL_A:
		setupFileDialog(FileDialogType::SAVE_DIALOG);
		break;
	}
}

void Editor::setupConfirmDialog(void(*callback) (void*, void*, int))
{
	Frame* f = createConfirmDialog("Are you sure? You have unsaved changes.");

	cm->registerControl(f, KEY_LISTENER, callback);
	cm->setFocus(f);
}


void Editor::confirmNewCallback(void* caller, void* ptr, int input) //static
{
	Editor* e = (Editor*)caller;
	e->confirmDialogDriver((Controllable*)ptr, input, E_NEW);
}

void Editor::confirmOpenCallback(void* caller, void* ptr, int input) //static
{
	Editor* e = (Editor*)caller;
	e->confirmDialogDriver((Controllable*)ptr, input, E_OPEN);
}

void Editor::confirmQuitCallback(void* caller, void* ptr, int input) //static
{
	Editor* e = (Editor*)caller;
	e->confirmDialogDriver((Controllable*)ptr, input, E_QUIT);
}

void Editor::globalCallback(void* caller, void* ptr, int input) //static
{
	Editor* e = (Editor*)caller;
	e->processGlobalInput(input);
}

void Editor::fileDialogCallback(void* caller, void* ptr, int input) //static
{
	Editor* e = (Editor*)caller;
	e->fileDialogDriver((Controllable*)ptr, input);
}

void Editor::confirmDialogDriver(Controllable* c, int input, int confirmMethod)
{
	Frame* f = (Frame*)c;
	GridMenu* dialog = (GridMenu*)f->getControl();

	MenuItem* mi = NULL;
	switch (input)
	{
	case KEY_LEFT: dialog->driver(REQ_LEFT_ITEM); break;
	case KEY_RIGHT: dialog->driver(REQ_RIGHT_ITEM); break;
	case '\r':
		mi = dialog->getCurrentItem(); break;
	}

	if (mi != NULL)
	{
		switch (mi->index)
		{
		case 0: //no
			cm->popControl();
			//cm->setFocus(map);
			break;
		case 1: //yes
			cm->popControl();
			switch (confirmMethod)
			{
			case E_NEW: createNew(); 
				//cm->setFocus(map); 
				break;
			case E_OPEN: setupFileDialog(FileDialogType::OPEN_DIALOG); break;
			case E_QUIT:
				cm->popControl();
				cm->setActive(false);
				break;
			}

			break;
		}
	}
}

Frame* Editor::createConfirmDialog(std::string confirmMsg)
{
	//setup menu for dialog
	int width, height;  getmaxyx(stdscr, height, width);
	WINDOW* fWin = newwin(4, confirmMsg.length() + 2, (height - 4) / 2, (width - 40) / 2);
	WINDOW* cdWin = derwin(fWin, 1, confirmMsg.length(), 2, 1);
	GridMenu* cdMenu = new GridMenu(cdWin, 1, 2);
	cdMenu->setItem(new LineItem("No", 0, 0));
	cdMenu->setItem(new LineItem("Yes", 1, 1));
	cdMenu->post(true);

	//setup enclosing frame
	Frame* f = new Frame(fWin, cdMenu);
	f->setText(confirmMsg, 1, 1);
	f->setModal(true);
	return f;
}
	   
void Editor::setupFileDialog(FileDialogType dialogType)
{
	int height = 12;
	int width = 42;
	WINDOW* main = newwin(height, width, (getmaxy(stdscr) - height) / 2, (getmaxx(stdscr) - width) / 2);
	WINDOW* w = derwin(main, height - 2, width - 2, 1, 1);

	FileChooser* fd = new FileChooser(w, dialogDefPath, dialogType, extensionFilter);

	Frame* f = new Frame(main, fd);
	f->setModal(true);

	cm->registerControl(f, KEY_LISTENER, fileDialogCallback);
	cm->setFocus(f);
}

void Editor::fileDialogDriver(Controllable* dialog, int input)
{
	Frame* f = (Frame*)dialog;
	FileChooser* fd = (FileChooser*)f->getControl();

	std::string fileChosen;
	switch (input)
	{
	case KEY_DOWN: fd->driver(REQ_DOWN_ITEM);   break;
	case KEY_UP: fd->driver(REQ_UP_ITEM); break;
	case KEY_PGDN: fd->driver(REQ_SCR_DPAGE); break;
	case KEY_PGUP: fd->driver(REQ_SCR_UPAGE); break;
	case KEY_HOME: fd->driver(REQ_FIRST_ITEM); break;
	case KEY_END: fd->driver(REQ_LAST_ITEM); break;
	case CTRL_Q: cm->popControl(); 
		break;
	case '\r':
		fileChosen = fd->filePathDriver();
		break;
	default:
		fd->driver(input);
		break;
	}

	if (fileChosen.empty() == false)
	{
		switch (fd->getType())
		{
		case FileDialogType::OPEN_DIALOG: load(fileChosen); break;
		case FileDialogType::SAVE_DIALOG: save(fileChosen); break;
		}
		int pos = fileChosen.find_last_of('\\');
		fileName = fileChosen.substr(pos + 1, fileChosen.length());

		//save path that file was opened/saved from as the start point for next time
		dialogDefPath = fileChosen.substr(0, pos);
		setModified(false);

		cm->popControl();
	}
}


void Editor::updateFileNameLabel()
{
	std::string lblTxt;
	lblTxt = modified ? fileName.substr(0, 14) + "*" : fileName;

	fileNameLbl->setText(lblTxt);
}