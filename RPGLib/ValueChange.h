#pragma once
#include <sstream>
#include "TUI.h"
#include "Drawable.h"
#include "Format.h"

class ValueChange : public DrawablePiece
{
private:
	int* value = nullptr;
	int positiveColor = COLOR_GREEN_BOLD;
	int negativeColor = COLOR_RED_BOLD;
	bool drawZero = true;
	Format* fmt = nullptr;
public:
	ValueChange() {}
	ValueChange(Format* fmtIn, int* valueIn);
	
	void draw(WINDOW* win);

	//setters/getters
	void setValue(int* valueIn) { value = valueIn; }
	int* getValue() { return value; }

	void setFmt(Format* fmtIn) { fmt = fmtIn; }
	Format* getFmt() const { return fmt; }

	void setDrawZero(bool status) { drawZero = status; }

	void setPositiveColor(int positiveColorIn) { positiveColor = positiveColorIn; }
	void setNegativeColor(int negativeColorIn) { negativeColor = negativeColorIn; }
};


