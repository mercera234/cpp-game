#include "FileDialog.h"
#include "ExitCode.h"

FileDialog::FileDialog(const std::string& defaultFilePath, FileDialogType type, const std::string& extensionFilter)
{
	chooser = new FileChooser(defaultFilePath, type, extensionFilter);

	setControl(chooser);
}

void FileDialog::setWindow(WINDOW* win)
{
	DialogWindow::setWindow(win);
	chooser->setupChooser(innerWin);
}

ExitCode FileDialog::processInput(int input)
{
	switch (input)
	{
	case KEY_DOWN: chooser->driver(REQ_DOWN_ITEM);   break;
	case KEY_UP: chooser->driver(REQ_UP_ITEM); break;
	case KEY_PGDN: chooser->driver(REQ_SCR_DPAGE); break;
	case KEY_PGUP: chooser->driver(REQ_SCR_UPAGE); break;
	case KEY_HOME: chooser->driver(REQ_FIRST_ITEM); break;
	case KEY_END: chooser->driver(REQ_LAST_ITEM); break;
	case KEY_ESC:
	{
		return ExitCode::GO_BACK;
	}
	break;
	case '\r':
		fileChosen = chooser->filePathDriver();
		break;
	default:
		chooser->driver(input);
		break;
	}

	return ExitCode::HANDLED;
}