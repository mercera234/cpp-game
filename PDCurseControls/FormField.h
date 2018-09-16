#pragma once
#include <iostream>
#include "FormControl.h"
#include "TextField.h"
#include "TextLabel.h"

/* field justification */
#define NO_JUSTIFICATION	(0)
#define JUSTIFY_LEFT		(1)
#define JUSTIFY_CENTER		(2)
#define JUSTIFY_RIGHT		(3)

/* field options */
#define O_VISIBLE		(0x0001U)
#define O_ACTIVE		(0x0002U)
#define O_PUBLIC		(0x0004U)
#define O_EDIT			(0x0008U)
#define O_WRAP			(0x0010U)
#define O_BLANK			(0x0020U)
#define O_AUTOSKIP		(0x0040U)
#define O_NULLOK		(0x0080U)
#define O_PASSOK		(0x0100U)
#define O_STATIC		(0x0200U)

/* data types */
#define T_CHAR 0
#define T_SHORT 1
#define T_INTEGER 2
#define T_FLOAT 3
#define T_STRING 4

class FormField : public Controllable
{
private:
	TextLabel label;
	TextField field;
public:
	FormField() {}
	FormField(const std::string& labelText);
	void draw();
	virtual void setFocus(bool focusIn);

	void setContent(WINDOW* labelWin, const std::string& labelText, int textFieldLength, int textFieldY, int textFieldX);

	//setters/getters
	std::string getText() { return field.getText(); }
	void setText(const std::string& textIn) { field.setText(textIn); }

	void setLabelText(const std::string& labelText);
	TextLabel& getLabel() { return label; }

	void clear() { field.clear(); }
	TextField& getField() { return field; }
};

