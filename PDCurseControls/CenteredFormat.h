#pragma once
#include "Format.h"
#include <list>
#include <functional>

/*A fully centered format in the provided window*/
class CenteredFormat : public Format
{
private:
	std::list<std::string> lines; //the text broken up into separate lines
	size_t lastStrHash; //a hash of the last string that was printed (for convenience only)

	void buildLinesFromWords(int displayLength, const std::vector<std::string>& words);
public:
	//splits the text into separate lines
	void splitText(int displayLength, const std::string& text);

	//splits the text into separate lines
	void draw(WINDOW* win, const std::string& text);

	//setters/getters
	std::list<std::string>& getLines() { return lines; }
};