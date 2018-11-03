#include "MapEditor.h"
#include <Windows.h>

//EDITOR UI TESTER
int main()
{
	TUI tui;
	
	MapEditor me;

	int editing = HANDLED;
	while (editing <= HANDLED) //simulate input/process/update loop
	{
		me.draw();

		doupdate();
		int input = getch();

		editing = me.processInput(input);
	}
	
    return 0;
}

