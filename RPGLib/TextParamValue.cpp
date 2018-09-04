#include "TextParamValue.h"
#include <iomanip>


TextParamValue::TextParamValue()
{
}

TextParamValue::TextParamValue(Format* fmtIn, std::string textIn, BoundInt* valueIn)
{
	setFormat(fmtIn);
	setText(textIn);
	setValue(valueIn);
}

void TextParamValue::draw(WINDOW* win)
{
	fmt->draw(win, getExpandedText());
}

std::string TextParamValue::getExpandedText()
{
	std::ostringstream expandedText;	
	std::string s = std::to_string(value->getMax()); //get the max potential width of the field

	getParamValueString(expandedText, s.length(), value->getCurr());

	return expandedText.str();
}

void TextParamValue::insertJustification(std::ostringstream& expandedText)
{
	switch (innerJustf)
	{
	case Justf::LEFT: expandedText << std::left; break;
	case Justf::RIGHT: expandedText << std::right; break;
	}
}

//template <typename T>
void TextParamValue::getParamValueString(std::ostringstream& expandedText, int width, int valuePiece)
{
	expandedText << text << ' ';

	insertJustification(expandedText);
	expandedText << std::setw(width) << valuePiece;
}

void TextParamCurrMaxValue::getCurrMaxString(std::ostringstream& expandedText, int width, int currPiece, int maxPiece)
{
	getParamValueString(expandedText, width, currPiece);

	expandedText << '/';

	insertJustification(expandedText);
	expandedText << std::setw(width) << maxPiece;
}

std::string TextParamCurrMaxValue::getExpandedText()
{
	std::ostringstream expandedText;
	std::string s = std::to_string(value->getMax()); //get the max potential width of the field

	getCurrMaxString(expandedText, s.length(), value->getCurr(), value->getTempMax());

	return expandedText.str();
}


