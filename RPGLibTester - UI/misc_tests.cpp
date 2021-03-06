//#include "WadDatabase.h"
#include "actor_helper.h"
#include "MusicPlayer.h"
#include "misc_tests.h"
//#include "ActorRepository.h"
#include "TextParamValue.h"
#include "TextBoard.h"

//void wadDatabaseTest()
//{
//	WadDatabase wd;
//
//	std::string dataDir = "data";
//	std::string wadFileName = "data\\mainwad.dat";
//
//	//build wad file from separate data files in dataDirName
//	//then write data out to one wad file
//	wd.buildWad(dataDir); 
//	std::ofstream os(wadFileName, std::ios::binary);
//	wd.save(&os);
//	os.close();
//
//	wd.print();
//
//	system("pause");
//
//	//use a second waddatabase to load the data just from the wad file
//	//print methods should match except no individual source file names will be listed for the second call
//	WadDatabase wd2;
//	std::ifstream is(wadFileName, std::ios::binary);
//	wd2.load(&is);
//	is.close();
//
//	wd2.print();
//	system("pause");
//}





void statusDisplayTest()
{
	/*WINDOW* win = newwin(18, 31, 0, 0);
	box(win, 0, 0);

	Actor player;
	BoundInt hp = player.getStat(StatType::HP);
	hp.setCurrMax(9999);
	hp.setCurr(50);

	BoundInt mp = player.getStat(StatType::MP);
	mp.setCurrMax(999);
	mp.setCurr(2);

	int maxStat = 255;
	BoundInt str = player.getStat(StatType::STRENGTH);
	str.setCurrMax(maxStat);
	str.setCurr(maxStat);

	BoundInt def = player.getStat(StatType::DEFENSE);
	def.setCurrMax(maxStat);
	def.setCurr(maxStat);

	BoundInt intl = player.getStat(StatType::INTELLIGENCE);
	intl.setCurrMax(maxStat);
	intl.setCurr(maxStat);

	BoundInt wil = player.getStat(StatType::WILL);
	wil.setCurrMax(maxStat);
	wil.setCurr(maxStat);

	BoundInt agi = player.getStat(StatType::AGILITY);
	agi.setCurrMax(maxStat);
	agi.setCurr(maxStat);

	BoundInt exp = player.getStat(StatType::EXP);
	exp.setCurrMax(9999999);
	exp.setCurr(9999999);

	int toNext = 9999999;

	mvwprintw(win, 1, 1, "      HP %+4u/%+4u", hp.getCurr(), hp.getCurrMax());
	mvwprintw(win, 2, 1, "      MP %+4u/%+4u", mp.getCurr(), mp.getCurrMax());
	mvwprintw(win, 3, 1, "Strength %-3u", str.getCurr());
	mvwprintw(win, 4, 1, " Defense %-3u", def.getCurr());
	mvwprintw(win, 5, 1, "   Intel %-3u", intl.getCurr());
	mvwprintw(win, 6, 1, "    Will %-3u", wil.getCurr());
	mvwprintw(win, 7, 1, " Agility %-3u", agi.getCurr());
	mvwprintw(win, 8, 1, "     Exp %-7u", exp.getCurr());
	mvwprintw(win, 9, 1, " To Next %-7u", toNext);
	

	wnoutrefresh(win);
	doupdate();
	getch();*/
}

void textParamValueTest()
{
	//WINDOW* win = newwin(5, 30, 1, 1);
	//wbkgd(win, COLOR_RED << BKGDCOLOR_OFFSET);//use this more often! very helpful!

	//BoundInt gold$(0, 9999999);
	//TextParamValue<BoundInt> tpv;
	//tpv.setFormat(new LineFormat(0, Justf::RIGHT));
	//tpv.setText("Gold$");
	//tpv.setValue(&gold$);

	//tpv.draw(win);
	//wnoutrefresh(win);
	//doupdate();
	//getch();

	//gold$.setCurr(10000000);

	//tpv.draw(win);
	//wnoutrefresh(win);
	//doupdate();
	//getch();
}

void textBoardTest()
{
	//WINDOW* win = newwin(5, 30, 1, 1);
	//wbkgd(win, COLOR_RED << BKGDCOLOR_OFFSET);//use this more often! very helpful!

	//BoundInt hp(0, 9999, 245);
	//BoundInt mp(0, 999, 200);
	//hp.setCurrMax(500);
	//mp.setCurrMax(333);

	//BoundInt strength(0, 250, 47);
	//int x = 35;
	//std::string label = "Some words";

	//TextBoard board;
	//board.setWindow(win);
	//board.addPiece(new TextParamValue<BoundInt>(new LineFormat(0, Justf::RIGHT), "HP", &hp, 5));
	//board.addPiece(new TextParamCurrMaxValue(new LineFormat(1, Justf::RIGHT), "MP:", &mp, 4));
	//board.addPiece(new TextParamValue<BoundInt>(new LineFormat(2, Justf::RIGHT), "Strength", &strength));
	//board.addPiece(new TextParamValue<int>(new LineFormat(3, Justf::RIGHT), "X:", &x, 4));
	//board.addPiece(new TextParamValue<std::string>(new LineFormat(4, Justf::RIGHT), "Label:", &label, label.length()));
	//
	//
	//board.draw();
	//wnoutrefresh(win);
	//doupdate();
	//getch();

	//strength.setCurr(358);
	//hp.setCurr(3);
	//mp.setCurrMax(350);
	//mp.setCurr(250);
	//x = 9648;
	//label = "What?";

	//board.draw();
	//wnoutrefresh(win);
	//doupdate();
	//getch();
}


//void dataPkgLoadTest()
//{
//	DataPkg pkg;
//	std::ifstream is("data\\datapkg.dat", std::ios::binary); //this file was created from dataPkgTest()
//
//	pkg.load(&is);
//
//	ActorDef* def = new ActorDef();
//	
//	pkg.readData(def->name.c_str(), 16, 0);
//
//	int size = 130;
//	char* testArr = new char[size];
//	memset(testArr, 0, size);
//	pkg.readNextData(testArr, size);
//
//	
//	is.close();
//}


//void dataPkgFactoryTest()
//{
//	DataPkgFactory factory;
//
//	//create pkg from actor and persist to file
//	Actor* actor = createActor("hero.actr", AT_HUMAN);
//
//	DataPkg* pkg = factory.createActorDefPackage(actor->def);
//
//	std::ofstream os("data\\actorpkg.dat", std::ios::binary);
//	pkg->save(&os);
//
//	os.close();
//
//	//load actor definition from same file and package
//	std::ifstream is("data\\actorpkg.dat", std::ios::binary);
//	DataPkg* pkg2 = new DataPkg();
//	pkg2->load(&is);
//
//	ActorDef* def = factory.getActorDefFromPkg(pkg2);
//
//	is.close();
//
//}





//
//void actorEditorTest()
//{
//	ActorEditor* ae = new ActorEditor();
//	resize_term(30, 150);
//	
//	bool editing = true;
//	while (editing) //simulate input/process/update loop
//	{
//		ae->draw();
//
//		doupdate();
//		int input = getch();
//
//		editing = ae->processInput(input);
//	}
//}



//
//void masterEditorTest()
//{
//	MasterEditor* editor = new MasterEditor();
//
//	bool editing = true;
//	while (editing) //simulate input/process/update loop
//	{
//		editor->draw();
//
//		doupdate();
//		int input = getch();
//
//		editing = editor->processInput(input);
//	}
//}
