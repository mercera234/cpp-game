#include "FormField.h"
#include <algorithm>

FormField::FormField(const std::string& labelText)
{
	setLabelText(labelText);
}

void FormField::setContent(WINDOW* labelWin, const std::string& labelText, int textFieldLength, int textFieldY, int textFieldX)
{
	label.setWindow(labelWin);
	setLabelText(labelText);
	field.setupField(textFieldLength, textFieldY, textFieldX);

	//setup window big enough to encompass both label and field
	
	int lBegY, lBegX, tBegY, tBegX;
	int lEndY, lEndX, tEndY, tEndX;
	getbegyx(label.getWindow(), lBegY, lBegX);
	getbegyx(field.getWindow(), tBegY, tBegX);
	getmaxyx(label.getWindow(), lEndY, lEndX);
	getmaxyx(field.getWindow(), tEndY, tEndX);
	lEndY += lBegY;
	lEndX += lBegX;
	tEndY += tBegY;
	tEndX += tBegX;
	
	int startY, startX;
	int endY, endX;

	startY = std::min(lBegY, tBegY);
	startX = std::min(lBegX, tBegX);

	endY = std::max(lEndY, tEndY);
	endX = std::max(lEndX, tEndX);

	setWindow(newwin(endY - startY, endX - startX, startY, startX));
}

void FormField::setLabelText(const std::string& labelText)
{
	label.setText(labelText);
	label.setFormat(new LineFormat);
}

void FormField::setFocus(bool focusIn)
{
	field.setFocus(focusIn);
}


void FormField::draw()
{
	label.draw();
	field.draw();
}


