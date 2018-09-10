#pragma once
#include <list>
#include "ControlManager.h"
#include "Frame.h"
#include "TextLabel.h"
#include "FileChooser.h"
#include "SimpleControlCommand.h"
#include "ConfirmCommand.h"
#include "EditMode.h"
#include "TUI.h"
#include "LineItem.h"

template <typename Receiver>
class Editor : public Controllable
{
protected:
	ControlManager* cm;

	SimpleControlCommand<Receiver> fileDialogCmd;
	ConfirmCommand<Receiver> newCmd;
	ConfirmCommand<Receiver> openCmd;
	ConfirmCommand<Receiver> quitCmd;

	//file data (this should be moved to Master Editor)
	
	TextLabel fileNameLbl;

	std::list<EditMode*> modes;
	std::list<EditMode*>::iterator mode;

	void updateFileNameLabel();
	
	Frame* createConfirmDialog(std::string confirmMsg);
	
	void setupConfirmDialog(ControlCommand* cmd);
	
	void setupFileDialog(FileDialogType dialogType);

	virtual void load(const std::string& fileName) = 0;
	virtual void save(const std::string& fileName) = 0;
	virtual void createNew() = 0; //for creating a new instance of whatever is being edited
	virtual void cycleMode() = 0;

public:
	virtual void draw() = 0;
	virtual int processInput(int input) = 0;
	
	int processGlobalInput(Controllable* c, int input);

	int confirmNewDialogDriver(Controllable* dialog, int input);
	int confirmOpenDialogDriver(Controllable* dialog, int input);
	int confirmQuitDialogDriver(Controllable* dialog, int input);
	int fileDialogDriver(Controllable* dialog, int input);
};





template <typename Receiver>
int Editor<Receiver>::processGlobalInput(Controllable* c, int input)
{
	int exitCode = HANDLED;
	bool modified = (*mode)->isModified();
	//handle preliminary input
	switch (input)
	{
	case KEY_ESC: //quit
		if (modified)
		{
			setupConfirmDialog(&quitCmd);
		}
		else
		{
			exitCode = ExitCode::TERMINATE;
		}
		break;
	case CTRL_N:
		if (modified)
		{
			setupConfirmDialog(&newCmd);
		}
		else
		{
			createNew();
		}
		break;
	case CTRL_O:
		if (modified)
		{
			setupConfirmDialog(&openCmd);
		}
		else
		{
			setupFileDialog(FileDialogType::OPEN_DIALOG);
		}

		break;
	case CTRL_S:
		if (modified) //save only if there are changes
		{
			if ((*mode)->hasSaved() == false) //bring up filechooser if default name is still used 
				setupFileDialog(FileDialogType::SAVE_DIALOG);
			else
			{
				(*mode)->save();
			}

		}
		break;
	case CTRL_A:
		setupFileDialog(FileDialogType::SAVE_DIALOG);
		break;
	case CTRL_M:
		cycleMode();
		break;
	}

	return exitCode;
}

template <typename Receiver>
void Editor<Receiver>::setupConfirmDialog(ControlCommand* cmd)
{
	Frame* f = createConfirmDialog("Are you sure? You have unsaved changes.");

	cm->registerControl(f, KEY_LISTENER, cmd);
	cm->setFocus(f);
}

template <typename Receiver>
int Editor<Receiver>::confirmNewDialogDriver(Controllable* c, int input)
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
			break;
		case 1: //yes
			cm->popControl();
			createNew();
			break;
		}
	}
	return HANDLED;
}

template <typename Receiver>
int Editor<Receiver>::confirmOpenDialogDriver(Controllable* c, int input)
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
			break;
		case 1: //yes
			cm->popControl();
			setupFileDialog(FileDialogType::OPEN_DIALOG);
			break;
		}
	}
	return HANDLED;
}

template <typename Receiver>
int Editor<Receiver>::confirmQuitDialogDriver(Controllable* c, int input)
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

	int exitCode = HANDLED;
	if (mi != NULL)
	{
		switch (mi->index)
		{
		case 0: //no
			cm->popControl();
			break;
		case 1: //yes
			cm->popControl();
			exitCode = ExitCode::TERMINATE;
			break;
		}
	}
	return exitCode;
}

template <typename Receiver>
Frame* Editor<Receiver>::createConfirmDialog(std::string confirmMsg)
{
	//setup menu for dialog
	int width, height;  getmaxyx(stdscr, height, width);
	WINDOW* fWin = newwin(4, confirmMsg.length() + 2, (height - 4) / 2, (width - 40) / 2);
	WINDOW* cdWin = derwin(fWin, 1, confirmMsg.length(), 2, 1);
	GridMenu* cdMenu = new GridMenu(cdWin, 1, 2);
	cdMenu->setItem(new LineItem("No", 0, 0));
	cdMenu->setItem(new LineItem("Yes", 1, 1));
	cdMenu->setCurrentItem(0);
	cdMenu->post(true);

	//setup enclosing frame
	Frame* f = new Frame(fWin, cdMenu);
	f->setText(confirmMsg, 1, 1);
	f->setModal(true);
	return f;
}

template <typename Receiver>
void Editor<Receiver>::setupFileDialog(FileDialogType dialogType)
{
	int height = 12;
	int width = 42;
	WINDOW* main = newwin(height, width, (getmaxy(stdscr) - height) / 2, (getmaxx(stdscr) - width) / 2);
	WINDOW* w = derwin(main, height - 2, width - 2, 1, 1);

	FileChooser* fd = new FileChooser(w, (*mode)->getDefaultFilePath(), dialogType, (*mode)->extensionFilter);

	Frame* f = new Frame(main, fd);
	f->setModal(true);

	cm->registerControl(f, KEY_LISTENER, &fileDialogCmd);
	cm->setFocus(f);
}

template <typename Receiver>
int Editor<Receiver>::fileDialogDriver(Controllable* dialog, int input)
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
		(*mode)->setFileName(fileChosen.substr(pos + 1, fileChosen.length()));

		//save path that file was opened/saved from as the start point for next time
		(*mode)->setDefaultFilePath(fileChosen.substr(0, pos));
		(*mode)->setModified(false);

		cm->popControl();
	}
	return HANDLED;
}

template <typename Receiver>
void Editor<Receiver>::updateFileNameLabel()
{
	fileNameLbl.setText((*mode)->getFileNameModified());
}