#include "misc_tests.h"
#include "MainMenu.h"
#include "InputManager.h"
#include "defaults.h"
#include "GameInput.h"
#include "actor_helper.h"
#include "BattleProcessor.h"
#include "ResourceManager.h"
#include "ExplorationProcessor.h"
#include "MusicPlayer.h"
#include "GameItem.h"
#include "TitleScreen.h"

void titleScreenTest()
{
	TitleScreen titleScreen;

	InputManager inputManager;
	defaultGameInputs(inputManager);

	bool playing = true;
	while (playing)
	{
		titleScreen.draw();
		doupdate();

		int input = getInput(inputManager);

		switch (input)
		{
		case GameInput::QUIT_INPUT:
			playing = false;
			break;
		default:
		{
			switch (titleScreen.processInput(input))
			{

			case ExitCode::TERMINATE: playing = false; break;
			}

		}
		break;
		}
	}
}

void mainMenuTest()
{
	Actor player1;
	initTestActor(player1);
	player1.name = "Test guy9012345";
	player1.stats.hp.setCurrMax(100);
	player1.stats.hp.setCurr(97);

	Actor player2;
	initTestActor(player2);
	player2.name = "2nd test guy   ";
	player2.stats.hp.setCurrMax(68);
	player2.stats.hp.setCurr(62);

	std::vector<Actor*> playerParty;
	playerParty.push_back(&player1);
	playerParty.push_back(&player2);

	ResourceManager rm;
	setupDefaultDataKeys(rm);

	MainMenu mm;
	mm.setResourceManager(&rm);
	mm.setWindow(newwin(screenHeight, screenWidth, 0, 0));
	mm.addPlayerParty(playerParty);
	

	InputManager inputManager;
	defaultGameInputs(inputManager);

	

	bool playing = true;
	while (playing)
	{
		mm.draw();
		doupdate();

		int input = getInput(inputManager);

		switch (input)
		{
		case KEY_ESC:
			playing = false;
			break;
		case GameInput::FIGHT_TRIGGER: //just testing
			rm.theData.updateIntData("Gold$", 50);
			break;
		default:
		{
			playing = (mm.processInput(input) == HANDLED);
			
		}
		break;
		}
	}

	



}


void battleProcessorTest()
{
	bool testing = true;
	while (testing)
	{
		InputManager inputManager;
		defaultGameInputs(inputManager);

		//retrieve all actors from a wad file (just for testing, this should be refined)
		ResourceManager rm;
		setupDefaultDataKeys(rm);

		std::ifstream is(actorFile);
		rm.loadActorsFromTextFile(is);

		Actor p1, p2, p3, p4;
		Actor e1, e2, e3, e4;

		p1 = rm.actors.find("Hero")->second;
		p2 = rm.actors.find("Lab Tech")->second;
		p3 = rm.actors.find("Navigator")->second;
		p4 = rm.actors.find("Mechanic")->second;

		p1.type = ActorType::HUMAN;
		p2.type = ActorType::HUMAN;
		p3.type = ActorType::HUMAN;
		p4.type = ActorType::HUMAN;

		std::list<Actor*> players;

		players.push_back(&p1);
	/*	players.push_back(&p2);
		players.push_back(&p3);*/
		//players.push_back(&p4);
		

		std::list<Actor*> enemies;

		e1 = rm.actors.find("Toadie")->second;
		e2 = rm.actors.find("Bigbug")->second;
		e3 = rm.actors.find("Skittler")->second;
		e4 = rm.actors.find("Wispwing")->second;
		
		e1.type = ActorType::CPU;
		e2.type = ActorType::CPU;
		e3.type = ActorType::CPU;
		e4.type = ActorType::CPU;

		enemies.push_back(&e1);
		/*enemies.push_back(&e2);
		enemies.push_back(&e3);
		enemies.push_back(&e4);*/
	

		int totalRows = 23;
		int totalCols = 51;
		resize_term(totalRows, totalCols);

		BattleProcessor bp;
		bp.setResourceManager(&rm);
		bp.setWindow(newwin(totalRows, totalCols, 0, 0));
		bp.addParticipants(players, enemies);
		bp.begin();

		

		int playing = NOT_HANDLED;
		while (playing <= 0)
		{
			bp.draw();
			doupdate();

			int input = getInput(inputManager);

			switch (input)
			{
			case GameInput::QUIT_INPUT:
				playing = TERMINATE;
				testing = false;
				break;
			default:
			{
				playing = bp.processInput(input);
			}
			break;
			}
		}
	}
}

