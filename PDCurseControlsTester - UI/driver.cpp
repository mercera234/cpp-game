#include "TUI.h"
#include "file_chooser_tests.h"
#include "menu_tests.h"

//PDCURSE CONTROLS UI TESTER
int main()
{
	TUI tui;
	tui.init();
	
	//palettePrototypeTest();
	paletteTest();

	tui.shutdown();
		
	return 0;
}

