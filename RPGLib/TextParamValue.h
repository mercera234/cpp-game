#pragma once
#include "TextPiece.h"
#include <iomanip>
#include "BoundInt.h"

template <typename T>
class TextParamValue : public TextPiece
{
protected:
	T* value;
	Justf innerJustf = Justf::RIGHT; //inner justification for the values, not the whole text piece
	void getParamValueString(std::ostringstream& text);
	void insertJustification(std::ostringstream& text);
public:
	int valueWidth;
	TextParamValue() {}
	TextParamValue(Format* fmtIn, std::string textIn, T* valueIn, int valueWidthIn);
	void draw(WINDOW* win);
	virtual std::string getExpandedText();
	void setValue(T* val) { value = val; }
	void setInnerJustf(Justf jf) { innerJustf = jf; }
};

template <typename T>
TextParamValue<T>::TextParamValue(Format* fmtIn, std::string textIn, T* valueIn, int valueWidthIn)
{
	setFormat(fmtIn);
	setText(textIn);
	setValue(valueIn);
	valueWidth = valueWidthIn;
}

template <typename T>
void TextParamValue<T>::draw(WINDOW* win)
{
	fmt->draw(win, getExpandedText());
}

template <typename T>
std::string TextParamValue<T>::getExpandedText()
{
	std::ostringstream expandedText;
	getParamValueString(expandedText);

	return expandedText.str();
}

template <typename T>
void TextParamValue<T>::insertJustification(std::ostringstream& expandedText)
{
	switch (innerJustf)
	{
	case Justf::LEFT: expandedText << std::left; break;
	case Justf::RIGHT: expandedText << std::right; break;
	}
}

template <typename T>
void TextParamValue<T>::getParamValueString(std::ostringstream& expandedText)
{
	expandedText << text << ' ';

	insertJustification(expandedText);
	expandedText << std::setw(valueWidth) << *value;
}


/*Text containing both a parameter name and value to be drawn as a text piece.*/
template<>
class TextParamValue<BoundInt> : public TextPiece
{
protected:
	BoundInt* value; 
	
	Justf innerJustf = Justf::RIGHT; //inner justification for the values, not the whole text piece
	void getParamValueString(std::ostringstream& text);
	void insertJustification(std::ostringstream& text);
public:
	int valueWidth = -1;
	TextParamValue() {}
	TextParamValue(Format* fmtIn, std::string textIn, BoundInt* valueIn, int valueWidthIn = -1);
	void draw(WINDOW* win);
	virtual std::string getExpandedText();
	void setValue(BoundInt* val) { value = val; }
	void setInnerJustf(Justf jf) { innerJustf = jf; }
};



class TextParamCurrMaxValue : public TextParamValue<BoundInt>
{
protected:
	void getCurrMaxString(std::ostringstream& expandedText);
public:
	TextParamCurrMaxValue(Format* fmtIn, std::string textIn, BoundInt* valueIn, int valueWidthIn = -1)
		: TextParamValue<BoundInt>(fmtIn, textIn, valueIn, valueWidthIn) {}
	std::string getExpandedText();
};