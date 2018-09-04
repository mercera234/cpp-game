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

class FileChooser : public Controllable
{
protected:
	FileDirectory directory;
	GridMenu fileMenu;
	
	TextLabel purposeLbl;
	TextLabel pathLbl;
	ScrollBar vScroller;
	TextField fileNameField;
	std::string filter;
	FileDialogType type;

	//alignment variables
	int pathWidth;
	bool checkFilterMatch(const std::string& fileName);
	void init(FileDialogType type, std::string filter = "");

	bool navigateUpDirectory();
	bool navigateDownDirectory(const std::string& dirName);
public:
	FileChooser();
	FileChooser(const std::string& workingDir, FileDialogType type, std::string filter = "");
	void setupChooser(WINDOW* win);

	FileChooser(WINDOW* win, const std::string& workingDir, FileDialogType type, std::string filter = "");
	void resetDirEntries();
	
	void driver(int input);
	std::string filePathDriver();
	void draw();

	
	//getters/setters
	void setFocus() { fileNameField.setFocus(true); }
	void setFilter(const std::string& filter) { this->filter = filter; }
	void setType(FileDialogType typeIn) { type = typeIn; }
	void setDirectory(const std::string& workingDir);
	FileDialogType getType() { return type; }
};