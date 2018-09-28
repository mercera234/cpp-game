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
#include "dialog_window_tests.h"
#include "twod_storage_tests.h"

//PDCURSE CONTROLS UI TESTER
int main()
{
	TUI tui;
	
	testProxyWithoutFile();
	//dialogueTest();
	//controlManagerTest();
	//fileChooserTest(FileDialogType::OPEN_DIALOG);
	
	
		
	return 0;
}

