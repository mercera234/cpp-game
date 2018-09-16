#include "ConfigMenuItem.h"
#include "TUI.h"
#include <string>
#include "key_strings.h"


ConfigMenuItem::~ConfigMenuItem()
{
}

ConfigMenuItem::ConfigMenuItem(int keyIn, Input& inputIn)
{
	setConfigInput(keyIn, inputIn);
}

void ConfigMenuItem::draw()
{
	MenuItem::draw();

	//draw item
	mvwprintw(win, topRow, itemX, "%s = ", input.name.c_str());

	std::string keyStr = getStringFromKey(key);

	if (editing)
	{
		wattron(win, COLOR_GREEN_BOLD << TEXTCOLOR_OFFSET);
		wprintw(win, "%s", keyStr.c_str());
		wattroff(win, COLOR_GREEN_BOLD << TEXTCOLOR_OFFSET);
	}
	else
		wprintw(win, "%s", keyStr.c_str());
		
}