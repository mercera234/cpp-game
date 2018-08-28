#include "WadDatabase.h"
#include "ExplorationProcessor.h"
#include "actor_helper.h"
#include "MusicPlayer.h"
#include "BattleProcessor.h"
#include "MapEditor.h"
#include "MasterEditor.h"
#include "misc_tests.h"
#include "ActorRepository.h"


void repositoryTest()
{
	/*Repository2<unsigned short, Map> mapRepo(lessThan);

	Factory f;
	Map* m = f.createMap(stdscr, 0, screenHeight, screenWidth, 'Y', 0, 0);

	unsigned short id = 0;
	mapRepo.add(id, *m);

	Map* m2 = mapRepo.find(id);

	_ASSERT(m == m2);*/


	//Repository2<std::string, ActorDef> mapRepo;

	//Actor* actor = createActor("data\\hero.actr", AT_HUMAN);
	//mapRepo.add(actor->def->name, *(actor->def));

	//ActorDef* act2 = mapRepo.find(actor->def->name);


}

void actorRepositoryTest()
{
	std::ifstream is("data\\actors.txt");
	if (!is)
		return;

	ActorRepository repo;
	repo.loadFromTextFile(is);


}

void wadDatabaseTest()
{
	WadDatabase wd;

	std::string dataDir = "data";
	std::string wadFileName = "data\\mainwad.dat";

	//build wad file from separate data files in dataDirName
	//then write data out to one wad file
	wd.buildWad(dataDir); 
	std::ofstream os(wadFileName, std::ios::binary);
	wd.save(&os);
	os.close();

	wd.print();

	system("pause");

	//use a second waddatabase to load the data just from the wad file
	//print methods should match except no individual source file names will be listed for the second call
	WadDatabase wd2;
	std::ifstream is(wadFileName, std::ios::binary);
	wd2.load(&is);
	is.close();

	wd2.print();
	system("pause");
}

void wadDatabasePrintEmptyTest()
{
	WadDatabase wd;
	wd.print(); //won't crash to print empty database

	system("pause");
}

void exploreTest()
{
	int scrHeight = 23;
	int scrWidth = 51;
	resize_term(scrHeight, scrWidth);

	bool playing = true;

	WINDOW* screen = newwin(scrHeight, scrWidth, 0, 0);

	//create 2 maps
	Map map0(screen, "data\\water_templ.map");
	Map map1(screen, "data\\water_templ2.map");
	Map map2(screen, "data\\labyrinth.map");
	//	Map map2 (screen, "data\\crypt.map");

	//setup main character
	Actor* mainC = loadActor("hero.actr", ActorType::HUMAN);
	mainC->symbol = 'A';

	mainC->x = 45;
	mainC->y = 37;

	map0.setControlActor(mainC);

	curs_set(CURSOR_INVISIBLE);

	MapRepository* repo = new MapRepository(64, 64);

	ExplorationProcessor* mp = new ExplorationProcessor(&(mainC->y), &(mainC->x), repo);
	unsigned short id0 = 0;
	unsigned short id1 = 1;
	unsigned short id2 = 2;
	map0.setId(id0);
	map1.setId(id1);
	map2.setId(id2);

	repo->addMapSeam(map0, map1, true, 0, 1, 1);
	repo->addMapSeam(map2, map0, false, 0, 0, 1);
	repo->addMapSeam(map2, map1, true, 0, 0, 1);
	repo->add(map0); //could combine this without previous method
	repo->add(map1);
	repo->add(map2);

	int currId = map0.getId();
	mp->setCurrMap(currId);
	mp->setViewMode(ViewMode::DYNAMIC); //position map so character is visible (not sure if this is the best way to do this)

	MusicPlayer musicPlayer;
	int ret = musicPlayer.playFile("01-02- 12 Microtonal Etudes, Op 28 I 16 notes Andantino.wav");

	while (playing)
	{
		//draw map
		mp->getCurrMap()->draw();

		//update music if changing maps
		if ((currId <= 1) && mp->getCurrMap()->getId() == 2)
		{
			musicPlayer.stop();
			musicPlayer.playFile("11 CONTEMPLATIVE INTERMISSION.wav");
			currId = mp->getCurrMap()->getId();
		}
		else if (currId == 2 && mp->getCurrMap()->getId() <= 1)
		{
			musicPlayer.stop();
			musicPlayer.playFile("01-02- 12 Microtonal Etudes, Op 28 I 16 notes Andantino.wav");
			currId = mp->getCurrMap()->getId();
		}


		//add y,x coordinates to screen
		mvwprintw(screen, scrHeight - 2, scrWidth - 16, "y:%+4u x:%+4u", mainC->y, mainC->x);
		wnoutrefresh(screen);

		doupdate();

		//process input
		int input = getch();
		switch (input)
		{
		case KEY_ESC: playing = false; break;
		case KEY_RIGHT:
		case KEY_LEFT:
		case KEY_UP:
		case KEY_DOWN:
			mp->processMovementInput(input); break;
		case '\t': //toggle automap
			break;
		default:
			break;
		}
	}

	delwin(screen); //when a window is shared by multiple controllable objects, they cannot be responsible for deleting the window, because other objects can't tell that it is deleted
}


