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
#include "ConfigMenu.h"
#include "Barrier.h"
#include "ItemBrowser.h"
#include "CenteredFormat.h"

void titleScreenTest()
{
	TitleScreen titleScreen;

	ResourceManager rm;

	loadDataFiles(rm);

	bool playing = true;
	while (playing)
	{
		titleScreen.draw();
		doupdate();

		int input = rm.getInputManager().getInput();

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




void inventoryTest()
{
	ItemBrowser inventory;
	inventory.setWindow(newwin(10, 20, 1, 1));



	std::vector<OwnedItem*> items;
	
	OwnedItem blankItem;
	blankItem.item = nullptr;
	blankItem.quantity = 0;
	items.push_back(&blankItem);

	/*GameItem potion;
	potion.name = "Potion";
	potion.description = "Restores 50 hp";

	GameItem knife;
	knife.name = "Knife";
	knife.description = "A weapon for stabbing";

	OwnedItem item;
	item.item = &potion;
	item.quantity = 1;

	OwnedItem item2;
	item2.item = &knife;
	item2.quantity = 1;

	items.push_back(&item);
	items.push_back(&item2);*/
	
	
	
	inventory.setItems(items);

	Frame playerFrame;
	Frame descFrame;
	TextLabel descLbl;
	descLbl.setFormat(new CenteredFormat);
	descLbl.setWindow(newwin(4, 18, 12, 2));
	descFrame.setControl(&descLbl);

	inventory.setPlayerFrame(&playerFrame); //TODO will need to use a real player menu at some point
	inventory.setDescFrame(&descFrame);
	descFrame.setWindow(newwin(6, 20, 11, 1));

	inventory.registerControls();

	InputManager mgr;
	loadHardCodedInputs(mgr.getInputs());
	
	bool playing = true;
	while (playing)
	{
		descFrame.draw();
		inventory.draw();
		doupdate();

		inventory.setInput(mgr.getInput());
		inventory.processInput();	
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

	ResourceManager rm;
	GameData& data = rm.getData();

	loadDataFiles(rm);

	rm.playerParty.push_back(player1);
	rm.playerParty.push_back(player2);

	//load one megamap so we can test that it's properties display in the main menu
	rm.currMap = &data.getMap("TestRegion");
	rm.currMap->setUnitHeight(gameScreenHeight);
	rm.currMap->setUnitWidth(gameScreenWidth);

	Pos pos(32, 54);
	rm.currMap->setCursor(&pos.y, &pos.x);

	MainMenu mm(&rm);
	mm.setWindow(newwin(gameScreenHeight, gameScreenWidth, 0, 0));

	//mm.addPlayerParty(rm.playerParty);
	
	//setup some items
	/*OwnedItem item1;
	item1.item = &data.getItem("Potion");
	item1.quantity = 1;

	OwnedItem item2;
	item2.item = &data.getItem("Knife");
	item2.quantity = 1;

	rm.inventory.push_back(&item1);
	rm.inventory.push_back(&item2);*/

	
	bool playing = true;
	while (playing)
	{
		mm.draw();
		doupdate();

		int input = rm.getInputManager().getInput();

		switch (input)
		{
		case GameInput::QUIT_INPUT:
			playing = false;
			break;
		case GameInput::FIGHT_TRIGGER: //just testing
			data.updateIntData("Gold$", 50);
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
		ResourceManager rm;
		GameData& data = rm.getData();
		loadDataFiles(rm);

		Actor p1, p2, p3, p4;
		Actor e1, e2, e3, e4;

		
		p1 = data.getActor("Hero");
		p2 = data.getActor("Lab Tech");
		p3 = data.getActor("Navigator");
		p4 = data.getActor("Mechanic");

		p1.type = ActorType::HUMAN;
		p2.type = ActorType::HUMAN;
		p3.type = ActorType::HUMAN;
		p4.type = ActorType::HUMAN;

		std::list<Actor*> players;

		players.push_back(&p1);
		players.push_back(&p2);
		players.push_back(&p3);
		players.push_back(&p4);
		

		std::list<Actor*> enemies;

		e1 = data.getActor("Toadie");
		e2 = data.getActor("Bigbug");
		e3 = data.getActor("Skittler");
		e4 = data.getActor("Wispwing");
		
		e1.type = ActorType::CPU;
		e2.type = ActorType::CPU;
		e3.type = ActorType::CPU;
		e4.type = ActorType::CPU;

		enemies.push_back(&e1);
		enemies.push_back(&e2);
		enemies.push_back(&e3);
		enemies.push_back(&e4);
	

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

			int input = rm.getInputManager().getInput();

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

//only saving this for musicplayer code
void exploreMegaMapTest()
{
	//MusicPlayer musicPlayer;
	//std::string song1 = "01-02- 12 Microtonal Etudes, Op 28 I 16 notes Andantino.wav";
	//std::string song2 = "11 CONTEMPLATIVE INTERMISSION.wav";
	//int ret = musicPlayer.playFile(song1);

	

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
}


void exploreOneMapTest()
{
	resize_term(gameScreenHeight, gameScreenWidth);

	bool playing = true;

	ResourceManager rm;
	loadDataFiles(rm);
	GameData& data = rm.getData();

	//setup main character
	Actor& player1 = data.getActor(player1Name);
	player1.type = ActorType::HUMAN;

	rm.playerParty.push_back(player1);

	

	
	rm.currMap = &data.getMap("TestRegion");
	rm.currMap->setUnitHeight(gameScreenHeight);
	rm.currMap->setUnitWidth(gameScreenWidth);
	rm.currMap->setFloor(0);
	
	ExplorationProcessor mp;
	mp.setResourceManager(&rm);
	mp.setControlActor(&player1);
	Pos pos(32, 54);
	mp.setCursor(pos);
	mp.setViewMode(ViewMode::DYNAMIC); //position map so character is visible (not sure if this is the best way to do this)

	Sprite s;
	s.pos.y = 13;
	s.pos.x = 23;
	s.quantity = 5;
	s.symbol = '$' | COLOR_YELLOW_BOLD << TEXTCOLOR_OFFSET;
	s.thing = &data.getItem(GOLD$);
	s.impassible = false;

	Sprite s2;
	s2.pos.y = 13;
	s2.pos.x = 33;
	s2.quantity = 1;
	s2.symbol = '*' | COLOR_MAGENTA_BOLD << TEXTCOLOR_OFFSET;
	s2.thing = &data.getItem("Potion");
	s2.impassible = false;

	Sprite s3;
	s3.pos.y = 2;
	s3.pos.x = 2;
	s3.height = 3;
	s3.width = 4;
	s3.impassible = true;
	s3.quantity = 1;
	s3.symbol = '&';

	SimpleCommand<ExplorationProcessor> barrierCmd;
	barrierCmd.setReceiver(&mp);
	barrierCmd.setAction(&ExplorationProcessor::barrierRoutine);

	Reactor<ExplorationProcessor> barrier;
	barrier.cmd = &barrierCmd;

	s3.thing = &barrier;

	Sprite s4;
	s4.pos.y = 7;
	s4.pos.x = 40;
	s4.impassible = true;
	s4.quantity = 1;
	s4.symbol = '!';

	SimpleCommand<ExplorationProcessor> signCmd;
	signCmd.setReceiver(&mp);
	signCmd.setAction(&ExplorationProcessor::signRoutine);

	Reactor<ExplorationProcessor> sign;
	sign.cmd = &signCmd;

	s4.thing = &sign;

	Sprite s5;
	s5.pos.y = 7;
	s5.pos.x = 30;
	s5.impassible = true;
	s5.quantity = 1;
	s5.symbol = '#' | COLOR_BLUE << TEXTCOLOR_OFFSET;

	SimpleCommand<ExplorationProcessor> blockCmd;
	blockCmd.setReceiver(&mp);
	blockCmd.setAction(&ExplorationProcessor::blockRoutine);

	Reactor<ExplorationProcessor> block;
	block.cmd = &blockCmd;

	s5.thing = &block;

	MapRoom& itemRoom = data.getRoom("Labyrinth2");
	
	WINDOW* screen = mp.getScreen();
	while (playing)
	{
		//draw map
		mp.draw();

		//add y,x coordinates to screen
		Pos cursor = mp.getCursor();
		mvwprintw(screen, gameScreenHeight - 2, gameScreenWidth - 16, "y:%+4u x:%+4u", cursor.y, cursor.x);
		wnoutrefresh(screen);

		doupdate();

		//process input
		int input = rm.getInputManager().getInput();
		switch (input)
		{
		case GameInput::QUIT_INPUT: playing = false; break;
		//case '\t': //toggle automap
		//	break;
	/*	case '+': mp.setClipMode(true); break;
		case '-': mp.setClipMode(false); break;*/
		default:
			mp.processInput(input); break;
			break;
		}
	}
}



void configMenuTest()
{
	ResourceManager rm;
	loadDataFiles(rm);

	ConfigMenu menu;
	WINDOW* win = TUI::winMgr.newWin(10, 20, 1, 1);
	wbkgd(win, COLOR_RED << BKGDCOLOR_OFFSET);
	menu.setWindow(win);
	menu.setResourceManager(&rm);
	

	bool playing = true;
	while (playing)
	{
		menu.draw();
		doupdate();
		int c = rm.getInputManager().getInput();

		switch(c)
		{
		case QUIT_INPUT: playing = false; continue;
		default:
			menu.processInput(c);
		}

		
	}
	
}