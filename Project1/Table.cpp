#include "Table.h"

Table::Table(int rows, int cols, short y, short x, Controllable** controls)
{
	this->y = y;
	this->x = x;

	tblRows = rows;
	tblCols = cols;
	
	capacity = rows * cols;
	cells = controls;

	//collect measurements
	rowHeights = new short[rows];
	colWidths = new short[cols];
	int totalHeight = 0;
	int totalWidth = 0;

	//get tallest cell in each row
	for (int row = 0; row < rows; row++) 
	{
		int tallestCell = getTallestCell(row);
		rowHeights[row] = tallestCell;
		totalHeight += tallestCell;
	}

	//get tallest cell in each column
	for (int col = 0; col < cols; col++) //rearrange where the windows are for the controls
	{
		int widestCell = getWidestCell(col);
		colWidths[col] = widestCell;
		totalWidth += widestCell;
	}
	
	//set the row and col measurements based on the max dimensions of the controls
	int rowStart = 0;
	for (int row = 0; row < rows; row++) //rearrange where the windows are for the controls
	{
		int colStart = 0;
		for (int col = 0; col < cols; col++)
		{
			int element = row * cols + col;
			if (cells[element] == NULL)
				continue;

			cells[element]->move(y + rowStart, x + colStart);
			colStart += colWidths[col];
		}
		rowStart += rowHeights[row];
	}

	win = newwin(totalHeight, totalWidth, y, x); //encompasses all cells
}


int Table::getTallestCell(int row)
{
	int tallestCell = 0;
	for (int col = 0; col < tblCols; col++)
	{
		int element = row * tblCols + col;
		if (cells[element] == NULL)
			continue;

		int cellHeight = getmaxy(cells[element]->getWindow());
		if (cellHeight > tallestCell)
			tallestCell = cellHeight;
	}

	return tallestCell;
}

int Table::getWidestCell(int col)
{
	int widestCell = 0;
	for (int row = 0; row < tblRows; row++)
	{
		int element = row * tblCols + col;
		if (cells[element] == NULL)
			continue;

		int cellWidth = getmaxx(cells[element]->getWindow());
		if (cellWidth > widestCell)
			widestCell = cellWidth;
	}
	return widestCell;
}

void Table::setCell(Controllable* c, int row, int col)
{
	int element = row * tblCols + col;
	cells[element] = c;
}

void Table::draw()
{
	//wclear(win); //clear the window
	//wbkgdset(win, '%');// | COLOR_PAIR(3)); //does this not work?

	//draw something in table window if we want
	wbkgd(win, '%' | COLOR_PAIR(3));

	//for (int row = 0; row < getmaxy(win); row++)
	//{
	//	//const chtype c = '#';
	//	
	//	//mvwaddnstr(win, row, 0, "###################################################################", getmaxx(win));
	//	//mvwaddchn(win, 0, 0, "does this render?");
	//}
	
	wnoutrefresh(win);

	//draw each cell
	for (int i = 0; i < capacity; i++)
	{
		if (cells[i] != NULL)//draw cells with content
		{
			cells[i]->draw();
		}
	}
	
}

void Table::move(int newY, int newX)
{
	//save the current coordinates of the table
	int preY = y;
	int preX = x;
	
	Controllable::move(newY, newX); //move table to the new position
	y = newY;
	x = newX;

	//determine the movement offset between start and end positions
	int offY = newY - preY;
	int offX = newX - preX;

	//cycle through all controls and move appropriately
	for (int i = 0; i < capacity; i++)
	{
		moveCellByOffset(cells[i], offY, offX);
	}
}

bool Table::setRowHeight(int row, int height)
{
	if (row >= tblRows)
		return false;

	//save and set new row height
	short prevHeight = rowHeights[row];
	rowHeights[row] = height;
	short heightOffset = height - prevHeight;

	//adjust position of cells below the adjusted row
	for (int i = (row + 1) * tblCols; i < capacity; i++)
	{
		moveCellByOffset(cells[i], heightOffset, 0);
	}

	//adjust total size of table
	int totalHeight = 0;
	for (int i = 0; i < tblRows; i++)
	{
		totalHeight += rowHeights[i];
	}
	wresize(win, totalHeight, getmaxx(win));

	return true;
}


bool Table::moveCellByOffset(Controllable* cell, int yOff, int xOff)
{
	if (cell == NULL)//only move cells with content
		return false;

	int cellY, cellX;
	WINDOW* cellWin = cell->getWindow();
	getbegyx(cellWin, cellY, cellX);

	cell->move(cellY + yOff, cellX + xOff);
	
	return true;
}


bool Table::setColWidth(int col, int width)
{
	if (col >= tblCols)
		return false;

	//save and set new col width
	short prevWidth = colWidths[col];
	colWidths[col] = width;
	short widthOffset = width - prevWidth;

	//adjust position of cells to right of the adjusted col
	/*This algorithm is noticeably different than what we used for rows since the table is in row major format*/
	for (int i = col + 1; i < tblCols; i++) //iterate through one column
	{
		for (int cell = i; cell < capacity; cell += tblCols)
		{
			moveCellByOffset(cells[cell], 0, widthOffset);
		}
	}

	//adjust total size of table
	int totalWidth = 0;
	for (int i = 0; i < tblCols; i++)
	{
		totalWidth += colWidths[i];
	}
	wresize(win, getmaxy(win), totalWidth);
	return true;
}
