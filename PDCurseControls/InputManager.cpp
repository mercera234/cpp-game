#include <string>
#include <fstream>
#include "InputManager.h"
#include "key_strings.h"

const char commentRecordId = '#';
const int unknownInput = -1;

int InputManager::loadConfigurationFile(std::ifstream& textFile)
{
	char lineFirstChar;
	while ((lineFirstChar = (char)textFile.peek()) != EOF)
	{
		if (lineFirstChar == commentRecordId) //skip to next line
		{
			//see max conflict with windef.h, hence the extra ()s
			textFile.ignore((std::numeric_limits<int>::max)(), '\n');
			continue;
		}

		std::string textInput;
		std::string textKey;

		textFile >> textInput >> textKey;
		int key = getKeyFromString(textKey);

		//inputs are setup with unknown input codes to be set by client app
		inputs.insert(std::make_pair(key, Input(textInput, unknownInput)));
	}

	return inputs.size();
}

int InputManager::getInput()
{
	int key = getch();
	return getInput(key);
}

int InputManager::getInput(int key)
{
	if (getUseRawInput())
		return key;

	auto it = inputs.find(key);

	return (it != inputs.end()) ? it->second.code : unknownInput;
}