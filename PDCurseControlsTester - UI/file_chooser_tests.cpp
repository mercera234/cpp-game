#include "file_chooser_tests.h"
#include "Frame.h"

void fileChooserTest(FileDialogType dialogType)
{
	char buf[256];
	GetFullPathName(".", 256, buf, NULL);
	std::string fullPath(buf);

	int height = 14; //height required is a minimum of 7 for enclosing FileChooser, which requires minimum of 5
	int width = 42; //min of 15 required

	WINDOW* main = newwin(height, width, (getmaxy(stdscr) - height) / 2, (getmaxx(stdscr) - width) / 2);
	WINDOW* sub = derwin(main, height - 2, width - 2, 1, 1);

	FileChooser* fd = new FileChooser(sub, fullPath, dialogType, ".map");

	Frame* f = new Frame(main, fd);
	std::string fileChosen;
	bool playing = true;
	while (playing)
	{
		f->draw();
		fd->setFocus();
		doupdate();

		int c = getch();

		//driver here
		switch (c)
		{
		case KEY_DOWN: fd->driver(REQ_DOWN_ITEM); break;
		case KEY_UP: fd->driver(REQ_UP_ITEM); break;
		case KEY_PGDN: fd->driver(REQ_SCR_DPAGE); break;
		case KEY_PGUP: fd->driver(REQ_SCR_UPAGE); break;
		case KEY_HOME: fd->driver(REQ_FIRST_ITEM); break;
		case KEY_END: fd->driver(REQ_LAST_ITEM); break;
		case KEY_ESC: playing = false; break;
		case '\r':
			fileChosen = fd->filePathDriver();
			break;
		default:
			fd->driver(c);
			break;
		}

		if (fileChosen.empty() == false)
		{
			//success!
			mvaddstr(0, 0, fileChosen.c_str());
		}

	}
}

void saveDialogTest()
{
	fileChooserTest(FileDialogType::SAVE_DIALOG);
}

void openDialogTest()
{
	fileChooserTest(FileDialogType::OPEN_DIALOG);
}

void fileDirectoryTest()
{
	FileDirectory dir("data");

	std::cout << "Getting all .actr files" << std::endl;
	std::list<dirent> files = dir.getFiles(false, ".actr");

	for each (dirent file in files)
	{
		std::cout << file.d_name << std::endl;
	}
	system("pause");

	std::cout << "Now getting all files" << std::endl;
	files = dir.getFiles(true);

	for each (dirent file in files)
	{
		std::cout << file.d_name << std::endl;
	}
	system("pause");

	std::cout << "Now getting all .map and dirs" << std::endl;
	files = dir.getFiles(true, ".map");

	for each (dirent file in files)
	{
		std::cout << file.d_name << std::endl;
	}
}


void direntTest()
{
	DIR* dir = opendir(".");

	//DIR* dir = opendir("F:\\Users\\Alex Mercer\\Documents\\Visual Studio 2015\\Projects\\Project1\\Project1");

	dirent* entry;

	while ((entry = readdir(dir)) != NULL)
	{
		clear();
		mvaddstr(1, 1, entry->d_name);
		wnoutrefresh(stdscr);
		doupdate();
		getch();
	}
}

void wdirectoryTest()
{
	DIR* dir = opendir(".");

	//DIR* dir = opendir("F:\\Users\\Alex Mercer\\Documents\\Visual Studio 2015\\Projects\\Project1\\Project1");
	WDIR* wDir = dir->wdirp;

	mvaddwstr(1, 1, wDir->patt);

	wnoutrefresh(stdscr);
	doupdate();
	getch();

	wdirent* entry;
	while ((entry = wreaddir(wDir)) != NULL)
	{
		clear();
		mvaddwstr(1, 1, entry->d_name);
		wnoutrefresh(stdscr);
		doupdate();
		getch();
	}
}