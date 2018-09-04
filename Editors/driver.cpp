#include "MapEditor.h"

void mapEditorTest()
{
	/*Good lesson learned here. Always ensure that your terminal is sized to contain all windows that it renders or else window creation routines will return null*/
	resize_term(30, 150);


	MapEditor me;

	mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
	int editing = HANDLED;
	while (editing <= HANDLED) //simulate input/process/update loop
	{
		me.draw();

		doupdate();
		int input = getch();

		editing = me.processInput(input);
		if (editing != HANDLED)
		{
			int x = 3;
		}
	}
	
}

//EDITOR UI TESTER
int main()
{
	TUI tui;
	tui.init();

	mapEditorTest();
	
	tui.shutdown();
    return 0;
}

