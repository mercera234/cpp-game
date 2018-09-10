#include "TUI.h"
#include "misc_tests.h"

//GAMELIB UI TESTER
int main()
{
	TUI tui;
	tui.init();
	resize_term(screenHeight, screenWidth);




	mainMenuTest();



	tui.shutdown();

	return 0;
}

