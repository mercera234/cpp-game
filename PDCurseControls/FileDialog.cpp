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

void FileDialog::processInput()
{
	exitCode = ExitCode::HANDLED;

	switch (input)
	{
	case KEY_DOWN: ::processInput(*chooser, REQ_DOWN_ITEM);
	case KEY_UP: ::processInput(*chooser, REQ_UP_ITEM); break;
	case KEY_PGDN: ::processInput(*chooser, REQ_SCR_DPAGE); break;
	case KEY_PGUP: ::processInput(*chooser, REQ_SCR_UPAGE); break;
	case KEY_HOME: ::processInput(*chooser, REQ_FIRST_ITEM); break;
	case KEY_END: ::processInput(*chooser, REQ_LAST_ITEM); break;
	case KEY_ESC:
	{
		exitCode = ExitCode::GO_BACK;
	}
	break;
	case '\r':
		fileChosen = chooser->filePathDriver();
		break;
	default:
		::processInput(*chooser, input);
		break;
	}
}