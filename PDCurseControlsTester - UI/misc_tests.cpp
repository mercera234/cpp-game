#include "GridMenu.h"
#include "LineItem.h"
#include "TestCommand.h"

void commandTest()
{
	GridMenu* menu = new GridMenu(newwin(5, 20, 2, 1), 1, 1);
	menu->setColorPair(COLOR_BLUE);
	menu->setItem(new LineItem("Red", 0, -1));
	menu->setItem(new LineItem("Green", 1, -1));

	Command* cmd = new TestCommand();

	bool inMenus = true;
	while (inMenus)
	{
		clear();

		/*if (cmd != NULL)
		cmd->execute();*/

		wnoutrefresh(stdscr);

		doupdate();
		int c = getch();



	}

}