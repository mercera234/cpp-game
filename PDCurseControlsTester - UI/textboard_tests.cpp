#include "textboard_tests.h"
#include "VarTextPiece.h"
#include "TextBoard.h"


void textBoardTest()
{
	TextBoard board;

	WINDOW* win = newwin(5, 20, 0, 0);

	board.setWindow(win);

	Pos p(2, 4);

	PosFormat posFmt;
	posFmt.pos = p;

	LineFormat lnFmt;
	lnFmt.line = 3;
	lnFmt.justf = Justf::RIGHT;

	TextPiece piece;
	piece.setFormat(&posFmt);
	piece.setText("words");

	TextPiece piece2;
	piece2.setFormat(&lnFmt);
	piece2.setText("man!!!!");


	LineFormat lnFmt2;
	lnFmt2.line = 4;
	lnFmt2.justf = Justf::CENTER;

	VarTextPiece<int> varPiece;
	varPiece.setFormat(&lnFmt2);
	varPiece.setText("%2u");
	varPiece.setValue(89);

	LineFormat lnFmt3;
	lnFmt3.line = 0;
	lnFmt3.justf = Justf::LEFT;

	VarTextPiece<const char*> varPiece2;
	varPiece2.setFormat(&lnFmt3);
	varPiece2.setText("%s");
	varPiece2.setValue("A string");

	board.addPiece(&piece);
	board.addPiece(&piece2);
	board.addPiece(&varPiece);
	board.addPiece(&varPiece2);



	board.draw();
	doupdate();
	getch();
}

void textBoardTest2()
{
	TextBoard board;

	WINDOW* win = newwin(5, 20, 0, 0);

	board.setWindow(win);

	PosFormat posFmt;
	Pos pos(0, 4);
	posFmt.pos = pos;


	std::string hpFormat = "%+4u";

	VarTextPiece<int> currHp;
	currHp.setFormat(&posFmt);
	currHp.setText(hpFormat);
	currHp.setValue(378);

	TextPiece slash;
	slash.setText("/");

	VarTextPiece<int> maxHp;
	maxHp.setFormat(&posFmt);
	maxHp.setText(hpFormat);
	maxHp.setValue(9999);

	MacroTextPiece hpRow;
	hpRow.setFormat(&posFmt);
	hpRow.appendPiece(&currHp);
	hpRow.appendPiece(&slash);
	hpRow.appendPiece(&maxHp);

	board.addPiece(&hpRow);

	board.draw();
	doupdate();
	getch();
}

void textBoardTest3()
{
	TextBoard board;

	WINDOW* win = newwin(5, 20, 0, 0);

	board.setWindow(win);

	LineFormat lnFmt;
	lnFmt.line = 0;
	lnFmt.justf = Justf::LEFT;

	std::string hpFormat = "%+4u";

	VarTextPiece<int> currHp;
	currHp.setText(hpFormat);
	currHp.setValue(378);

	TextPiece slash;
	slash.setText("/");

	VarTextPiece<int> maxHp;
	maxHp.setText(hpFormat);
	maxHp.setValue(379);

	MacroTextPiece hpRow;
	hpRow.setFormat(&lnFmt);
	hpRow.appendPiece(&currHp);
	hpRow.appendPiece(&slash);
	hpRow.appendPiece(&maxHp);

	board.addPiece(&hpRow);

	board.draw();
	doupdate();
	getch();
}

template <typename T>
void getParamValueText(MacroTextPiece& macroPiece,
	const std::string& fieldLabel, const std::string& format, T value, int line, Justf justf)
{
	TextPiece* field = new TextPiece;
	field->setText(fieldLabel);

	VarTextPiece<T>* valuePiece = new VarTextPiece<T>;
	valuePiece->setText(format);
	valuePiece->setValue(value);

	LineFormat* lnFmt = new LineFormat;
	lnFmt->line = line;
	lnFmt->justf = justf;

	macroPiece.setFormat(lnFmt);
	macroPiece.appendPiece(field);
	macroPiece.appendPiece(valuePiece);
}

void getCurrMaxParamValueText(MacroTextPiece& macroPiece,
	const std::string& fieldLabel, const std::string& format, int curr, int max, int line, Justf justf)
{
	TextPiece* field = new TextPiece;
	field->setText(fieldLabel);

	VarTextPiece<int>* currPiece = new VarTextPiece<int>;
	currPiece->setText(format);
	currPiece->setValue(curr);

	TextPiece* slash = new TextPiece;
	slash->setText("/");

	VarTextPiece<int>* maxPiece = new VarTextPiece<int>;
	maxPiece->setText(format);
	maxPiece->setValue(max);

	LineFormat* lnFmt = new LineFormat;
	lnFmt->line = line;
	lnFmt->justf = justf;

	macroPiece.setFormat(lnFmt);
	macroPiece.appendPiece(field);
	macroPiece.appendPiece(currPiece);
	macroPiece.appendPiece(slash);
	macroPiece.appendPiece(maxPiece);
}


void statusBoard()
{
	WINDOW* frame = newwin(20, 31, 0, 0);
	WINDOW* win = newwin(18, 29, 1, 1);
	TextBoard board;
	board.setWindow(win);

	//hp
	MacroTextPiece hpRow;
	getCurrMaxParamValueText(hpRow, "HP ", "%+4u", 378, 379, 0, Justf::RIGHT);
	MacroTextPiece mpRow;
	getCurrMaxParamValueText(mpRow, "MP ", "%+4u", 25, 25, 1, Justf::RIGHT);
	MacroTextPiece strRow;
	getParamValueText<int>(strRow, "Strength ", "%+3u", 255, 2, Justf::RIGHT);
	MacroTextPiece defRow;
	getParamValueText<int>(defRow, "Defense ", "%+3u", 127, 3, Justf::RIGHT);
	MacroTextPiece intRow;
	getParamValueText<int>(intRow, "Intel ", "%+3u", 10, 4, Justf::RIGHT);
	MacroTextPiece wilRow;
	getParamValueText<int>(wilRow, "Will ", "%+3u", 25, 5, Justf::RIGHT);
	MacroTextPiece agiRow;
	getParamValueText<int>(agiRow, "Agility ", "%+3u", 60, 6, Justf::RIGHT);

	//build row
	board.addPiece(&hpRow);
	board.addPiece(&mpRow);
	board.addPiece(&strRow);
	board.addPiece(&defRow);
	board.addPiece(&intRow);
	board.addPiece(&wilRow);
	board.addPiece(&agiRow);

	box(frame, 0, 0);
	wnoutrefresh(frame);
	board.draw();
	doupdate();
	getch();

	//warning! All allocated objects are never deleted!

}