#include "textboard_tests.h"
#include "TextPiece.h"
#include "TextBoard.h"
#include <sstream>
#include <iomanip>




//void textBoardTest()
//{
//	TextBoard board;
//
//	WINDOW* win = newwin(5, 20, 0, 0);
//
//	board.setWindow(win);
//
//	Pos p(2, 4);
//
//	PosFormat posFmt;
//	posFmt.pos = p;
//
//	LineFormat lnFmt;
//	lnFmt.line = 3;
//	lnFmt.justf = Justf::RIGHT;
//
//	TextPiece piece;
//	piece.setFormat(&posFmt);
//	piece.setText("words");
//
//	TextPiece piece2;
//	piece2.setFormat(&lnFmt);
//	piece2.setText("man!!!!");
//
//
//	LineFormat lnFmt2;
//	lnFmt2.line = 4;
//	lnFmt2.justf = Justf::CENTER;
//
//	VarTextPiece<int> varPiece;
//	varPiece.setFormat(&lnFmt2);
//	varPiece.setText("%2u");
//	varPiece.setValue(89);
//
//	LineFormat lnFmt3;
//	lnFmt3.line = 0;
//	lnFmt3.justf = Justf::LEFT;
//
//	VarTextPiece<const char*> varPiece2;
//	varPiece2.setFormat(&lnFmt3);
//	varPiece2.setText("%s");
//	varPiece2.setValue("A string");
//
//	board.addPiece(&piece);
//	board.addPiece(&piece2);
//	board.addPiece(&varPiece);
//	board.addPiece(&varPiece2);
//
//
//
//	board.draw();
//	doupdate();
//	getch();
//}
//
//void textBoardTest2()
//{
//	TextBoard board;
//
//	WINDOW* win = newwin(5, 20, 0, 0);
//
//	board.setWindow(win);
//
//	PosFormat posFmt;
//	Pos pos(0, 4);
//	posFmt.pos = pos;
//
//
//	std::string hpFormat = "%+4u";
//
//	VarTextPiece<int> currHp;
//	currHp.setFormat(&posFmt);
//	currHp.setText(hpFormat);
//	currHp.setValue(378);
//
//	TextPiece slash;
//	slash.setText("/");
//
//	VarTextPiece<int> maxHp;
//	maxHp.setFormat(&posFmt);
//	maxHp.setText(hpFormat);
//	maxHp.setValue(9999);
//
//	MacroTextPiece hpRow;
//	hpRow.setFormat(&posFmt);
//	hpRow.appendPiece(&currHp);
//	hpRow.appendPiece(&slash);
//	hpRow.appendPiece(&maxHp);
//
//	board.addPiece(&hpRow);
//
//	board.draw();
//	doupdate();
//	getch();
//}
//
//void textBoardTest3()
//{
//	TextBoard board;
//
//	WINDOW* win = newwin(5, 20, 0, 0);
//
//	board.setWindow(win);
//
//	LineFormat lnFmt;
//	lnFmt.line = 0;
//	lnFmt.justf = Justf::LEFT;
//
//	std::string hpFormat = "%+4u";
//
//	VarTextPiece<int> currHp;
//	currHp.setText(hpFormat);
//	currHp.setValue(378);
//
//	TextPiece slash;
//	slash.setText("/");
//
//	VarTextPiece<int> maxHp;
//	maxHp.setText(hpFormat);
//	maxHp.setValue(379);
//
//	MacroTextPiece hpRow;
//	hpRow.setFormat(&lnFmt);
//	hpRow.appendPiece(&currHp);
//	hpRow.appendPiece(&slash);
//	hpRow.appendPiece(&maxHp);
//
//	board.addPiece(&hpRow);
//
//	board.draw();
//	doupdate();
//	getch();
//}


//template <typename T>
//void getParamValueText(MacroTextPiece& macroPiece,
//	TextPiece& field, VarTextPiece<T>& valuePiece, LineFormat& fmt)
//{
//	macroPiece.setFormat(&fmt);
//	macroPiece.appendPiece(&field);
//	macroPiece.appendPiece(&valuePiece);
//}
//
//void getCurrMaxParamValueText(MacroTextPiece& macroPiece,
//	TextPiece& field, VarTextPiece<int>& currPiece, VarTextPiece<int>& maxPiece, TextPiece& slash, LineFormat& fmt)
//{
//	macroPiece.setFormat(&fmt);
//	macroPiece.appendPiece(&field);
//	macroPiece.appendPiece(&currPiece);
//	macroPiece.appendPiece(&slash);
//	macroPiece.appendPiece(&maxPiece);
//}


