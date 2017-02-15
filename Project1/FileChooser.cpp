#include "FileChooser.h"
#include <dirent.h>
#include <sstream>

/*The window parameter should have a height of at least 5*/
FileChooser::FileChooser(WINDOW* win, string workingDir, int type, string filter)
{
	this->win = win;
	this->workingDir = workingDir;
	this->filter = filter;
	this->type = type;

	//construct object
	char* msg = NULL;
	switch (type)
	{
	case OPEN_DIALOG: msg = "Open from: ";  break;
	case SAVE_DIALOG: msg = "  Save to: "; break;
	}
	purposeLbl = new TextLabel(derwin(win, 1, 10, 0, 0), msg);

	indent = 11;
	int height = getmaxy(win); //height required is a minimum of 5
	int dialogWidth = getmaxx(win);
	pathWidth = dialogWidth - indent;

	pathLbl = new TextLabel(derwin(win, 1, pathWidth, 0, indent), ""); //text will be set when drawn

	fileMenu = new Menu(derwin(win, height - 4, dialogWidth - 1, 2, 0), 255, 1); //subtract 4 from height to account for 2 dividers, 1 label, and 1 textfield
	fileMenu->setMaxNameLength(dialogWidth - 3); //- 3 for scroll bar room, and marker
	fileMenu->setWrapAround(false);
	//set up menu entries
	fileMenu->clear();

	//set up text field at bottom
	fileNameField = new TextField(derwin(win, 1, 15, height - 1, 2));

	//get list of files in current dir to display in Menu
	resetDirEntries();

	//setup scroller
	vScroller = new ScrollBar(fileMenu);
}


void FileChooser::resetDirEntries()
{
	int currElement = 0;
	
	fileMenu->clear();

	DIR* dir = opendir(workingDir.c_str());
	dirent* entry;
	while ((entry = readdir(dir)) != NULL)
	{
		bool setItem = true;
		if (entry->d_type == DT_REG)
		{
			setItem = checkFilterMatch(entry->d_name);
		}
		
		if(setItem)
		{
			fileMenu->setItem(entry->d_name, "", currElement++, entry->d_type);
		}
		
	}

	//truncate path name so we can see as much of the end as possible
	string openPathStr;
	openPathStr.assign(workingDir);

	if (openPathStr.length() > pathWidth)
	{
		ostringstream oss;
		oss << "..." << openPathStr.substr(openPathStr.length() - pathWidth + 3);
		openPathStr = oss.str();
	}
	pathLbl->setText(openPathStr);
}

bool FileChooser::checkFilterMatch(string fileName)
{
	if (filter.empty())
		return true; //include all files when filter isn't set

	string checkStr = fileName.substr(fileName.length() - filter.length());
	if (checkStr.compare(filter) != 0)
		return false;

	return true;
}

void FileChooser::draw()
{
	wclear(win);
	purposeLbl->draw();
	pathLbl->draw();
	fileNameField->draw();
	vScroller->draw(); //will draw the menu as well
	
	//set up dividers
	char* divider = "----------------------------------------";
	mvwaddstr(win, 1, 0, divider);
	mvwaddstr(win, getmaxy(win) - 2, 0, divider);
	wnoutrefresh(win);
}

string FileChooser::driver(int input)
{
	string retPath;
	if (input >= MIN_MENU_COMMAND && input <= MAX_MENU_COMMAND) //route input to menu
	{
		fileMenu->driver(input);
		MenuItem* choice = fileMenu->getCurrentItem();

		switch (input)
		{
		case REQ_TOGGLE_ITEM: 
			
			if (fileNameField->getText().compare("") != 0) //if hit enter and there is text in field)
			{
				retPath.append(workingDir);
				retPath.append(1, '\\');
				retPath.append(fileNameField->getText());
			}
			else //no text in field, which means, must be directory
			{
				if (choice->name.compare("..") == 0)
				{
					//navigate up one dir
					int pos = workingDir.find_last_of('\\');
					workingDir = workingDir.substr(0, pos);
				}
				else //navigate down into dir
				{
					workingDir.append(1, '\\');
					workingDir.append(choice->name);
				}
				resetDirEntries();
				break;
			}
			break;
		default: //up or down processing
			if (choice->crossref == DT_REG) //this is done only when moving up and down in a file chooser
				fileNameField->setText(choice->name);
			else if (choice->crossref == DT_DIR)
				fileNameField->setText("");
			break;
		}
	}
	else //printable input may have occurred
	{
		fileNameField->inputChar(input);
	}

	return retPath;
}

