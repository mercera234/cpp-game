#include "TUI.h"
#include "file_chooser_tests.h"
#include "menu_tests.h"
#include "pdcurses_tests.h"
#include "tui_tests.h"

//PDCURSE CONTROLS UI TESTER
int main()
{
	TUI tui;
	tui.init();
	//DO NOT MODIFY ABOVE THIS LINE
	
	
	mockMainMenuTest();
	
	
	
	//DO NOT MODIFY BELOW THIS LINE
	tui.shutdown();
		
	return 0;
}

