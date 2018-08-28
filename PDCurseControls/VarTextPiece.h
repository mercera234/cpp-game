#pragma once

#include "TextPiece.h"

template <typename T>
class VarTextPiece : public TextPiece
{
protected:
	T value;
public:
	virtual void draw(WINDOW* win);
	virtual std::string getText();

	void setValue(T val) { value = val; }
};

template <typename T>
void VarTextPiece<T>::draw(WINDOW* win)
{
	char output[100];
	sprintf_s(output, text.c_str(), value);

	fmt->draw(win, output);
}

template <typename T>
std::string VarTextPiece<T>::getText()
{
	char output[100];
	sprintf_s(output, text.c_str(), value);

	std::string s(output);
	return s;
}