void exploreMegaMapTest()
{
	//resize_term(screenHeight, screenWidth);

	//bool playing = true;

	//WINDOW* screen = newwin(screenHeight, screenWidth, 0, 0);

	//ResourceManager rm;
	//setupDefaultDataKeys(rm);

	//rm.loadNullResources();
	////std::ifstream is(actorFile);
	////rm.loadActorsFromTextFile(is);

	//FileDirectory dataDir(dataDirName);
	//rm.loadGameMapsFromDir(dataDir);

	//Map map0 = rm.gameMaps[11];
	//map0.setWindow(screen);

	//Map map1 = rm.gameMaps[12];
	//Map map2 = rm.gameMaps[5];
	//map1.setWindow(screen);
	//map2.setWindow(screen);

	////setup main character
	//Actor mainC;
	//initTestActor(mainC);
	//mainC.name = "hero.actr";
	//mainC.type = ActorType::HUMAN;
	//mainC.symbol = 'A' | COLOR_YELLOW_BOLD << TEXTCOLOR_OFFSET;
	//mainC.x = 48;
	//mainC.y = 20;

	//map0.setControlActor(&mainC);

	////MapRepository repo(23, 51);

	//ExplorationProcessor mp(&(mainC.y), &(mainC.x));
	//unsigned short id0 = 0;
	//unsigned short id1 = 1;
	//unsigned short id2 = 2;
	//map0.setId(id0);
	//map1.setId(id1);
	//map2.setId(id2);

	//MapRepository& repo = mp.getMapRepo();
	//repo.addMapSeam(map0, map1, Axis::HORIZONTAL, 0, 1, 1);
	//repo.addMapSeam(map2, map0, Axis::VERTICAL, 0, 0, 1);
	//repo.addMapSeam(map2, map1, Axis::HORIZONTAL, 0, 0, 1);
	//repo.add(map0); //could combine this without previous method
	//repo.add(map1);
	//repo.add(map2);

	//int currId = map0.getId();
	//mp.setCurrMap(currId);
	//mp.setViewMode(ViewMode::DYNAMIC); //position map so character is visible (not sure if this is the best way to do this)

	//MusicPlayer musicPlayer;
	//std::string song1 = "01-02- 12 Microtonal Etudes, Op 28 I 16 notes Andantino.wav";
	//std::string song2 = "11 CONTEMPLATIVE INTERMISSION.wav";
	//int ret = musicPlayer.playFile(song1);

	//while (playing)
	//{
	//	//draw map
	//	mp.draw();

	//	//add y,x coordinates to screen
	//	mvwprintw(screen, screenHeight - 2, screenWidth - 16, "y:%+4u x:%+4u", mainC.y, mainC.x);
	//	wnoutrefresh(screen);

	//	doupdate();


	//	//update music if changing maps
	//	if ((currId <= 1) && mp.getCurrMap()->getId() == 2)
	//	{
	//		musicPlayer.stop();
	//		musicPlayer.playFile(song2);
	//		currId = mp.getCurrMap()->getId();
	//	}
	//	else if (currId == 2 && mp.getCurrMap()->getId() <= 1)
	//	{
	//		musicPlayer.stop();
	//		musicPlayer.playFile(song1);
	//		currId = mp.getCurrMap()->getId();
	//	}


	//	//process input
	//	int input = getch();
	//	switch (input)
	//	{
	//	case KEY_ESC: playing = false; break;
	//	case KEY_RIGHT:
	//	case KEY_LEFT:
	//	case KEY_UP:
	//	case KEY_DOWN:
	//		mp.processMovementInput(input); break;
	//	case '\t': //toggle automap
	//		break;
	//	default:
	//		break;
	//	}
	//}

	//delwin(screen);
}


void exploreOneMapTest()
{
	resize_term(screenHeight, screenWidth);

	bool playing = true;

	ResourceManager rm;
	setupDefaultDataKeys(rm);

	std::ifstream is2(actorFile);
	rm.loadActorsFromTextFile(is2); //TODO we don't need this yet, but I need the ids to be populated correctly for now

	FileDirectory dataDir(dataDirName);
	rm.loadGameMapsFromDir(dataDir);

	std::ifstream is(mapFile);
	rm.loadMapsFromTextFile(is);

	//setup main character
	Actor mainC;
	initTestActor(mainC);
	mainC.name = "hero.actr";
	mainC.type = ActorType::HUMAN;
	mainC.symbol = 'A' | COLOR_YELLOW_BOLD << TEXTCOLOR_OFFSET;
	
	int itemRoomId = 15;

	MegaMap& mm = rm.gameMaps["TestRegion"];
	mm.setUnitHeight(screenHeight);
	mm.setUnitWidth(screenWidth);
	mm.setFloor(-1);
	

	//create an item in the item room
	GameItem money;
	money.id = 300; //making this large to avoid conflicting with maps
	money.name = GOLD$;
	money.type = GameItemType::MONEY;

	Sprite s;
	s.pos.y = 13;
	s.pos.x = 23;
	s.quantity = 5;
	s.symbol = '$' | COLOR_YELLOW_BOLD << TEXTCOLOR_OFFSET;
	s.thing = &money;

	GameItem item;
	item.name = "Potion";
	item.cost = 10;
	item.type = GameItemType::CONSUMABLE;
	item.value = 25;

	Sprite s2;
	s2.pos.y = 13;
	s2.pos.x = 33;
	s2.quantity = 1;
	s2.symbol = '*' | COLOR_MAGENTA_BOLD << TEXTCOLOR_OFFSET;
	s2.thing = &item;

	MapRoom& itemRoom = rm.mapRooms[itemRoomId];
	itemRoom.things.push_back(&s);
	itemRoom.things.push_back(&s2);

	ExplorationProcessor mp;
	mp.setMap(mm);
	mp.setResourceManager(&rm);
	mp.setControlActor(&mainC);
	Pos pos(32, 54);
	mp.setCursor(pos);
	mm.setCursor(&pos.y, &pos.x);
	mp.setViewMode(ViewMode::DYNAMIC); //position map so character is visible (not sure if this is the best way to do this)

	WINDOW* screen = mp.getScreen();
	while (playing)
	{
		//draw map
		mp.draw();

		//add y,x coordinates to screen
		Pos cursor = mp.getCursor();
		mvwprintw(screen, screenHeight - 2, screenWidth - 16, "y:%+4u x:%+4u", cursor.y, cursor.x);
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
			mp.processMovementInput(input); break;
		case '\t': //toggle automap
			break;
		case '+': mp.setClipMode(true); break;
		case '-': mp.setClipMode(false); break;
		default:
			break;
		}
	}

	delwin(screen);
}