#include "FileChooser.h"
#include <dirent.h>
#include <sstream>
#include <list>
#include "LineItem.h"

const char* divider = "----------------------------------------";

FileChooser::FileChooser()
{

}

FileChooser::FileChooser(const std::string& workingDir, FileDialogType type, std::string filter)
	: directory(workingDir)
{
	init(type, filter);
}

void FileChooser::init(FileDialogType type, std::string filter)
{
	setType(type);
	setFilter(filter);
}

void FileChooser::setDirectory(const std::string& workingDir)
{
	directory.setPath(workingDir);
}

/*The window parameter should have a height of at least 5*/
FileChooser::FileChooser(WINDOW* win, const std::string& workingDir, FileDialogType type, std::string filter)
	: directory(workingDir)
{
	init(type, filter);

	setupChooser(win);
}

void FileChooser::setupChooser(WINDOW* win)
{
	Controllable::setWindow(win);
	
	int begY, begX;
	getbegyx(win, begY, begX);

	//setup purpose label
	std::string msg;
	switch (type)
	{
	case FileDialogType::OPEN_DIALOG: msg = "Open from: ";  break;
	case FileDialogType::SAVE_DIALOG: msg = "  Save to: "; break;
	}
	int purposeLblWidth = 10;
	purposeLbl.setWindow(newwin(1, purposeLblWidth, begY, begX));
	purposeLbl.setText(msg);
	purposeLbl.setFormat(new LineFormat);

	//setup path label
	int height = getmaxy(win); //height required is a minimum of 5
	int dialogWidth = getmaxx(win);
	pathWidth = dialogWidth - purposeLblWidth - 1; //a gap between purpose and path labels

	pathLbl.setWindow(newwin(1, pathWidth, begY, begX + purposeLblWidth + 1));
	pathLbl.setFormat(new LineFormat);

	//setup file listing
	fileMenu.setWindow(newwin(height - 4, dialogWidth - 1, begY + 2, begX));
	fileMenu.resetItems(1, 1);

	int internalWidth = dialogWidth - 4; //- 1 for scroll bar room, 2 for mark

	fileMenu.setItemWidth(internalWidth);
	fileMenu.setWrapAround(false);

	vScroller.setTargetControl(&fileMenu);

	//set up file name field
	int menuCursorLength = fileMenu.getCursor().length();
	fileNameField.setupField(internalWidth, begY + height - 1, begX + menuCursorLength);

	//get list of files in current dir to display in Menu
	resetDirEntries();
}


void FileChooser::resetDirEntries()
{
	DIR* dir = opendir(directory.getPath().c_str());
	dirent* entry;

	int currElement = 0;
	std::list<LineItem*> lineItems;

	//create list of entries in directory
	while ((entry = readdir(dir)) != NULL)
	{
		if (curDir.compare(entry->d_name) == 0) //the current directory . is not needed
			continue;

		if (entry->d_type == DT_REG) //if a file is found
		{
			if (checkFilterMatch(entry->d_name) == false)
				continue;
		}
		
		lineItems.push_back(new LineItem(entry->d_name, currElement++, entry->d_type));
	}

	//add the entries to the menu
	fileMenu.resetItems(lineItems.size(), 1);
	for (auto it = lineItems.begin(); it != lineItems.end(); it++)
		fileMenu.setItem(*it);

	fileMenu.setCurrentItem(0);
	fileMenu.post(true);

	//truncate path name so we can see as much of the end as possible
	if ((int)directory.getPath().length() > pathWidth)
	{
		int truncatedLength = directory.getPath().length() - pathWidth + 3;
		pathLbl.setText("..." + directory.getPath().substr(truncatedLength));
	}
	else
		pathLbl.setText(directory.getPath());	
}

bool FileChooser::checkFilterMatch(const std::string& fileName)
{
	if (filter.empty())
		return true; //include all files when filter isn't set

	std::string checkStr = fileName.substr(fileName.length() - filter.length());
	if (checkStr.compare(filter) != 0)
		return false;

	return true;
}

void FileChooser::draw()
{
	wclear(win);

	//set up dividers
	//char* divider = "----------------------------------------";
	mvwaddstr(win, 1, 0, divider);
	mvwaddstr(win, getmaxy(win) - 2, 0, divider);
	wnoutrefresh(win);

	purposeLbl.draw();
	pathLbl.draw();
	fileNameField.draw();
	vScroller.draw(); //will draw the menu as well
}


std::string FileChooser::filePathDriver()
{
	std::string retPath;
	LineItem* choice = (LineItem*)fileMenu.getCurrentItem();
	
	if (choice != nullptr)
	{
		if (fileNameField.getText().compare("") != 0) //if hit enter and there is text in field, 
		{
			retPath.append(directory.getPath() + '\\' + fileNameField.getText());
			//retPath.append(fileNameField.getText()); //retPath will be fullpath and selected file name
		}
		else //no text in field, which means, must be directory
		{
			if (choice->name.compare("..") == 0)
			{
				navigateUpDirectory();
			}
			else 
			{
				navigateDownDirectory(choice->name);
			}
			
		}
	}

	return retPath;
}

bool FileChooser::navigateUpDirectory()
{
	directory.removeDirFromPath();
	resetDirEntries();

	return true;
}

bool FileChooser::navigateDownDirectory(const std::string& dirName)
{
	directory.addDirToPath(dirName);
	resetDirEntries();

	return true;
}


void FileChooser::driver(int input)
{
	if (input >= MIN_MENU_COMMAND && input <= MAX_MENU_COMMAND) //route input to menu
	{
		fileMenu.driver(input);
		LineItem* choice = (LineItem*)fileMenu.getCurrentItem();

		if (choice == nullptr)
			return;

		
		switch (choice->getCrossRef()) //this is done only when moving up and down in a file chooser
		{
		case DT_REG: 
			fileNameField.setText(choice->name);
			break;
		case DT_DIR: 
			fileNameField.setText("");
			break;
		}
	}
	else //printable input may have occurred
	{
		fileNameField.inputChar(input);
	}
}
