#include "TextParamValue.h"
#include <iomanip>


TextParamValue<BoundInt>::TextParamValue(Format* fmtIn, std::string textIn, BoundInt* valueIn, int valueWidthIn)
{
	setFormat(fmtIn);
	setText(textIn);
	setValue(valueIn);
	valueWidth = valueWidthIn;
}

void TextParamValue<BoundInt>::draw(WINDOW* win)
{
	fmt->draw(win, getExpandedText());
}

std::string TextParamValue<BoundInt>::getExpandedText()
{
	std::ostringstream expandedText;
	

	getParamValueString(expandedText);

	return expandedText.str();
}

void TextParamValue<BoundInt>::insertJustification(std::ostringstream& expandedText)
{
	switch (innerJustf)
	{
	case Justf::LEFT: expandedText << std::left; break;
	case Justf::RIGHT: expandedText << std::right; break;
	}
}


void TextParamValue<BoundInt>::getParamValueString(std::ostringstream& expandedText)
{
	expandedText << text << ' ';

	insertJustification(expandedText);

	if (valueWidth < 0)
	{
		std::string s = std::to_string(value->getMax()); //get the max potential width of the field
		valueWidth = s.length();
	}

	expandedText << std::setw(valueWidth) << value->getCurr();
}

void TextParamCurrMaxValue::getCurrMaxString(std::ostringstream& expandedText)
{
	getParamValueString(expandedText);

	expandedText << '/';

	insertJustification(expandedText);

	expandedText << std::setw(valueWidth) << value->getCurrMax();
}

std::string TextParamCurrMaxValue::getExpandedText()
{
	std::ostringstream expandedText;
	
	getCurrMaxString(expandedText);

	return expandedText.str();
}


