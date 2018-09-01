#pragma once

enum ExitCode
{
	//input to ControlManager was not handled by any registered controls
	NOT_HANDLED = -1,

	//input was handled and no code was returned. Values greater than 0 imply a return code
	HANDLED,

	//input was handled and current application should be exited back to a calling app
	GO_BACK,

	//input was handled and current application should back out to title screen of app
	QUIT_TO_TITLE,

	//input was handled and current application should terminate
	TERMINATE
};