void statusDisplayTest()
{
	WINDOW* win = newwin(18, 31, 0, 0);
	box(win, 0, 0);

	Actor player;
	BoundInt hp = player.getStat(StatType::HP);
	hp.setMax(9999);
	hp.setCurr(50);

	BoundInt mp = player.getStat(StatType::MP);
	mp.setMax(999);
	mp.setCurr(2);

	int maxStat = 255;
	BoundInt str = player.getStat(StatType::STRENGTH);
	str.setMax(maxStat);
	str.setCurr(maxStat);

	BoundInt def = player.getStat(StatType::DEFENSE);
	def.setMax(maxStat);
	def.setCurr(maxStat);

	BoundInt intl = player.getStat(StatType::INTELLIGENCE);
	intl.setMax(maxStat);
	intl.setCurr(maxStat);

	BoundInt wil = player.getStat(StatType::WILL);
	wil.setMax(maxStat);
	wil.setCurr(maxStat);

	BoundInt agi = player.getStat(StatType::AGILITY);
	agi.setMax(maxStat);
	agi.setCurr(maxStat);

	BoundInt exp = player.getStat(StatType::EXP);
	exp.setMax(9999999);
	exp.setCurr(9999999);

	int toNext = 9999999;

	mvwprintw(win, 1, 1, "      HP %+4u/%+4u", hp.getCurr(), hp.getMax());
	mvwprintw(win, 2, 1, "      MP %+4u/%+4u", mp.getCurr(), mp.getMax());
	mvwprintw(win, 3, 1, "Strength %-3u", str.getCurr());
	mvwprintw(win, 4, 1, " Defense %-3u", def.getCurr());
	mvwprintw(win, 5, 1, "   Intel %-3u", intl.getCurr());
	mvwprintw(win, 6, 1, "    Will %-3u", wil.getCurr());
	mvwprintw(win, 7, 1, " Agility %-3u", agi.getCurr());
	mvwprintw(win, 8, 1, "     Exp %-7u", exp.getCurr());
	mvwprintw(win, 9, 1, " To Next %-7u", toNext);
	

	wnoutrefresh(win);
	doupdate();
	getch();
}


