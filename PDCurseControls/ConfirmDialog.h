#pragma once
#include "DialogWindow.h"
#include "GridMenu.h"

enum class ConfirmMethod
{
	NEW,
	OPEN,
	SAVE,
	QUIT
};

const int yesOption = 1;
const int noOption = -1;

class ConfirmDialog : public DialogWindow
{
private:
	GridMenu* menu = nullptr;
	ConfirmMethod method;
public:
	ConfirmDialog(const std::string& msg, ConfirmMethod methodIn);

	int processInput(int input);


	void setMethod(ConfirmMethod methodIn) { method = methodIn; }
	ConfirmMethod getMethod() { return method; }

};
