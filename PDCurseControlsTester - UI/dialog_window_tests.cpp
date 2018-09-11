#include "dialog_window_tests.h"
#include "DialogWindow.h"
#include "ConfirmDialog.h"
#include "FileDialog.h"
#include "GridMenu.h"
#include "LineItem.h"
#include "input_return_codes.h"

void dialogWindowTest()
{
	DialogWindow dialog;

	WINDOW* win = newwin(5, 10, 1, 1);
	wbkgd(win, COLOR_RED << BKGDCOLOR_OFFSET);
	dialog.setWindow(win);

	dialog.draw();

	doupdate();
	getch();
}

void mockConfirmDialogWindowTest()
{
	DialogWindow confirmDialog;

	std::string confirmMsg = "!Confirm Overwrite?";
	WINDOW* win = newwin(3, confirmMsg.length() + 2, 1, 1);
	wattron(win, COLOR_WHITE << TEXTCOLOR_OFFSET | COLOR_BLUE << BKGDCOLOR_OFFSET);
	confirmDialog.setWindow(win);

	Frame& f = confirmDialog.getFrame();
	f.setText(confirmMsg, 0, 1);

	GridMenu* menu = new GridMenu();
	confirmDialog.setControl(menu);
	menu->resetItems(1, 2);
	menu->setItemWidth(5);
	menu->setItem(new LineItem("No", 0, 0), 0, 0);
	menu->setItem(new LineItem("Yes", 1, 1), 0, 1);
	menu->setCurrentItem(0);
	menu->post(true);

	
	bool playing = true;
	while (playing)
	{
		confirmDialog.draw();

		doupdate();
		int c = getch();

		MenuItem* item = AbstractMenu::basicMenuDriver(c, menu);

		if (item)
		{
			switch (item->index)
			{
			case 0: break;
			case 1: playing = false; break;
			}
		}


	}
	
}

void confirmDialogWindowTest()
{
	std::string confirmMsg = "!Confirm Overwrite?";
	ConfirmDialog dialog(confirmMsg, ConfirmMethod::NEW);
	dialog.setWindow(newwin(3, confirmMsg.length() + 2, 1, 1));

	bool playing = true;
	while (playing)
	{
		dialog.draw();

		doupdate();
		int c = getch();

		if (dialog.processInput(c) == yesOption)
			playing = false;
	}

}

void fileDialogWindowTest()
{
	char buf[256];
	GetFullPathName(".", 256, buf, NULL);
	std::string fullPath(buf);

	FileDialog dialog(fullPath, FileDialogType::OPEN_DIALOG, ".img");

	WINDOW* win = newwin(12, 42, 1, 1);
	dialog.setWindow(win);

	bool playing = true;
	while (playing)
	{
		dialog.setFocus(true);
		dialog.draw();

		doupdate();
		int c = getch();

		if (dialog.processInput(c) == ExitCode::GO_BACK)
			playing = false;
	}

}