void battleProcessorTest()
{
	bool testing = true;
	while (testing)
	{
		//retrieve all actors from a wad file (just for testing, this should be refined)
		/*WadDatabase wd;
		std::string wadFileName = "data\\mainwad.dat";
		std::ifstream is(wadFileName, std::ios::binary);
		wd.load(&is);*/

		//	std::map<std::string, ActorDef*, function<bool(std::string, std::string)>> mapRepo(stringCompare);
		//	std::map<std::string, Actor*, function<bool(std::string, std::string)>> mapRepo(stringCompare);


		//wd.getActorRepository(mapRepo, is);
		//is.close();
		/*	ActorDef p1Def, p2Def,p3Def,p4Def;
		ActorDef e1Def, e2Def, e3Def, e4Def;*/

		Actor* p1;
		Actor* e1;

		//p1Def = *(mapRepo.find("Hero")->second);
		/*	p2Def = *(mapRepo.find("Lab Tech")->second);
		p3Def = *(mapRepo.find("Navigator")->second);
		p4Def = *(mapRepo.find("Mechanic")->second);*/

		std::list<Actor*> players;
		/*	players.push_back(buildActorFromDef(&p1Def, ActorType.HUMAN));
		players.push_back(buildActorFromDef(&p2Def, AT_HUMAN));
		players.push_back(buildActorFromDef(&p3Def, AT_HUMAN));
		players.push_back(buildActorFromDef(&p4Def, AT_HUMAN));
		*/

		p1 = buildActor(ActorType::HUMAN);
		p1->stats.strength.setCurr(10);
		p1->stats.agility.setCurr(10);
		p1->name = "Hero";

		players.push_back(p1);
		/*players.push_back(buildActor(ActorType::HUMAN));
		players.push_back(buildActor(ActorType::HUMAN));
		players.push_back(buildActor(ActorType::HUMAN));*/


		//	e1 = *(mapRepo.find("Toadie")->second);
		/*	e2Def = *(mapRepo.find("Bigbug")->second);
		e3Def = *(mapRepo.find("Skittler")->second);
		e4Def = *(mapRepo.find("Wispwing")->second);*/

		std::list<Actor*> enemies;
		e1 = buildActor(ActorType::CPU);
		e1->stats.strength.setCurr(1);
		e1->stats.agility.setCurr(40);
		e1->stats.exp.setCurr(1);
		e1->money.setCurr(1);
		e1->name = "Toadie";
		enemies.push_back(e1);
		/*enemies.push_back(buildActor(ActorType::CPU));
		enemies.push_back(buildActor(ActorType::CPU));
		enemies.push_back(buildActor(ActorType::CPU));*/

		int totalRows = 23;
		int totalCols = 51;
		resize_term(totalRows, totalCols);

		BattleProcessor bp;
		bp.setWindow(newwin(totalRows, totalCols, 0, 0));
		bp.addParticipants(players, enemies);
		
		bool playing = true;
		while (playing)
		{
			bp.draw();
			doupdate();

			int input = getch();

			switch (input)
			{
			case KEY_ESC:
				playing = false;
				testing = false;
				break;
			default:
			{
				playing = bp.processInput(input);
			}
			break;
			}
		}

		delete p1;
		delete e1;

	}
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

//bool saveActorDef(string fileName, ActorDef* def)
//{
//	ofstream file;
//
//	file.open(fileName, ios::trunc | ios::binary);
//	if (file.is_open() == false)
//		return false;
//
//	//save only the necessary pieces of data
//	file.write((char*)def->name.c_str(), def->name.length());
//	file.write((char*)&def->symbol, sizeof(int));
//	file.write((char*)&def->level, sizeof(short));
//	file.write((char*)&def->exp, sizeof(int));
//	file.write((char*)&def->money, sizeof(int));
//	file.write((char*)&def->maxHp, sizeof(int));
//	file.write((char*)&def->maxMp, sizeof(int));
//	file.write((char*)&def->strength, sizeof(short));
//	file.write((char*)&def->defense, sizeof(short));
//	file.write((char*)&def->agility, sizeof(short));
//	file.write((char*)&def->accuracy, sizeof(int));
//	file.write((char*)&def->luck, sizeof(int));
//
//	//file.write((char*)dataBuf, size);
//	file.close();
//
//	return true;
//}



void mapEditorTest()
{
	/*Good lesson learned here. Always ensure that your terminal is sized to contain all windows that it renders or else window creation routines will return null*/
	resize_term(30, 150);


	MapEditor me;

	mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
	bool editing = true;
	while (editing) //simulate input/process/update loop
	{
		me.draw();

		doupdate();
		int input = getch();

		editing = me.processInput(input);
	}
}
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




void masterEditorTest()
{
	MasterEditor* editor = new MasterEditor();

	bool editing = true;
	while (editing) //simulate input/process/update loop
	{
		editor->draw();

		doupdate();
		int input = getch();

		editing = editor->processInput(input);
	}
}
