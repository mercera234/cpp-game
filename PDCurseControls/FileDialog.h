#pragma once
#include "DialogWindow.h"
#include "FileChooser.h"
#include "ExitCode.h"

class FileDialog : public DialogWindow, public InputProcessor
{
private:
	std::string fileChosen = "";
	FileChooser* chooser = nullptr;
	
public:
	FileDialog(const std::string& defaultFilePath, FileDialogType type, const std::string& extensionFilter);

	void processInput();

	void setWindow(WINDOW* win);
	std::string getFileChosen() { return fileChosen; }


	FileDialogType getType() { return chooser->getType(); }

};