#include "TUI.h"
#include "file_chooser_tests.h"

//PDCURSE CONTROLS UI TESTER
int main()
{
	TUI tui;
	tui.init();
	
	openDialogTest();

	tui.shutdown();
		
	return 0;
}

