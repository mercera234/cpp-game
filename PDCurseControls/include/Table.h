#pragma once
#include "Controllable.h"


class Table : public Controllable
{
private:
	short tblRows;
	short tblCols;
	int capacity;

	Controllable** cells;

	short* colWidths;
	short* rowHeights;
	short y, x;
	int getTallestCell(int row);
	int getWidestCell(int col);
	bool moveCellByOffset(Controllable* cell, int yOff, int xOff);
public:
	Table(int rows, int cols, short y, short x, Controllable** controls);
	void setCell(Controllable* c, int row, int col);
	void draw();
	void move(int y, int x);
	bool setRowHeight(int row, int height);
	bool setColWidth(int col, int width);
};