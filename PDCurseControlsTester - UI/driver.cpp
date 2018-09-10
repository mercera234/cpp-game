#include "TUI.h"

#include "file_chooser_tests.h"
#include "menu_tests.h"
#include "pdcurses_tests.h"
#include "tui_tests.h"
#include "image_tests.h"
#include "control_manager_tests.h"
#include "frame_tests.h"
#include "misc_tests.h"
#include "textboard_tests.h"
#include "text_tests.h"

//PDCURSE CONTROLS UI TESTER
int main()
{
	TUI tui;
	tui.init();
	//DO NOT MODIFY ABOVE THIS LINE
	
	
	statusBoardWithTextPieces();
	
	
	
	//DO NOT MODIFY BELOW THIS LINE
	tui.shutdown();
		
	return 0;
}

