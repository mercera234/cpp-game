#include "FormControl.h"
#include "TUI.h"

void FormControl::setEditable(bool editable)
{
	this->editable = editable;

	if (editable)
	{
		setFocus();
	}
	else
		curs_set(CURSOR_INVISIBLE);
}