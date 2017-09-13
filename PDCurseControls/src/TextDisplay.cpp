#include "TextDisplay.h"
#include "TUI.h"

TextDisplay::TextDisplay(WINDOW* win, int rows, int cols, int maxChars)
{
	focusable = true;

	totalRows = rows;
	totalCols = cols;
	display = new Image(rows, cols, win);
	setWindow(win);
	
	getbegyx(win, scrY, scrX);
	color = 0x00000000; //set to black and white by default

	setEditable(false);
	
	maxLen = maxChars;
	text = new char[maxLen];
	memset(text, BLANK_SPACE, maxLen);
	curLen = 0; //length of text stored
	textPtr = text;
	cursorPos = 0; //starts at beginning

	lineSpans.push_back(""); //allocate one string span
	curLine = lineSpans.begin();
	linePos = 0;
}

void TextDisplay::setEditable(bool editable)
{
	this->editable = editable;

	if (editable)
	{
		setFocus();
	}
	else
		curs_set(CURSOR_INVISIBLE);
}



void TextDisplay::setFocus()
{
	curs_set(CURSOR_NORMAL);
	//need to use :: to differentiate this from the Controllable move method
	
	//translate cursorPos to image position
	int offY = cursorPos / visibleCols;
	int offX = cursorPos % visibleCols;

	//apply move globally to screen
	::move(scrY + offY, scrX + offX); //a known issue with using wmove combined with getch is why we're using move
}

void TextDisplay::setColor(int bkgdColor, int textColor)
{
	color = bkgdColor << 28 | textColor << 24;
	color &= 0xff000000;
}

/*
Replace all text in display with new string(can contain newlines)
*/
void TextDisplay::setText(string text)
{
	memset(this->text, BLANK_SPACE, maxLen);
	const char* t = text.c_str();
	memcpy_s(this->text, maxLen, t, text.length());
	curLen = text.length();
	cursorPos = curLen; //set cursor to end of pasted text
	textPtr = this->text + cursorPos;
}


bool TextDisplay::deleteChar()
{
	if (cursorPos >= curLen)
		return false;
	//shift string beyond position up a space
	short moveSize = curLen - (cursorPos + 1); //length of printable characters - one beyond current position
	memcpy_s(textPtr, moveSize, textPtr + sizeof(char), moveSize);
	//clear last position
	text[curLen - 1] = BLANK_SPACE;
	curLen--; //decrement current length
	return true;
}


bool TextDisplay::insertChar(int c)
{
	if (curLen >= maxLen)
		return false;

	//verify character is within ascii range
	if ((c < ' ' && c != '\n') || c > '~')
		return false;

	short moveSize = maxLen - cursorPos - 1;
	memcpy_s(textPtr + sizeof(char), moveSize, textPtr, moveSize);

	*textPtr = c;

	moveCursor(1);
	
	curLen++;
	return true;
}


bool TextDisplay::inputChar(int c)
{
	bool result = false;
	switch (c)
	{
	case KEY_DC: //delete key
		result = deleteChar();
		break;
	case '\b': //backspace key
		if (cursorPos <= 0)
			return false;
		//backup one character and then use regular delete character routine
		moveCursor(-1);
		result = deleteChar(); 
		break;
	case KEY_LEFT:
		if (cursorPos <= 0)
			return false;

		moveCursor(-1); 
		break;
	case KEY_RIGHT:
		if (cursorPos >= curLen) //can't move write after last character
			return false;

		moveCursor(1);
		break;
	case KEY_HOME:
		result = moveHome();
		break;
	case KEY_END:
		
		
		while (cursorPos < curLen && *(textPtr + 1) != '\n') //
		{
			moveCursor(1);
		}
		break;
	case '\r':
	case '\n':
	case KEY_ENTER:
		result = insertNewLine();
		break;
	default: //all printable characters
		result = insertChar(c);
		break;
	}

	return result;
}

void TextDisplay::moveCursor(int amount)
{
	textPtr += amount;
	cursorPos += amount;
}

bool TextDisplay::moveHome()
{
	//get current position within view
	int x = getcurx(stdscr);
	int amount = scrX - x;
	if (amount == 0)
		return false; //nothing happened

	moveCursor(amount); //should always result in a negative value, which is what we want
	return true;
}

bool TextDisplay::moveEnd()
{
	

	return true;
}


bool TextDisplay::insertNewLine()
{
	int pos = cursorPos;
	if (insertChar('\n') == false) 
		return false; //wasn't enough room to add newline

							 //update all affected newlines
	for (list<int>::reverse_iterator it = newLines.rbegin(); it != newLines.rend(); it++)
	{
		int newLine = *it;

		if (newLine < pos) //don't need to do new lines before the one just added
			break;

		*it = newLine + 1;
	}

	newLines.push_back(pos); //insertChar already advanced cursor, so push previous position
	newLines.sort(); //put newline in correct position

	return true;
}


void TextDisplay::updateDisplay()
{
	TwoDStorage<chtype>* textSpace = display->getTileMap();
	int y = 0;
	int x = 0;
	int imageSize = display->getTotalTiles();
	for (int textNdx = 0, imageNdx = 0; textNdx < maxLen && imageNdx < imageSize; textNdx++, imageNdx++)
	{
		chtype c = BLANK_SPACE;
		if (textNdx < curLen)
		{
			c = *(text + textNdx); //assume no newlines for right now
		}
		
		textSpace->setDatum(imageNdx, c | color);
	}

	
}

void TextDisplay::draw()
{
	//update image with text data
	updateDisplay();


	//wclear(win);
	wbkgd(win, color);
	display->draw();
}

TextDisplay::~TextDisplay()
{
	delete display;
	delete text;
}