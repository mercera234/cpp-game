#pragma once
#include "GridMenu.h"
#include "TextLabel.h"
#include "ScrollBar.h"
#include "TextField.h"
/*
General rule of thumb when using a FileChooser
'ENTER' accept
'CTRL-Q' cancel (until I ever decide to use buttons)
*/
#define OPEN_DIALOG 0
#define SAVE_DIALOG 1

class FileChooser : public Controllable
{
protected:
	GridMenu* fileMenu;
	string workingDir;
	TextLabel* purposeLbl;
	TextLabel* pathLbl;
	ScrollBar* vScroller;
	TextField* fileNameField;
	string filter;
	unsigned short type;

	//alignment variables
	int indent;
	int pathWidth;
	bool checkFilterMatch(string fileName);
public:
	FileChooser(WINDOW* win, string workingDir, int type, string filter = "");
	void resetDirEntries();
	void setFocus() { fileNameField->setCursorFocus(); }
	void setFilter(string filter) { this->filter = filter; }
	unsigned short getType() { return type; }
	GridMenu* getMenu() { return fileMenu; }
	void draw();
	void driver(int input);
	string filePathDriver();
};