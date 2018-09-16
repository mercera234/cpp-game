#include "key_strings.h"
#include "TUI.h"
#include <string>
#include <algorithm>

//TODO there is easily more strings/keys that could be handled here

const int controlToAsciiDistance = 64;

void getControlKeyString(std::string& keyString, int key, bool verbose)
{
	keyString = "";

	if (verbose)
	{
		switch (key)
		{
		case 8: keyString.append("BACKSPACE"); break;
		case 13: keyString.append("ENTER"); break;
		case 27: keyString.append("ESCAPE"); break;
		}

		if (keyString.empty() == false)
			return;
	}

	

	keyString.append("CTRL_");

	switch (key)
	{
	case 0: keyString.append("AT"); break;
	default: //keys 1-26
		keyString.append(1, (char)(key + controlToAsciiDistance)); break;
	case 27: keyString.append("LEFT_BRACKET"); break;
	case 28: keyString.append("BACKSLASH"); break;
	case 29: keyString.append("RIGHT_BRACKET"); break;
	case 30: keyString.append("CARET"); break;
	case 31: keyString.append("UNDERSCORE"); break;
	}

	
}

std::string getStringFromKey(int key, bool verbose)
{
	//use ranges to determine how to return string
	if (key < 0)
		return "";

	std::string keyString = "";
	if (key >= 0 && key < 32)
	{
		getControlKeyString(keyString, key, verbose);
	}
	else if (key >= 32 && key <= 127)//get printable ascii string
	{
		if (verbose && key == ' ')
		{
			keyString.append("SPACE");
		}
		else
		{
			keyString.append(1, (char)key);
		}
	}
	else if (key >= KEY_DOWN && key <= KEY_RIGHT)
	{
		//get directional string
		switch (key)
		{
		case KEY_DOWN: keyString.append("DOWN"); break;
		case KEY_UP: keyString.append("UP"); break;
		case KEY_LEFT: keyString.append("LEFT"); break;
		case KEY_RIGHT: keyString.append("RIGHT"); break;
		}
	}
	//else string requested is not currently supported

	return keyString;
}


int getKeyFromString(const std::string& text)
{
	if (text.length() == 1)
		return (int)text[0];
	
	std::string upperText(text.length(), ' ');
	std::transform(text.begin(), text.end(), upperText.begin(), toupper);
	


	//TODO these strings should be stored globally
	if (upperText.compare("UP") == 0) return KEY_UP;
	if (upperText.compare("DOWN") == 0) return KEY_DOWN;
	if (upperText.compare("LEFT") == 0) return KEY_LEFT;
	if (upperText.compare("RIGHT") == 0) return KEY_RIGHT;
	if (upperText.compare("SPACE") == 0) return ' ';
	if (upperText.compare("ENTER") == 0) return '\r';

	return -1; //string could not be converted
}


