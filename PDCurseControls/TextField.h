#pragma once
#include <string>
#include <iostream>
#include "FormControl.h"
#include "curses.h"

class TextField : public FormControl
{
private:
	std::string text;
	unsigned short cursorPos = 0; //the position of the cursor for inputting characters

	short scrY, scrX; //beg y and x of window. Used for the move routine due to the issue with wmove
	
	bool deleteChar();
	bool insertChar(int c);
public:
	TextField();

	/* Main constructor for TextField object. Will create a window automatically since they should always be 1 row in height. 
	The max length of the text string is the same as the window length.*/
	TextField(unsigned int length, unsigned short y, unsigned short x);

	/*Sets the length and placement of the text field*/
	void setupField(unsigned int length, unsigned short y, unsigned short x);
	
	/* Main routine for accepting key strokes from user. Accepts non-alphabetic keys (home/end/backspace/delete) as well. */
	bool inputChar(int c);
	
	/*Clears the text field*/
	void clear();

	void draw();
	~TextField();

	//getters/setters

	/* Move cursor to this control. Set cursor to normal blinking underline. (Override) */
	void setFocus(bool focusIn);

	//void setCursorFocus(bool cursorFocusOn);

	/*Changes the stored text with the text parameter.*/
	void setText(const std::string& textIn);

	/*Sets text to the string representation of integer value*/
	void setText(int value);
	std::string getText(); //Gets only the text and no trailing/leading white space.
};