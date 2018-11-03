#pragma once
#include <list>
#include "ControlManager.h"
#include "Frame.h"
#include "TextLabel.h"
#include "FileChooser.h"
#include "SimpleCommand.h"
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
	ControlManager globalCm;

	SimpleCommand<Receiver> cmCmd;
	SimpleCommand<Receiver> fileDialogCmd;
	SimpleCommand<Receiver> confirmCmd;

	TextLabel fileNameLbl;
	TextLabel modeLbl;

	/*The different modes the editor can have.*/
	std::list<EditMode*> modes;

	//The currently selected mode
	std::list<EditMode*>::iterator mode;

	void updateFileNameLabel();
	
	void setupConfirmDialog(SimpleCommand<Receiver>* cmd, ConfirmMethod method);
	void setupFileDialog(FileDialogType dialogType);

	/*switches to the next mode in a circular fashion*/
	void cycleMode();

	virtual void load(const std::string& fileName) = 0;
	virtual void save(const std::string& fileName) = 0;
	virtual void createNew() = 0; //for creating a new instance of whatever is being edited
	

public:
	virtual void draw() = 0;
	virtual int processInput(int input) = 0;
	
	void processGlobalInput();
	void confirmDialogDriver();
	void fileDialogDriver();
};





template <typename Receiver>
void Editor<Receiver>::processGlobalInput()
{
	bool modified = (*mode)->isModified();
	int input = globalCm.getInput();
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
			globalCm.setExitCode(ExitCode::TERMINATE);
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
}

template <typename Receiver>
void Editor<Receiver>::setupConfirmDialog(SimpleCommand<Receiver>* cmd, ConfirmMethod method)
{
	std::string confirmMsg = "Are you sure? You have unsaved changes.";
	ConfirmDialog* dialog = new ConfirmDialog(confirmMsg, method);
		
	int width, height;  getmaxyx(win, height, width);
	int y = getPosition(Justf::CENTER, height, 3);
	int x = getPosition(Justf::CENTER, width, confirmMsg.length() + 2);
	dialog->setWindow(newwin(3, confirmMsg.length() + 2, y, x));
		
	globalCm.registerControl(dialog, KEY_LISTENER, cmd);
	globalCm.setFocusedControl(dialog);
}


template <typename Receiver>
void Editor<Receiver>::confirmDialogDriver()
{
	ConfirmDialog* dialog = (ConfirmDialog*)globalCm.getFocusedControl();
	int retCode = dialog->processInput(globalCm.getInput());

	switch (retCode)
	{
	case noOption: 
		globalCm.popControl();
		delete dialog;
		break;
	case yesOption: 
		globalCm.popControl();
		switch (dialog->getMethod())
		{
		case ConfirmMethod::NEW: 
			createNew();
			break;
		case ConfirmMethod::OPEN: 
			setupFileDialog(FileDialogType::OPEN_DIALOG);
			break;
		case ConfirmMethod::QUIT: 
			globalCm.setExitCode(ExitCode::TERMINATE);
			break;
		}
		delete dialog;
		break;
	}
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

	globalCm.registerControl(dialog, KEY_LISTENER, &fileDialogCmd);
	globalCm.setFocusedControl(dialog);
}

template <typename Receiver>
void Editor<Receiver>::fileDialogDriver()
{
	FileDialog* dialog = (FileDialog*)globalCm.getFocusedControl();
	
	ExitCode exitCode = dialog->processInput(globalCm.getInput());
	if (exitCode == ExitCode::GO_BACK)
	{
		globalCm.setExitCode(exitCode);
		globalCm.popControl();
		delete dialog;
		return;
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

		globalCm.popControl();
		delete dialog;
	}
}

template <typename Receiver>
void Editor<Receiver>::cycleMode()
{
	globalCm.unRegisterControl(&(*mode)->cm);

	mode++;
	if (mode == modes.end())
		mode = modes.begin();

	globalCm.registerControl(&(*mode)->cm, KEY_LISTENER | MOUSE_LISTENER, &cmCmd);
	globalCm.setFocusedControl(&(*mode)->cm);
	//globalCm.moveControlToTop(&(*mode)->cm);
	modeLbl.setText("MODE: " + (*mode)->modeName);
}

template <typename Receiver>
void Editor<Receiver>::updateFileNameLabel()
{
	fileNameLbl.setText((*mode)->getFileNameModified());
}