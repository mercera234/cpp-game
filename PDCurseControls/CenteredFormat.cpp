#include "CenteredFormat.h"


void CenteredFormat::splitText(int displayLength, const std::string& text)
{
	int length = text.length();

	if (length > displayLength)
	{
		//split text
		std::istringstream iss(text);
		std::vector<std::string> words(std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>());

	//	int minLines = (int)ceil((float)length / displayLength); //we prefer the less lines to display the text

		buildLinesFromWords(displayLength, words);
		//rebalance TODO
	}
	else
	{
		lines.push_back(text);
	}
}

void CenteredFormat::buildLinesFromWords(int displayLength, const std::vector<std::string>& words)
{
	//put as many words as possible per line
	std::string line;
	for (int i = 0; i < (int)words.size(); i++)
	{
		if (line.empty() == false) //add a space in between words
			line += ' ';

		line += words[i];

		if ((int)line.length() > displayLength)
		{
			//remove the last word and space
			line.erase(line.find_last_of(' '));

			//finish off the line
			lines.push_back(line);
			line.clear();
			line += words[i];
		}
		else if (line.length() == displayLength) //perfect fit
		{
			lines.push_back(line);
			line.clear();
		}

		if (i == words.size() - 1 && line.empty() == false) //last word
		{
			lines.push_back(line);
		}
	}
}

void CenteredFormat::draw(WINDOW* win, const std::string& text)
{
	std::hash<std::string> strHash;
	size_t newHash = strHash(text);
	if (lastStrHash != newHash)
	{
		lines.clear();
		splitText(getmaxx(win), text);
		lastStrHash = strHash(text); //save hash of text
	}
	
	int y = getPosition(Justf::CENTER, getmaxy(win), lines.size());
	for each (std::string line in lines)
	{
		int x = getPosition(Justf::CENTER, getmaxx(win), line.length());

		mvwaddstr(win, y++, x, line.c_str());
	}
	wnoutrefresh(win);
}