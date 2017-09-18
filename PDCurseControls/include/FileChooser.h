#pragma once
#include "GridMenu.h"
#include "TextLabel.h"
#include "ScrollBar.h"
#include "TextField.h"
#include "FileDirectory.h"
/*
General rule of thumb when using a FileChooser
'ENTER' accept
'CTRL-Q' cancel (until I ever decide to use buttons)
*/

enum class FileDialogType
{
	OPEN_DIALOG,
	SAVE_DIALOG
};

//const int OPEN_DIALOG = 0;
//const int SAVE_DIALOG = 1;

class FileChooser : public Controllable
{
protected:
	FileDirectory directory;
	GridMenu fileMenu;
	//std::string workingDir;
	TextLabel purposeLbl;
	TextLabel pathLbl;
	ScrollBar vScroller;
	TextField fileNameField;
	std::string filter;
	FileDialogType type;

	//alignment variables
	//int indent;
	int pathWidth;
	bool checkFilterMatch(const std::string& fileName);
	void init(std::string workingDir, FileDialogType type, std::string filter = "");

	bool navigateUpDirectory();
	bool navigateDownDirectory(const std::string& dirName);
public:
	FileChooser(std::string workingDir, FileDialogType type, std::string filter = "");
	void setupChooser(WINDOW* win);

	FileChooser(WINDOW* win, std::string workingDir, FileDialogType type, std::string filter = "");
	void resetDirEntries();
	
	void driver(int input);
	std::string filePathDriver();
	void draw();

	//~FileChooser();

	//getters/setters
	void setFocus() { fileNameField.setCursorFocus(); }
	void setFilter(const std::string& filter) { this->filter = filter; }
	FileDialogType getType() { return type; }
	//GridMenu* getMenu() { return fileMenu; }
};