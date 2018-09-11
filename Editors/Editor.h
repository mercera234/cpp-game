#pragma once
#include <list>
#include "ControlManager.h"
#include "Frame.h"
#include "TextLabel.h"
#include "FileChooser.h"
#include "SimpleControlCommand.h"
#include "EditMode.h"
#include "TUI.h"
#include "LineItem.h"
#include "DialogWindow.h"
#include "ConfirmDialog.h"
#include "FileDialog.h"

template <typename Receiver>
class Editor : public Controllable
{
protected:
	ControlManager* cm;

	SimpleControlCommand<Receiver> fileDialogCmd;
	SimpleControlCommand<Receiver> confirmCmd;

	TextLabel fileNameLbl;

	/*The different modes the editor can have.*/
	std::list<EditMode*> modes;

	//The currently selected mode
	std::list<EditMode*>::iterator mode;

	void updateFileNameLabel();
	
	void setupConfirmDialog(ControlCommand* cmd, ConfirmMethod method);
	void setupFileDialog(FileDialogType dialogType);

	/*switches to the next mode in a circular fashion*/
	void cycleMode();

	virtual void load(const std::string& fileName) = 0;
	virtual void save(const std::string& fileName) = 0;
	virtual void createNew() = 0; //for creating a new instance of whatever is being edited
	

public:
	virtual void draw() = 0;
	virtual int processInput(int input) = 0;
	
	int processGlobalInput(Controllable* c, int input);
	int confirmDialogDriver(Controllable* c, int input);
	int fileDialogDriver(Controllable* c, int input);
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
			setupConfirmDialog(&confirmCmd, ConfirmMethod::QUIT);
		}
		else
		{
			exitCode = ExitCode::TERMINATE;
		}
		break;
	case CTRL_N:
		if (modified)
		{
			setupConfirmDialog(&confirmCmd, ConfirmMethod::NEW);
		}
		else
		{
			createNew();
		}
		break;
	case CTRL_O:
		if (modified)
		{
			setupConfirmDialog(&confirmCmd, ConfirmMethod::OPEN);
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
void Editor<Receiver>::setupConfirmDialog(ControlCommand* cmd, ConfirmMethod method)
{
	std::string confirmMsg = "Are you sure? You have unsaved changes.";
	ConfirmDialog* dialog = new ConfirmDialog(confirmMsg, method);
		
	int width, height;  getmaxyx(win, height, width);
	int y = getPosition(Justf::CENTER, height, 3);
	int x = getPosition(Justf::CENTER, width, confirmMsg.length() + 2);
	dialog->setWindow(newwin(3, confirmMsg.length() + 2, y, x));
		
	cm->registerControl(dialog, KEY_LISTENER, cmd);
	cm->setFocus(dialog);
}


template <typename Receiver>
int Editor<Receiver>::confirmDialogDriver(Controllable* c, int input)
{
	ConfirmDialog* dialog = (ConfirmDialog*)c;
	int retCode = dialog->processInput(input);

	int exitCode = HANDLED;
	switch (retCode)
	{
	case noOption: 
		cm->popControl();
		delete dialog;
		break;
	case yesOption: 
		cm->popControl();
		switch (dialog->getMethod())
		{
		case ConfirmMethod::NEW: 
			createNew();
			break;
		case ConfirmMethod::OPEN: 
			setupFileDialog(FileDialogType::OPEN_DIALOG);
			break;
		case ConfirmMethod::QUIT: 
			exitCode = ExitCode::TERMINATE;
			break;
		}
		delete dialog;
		break;
	}
	
	return exitCode;
}




template <typename Receiver>
void Editor<Receiver>::setupFileDialog(FileDialogType dialogType)
{
	int height = 12;
	int width = 42;

	FileDialog* dialog = new FileDialog((*mode)->getDefaultFilePath(), dialogType, (*mode)->extensionFilter);
	int y = getPosition(Justf::CENTER, getmaxy(win), height);
	int x = getPosition(Justf::CENTER, getmaxx(win), width);  
	dialog->setWindow(newwin(height, width, y, x));

	/*WINDOW* main = newwin(height, width, (getmaxy(stdscr) - height) / 2, (getmaxx(stdscr) - width) / 2);
	WINDOW* w = derwin(main, height - 2, width - 2, 1, 1);

	FileChooser* fd = new FileChooser(w, (*mode)->getDefaultFilePath(), dialogType, (*mode)->extensionFilter);*/

	/*Frame* f = new Frame(main, fd);
	f->setModal(true);*/

	cm->registerControl(dialog, KEY_LISTENER, &fileDialogCmd);
	cm->setFocus(dialog);
}

template <typename Receiver>
int Editor<Receiver>::fileDialogDriver(Controllable* c, int input)
{
	FileDialog* dialog = (FileDialog*)c;
	
	int exitCode = dialog->processInput(input);
	if (exitCode == ExitCode::GO_BACK)
	{
		cm->popControl();
		delete dialog;
		return ExitCode::HANDLED;
	}

	std::string fileChosen = dialog->getFileChosen();

	if (fileChosen.empty() == false)
	{
		switch (dialog->getType())
		{
		case FileDialogType::OPEN_DIALOG: load(fileChosen); break;
		case FileDialogType::SAVE_DIALOG: save(fileChosen); break;
		}
		
		(*mode)->storeLastOpened(fileChosen);

		cm->popControl();
		delete dialog;
	}
	return HANDLED;
}

template <typename Receiver>
void Editor<Receiver>::cycleMode()
{
	mode++;
	if (mode == modes.end())
		mode = modes.begin();
}

template <typename Receiver>
void Editor<Receiver>::updateFileNameLabel()
{
	fileNameLbl.setText((*mode)->getFileNameModified());
}