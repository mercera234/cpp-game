#pragma once
#include "TextPiece.h"
#include "BoundInt.h"

/*Text containing both a parameter name and value to be drawn as a text piece.*/
class TextParamValue : public TextPiece
{
protected:
	BoundInt* value; //this will be changed to a boundint at some point
	Justf innerJustf = Justf::RIGHT; //inner justification for the values, not the whole text piece
	void getParamValueString(std::ostringstream& text, int width, int valuePiece);
	void insertJustification(std::ostringstream& text);
public:
	TextParamValue();
	TextParamValue(Format* fmtIn, std::string textIn, BoundInt* valueIn);
	void draw(WINDOW* win);
	virtual std::string getExpandedText();
	void setValue(BoundInt* val) { value = val; }
	void setInnerJustf(Justf jf) { innerJustf = jf; }
};

class TextParamCurrMaxValue : public TextParamValue
{
protected:
	void getCurrMaxString(std::ostringstream& expandedText, int width, int currPiece, int maxPiece);
public:
	std::string getExpandedText();
};