//void statusBoard()
//{
//	WINDOW* frame = newwin(20, 31, 0, 0);
//	WINDOW* win = newwin(18, 29, 1, 1);
//	TextBoard board;
//	board.setWindow(win);
//
//	Justf right = Justf::RIGHT;
//	const std::string threeDigitFmt = "%+3u";
//	const std::string fourDigitFmt = "%+4u";
//	const std::string slash = "/";
//
//	//hp
//	TextPiece slashPiece;
//	slashPiece.setText(slash);
//
//	TextPiece fields[7];
//	fields[0].setText("HP ");
//	fields[1].setText("MP ");
//	fields[2].setText("Strength ");
//	fields[3].setText("Defense ");
//	fields[4].setText("Intel ");
//	fields[5].setText("Will ");
//	fields[6].setText("Agility ");
//
//	VarTextPiece<int> statVals[7];
//
//	/*for each and for with element : range syntax do not work here!
//	I think it is because you end up declaring a stack allocated variable that goes out of scope once the block is terminated*/
//	for (int i = 0; i < 7; i++)
//	{
//		statVals[i].setText(threeDigitFmt);
//	}
//	statVals[0].setValue(378);
//	statVals[1].setValue(25);
//	statVals[2].setValue(255);
//	statVals[3].setValue(127);
//	statVals[4].setValue(10);
//	statVals[5].setValue(25);
//	statVals[6].setValue(60);
//
//	statVals[0].setText(fourDigitFmt);
//	statVals[1].setText(fourDigitFmt);
//
//	VarTextPiece<int> maxVals[2];
//	maxVals[0].setText(fourDigitFmt);
//	maxVals[1].setText(fourDigitFmt);
//
//	maxVals[0].setValue(379);
//	maxVals[1].setValue(25);
//
//	
//	LineFormat fmts[7];
//	int row = 0;
//	for (int i = 0; i < 7; i++)
//	{
//		fmts[i].justf = right;
//		fmts[i].line = row++;
//	}
//
//	MacroTextPiece rows[7];
//	getCurrMaxParamValueText(rows[0], fields[0], statVals[0], maxVals[0], slashPiece, fmts[0]);
//	getCurrMaxParamValueText(rows[1], fields[1], statVals[1], maxVals[1], slashPiece, fmts[1]);
//	getParamValueText<int>(rows[2], fields[2], statVals[2], fmts[2]);
//	getParamValueText<int>(rows[3], fields[3], statVals[3], fmts[3]);
//	getParamValueText<int>(rows[4], fields[4], statVals[4], fmts[4]);
//	getParamValueText<int>(rows[5], fields[5], statVals[5], fmts[5]);
//	getParamValueText<int>(rows[6], fields[6], statVals[6], fmts[6]);
//
//	row = 0;
//	for each (MacroTextPiece mPiece in rows)
//	{
//		board.addPiece(&rows[row++]);
//	}
//
//	box(frame, 0, 0);
//	wnoutrefresh(frame);
//	board.draw();
//	doupdate();
//	getch();
//}


void insertJustification(std::ostringstream& text, Justf justf)
{
	switch (justf)
	{
		/*case Justf::LEFT: text << std::left; break;*/ //text is left justified by default!
	case Justf::RIGHT: text << std::right; break;
	}
}

template <typename T>
void getParamValueString(std::ostringstream& text, int width, Justf justf, 
	const std::string& field, T valuePiece)
{
	text << field << ' ';
	
	insertJustification(text, justf);
	text << std::setw(width) << valuePiece;
}


void getCurrMaxString(std::ostringstream& text, int width, Justf justf,
	const std::string& field, int currPiece, int maxPiece)
{
	getParamValueString<int>(text, width, justf, field, currPiece);

	text << '/';
	
	insertJustification(text, justf);
	text << std::setw(width) << maxPiece;
}

void statusBoardWithStrings()
{
	WINDOW* frame = newwin(20, 31, 0, 0);
	WINDOW* win = newwin(18, 29, 1, 1);
	
	Justf right = Justf::RIGHT;	


	std::ostringstream hpString;
	std::ostringstream mpString;
	std::ostringstream strString;
	std::ostringstream defString;
	
	getCurrMaxString(hpString, 4, right, "HP", 378, 379);
	getCurrMaxString(mpString, 4, right, "MP", 5, 25);
	getParamValueString<int>(strString, 3, right, "Strength", 3);
	getParamValueString<int>(defString, 3, Justf::LEFT, "Defense", 3);

	/*ostringstream
	Format
	*/
	box(frame, 0, 0);
	wnoutrefresh(frame);
	mvwaddstr(win, 0, 0, hpString.str().c_str());
	mvwaddstr(win, 1, 0, mpString.str().c_str());
	mvwaddstr(win, 2, 0, strString.str().c_str());
	mvwaddstr(win, 3, 0, defString.str().c_str());

	wnoutrefresh(win);
	doupdate();
	getch();
}

void statusBoardWithTextPieces()
{
	WINDOW* frame = newwin(20, 31, 0, 0);
	WINDOW* win = newwin(18, 29, 1, 1);

	TextBoard board;
	board.setWindow(win);

	Justf right = Justf::RIGHT;

	//populate the ostringstreams
	std::ostringstream hpString;
	std::ostringstream mpString;
	std::ostringstream strString;
	std::ostringstream defString;

	getCurrMaxString(hpString, 4, right, "HP", 378, 379);
	getCurrMaxString(mpString, 4, right, "MP", 5, 25);
	getParamValueString<int>(strString, 3, right, "Strength", 3);
	getParamValueString<int>(defString, 3, Justf::LEFT, "Defense", 3);

	//set up the textlets
	TextPiece hpText, mpText, strText, defText;

	LineFormat* fmts[4];
	for (int i = 0; i < 4; i++)
	{
		fmts[i] = new LineFormat;
		fmts[i]->line = i;
		fmts[i]->justf = right;
	}

	hpText.setFormat(fmts[0]);
	mpText.setFormat(fmts[1]);
	strText.setFormat(fmts[2]);
	defText.setFormat(fmts[3]);

	hpText.setText(hpString.str());
	mpText.setText(mpString.str());
	strText.setText(strString.str());
	defText.setText(defString.str());

	//add textlets to board and display
	board.addPiece(&hpText);
	board.addPiece(&mpText);
	board.addPiece(&strText);
	board.addPiece(&defText);

	box(frame, 0, 0);
	wnoutrefresh(frame);
	board.draw();
	doupdate();
	getch();
}

