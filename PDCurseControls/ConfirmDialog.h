#pragma once
#include "DialogWindow.h"
#include "GridMenu.h"
#include "ExitCode.h"

enum class ConfirmMethod
{
	NEW,
	OPEN,
	SAVE,
	QUIT
};

const ExitCode yesOption = ExitCode::GO_BACK;
const ExitCode noOption = ExitCode::NOT_HANDLED;

class ConfirmDialog : public DialogWindow
{
private:
	GridMenu* menu = nullptr;
	ConfirmMethod method;
public:
	ConfirmDialog(const std::string& msg, ConfirmMethod methodIn);

	ExitCode processInput(int input);


	void setMethod(ConfirmMethod methodIn) { method = methodIn; }
	ConfirmMethod getMethod() { return method; }

};
