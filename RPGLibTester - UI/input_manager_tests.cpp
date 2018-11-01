#include "input_manager_tests.h"
#include "InputManager.h"
#include "TUI.h"
#include <fstream>

void getInputTest()
{
	TUI tui; //to allow use of getch
	InputManager mgr;

	int input = mgr.getInput();
	std::cout << input << std::endl;

	mgr.setUseRawInput(true);
	input = mgr.getInput();
	std::cout << input << std::endl;

	system("pause");
}

void loadConfigurationTextFile()
{
	std::ifstream is;
	is.open("data\\configuration.ini");

	InputManager mgr;
	int count = mgr.loadConfigurationFile(is);

	std::cout << "Total: " << count << std::endl;
	system("pause");
}