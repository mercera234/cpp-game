#include "ConfirmDialog.h"
#include "LineItem.h"

ConfirmDialog::ConfirmDialog(const std::string& msg, ConfirmMethod methodIn)
{
	frame.setText(msg, 0, 1);

	setMethod(methodIn);

	menu = new GridMenu();
	setControl(menu);
	menu->resetItems(1, 2);
	menu->setItemWidth(5);
	menu->setItem(new LineItem("No", 0, 0), 0, 0);
	menu->setItem(new LineItem("Yes", 1, 1), 0, 1);
	menu->setCurrentItem(0);
	menu->post(true);
}

ExitCode ConfirmDialog::processInput(int input)
{
	MenuItem* item = AbstractMenu::basicMenuDriver(input, menu);

	ExitCode exitCode = ExitCode::HANDLED;
	if (item)
	{
		switch (item->index)
		{
		case 0: exitCode = noOption; break;
		case 1: exitCode = yesOption;


			break;
		}
	}

	return exitCode;
}
