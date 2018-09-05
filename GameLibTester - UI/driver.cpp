#include "TUI.h"
#include "misc_tests.h"

//PROJECT1 UI TESTER
int main()
{
	TUI tui;
	tui.init();
	resize_term(screenHeight, screenWidth);




	exploreOneMapTest();



	tui.shutdown();

	return 0;
}

