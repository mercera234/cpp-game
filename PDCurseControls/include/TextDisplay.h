#pragma once
#include "Image.h"
#include <list>
#include <iterator>
using namespace std;

#define BLANK_SPACE ' '

class TextDisplay : public Controllable
{
private:
	Image* display; //holds the editing region(may extend beyond the borders of the view)

	char* text; //holds the sequence of text characters
	unsigned int curLen;
	unsigned int maxLen;

	list<string> lineSpans;
	list<string>::iterator curLine;
	unsigned short linePos;

	//these 2 vars must be adjusted in tandem, and are done so with the moveCursor method. The position should not be altered through other mechanisms
	char* textPtr;
	unsigned short cursorPos;

	list<int> newLines; //stores the text position of every newline in the display (none will have the same value)

	bool editable;

	chtype color; //a mask to indicate the color
	short scrY, scrX; //beg y and x of window. Used for the move routine due to the issue with wmove
	void updateDisplay();
	bool deleteChar();
	bool insertChar(int c);
	void moveCursor(int amount);
	bool moveHome();
	bool moveEnd();
	bool insertNewLine();
public:
	TextDisplay(WINDOW* win, int rows, int cols, int maxChars);
	//void addLine(string line);

	void setEditable(bool editable);
	void setFocus();
	
	bool inputChar(int c);
	void setText(string text);
	/*void setText(int value);
	
	string getText();
	WINDOW* getWindow();
	*/
	void setColor(int bkgdColor, int textColor = COLOR_WHITE);
	void draw();

	~TextDisplay();

};