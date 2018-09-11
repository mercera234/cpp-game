#pragma once
#include "DialogWindow.h"
#include "FileChooser.h"

class FileDialog : public DialogWindow
{
private:
	std::string fileChosen = "";
	FileChooser* chooser = nullptr;
	
public:
	FileDialog(const std::string& defaultFilePath, FileDialogType type, const std::string& extensionFilter);

	int processInput(int input);

	void setWindow(WINDOW* win);
	std::string getFileChosen() { return fileChosen; }


	FileDialogType getType() { return chooser->getType(); }

};